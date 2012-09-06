/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwData/PatientDB.hpp>
#include <fwData/Patient.hpp>
#include <fwData/Composite.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/ObjectMsg.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>

#include <fwComEd/PatientDBMsg.hpp>
#include <fwComEd/fieldHelper/BackupHelper.hpp>

#include <fwGui/dialog/MessageDialog.hpp>

#include "uiPatientDB/action/ErasePatientDB.hpp"


namespace uiPatientDB
{
namespace action
{

fwServicesRegisterMacro( ::fwGui::IActionSrv , ::uiPatientDB::action::ErasePatientDB , ::fwData::PatientDB ) ;

//------------------------------------------------------------------------------

ErasePatientDB::ErasePatientDB( ) throw()
{}

//------------------------------------------------------------------------------

ErasePatientDB::~ErasePatientDB() throw()
{}

//------------------------------------------------------------------------------

void ErasePatientDB::info(std::ostream &_sstream )
{
    _sstream << "Action for erase PatientDB" << std::endl;
}
//------------------------------------------------------------------------------

void ErasePatientDB::updating( ) throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    ::fwData::PatientDB::sptr pPatientDB = this->getObject< ::fwData::PatientDB >();

    if(!pPatientDB || pPatientDB->getNumberOfPatients()==0)
    {
        return;
    }

    ::fwGui::dialog::MessageDialog messageBox;
    messageBox.setTitle("Erase all patients");
    messageBox.setMessage( "Are you sure to erase all patients ?" );
    messageBox.setIcon(::fwGui::dialog::IMessageDialog::QUESTION);
    messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
    messageBox.addButton(::fwGui::dialog::IMessageDialog::CANCEL);
    ::fwGui::dialog::IMessageDialog::Buttons answer = messageBox.show();

    if ( answer == ::fwGui::dialog::IMessageDialog::OK )
    {
        ::fwData::Patient::sptr pPatientBAK = ::fwComEd::fieldHelper::BackupHelper::getSelectedPatient(pPatientDB);
        ::fwData::PatientDB::PatientContainerType emptyPatient;
        pPatientDB->setPatients(emptyPatient);

        pPatientDB->removeField("SELECTED_PATIENT");
        pPatientDB->removeField("SELECTED_STUDY");
        pPatientDB->removeField("SELECTED_ACQUISITION");

        ::fwComEd::PatientDBMsg::NewSptr msg;
        msg->addEvent(::fwComEd::PatientDBMsg::CLEAR_PATIENT);
        ::fwServices::IEditionService::notify(this->getSptr(), pPatientDB, msg);
    }
}

//------------------------------------------------------------------------------

void ErasePatientDB::configuring() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::initialize();
}

//------------------------------------------------------------------------------

void ErasePatientDB::starting() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::actionServiceStarting();
}

//------------------------------------------------------------------------------

void ErasePatientDB::updating( ::fwServices::ObjectMsg::csptr _msg ) throw (::fwTools::Failed)
{}

//------------------------------------------------------------------------------

void ErasePatientDB::stopping() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::actionServiceStopping();
}

//------------------------------------------------------------------------------


} // namespace action
} // namespace uiPatientDB
