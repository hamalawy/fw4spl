/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwComEd/ImageMsg.hpp>

#include <io/IReader.hpp>

#include <fwCore/base.hpp>

#include <fwData/Image.hpp>
#include <fwData/location/Folder.hpp>

#include <fwXML/reader/FwXMLObjectReader.hpp>
#include <fwXML/writer/fwxmlextension.hpp>

#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/dialog/ProgressDialog.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/Cursor.hpp>

#include "ioXML/FwXMLImageReaderService.hpp"


namespace ioXML
{

REGISTER_SERVICE( ::io::IReader , ::ioXML::FwXMLImageReaderService , ::fwData::Image ) ;

//------------------------------------------------------------------------------

FwXMLImageReaderService::FwXMLImageReaderService() throw() :
    m_bServiceIsConfigured(false),
    m_fsImagePath("")
{}

//------------------------------------------------------------------------------

FwXMLImageReaderService::~FwXMLImageReaderService() throw()
{}

//------------------------------------------------------------------------------

void FwXMLImageReaderService::configuring() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    // Test if in the service configuration the tag filename is defined. If it is defined, the image path is initialized and we tag the service as configured.
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getExistingAttributeValue("id") ;
        m_fsImagePath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = ::boost::filesystem::exists(m_fsImagePath);
        OSLM_TRACE("Filename found in service configuration : img path = " << filename ) ;
    }
}

//------------------------------------------------------------------------------

void FwXMLImageReaderService::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle( "Choose a " FWXML_ARCHIVE_EXTENSION " or a xml file" );
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.addFilter("fwXML archive","*." FWXML_ARCHIVE_EXTENSION);
    dialogFile.addFilter("fwXML archive","*.xml");
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::READ);
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::FILE_MUST_EXIST);

    ::fwData::location::SingleFile::sptr  result;
    result= ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath = result->getPath().parent_path();
        m_fsImagePath = result->getPath();
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
        m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void FwXMLImageReaderService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void FwXMLImageReaderService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void FwXMLImageReaderService::info(std::ostream &_sstream )
{
    _sstream << "FwXMLImageReaderService::info" ;
}

//------------------------------------------------------------------------------

::fwData::Image::sptr FwXMLImageReaderService::createImage( const ::boost::filesystem::path inrFileDir )
{
    SLM_TRACE_FUNC();
    ::fwXML::reader::FwXMLObjectReader myLoader;
    ::fwData::Image::sptr pImage;
    myLoader.setFile(inrFileDir);
    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Loading Image ");
        myLoader.addHandler( progressMeterGUI );
        myLoader.read();
        pImage = ::fwData::Image::dynamicCast( myLoader.getObject() );
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during loading : " << e.what();
        ::fwGui::dialog::MessageDialog::showMessageDialog("Warning", ss.str(), ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        std::stringstream xmlFile;
        xmlFile << "Sorry, the xml file \""
                << m_fsImagePath.string()
                << "\" does not content a Image. This xml file has not been loaded.";

        ::fwGui::dialog::MessageDialog::showMessageDialog("FwXML Image Reader",
                xmlFile.str(),
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
    return pImage;
}

//------------------------------------------------------------------------------

void FwXMLImageReaderService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    if( m_bServiceIsConfigured )
    {
        ::fwData::Image::sptr image = createImage( m_fsImagePath );

        if ( image )
        {
            // Retrieve dataStruct associated with this service
            ::fwData::Image::sptr associatedImage = this->getObject< ::fwData::Image >();
            SLM_ASSERT("associatedImage not instanced", associatedImage);

            associatedImage->shallowCopy( image );

            ::fwGui::Cursor cursor;
            cursor.setCursor(::fwGui::ICursor::BUSY);

            notificationOfDBUpdate();
            cursor.setDefaultCursor();
        }
    }
}

//------------------------------------------------------------------------------

void FwXMLImageReaderService::notificationOfDBUpdate()
{
    SLM_TRACE_FUNC();
    ::fwData::Image::sptr pImage = this->getObject< ::fwData::Image >();
    SLM_ASSERT("pImage not instanced", pImage);

    ::fwComEd::ImageMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::ImageMsg::NEW_IMAGE ) ;
    msg->addEvent( ::fwComEd::ImageMsg::BUFFER ) ;
    msg->addEvent( ::fwComEd::ImageMsg::REGION ) ;
    msg->addEvent( ::fwComEd::ImageMsg::SPACING ) ;
    msg->addEvent( ::fwComEd::ImageMsg::PIXELTYPE ) ;

    ::fwServices::IEditionService::notify(this->getSptr(),  pImage, msg);
}

//------------------------------------------------------------------------------

} // namespace ioXML
