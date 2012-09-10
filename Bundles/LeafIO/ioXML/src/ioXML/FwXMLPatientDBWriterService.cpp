/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwComEd/PatientDBMsg.hpp>

#include <io/IWriter.hpp>

#include <fwCore/base.hpp>

#include <fwTools/System.hpp>

#include <fwData/PatientDB.hpp>
#include <fwData/location/Folder.hpp>

#include <fwXML/writer/FwXMLObjectWriter.hpp>
#include <fwXML/writer/fwxmlextension.hpp>

#include <fwGui/dialog/ProgressDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwZip/ZipFolder.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/Cursor.hpp>

#include "ioXML/FwXMLPatientDBWriterService.hpp"

namespace ioXML
{

fwServicesRegisterMacro( ::io::IWriter , ::ioXML::FwXMLPatientDBWriterService , ::fwData::PatientDB ) ;

//------------------------------------------------------------------------------

FwXMLPatientDBWriterService::FwXMLPatientDBWriterService() throw()
{}

//------------------------------------------------------------------------------

FwXMLPatientDBWriterService::~FwXMLPatientDBWriterService() throw()
{}

//------------------------------------------------------------------------------

::io::IOPathType FwXMLPatientDBWriterService::getIOPathType() const
{
    return ::io::FILE;
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle( "Choose a " FWXML_ARCHIVE_EXTENSION " or a xml file" );
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.addFilter("fwXML archive","*." FWXML_ARCHIVE_EXTENSION);
    dialogFile.addFilter("fwXML archive","*.xml");
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);

    ::fwData::location::SingleFile::sptr  result;
    result= ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath = result->getPath().parent_path() ;
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
        this->setFile(result->getPath());
    }
    else
    {
        this->clearLocations();
    }
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::info(std::ostream &_sstream )
{
    _sstream << "FwXMLPatientDBWriterService::info" ;
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::savePatientDB( const ::boost::filesystem::path inrFileDir, ::fwData::PatientDB::sptr _pPatient )
{
    SLM_TRACE_FUNC();
    ::fwXML::writer::FwXMLObjectWriter::NewSptr myWriter;

    myWriter->setObject(_pPatient);
    myWriter->setFile(inrFileDir);

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Saving Image ");
        myWriter->addHandler( progressMeterGUI );
        myWriter->write();
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during loading : " << e.what();
        ::fwGui::dialog::MessageDialog::showMessageDialog("Warning", ss.str(), ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog("Warning",
                "Warning during loading",
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    if( this->hasLocationDefined() )
    {
        // Retrieve dataStruct associated with this service
        ::fwData::PatientDB::sptr associatedPatientDB = this->getObject< ::fwData::PatientDB >();
        SLM_ASSERT("associatedPatientDB not instanced", associatedPatientDB);

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);
        boost::filesystem::path path = correctFileFormat( this->getFile() );

        if ( isAnFwxmlArchive( path ) )
        {
            manageZipAndSavePatientDB(path,associatedPatientDB);
        }
        else
        {
            savePatientDB(this->getFile(),associatedPatientDB);
        }
        cursor.setDefaultCursor();
    }
}

//------------------------------------------------------------------------------

bool FwXMLPatientDBWriterService::isAnFwxmlArchive( const ::boost::filesystem::path filePath ) const
{
    return ( ::boost::filesystem::extension(filePath) == ("." FWXML_ARCHIVE_EXTENSION) );
}

//------------------------------------------------------------------------------

::boost::filesystem::path FwXMLPatientDBWriterService::correctFileFormat( const ::boost::filesystem::path _filePath ) const
{
    ::boost::filesystem::path newPath = _filePath;
    OSLM_TRACE("Data saved in : " << _filePath.string());
    if ( ::boost::filesystem::extension(_filePath) != ("." FWXML_ARCHIVE_EXTENSION) && ::boost::filesystem::extension(_filePath) != ".xml" )
    {
        newPath = _filePath.string() + "." FWXML_ARCHIVE_EXTENSION;
        OSLM_TRACE("File renamed in : " << newPath.string());
    }

    return newPath;
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::manageZipAndSavePatientDB( const ::boost::filesystem::path inrFileDir, ::fwData::PatientDB::sptr _pPatient )
{
    // Save in tmp folder
    ::boost::filesystem::path srcFolder = ::fwTools::System::getTemporaryFolder() / "fwxmlArchiveFolder";
    ::boost::filesystem::path xmlfile = srcFolder / "patient.xml";
    ::boost::filesystem::create_directories( srcFolder );
    savePatientDB(xmlfile,_pPatient);

    // Zip
    ::fwZip::ZipFolder::NewSptr zip;
    ::fwGui::dialog::ProgressDialog progress("Saving patient");
    zip->addHandler( progress );
    zip->packFolder( srcFolder, inrFileDir );

    // Remove temp folder
    ::boost::filesystem::remove_all( srcFolder );
}

//------------------------------------------------------------------------------
} // namespace ioXML
