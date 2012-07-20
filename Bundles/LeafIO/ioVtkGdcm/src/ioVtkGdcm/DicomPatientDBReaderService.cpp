/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/bind.hpp>

#include <fwCore/base.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwComEd/PatientDBMsg.hpp>

#include <fwTools/ProgressToLogger.hpp>

#include <fwGui/dialog/ProgressDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/Cursor.hpp>

#include <io/IReader.hpp>

#include <fwData/PatientDB.hpp>

#include <vtkGdcmIO/DicomPatientDBReader.hpp>

#include "ioVtkGdcm/DicomPatientDBReaderService.hpp"


namespace ioVtkGdcm
{

fwServicesRegisterMacro( ::io::IReader , ::ioVtkGdcm::DicomPatientDBReaderService , ::fwData::PatientDB ) ;

//------------------------------------------------------------------------------

DicomPatientDBReaderService::DicomPatientDBReaderService() throw()
{}

//------------------------------------------------------------------------------

DicomPatientDBReaderService::~DicomPatientDBReaderService() throw()
{}

//------------------------------------------------------------------------------

void DicomPatientDBReaderService::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle(this->getSelectorDialogTitle());
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::READ);
    dialogFile.setType(::fwGui::dialog::LocationDialog::FOLDER);

    ::fwData::location::Folder::sptr  result;
    result= ::fwData::location::Folder::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath = result->getFolder();
        this->setFolder( result->getFolder() );
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    }
}

//------------------------------------------------------------------------------

void DicomPatientDBReaderService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void DicomPatientDBReaderService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void DicomPatientDBReaderService::info(std::ostream &_sstream )
{
    _sstream << "DicomPatientDBReaderService::info" ;
}

//------------------------------------------------------------------------------

std::vector< std::string > DicomPatientDBReaderService::getSupportedExtensions()
{
    std::vector< std::string > extensions ;
    return extensions ;
}

//------------------------------------------------------------------------------

std::string DicomPatientDBReaderService::getSelectorDialogTitle()
{
    return "Choose a directory with DICOM images";
}


//------------------------------------------------------------------------------

::fwData::PatientDB::sptr DicomPatientDBReaderService::createPatientDB( const ::boost::filesystem::path dicomDir )
{
    SLM_TRACE_FUNC();
    ::vtkGdcmIO::DicomPatientDBReader::NewSptr myLoader;
    ::fwData::PatientDB::NewSptr dummy;
    myLoader->setObject( dummy );
    myLoader->setFolder(dicomDir);

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Loading Dicom Image");
        myLoader->addHandler( progressMeterGUI );
        myLoader->read();
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during loading : " << e.what();
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning", ss.str(), ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning", "Warning during loading", ::fwGui::dialog::IMessageDialog::WARNING);
    }

    return myLoader->getConcreteObject();
}

//------------------------------------------------------------------------------

void DicomPatientDBReaderService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    if( this->hasLocationDefined() )
    {
        ::fwData::PatientDB::sptr patientDB = createPatientDB( this->getFolder() );

        if( patientDB->getNumberOfPatients() > 0 )
        {
            // Retrieve dataStruct associated with this service
            ::fwData::PatientDB::sptr associatedPatientDB = this->getObject< ::fwData::PatientDB >();
            SLM_ASSERT("associatedPatientDB not instanced", associatedPatientDB);
            associatedPatientDB->shallowCopy( patientDB ) ;

            ::fwGui::Cursor cursor;
            cursor.setCursor(::fwGui::ICursor::BUSY);
            this->notificationOfDBUpdate();
            cursor.setDefaultCursor();
        }
        else
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog(
                    "Image Reader","This file can not be read. Retry with another file reader.", ::fwGui::dialog::IMessageDialog::WARNING);
        }
    }
}

//------------------------------------------------------------------------------

void DicomPatientDBReaderService::notificationOfDBUpdate()
{
    SLM_TRACE_FUNC();
    ::fwData::PatientDB::sptr pDPDB = this->getObject< ::fwData::PatientDB >() ;
    SLM_ASSERT("pDPDB not instanced", pDPDB);

    ::fwComEd::PatientDBMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::PatientDBMsg::NEW_PATIENT );

    ::fwServices::IEditionService::notify(this->getSptr(),  pDPDB, msg);
}

//-----------------------------------------------------------------------------

::io::IOPathType DicomPatientDBReaderService::getIOPathType() const
{
    return ::io::FOLDER;
}

//------------------------------------------------------------------------------

} // namespace ioInr
