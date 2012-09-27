/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwComEd/PatientDBMsg.hpp>

#include <fwTools/ProgressToLogger.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/ProgressDialog.hpp>

#include <io/IWriter.hpp>

#include <fwData/PatientDB.hpp>
#include <fwData/location/Folder.hpp>

#include <itkIO/DicomPatientDBWriter.hpp>

#include "ioITK/DicomPatientDBWriterService.hpp"


namespace ioITK
{

fwServicesRegisterMacro( ::io::IWriter , ::ioITK::DicomPatientDBWriterService , ::fwData::PatientDB ) ;

//------------------------------------------------------------------------------

DicomPatientDBWriterService::DicomPatientDBWriterService() throw()
{}

//------------------------------------------------------------------------------

DicomPatientDBWriterService::~DicomPatientDBWriterService() throw()
{}

//------------------------------------------------------------------------------

::io::IOPathType DicomPatientDBWriterService::getIOPathType() const
{
    return ::io::FOLDER;
}

//------------------------------------------------------------------------------

void DicomPatientDBWriterService::configureWithIHM()
{
    SLM_TRACE_FUNC();
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle(this->getSelectorDialogTitle());
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.setType(::fwGui::dialog::LocationDialog::FOLDER);

    ::fwData::location::Folder::sptr  result;
    result= ::fwData::location::Folder::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath = result->getFolder();
        this->setFolder(result->getFolder());
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    }
    else
    {
        this->clearLocations();
    }
}

//------------------------------------------------------------------------------

void DicomPatientDBWriterService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void DicomPatientDBWriterService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void DicomPatientDBWriterService::info(std::ostream &_sstream )
{
    _sstream << "DicomPatientDBWriterService::info" ;
}

//------------------------------------------------------------------------------

std::vector< std::string > DicomPatientDBWriterService::getSupportedExtensions()
{
    std::vector< std::string > extensions ;
    return extensions ;
}

//------------------------------------------------------------------------------

std::string DicomPatientDBWriterService::getSelectorDialogTitle()
{
    return "Choose a directory for DICOM images";
}


//------------------------------------------------------------------------------

void DicomPatientDBWriterService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    if( this->hasLocationDefined() )
    {
        // Retrieve dataStruct associated with this service
        ::fwData::PatientDB::sptr associatedPatientDB = this->getObject< ::fwData::PatientDB >();
        SLM_ASSERT("associatedPatientDB not instanced", associatedPatientDB);

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);
        savePatientDB(this->getFolder(), associatedPatientDB);
        cursor.setDefaultCursor();
    }
}


//------------------------------------------------------------------------------

void DicomPatientDBWriterService::savePatientDB( const ::boost::filesystem::path _patientDBPath,
        ::fwData::PatientDB::sptr _pPatientDB )
{
    SLM_TRACE_FUNC();
    ::itkIO::DicomPatientDBWriter::NewSptr myWriter;

    myWriter->setObject(_pPatientDB);
    ::fwData::location::Folder::NewSptr loc;
    loc->setFolder(_patientDBPath);
    myWriter->setLocation(loc);

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Saving PatientDB ");
        myWriter->addHandler( progressMeterGUI );
        myWriter->write();
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during saving : " << e.what();
        ::fwGui::dialog::MessageDialog::showMessageDialog("Warning",
                ss.str(),
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog("Warning",
                "Warning during saving",
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
}

//------------------------------------------------------------------------------

} // namespace ioITK

