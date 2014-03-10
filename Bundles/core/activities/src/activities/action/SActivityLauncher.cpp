/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStandardItemModel>

#include <fwRuntime/Convert.hpp>

#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwTools/UUID.hpp>
#include <fwTools/fwID.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/AppConfig.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>
#include <fwServices/AppConfigManager.hpp>

#include <fwData/Composite.hpp>
#include <fwData/String.hpp>
#include <fwData/Vector.hpp>
#include <fwMedData/ActivitySeries.hpp>

#include <fwComEd/VectorMsg.hpp>

#include <fwGui/dialog/SelectorDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <fwDataCamp/getObject.hpp>

#include <fwActivities/IBuilder.hpp>
#include <fwActivities/IValidator.hpp>

#include "activities/helper/Activity.hpp"
#include "activities/action/SActivityLauncher.hpp"

Q_DECLARE_METATYPE(::fwActivities::registry::ActivityInfo)

namespace activities
{
namespace action
{

//------------------------------------------------------------------------------

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::activities::action::SActivityLauncher, ::fwData::Vector );

//------------------------------------------------------------------------------

const ::fwCom::Slots::SlotKeyType SActivityLauncher::s_LAUNCH_SERIES_SLOT = "launchSeries";
const ::fwCom::Signals::SignalKeyType SActivityLauncher::s_ACTIVITY_LAUNCHED_SIG = "activityLaunched";

//------------------------------------------------------------------------------

SActivityLauncher::SActivityLauncher() throw():
    m_mode("message")
{
    m_sigActivityLaunched = ActivityLaunchedSignalType::New();
    m_signals( s_ACTIVITY_LAUNCHED_SIG,  m_sigActivityLaunched);

    m_slotLaunchSeries = ::fwCom::newSlot( &SActivityLauncher::launchSeries, this ) ;

    ::fwCom::HasSlots::m_slots( s_LAUNCH_SERIES_SLOT, m_slotLaunchSeries );

#ifdef COM_LOG
    ::fwCom::HasSlots::m_slots.setID();
    ::fwCom::HasSignals::m_signals.setID();
#endif
    this->setWorker( m_associatedWorker );
}

//------------------------------------------------------------------------------

SActivityLauncher::~SActivityLauncher() throw()
{}

//------------------------------------------------------------------------------

void SActivityLauncher::starting() throw(::fwTools::Failed)
{
    this->actionServiceStarting();
    this->updateState();
}

//------------------------------------------------------------------------------

void SActivityLauncher::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->actionServiceStopping();
}

//------------------------------------------------------------------------------

void SActivityLauncher::configuring() throw(fwTools::Failed)
{
    this->::fwGui::IActionSrv::initialize();
    typedef ::fwServices::IService::ConfigType ConfigType;

    m_parameters.clear();
    if(this->getConfigTree().get_child("service").count("config") > 0)
    {
        SLM_ASSERT("Sorry you must have one (and only one) <config/> element.",
                this->getConfigTree().get_child("service").count("config") == 1 );

        const ::fwServices::IService::ConfigType srvconfig = this->getConfigTree().get_child("service");
        const ::fwServices::IService::ConfigType &config = srvconfig.get_child("config");

        m_mode = config.get_optional<std::string>("mode").get_value_or("message");
        SLM_ASSERT("SActivityLauncher mode must be 'immediate' or 'message'",
                   "message" == m_mode || "immediate" == m_mode);

        if(config.count("parameters") == 1 )
        {
            const ::fwServices::IService::ConfigType &configParameters = config.get_child("parameters");
            BOOST_FOREACH( const ConfigType::value_type &v, configParameters.equal_range("parameter") )
            {
                ParametersType::value_type parameter( v.second );
                m_parameters.push_back( parameter );
            }
        }
        OSLM_ASSERT("At most 1 <parameters> tag is allowed", config.count("parameters") < 2);

        if(config.count("filter") == 1 )
        {
            const ::fwServices::IService::ConfigType &configFilter = config.get_child("filter");
            OSLM_ASSERT("At most 1 <mode> tag is allowed", configFilter.count("mode") < 2);

            const std::string mode = configFilter.get< std::string >("mode");
            OSLM_ASSERT("'" << mode << "' value for <mode> tag isn't valid. Allowed values are : 'include', 'exclude'.", mode == "include" || mode == "exclude");
            m_filterMode = mode;

            BOOST_FOREACH( const ConfigType::value_type &v, configFilter.equal_range("id") )
            {
                m_keys.push_back(v.second.get<std::string>(""));
            }
        }
        OSLM_ASSERT("At most 1 <filter> tag is allowed", config.count("filter") < 2);

        if(config.count("quickLaunch") == 1 )
        {
            m_quickLaunch.clear();
            const ::fwServices::IService::ConfigType &configQuickLaunch = config.get_child("quickLaunch");
            BOOST_FOREACH( const ConfigType::value_type &v, configQuickLaunch.equal_range("association") )
            {
                const ::fwServices::IService::ConfigType &association = v.second;
                const ::fwServices::IService::ConfigType xmlattr = association.get_child("<xmlattr>");

                SLM_FATAL_IF( "Sorry, attribute \"type\" is missing", xmlattr.count("type") != 1 );
                SLM_FATAL_IF( "Sorry, attribute \"id\" is missing", xmlattr.count("id") != 1 );

                std::string type = xmlattr.get<std::string>("type");
                std::string id = xmlattr.get<std::string>("id");

                m_quickLaunch[type] = id;
            }
        }
        SLM_ASSERT("At most 1 <quickLaunch> tag is allowed", config.count("quickLaunch") < 2);
    }
}

//------------------------------------------------------------------------------

::fwActivities::registry::ActivityInfo SActivityLauncher::show( const ActivityInfoContainer & infos )
{
    QWidget *parent = qApp->activeWindow();

    QDialog* dialog = new QDialog(parent);
    dialog->setWindowTitle(QString::fromStdString("Choose an activity"));


    QStandardItemModel *model = new QStandardItemModel(dialog);
    BOOST_FOREACH( ::fwActivities::registry::ActivityInfo info, infos)
    {
        std::string text;
        if(info.title.empty())
        {
            text = info.id;
        }
        else
        {
            text = info.title + (info.description.empty() ? "" : "\n" + info.description);
        }

        QStandardItem* item = new QStandardItem(QIcon(info.icon.c_str()), QString::fromStdString(text));
        item->setData(QVariant::fromValue(info));
        item->setEditable(false);
        model->appendRow(item);
    }


    QListView * selectionList = new QListView();
    selectionList->setIconSize(QSize(100,100));
    selectionList->setUniformItemSizes(true);
    selectionList->setModel(model);

    QModelIndex index = model->index( 0, 0 );
    if ( index.isValid() )
    {
        selectionList->selectionModel()->select( index, QItemSelectionModel::Select );
    }

    QPushButton* okButton = new QPushButton("Ok");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(okButton);
    hLayout->addWidget(cancelButton);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(selectionList);
    vLayout->addLayout(hLayout);

    dialog->setLayout(vLayout);
    QObject::connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));
    QObject::connect(selectionList, SIGNAL(doubleClicked( const QModelIndex &  )), dialog, SLOT(accept()));

    ::fwActivities::registry::ActivityInfo info;
    if(dialog->exec())
    {
        QModelIndex currentIndex = selectionList->selectionModel()->currentIndex();
        QStandardItem *item = model->itemFromIndex( currentIndex );
        QVariant var = item->data();
        info = var.value< ::fwActivities::registry::ActivityInfo >();
    }

    return info;
}

//------------------------------------------------------------------------------

SActivityLauncher::ActivityInfoContainer SActivityLauncher::getEnabledActivities(const ActivityInfoContainer& infos)
{
    ActivityInfoContainer configs;

    if(m_filterMode == "include" || m_filterMode == "exclude")
    {
        const bool isIncludeMode = m_filterMode == "include";

        for(ActivityInfoContainer::const_iterator iter = infos.begin(); iter != infos.end(); ++iter)
        {
            KeysType::iterator keyIt = std::find(m_keys.begin(), m_keys.end(), iter->id);

            if(keyIt != m_keys.end() && isIncludeMode)
            {
                configs.push_back(*iter);
            }
            else if(keyIt == m_keys.end() && !isIncludeMode)
            {
                configs.push_back(*iter);
            }
        }
    }
    else
    {
        configs = infos;
    }

    return configs;
}

//------------------------------------------------------------------------------

void SActivityLauncher::updating() throw(::fwTools::Failed)
{
    ::fwData::Vector::sptr selection = this->getObject< ::fwData::Vector >();

    bool launchAS = this->launchAS(selection);
    if (!launchAS)
    {
        ActivityInfoContainer infos = ::fwActivities::registry::Activities::getDefault()->getInfos(selection);
        infos = this->getEnabledActivities(infos);

        if ( ! infos.empty())
        {
            ::fwActivities::registry::ActivityInfo info;
            if((m_keys.size() == 1 && m_filterMode == "include") || (infos.size() == 1))
            {
                info = infos[0];
            }
            else
            {
                info = this->show( infos );
            }

            if( !info.id.empty() )
            {
                this->sendConfig( info );
            }
        }
        else
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog("Activity launcher",
                                                              "Not available activity for the current selection.",
                                                              ::fwGui::dialog::MessageDialog::WARNING);
        }
    }
}

//------------------------------------------------------------------------------

void SActivityLauncher::receiving( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    this->updateState();
}

//------------------------------------------------------------------------------

void SActivityLauncher::updateState()
{
    ::fwData::Vector::sptr selection = this->getObject< ::fwData::Vector >();

    bool isExecutable = false;

    if(selection->size() == 1 && ::fwMedData::ActivitySeries::dynamicCast((*selection)[0]))
    {
        ::fwMedData::ActivitySeries::sptr as = ::fwMedData::ActivitySeries::dynamicCast((*selection)[0]);
        isExecutable = ::fwActivities::registry::Activities::getDefault()->hasInfo(as->getActivityConfigId());
    }
    else
    {
        ::fwActivities::registry::ActivityInfo::DataCountType dataCount;
        dataCount = ::fwActivities::registry::Activities::getDefault()->getDataCount(selection);
        if(m_filterMode.empty() && dataCount.size() == 1)
        {
            ::fwData::Object::sptr obj = selection->front();
            if (::fwMedData::ActivitySeries::dynamicCast(obj))
            {
                isExecutable = true;
            }
        }

        ActivityInfoContainer infos = ::fwActivities::registry::Activities::getDefault()->getInfos(selection);
        infos = this->getEnabledActivities(infos);

        isExecutable |= !infos.empty();
    }

    this->setIsExecutable(isExecutable);
}

//------------------------------------------------------------------------------

void SActivityLauncher::info( std::ostream &_sstream )
{}

//------------------------------------------------------------------------------

void SActivityLauncher::buildActivity(const ::fwActivities::registry::ActivityInfo & info, const ::fwData::Vector::sptr& selection)
{
    ::fwData::Composite::sptr replaceMap = ::fwData::Composite::New();
    ::fwActivities::IBuilder::sptr builder;
    builder = ::fwActivities::builder::factory::New(info.builderImpl);
    OSLM_ASSERT(info.builderImpl << " instantiation failed", builder);

    ::fwMedData::ActivitySeries::sptr actSeries;
    actSeries = builder->buildData(info, selection);
    SLM_ASSERT("ActivitySeries instantiation failed", actSeries);
    ParametersType parameters = this->translateParameters(m_parameters);
    ::fwServices::ObjectMsg::sptr msg = helper::buildActivityMsg(actSeries, info, parameters);

    if( m_mode == "message" )
    {
        fwServicesNotifyMsgMacro(this->getLightID(), m_sigActivityLaunched, msg);
    }
    else
    {
        ::fwGui::LockAction lock(this->getSptr());
        ::fwData::String::csptr msgData = ::fwData::String::dynamicConstCast(msg->getDataInfo("NEW_CONFIGURATION_HELPER"));
        const std::string viewConfigFieldID = "VIEWCONFIGID";
        const std::string fieldID           = "APPCONFIG";

        std::string viewConfigID = msgData->getField(viewConfigFieldID, ::fwData::String::New(""))->value();
        ::fwData::Composite::sptr replaceMap = msgData->getField(fieldID, ::fwData::Composite::New());

        ::fwRuntime::ConfigurationElement::csptr config =
            ::fwServices::registry::AppConfig::getDefault()->getAdaptedTemplateConfig( viewConfigID, replaceMap);

        ::fwServices::AppConfigManager::sptr helper = ::fwServices::AppConfigManager::New();
        helper->setConfig( config );
        helper->launch();
        helper->stopAndDestroy();
    }
}

//------------------------------------------------------------------------------

void SActivityLauncher::sendConfig( const ::fwActivities::registry::ActivityInfo & info )
{
    ::fwData::Vector::sptr selection = this->getObject< ::fwData::Vector >();

    if(!info.validatorImpl.empty())
    {
        ::fwActivities::IValidator::sptr validator = ::fwActivities::validator::factory::New(info.validatorImpl);
        OSLM_ASSERT(info.validatorImpl << " instantiation failed", validator);

        ::fwActivities::IValidator::ValidationType valid = validator->validate(info, selection);
        if(!valid.first)
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog("Activity could not be launched",
                    "The activity " + info.title + " can't be launched.\nReason : " + valid.second,
                    ::fwGui::dialog::MessageDialog::WARNING);
        }
        else
        {
            this->buildActivity(info, selection);
        }
    }
    else
    {
        this->buildActivity(info, selection);
    }
}

//------------------------------------------------------------------------------

bool SActivityLauncher::launchAS(::fwData::Vector::sptr &selection)
{
    bool launchAS = false;
    ::fwActivities::registry::ActivityInfo::DataCountType dataCount;
    dataCount = ::fwActivities::registry::Activities::getDefault()->getDataCount(selection);
    if(dataCount.size() == 1)
    {
        BOOST_FOREACH(::fwData::Object::sptr obj, *selection)
        {
            ::fwMedData::ActivitySeries::sptr as = ::fwMedData::ActivitySeries::dynamicCast(obj);
            if (!as)
            {
                launchAS = false;
                break;
            }
            else
            {
                ::fwActivities::registry::ActivityInfo info;
                info = ::fwActivities::registry::Activities::getDefault()->getInfo(as->getActivityConfigId());
                ParametersType parameters = this->translateParameters(m_parameters);
                ::fwServices::ObjectMsg::sptr msg = helper::buildActivityMsg(as, info, parameters);

                fwServicesNotifyMsgMacro(this->getLightID(), m_sigActivityLaunched, msg);
                launchAS = true;
            }
        }
    }
    return launchAS;
}

//------------------------------------------------------------------------------

void SActivityLauncher::launchSeries(::fwMedData::Series::sptr series)
{
    ::fwMedData::ActivitySeries::sptr as = ::fwMedData::ActivitySeries::dynamicCast(series);
    if (as)
    {
        ::fwActivities::registry::ActivityInfo info;
        info = ::fwActivities::registry::Activities::getDefault()->getInfo(as->getActivityConfigId());
        ParametersType parameters = this->translateParameters(m_parameters);
        ::fwServices::ObjectMsg::sptr msg = helper::buildActivityMsg(as, info, parameters);

        fwServicesNotifyMsgMacro(this->getLightID(), m_sigActivityLaunched, msg);
    }
    else
    {
        ::fwData::Vector::sptr selection = ::fwData::Vector::New();
        selection->getContainer().push_back(series);
        ActivityInfoContainer infos = ::fwActivities::registry::Activities::getDefault()->getInfos(selection);

        if( m_quickLaunch.find( series->getClassname() ) != m_quickLaunch.end() )
        {
            std::string activityId = m_quickLaunch[ series->getClassname() ];
            SLM_ASSERT("Activity information not found for" + activityId,
                    ::fwActivities::registry::Activities::getDefault()->hasInfo(activityId) );
            this->sendConfig( ::fwActivities::registry::Activities::getDefault()->getInfo(activityId) );
        }
        else if ( ! infos.empty() )
        {
            this->sendConfig( infos.front() );
        }
        else
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog("Activity launcher",
                                                              "Not available activity for the current selection.",
                                                              ::fwGui::dialog::MessageDialog::WARNING);
        }
    }
}

//------------------------------------------------------------------------------

SActivityLauncher::ParametersType SActivityLauncher::translateParameters( const ParametersType& parameters )
{
    ParametersType transParams = parameters;
    ::fwData::Object::sptr obj = this->getObject();
    BOOST_FOREACH(ParametersType::value_type& param, transParams)
    {
        if(param.isSeshat())
        {
            ::fwData::Object::sptr subObj = ::fwDataCamp::getObject(obj, param.by);
            OSLM_ASSERT("Invalid seshat path : '"<<param.by<<"'", subObj);
            param.by = subObj->getID();
        }
    }
    return transParams;
}

}
}

