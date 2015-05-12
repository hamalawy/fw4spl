/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "uiMedDataQt/editor/SSeries.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slots.hxx>

#include <fwComEd/VectorMsg.hpp>
#include <fwComEd/helper/SeriesDB.hpp>

#include <fwData/Vector.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGuiQt/container/QtContainer.hpp>

#include <fwMedData/Patient.hpp>
#include <fwMedData/Study.hpp>

#include <fwMedDataTools/functions.hpp>

#include <fwServices/macros.hpp>

#include <fwTools/Object.hpp>

#include "uiMedDataQt/widget/PatientEditor.hpp"
#include "uiMedDataQt/widget/StudyEditor.hpp"
#include "uiMedDataQt/widget/EquipmentEditor.hpp"
#include "uiMedDataQt/widget/SeriesEditor.hpp"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace uiMedData
{
namespace editor
{

fwServicesRegisterMacro( ::gui::editor::IEditor, ::uiMedData::editor::SSeries, ::fwData::Vector );

const ::fwCom::Signals::SignalKeyType SSeries::s_EXPORT_SLOT         = "export";
const ::fwCom::Signals::SignalKeyType SSeries::s_SERIES_EXPORTED_SIG = "seriesExported";
const ::fwCom::Signals::SignalKeyType SSeries::s_CAN_EXPORT_SIG      = "canExport";

//------------------------------------------------------------------------------

SSeries::SSeries()
{
    m_slotExport = ::fwCom::newSlot( &SSeries::onExportClicked, this );
    ::fwCom::HasSlots::m_slots( s_EXPORT_SLOT, m_slotExport );

    m_sigCanExport      = CanExportSignalType::New();
    m_sigSeriesExported = SeriesExportedSignalType::New();

    ::fwCom::HasSignals::m_signals
        (s_CAN_EXPORT_SIG, m_sigCanExport)
        (s_SERIES_EXPORTED_SIG, m_sigSeriesExported)
    ;

#ifdef COM_LOG
    m_slotExport->setID(s_EXPORT_SLOT);
    m_sigCanExport->setID(s_CAN_EXPORT_SIG);
    m_sigSeriesExported->setID(s_SERIES_EXPORTED_SIG);
#endif

    // worker was set by IService
    ::fwCom::HasSlots::m_slots.setWorker( m_associatedWorker );
}

//------------------------------------------------------------------------------

SSeries::~SSeries() throw()
{
}

//------------------------------------------------------------------------------

void SSeries::starting() throw(::fwTools::Failed)
{
    this->::fwGui::IGuiContainerSrv::create();

    ::fwGuiQt::container::QtContainer::sptr qtContainer
        = ::fwGuiQt::container::QtContainer::dynamicCast(this->getContainer());

    QWidget* const container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);

    m_patientEditor   = new ::uiMedData::widget::PatientEditor();
    m_studyEditor     = new ::uiMedData::widget::StudyEditor();
    m_equipmentEditor = new ::uiMedData::widget::EquipmentEditor();
    m_seriesEditor    = new ::uiMedData::widget::SeriesEditor();


    QVBoxLayout* studyEquipmentLayout = new QVBoxLayout();
    studyEquipmentLayout->addWidget(m_studyEditor);
    studyEquipmentLayout->addWidget(m_equipmentEditor);

    QHBoxLayout* editorLayout = new QHBoxLayout();
    editorLayout->addWidget(m_patientEditor);
    editorLayout->addLayout(studyEquipmentLayout);
    editorLayout->addWidget(m_seriesEditor);


    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->addLayout(editorLayout);

    m_btnExport = new QPushButton(tr("Export series"));
    m_btnExport->setEnabled(false);
    m_btnExport->setVisible(m_sigCanExport->getNumberOfConnections() == 0);
    fwServicesNotifyMacro(this->getLightID(), m_sigCanExport, (false));
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->setSizeConstraint(QLayout::SetFixedSize);
    btnLayout->addWidget(m_btnExport);
    layout->addLayout(btnLayout);
    QObject::connect(m_btnExport, SIGNAL(clicked()), this, SLOT(onExportClicked()));
}

//------------------------------------------------------------------------------

void SSeries::stopping() throw(::fwTools::Failed)
{
    this->getContainer()->clean();
    this->::fwGui::IGuiContainerSrv::destroy();
}

//------------------------------------------------------------------------------

void SSeries::updating() throw(::fwTools::Failed)
{
    ::fwData::Vector::sptr vector = this->getObject< ::fwData::Vector >();
    SLM_ASSERT("Failed to retrieve vector", vector);

    m_btnExport->setVisible(m_sigCanExport->getNumberOfConnections() == 0);

    if(!vector->empty())
    {

        ::fwTools::Object::sptr obj = ::fwTools::fwID::getObject(m_seriesId);
        SLM_ASSERT("Failed to retrieve object with UID '" + m_seriesId + "'", obj);
        ::fwMedData::Series::sptr seriesSrc = ::fwMedData::Series::dynamicCast(obj);
        SLM_ASSERT("Failed to retrieve a ::fwMedData::Series from object '" << m_seriesId << "'", seriesSrc);

        ::fwMedData::Series::sptr series = ::fwMedData::Series::dynamicCast((*vector)[0]);
        SLM_ASSERT("Failed to retrieve ::fwMedData::Series from vector", series);
        series->setDescription(seriesSrc->getDescription());
        series->setPerformingPhysiciansName(seriesSrc->getPerformingPhysiciansName());

        m_patientEditor->setSeries(series);
        m_studyEditor->setSeries(series);
        m_equipmentEditor->setSeries(series);
        m_seriesEditor->setSeries(series);
        m_btnExport->setEnabled(true);
        fwServicesNotifyMacro(this->getLightID(), m_sigCanExport, (true));
    }
    else
    {
        m_btnExport->setEnabled(false);
        fwServicesNotifyMacro(this->getLightID(), m_sigCanExport, (false));
    }
}

//------------------------------------------------------------------------------

void SSeries::onExportClicked()
{
    SLM_TRACE_FUNC();

    ::fwTools::Object::sptr obj = ::fwTools::fwID::getObject(m_seriesId);
    SLM_ASSERT("Failed to retrieve object with UID '" + m_seriesId + "'", obj);
    ::fwMedData::Series::sptr series = ::fwMedData::Series::dynamicCast(obj);
    SLM_ASSERT("Failed to retrieve a ::fwMedData::Series from object '" << m_seriesId << "'", series);

    ::fwTools::Object::sptr objDB = ::fwTools::fwID::getObject(m_seriesDBId);
    SLM_ASSERT("Failed to retrieve object with UID '" + m_seriesDBId + "'", objDB);
    ::fwMedData::SeriesDB::sptr seriesDB = ::fwMedData::SeriesDB::dynamicCast(objDB);
    SLM_ASSERT("Failed to retrieve a ::fwMedData::SeriesDB from object '" << m_seriesDBId << "'", seriesDB);

    if(m_patientEditor->isValid() && m_studyEditor->isValid()
       && m_equipmentEditor->isValid() && m_seriesEditor->isValid())
    {

        ::fwData::Vector::sptr vector = this->getObject< ::fwData::Vector >();
        SLM_ASSERT("Failed to retrieve vector", vector);
        ::fwMedData::Series::sptr seriesVec = ::fwMedData::Series::dynamicCast((*vector)[0]);
        SLM_ASSERT("Failed to retrieve ::fwMedData::Series from vector", seriesVec);

        ::fwMedData::Patient::sptr patient = m_patientEditor->getPatient();
        ::fwMedData::Study::sptr study     = m_studyEditor->getStudy();

        // If a new patient is being created but the study edition didn't change a new study instance UID must be set
        if(patient->getPatientId() != seriesVec->getPatient()->getPatientId())
        {
            study->setInstanceUID(::fwMedDataTools::generateStudyInstanceUid());
        }

        series->setPatient(patient);
        series->setStudy(study);
        series->setEquipment(m_equipmentEditor->getEquipment());

        ::fwMedData::Series::sptr seriesInfo = m_seriesEditor->getSeries();
        series->setModality(seriesInfo->getModality());
        series->setDate(seriesInfo->getDate());
        series->setTime(seriesInfo->getTime());
        series->setDescription(seriesInfo->getDescription());
        series->setPerformingPhysiciansName(seriesInfo->getPerformingPhysiciansName());

        ::fwComEd::helper::SeriesDB helper(seriesDB);
        ::fwMedData::SeriesDB::iterator it = std::find(seriesDB->begin(), seriesDB->end(), series);
        if(it != seriesDB->end())
        {
            ::fwGui::dialog::MessageDialog dlg;
            dlg.setTitle("The series already exists");
            dlg.setMessage("This series has already been exported in series selector");
            dlg.setIcon(::fwGui::dialog::IMessageDialog::INFO);

            dlg.show();
        }
        else
        {
            helper.add(series);
            helper.notify(this->getSptr());
        }

        fwServicesNotifyMacro(this->getLightID(), m_sigSeriesExported, ());
    }
    else
    {
        ::fwGui::dialog::MessageDialog dlg;
        dlg.setTitle("Error : information validation failed");
        dlg.setMessage("Given information are not valid : please fix incorrect values before exporting");
        dlg.setIcon(::fwGui::dialog::IMessageDialog::CRITICAL);

        dlg.show();
    }
}

//------------------------------------------------------------------------------

void SSeries::configuring() throw(::fwTools::Failed)
{
    this->::fwGui::IGuiContainerSrv::initialize();

    std::vector< ::fwRuntime::ConfigurationElement::sptr > seriesCfg = m_configuration->find("seriesId");
    SLM_ASSERT("Missing tag 'seriesId'", !seriesCfg.empty());

    m_seriesId = seriesCfg.front()->getValue();
    SLM_ASSERT("seriesId must not be empty", !m_seriesId.empty());

    std::vector< ::fwRuntime::ConfigurationElement::sptr > seriesDBCfg = m_configuration->find("seriesDBId");
    SLM_ASSERT("Missing tag 'seriesDBId'", !seriesDBCfg.empty());

    m_seriesDBId = seriesDBCfg.front()->getValue();
    SLM_ASSERT("seriesDBId must not be empty", !m_seriesDBId.empty());
}

//------------------------------------------------------------------------------

void SSeries::receiving(::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    ::fwComEd::VectorMsg::csptr vectorMsg = ::fwComEd::VectorMsg::dynamicConstCast(_msg);

    if ( vectorMsg && (vectorMsg->hasEvent( ::fwComEd::VectorMsg::ADDED_OBJECTS )
                       || vectorMsg->hasEvent( ::fwComEd::VectorMsg::REMOVED_OBJECTS ) ))
    {
        this->updating();
    }
}

//------------------------------------------------------------------------------

}   // namespace editor
}   // namespace uiMedData

