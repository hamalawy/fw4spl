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

#include <io/IWriter.hpp>

#include <fwCore/base.hpp>

#include <fwTools/System.hpp>

#include <fwData/Acquisition.hpp>
#include <fwData/location/Folder.hpp>

#include <fwXML/writer/FwXMLObjectWriter.hpp>
#include <fwXML/writer/fwxmlextension.hpp>

#include <fwGui/dialog/ProgressDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwZip/ZipFolder.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/Cursor.hpp>

#include "ioXML/FwXMLGenericWriterService.hpp"

namespace ioXML
{

fwServicesRegisterMacro( ::io::IWriter , ::ioXML::FwXMLGenericWriterService , ::fwData::Object ) ;

//------------------------------------------------------------------------------

FwXMLGenericWriterService::FwXMLGenericWriterService() throw()
                : m_archiveExtenstion ("." FWXML_ARCHIVE_EXTENSION)
{}

//------------------------------------------------------------------------------

FwXMLGenericWriterService::~FwXMLGenericWriterService() throw()
{}

//------------------------------------------------------------------------------

void FwXMLGenericWriterService::configuring() throw(::fwTools::Failed)
{
    ::io::IWriter::configuring();


    typedef std::vector < SPTR(::fwRuntime::ConfigurationElement) >  ConfigurationElementContainer;
    ConfigurationElementContainer extension = m_configuration->find("archiveExtension");

    SLM_ASSERT("The configuration accepts at most one <archiveExtension> and/or one <filename> element.", extension.size() <= 1 );

    if( extension.size() > 0 )
    {
        ConfigurationElementContainer::iterator iter = extension.begin() ;
        SLM_ASSERT("The <"<< (*iter)->getName() <<"> element can be set at most once.", extension.size() == 1 );
        SLM_ASSERT("The <"<< (*iter)->getName() <<"> element value can not be empty.", !(*iter)->getValue().empty() );
        m_archiveExtenstion =  (*iter)->getValue();
    }

}

//------------------------------------------------------------------------------

::io::IOPathType FwXMLGenericWriterService::getIOPathType() const
{
    return ::io::FILE;
}

//------------------------------------------------------------------------------

void FwXMLGenericWriterService::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    std::stringstream sstrTitle;
    sstrTitle << "Choose a " << m_archiveExtenstion.substr(1,m_archiveExtenstion.size()-1) << " or a xml file";
    dialogFile.setTitle( sstrTitle.str() );
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    std::stringstream archExt;
    archExt << "*" << m_archiveExtenstion;
    dialogFile.addFilter("fwXML compressed archive", archExt.str() );
    dialogFile.addFilter("fwXML archive","*.xml");
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);

    ::fwData::location::SingleFile::sptr  result;
    result= ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath = result->getPath().parent_path();
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
        this->setFile(result->getPath());
    }
    else
    {
        this->clearLocations();
    }
}

//------------------------------------------------------------------------------

void FwXMLGenericWriterService::setArchiveExtension( const std::string & _archiveExtenstion )
{
    m_archiveExtenstion = _archiveExtenstion;
}

//------------------------------------------------------------------------------

void FwXMLGenericWriterService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void FwXMLGenericWriterService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void FwXMLGenericWriterService::info(std::ostream &_sstream )
{
    _sstream << "FwXMLGenericWriterService::info" ;
}

//------------------------------------------------------------------------------

void FwXMLGenericWriterService::saveData( const ::boost::filesystem::path path, ::fwTools::Object::sptr _obj )
{
    SLM_TRACE_FUNC();
    ::fwXML::writer::FwXMLObjectWriter::NewSptr myWriter;

    myWriter->setObject(_obj);
    myWriter->setFile(path);

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Saving data ");
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

void FwXMLGenericWriterService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    if( this->hasLocationDefined() )
    {
        // Retrieve dataStruct associated with this service
        ::fwData::Object::sptr obj = this->getObject();
        SLM_ASSERT("data is null", obj);

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);

        ::boost::filesystem::path path = correctFileFormat( this->getFile() );
        if ( isAnFwxmlArchive( path ) )
        {
            manageZipAndSaveData( path, obj);
        }
        else
        {
            saveData(this->getFile(), obj);
        }
        cursor.setDefaultCursor();
    }
}

//------------------------------------------------------------------------------

::boost::filesystem::path FwXMLGenericWriterService::correctFileFormat( const ::boost::filesystem::path _filePath ) const
{
    ::boost::filesystem::path newPath = _filePath;
    if ( ::boost::filesystem::extension(_filePath) != m_archiveExtenstion && ::boost::filesystem::extension(_filePath) != ".xml" )
    {
        newPath = _filePath.string() + m_archiveExtenstion;
    }

    return newPath;
}

//------------------------------------------------------------------------------

bool FwXMLGenericWriterService::isAnFwxmlArchive( const ::boost::filesystem::path filePath )
{
    return ( ::boost::filesystem::extension(filePath) == m_archiveExtenstion );
}

//------------------------------------------------------------------------------

void FwXMLGenericWriterService::manageZipAndSaveData( const ::boost::filesystem::path path, ::fwTools::Object::sptr _obj )
{
    // Save in tmp folder
    ::boost::filesystem::path srcFolder = ::fwTools::System::getTemporaryFolder() / "fwxmlArchiveFolder";
    ::boost::filesystem::path xmlfile = srcFolder / "root.xml";
    ::boost::filesystem::create_directories( srcFolder );
    saveData(xmlfile,_obj);

    // Zip
    ::fwZip::ZipFolder::NewSptr zip;
    ::fwGui::dialog::ProgressDialog progress("Saving");
    zip->addHandler( progress );
    zip->packFolder( srcFolder, path );

    // Remove temp folder
    ::boost::filesystem::remove_all( srcFolder );
}

//------------------------------------------------------------------------------

} // namespace ioXML
