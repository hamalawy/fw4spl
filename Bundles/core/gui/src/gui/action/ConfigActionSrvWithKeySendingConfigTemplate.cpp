/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <set>
#include <boost/assign/list_of.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/AppConfig.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwTools/UUID.hpp>
#include <fwTools/fwID.hpp>
#include <fwData/Composite.hpp>
#include <fwData/String.hpp>
#include <fwData/Boolean.hpp>
#include <fwComEd/CompositeMsg.hpp>


#include "gui/action/ConfigActionSrvWithKeySendingConfigTemplate.hpp"

namespace gui
{
namespace action
{

//------------------------------------------------------------------------------

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::gui::action::ConfigActionSrvWithKeySendingConfigTemplate, ::fwData::Composite );

//------------------------------------------------------------------------------

ConfigActionSrvWithKeySendingConfigTemplate::ConfigActionSrvWithKeySendingConfigTemplate() throw() :
        m_tooltipConfigTitleKey(""),
        m_iconConfigId(""),
        m_viewConfigTitlePrefixKey (""),
        m_isUnique(false),
        m_fieldAdaptors(::fwData::Composite::New())
{
    m_closableConfig = true;
    addNewHandledEvent( ::fwComEd::CompositeMsg::ADDED_KEYS );
    addNewHandledEvent( ::fwComEd::CompositeMsg::REMOVED_KEYS );
}

//------------------------------------------------------------------------------

ConfigActionSrvWithKeySendingConfigTemplate::~ConfigActionSrvWithKeySendingConfigTemplate() throw()
{}

//------------------------------------------------------------------------------

void ConfigActionSrvWithKeySendingConfigTemplate::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    actionServiceStarting();
    bool executable = true;
    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >();
    std::map< std::string, std::string >::const_iterator itr;
    for(itr = m_keyAdaptors.begin(); itr != m_keyAdaptors.end(); ++itr)
    {
        executable &= (composite->find(itr->second)!= composite->end());
    }
    this->::fwGui::IActionSrv::setIsExecutable( executable );
}

//------------------------------------------------------------------------------

void ConfigActionSrvWithKeySendingConfigTemplate::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    actionServiceStopping();
}

//------------------------------------------------------------------------------

void ConfigActionSrvWithKeySendingConfigTemplate::configuring() throw(fwTools::Failed)
{
    SLM_ASSERT("Sorry you must have one (and only one) <config id=... /> element.",
               this->getConfigTree().get_child("service").count("config") == 1 );

    const ::fwServices::IService::ConfigType srvconfig = this->getConfigTree().get_child("service");
    const ::fwServices::IService::ConfigType &config = srvconfig.get_child("config");
    const ::fwServices::IService::ConfigType &configAttr = config.get_child("<xmlattr>");

    this->::fwGui::IActionSrv::initialize();


    SLM_ASSERT( "Sorry, missing attribute 'id' in <config> xml element.", configAttr.count("id") == 1 );
    SLM_ASSERT( "Sorry, missing attribute 'title' in <config> xml element.", configAttr.count("title") == 1 );

    m_viewConfigId    = configAttr.get<std::string>("id");
    m_viewConfigTitle = configAttr.get<std::string>("title");

    m_iconConfigId             = configAttr.get<std::string>("icon", m_iconConfigId);
    m_viewConfigTitlePrefixKey = configAttr.get<std::string>("titlePrefixKey", m_viewConfigTitlePrefixKey);
    m_tooltipConfigTitleKey    = configAttr.get<std::string>("tooltipKey", m_tooltipConfigTitleKey);

    std::string unique_str = configAttr.get("unique", "false");
    OSLM_ASSERT("Wrong value for attribute 'unique': "<<unique_str,
                unique_str == ("yes") || unique_str == ("no")|| unique_str == ("true")|| unique_str == ("false"));
    m_isUnique = (unique_str == "true" || unique_str == "yes");

    m_closableConfig = configAttr.get<std::string>("closable", "yes") != "no";

    SLM_ASSERT( "Sorry, the attribute id in <config> xml element is empty.", ! m_viewConfigId.empty() );

    std::string adaptor("");
    std::string pattern("");

    BOOST_FOREACH( const ::fwServices::IService::ConfigType::value_type &v, srvconfig.equal_range("replace") )
    {
        const ::fwServices::IService::ConfigType &attr = v.second.get_child("<xmlattr>");
        adaptor = attr.get("val","");
        pattern = attr.get("pattern","");
        SLM_ASSERT("<replace> tag must have one non-empty attribute 'val'.", !adaptor.empty());
        SLM_ASSERT("<replace> tag must have one non-empty attribute 'pattern'.", !pattern.empty());
        (*m_fieldAdaptors)[pattern] = ::fwData::String::New(adaptor);
    }

    BOOST_FOREACH( const ::fwServices::IService::ConfigType::value_type &v, srvconfig.equal_range("key") )
    {
        const ::fwServices::IService::ConfigType &attr = v.second.get_child("<xmlattr>");
        adaptor = attr.get("id","");
        pattern = attr.get("pattern","");
        SLM_ASSERT("<replace> tag must have one non-empty attribute 'id'.", !adaptor.empty());
        SLM_ASSERT("<replace> tag must have one non-empty attribute 'pattern'.", !pattern.empty());
        m_keyAdaptors[pattern] = adaptor;
    }
}

//------------------------------------------------------------------------------

void ConfigActionSrvWithKeySendingConfigTemplate::updating() throw(::fwTools::Failed)
{
    this->sendConfig();
}

//------------------------------------------------------------------------------

void ConfigActionSrvWithKeySendingConfigTemplate::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    //this->::gui::action::ConfigActionSrv::updating(_msg);
    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >();
    bool executable = true;
    std::map< std::string, std::string >::const_iterator itr;
    ::fwComEd::CompositeMsg::csptr compositeMsg = ::fwComEd::CompositeMsg::dynamicConstCast (_msg);
    if ( compositeMsg->hasEvent( ::fwComEd::CompositeMsg::ADDED_KEYS ) )
    {
        for(itr = m_keyAdaptors.begin(); itr != m_keyAdaptors.end(); ++itr)
        {
            executable &= (composite->find(itr->second)!= composite->end());
        }
    }
    if ( compositeMsg->hasEvent( ::fwComEd::CompositeMsg::REMOVED_KEYS ) )
    {
        for(itr = m_keyAdaptors.begin(); itr != m_keyAdaptors.end(); ++itr)
        {
            executable &= (composite->find(itr->second)!= composite->end());
        }
    }
    this->::fwGui::IActionSrv::setIsExecutable( executable );
}

//------------------------------------------------------------------------------

void ConfigActionSrvWithKeySendingConfigTemplate::info( std::ostream &_sstream )
{}

//------------------------------------------------------------------------------

void ConfigActionSrvWithKeySendingConfigTemplate::sendConfig()
{
    //AddGenericUidToFieldApadtor();
    // Generate generic UID
    std::string genericUidAdaptor = ::fwServices::registry::AppConfig::getUniqueIdentifier( this->getID(), true);
    // Init manager
    (*m_fieldAdaptors)["GENERIC_UID"] = ::fwData::String::New(genericUidAdaptor);


    ::fwData::Composite::NewSptr finalMap;
    finalMap->deepCopy( m_fieldAdaptors );
    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >();

    std::map< std::string, std::string >::const_iterator itr;
    for(itr = m_keyAdaptors.begin(); itr != m_keyAdaptors.end(); ++itr)
    {
        std::string key = itr->second;
        if ( key == "self" )
        {
            std::string fwID = composite->getID();
            (*finalMap)[itr->first] = ::fwData::String::New(fwID);
        }
        else
        {
            std::string fwID = (*composite)[key]->getID() ;
            (*finalMap)[itr->first] = ::fwData::String::New(fwID);
        }
    }

    // Init manager
    //::fwRuntime::ConfigurationElement::csptr config =
    //        ::fwServices::registry::AppConfig::getDefault()->getAdaptedTemplateConfig( m_viewConfigId, finalMap );
    //::fwServices::AppConfigManager::sptr configTemplateManager = ::fwServices::AppConfigManager::New();
    //configTemplateManager->setConfig( config );


    std::string fieldID         = "::fwServices::registry::AppConfig";
    std::string viewConfigID    = "viewConfigID";
    std::string closableFieldID = "closable";
    std::string iconFieldID     = "icon";
    std::string tooltipFieldID  = "tooltip";
    std::string tabIDFieldID    = "tabID";
    std::string tabID;
    if(m_isUnique)
    {
        tabID = "TABID";
        std::map< std::string, std::string >::const_iterator itr;
        for(itr = m_keyAdaptors.begin(); itr != m_keyAdaptors.end(); ++itr)
        {
            ::fwData::Composite::ContainerType::iterator key = composite->find(itr->second);
            OSLM_ASSERT("Missing key "<<itr->second<<" in composite", key!= composite->end());
            ::fwTools::Object::sptr obj = key->second;
            tabID += "_" + obj->getID();
        }
    }
    else
    {
        tabID = "TABID_" + ::fwTools::UUID::generateUUID();
    }
    ::fwServices::ObjectMsg::sptr  msg  = ::fwServices::ObjectMsg::New();
    ::fwData::String::NewSptr title;

    std::stringstream ss;
    if (    ! m_viewConfigTitlePrefixKey.empty() &&
            composite->find( m_viewConfigTitlePrefixKey ) != composite->end() )
    {
        ::fwData::String::sptr prefix = ::fwData::String::dynamicCast( (*composite)[m_viewConfigTitlePrefixKey] );
        ss << prefix->getValue() << " - " << m_viewConfigTitle;
    }
    else
    {
        ss << m_viewConfigTitle;
    }
    if ( ! m_tooltipConfigTitleKey.empty() &&
            composite->find( m_tooltipConfigTitleKey ) != composite->end() )
    {
        ::fwData::String::sptr tooltip = ::fwData::String::dynamicCast( (*composite)[m_tooltipConfigTitleKey] );
        title->setField( tooltipFieldID, tooltip );
    }

    title->value() = ss.str();
    msg->addEvent( "NEW_CONFIGURATION_HELPER", title );
    title->setField( fieldID , finalMap );
    title->setField( viewConfigID, ::fwData::String::New(m_viewConfigId) );
    title->setField( closableFieldID, ::fwData::Boolean::New(m_closableConfig));
    title->setField( tabIDFieldID, ::fwData::String::New(tabID));
    title->setField( iconFieldID, ::fwData::String::New(m_iconConfigId) );

    ::fwServices::IEditionService::notify(this->getSptr(), composite, msg);
}

//------------------------------------------------------------------------------

}
}

