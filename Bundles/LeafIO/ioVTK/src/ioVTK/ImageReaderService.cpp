/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
#include <boost/filesystem/operations.hpp>

#include <fwCore/base.hpp>

#include <fwData/Image.hpp>
#include <fwData/location/Folder.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwComEd/ImageMsg.hpp>

#include <io/IReader.hpp>

#include <fwGui/ProgressDialog.hpp>
#include <vtkIO/ImageReader.hpp>

#include <fwGui/MessageDialog.hpp>
#include <fwGui/LocationDialog.hpp>
#include <fwGui/Cursor.hpp>

#include "ioVTK/ImageReaderService.hpp"

namespace ioVTK
{

//------------------------------------------------------------------------------

// Register a new reader of ::data::Image
REGISTER_SERVICE( ::io::IReader , ::ioVTK::ImageReaderService , ::fwData::Image );

//------------------------------------------------------------------------------

ImageReaderService::ImageReaderService() throw() :
    m_bServiceIsConfigured(false),
    m_fsImgPath("")
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

ImageReaderService::~ImageReaderService() throw()
{
    SLM_TRACE_FUNC();
    // This method does nothing
}

//------------------------------------------------------------------------------

void ImageReaderService::configuring() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    // Test if in the service configuration the tag filename is defined. If it is defined, the image path is initialized and we tag the service as configured.
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getExistingAttributeValue("id") ;
        m_fsImgPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = ::boost::filesystem::exists(m_fsImgPath);
        OSLM_TRACE("Filename found in service configuration : img path = " << filename ) ;
    }
}

//------------------------------------------------------------------------------

void ImageReaderService::configureWithIHM()
{
    SLM_TRACE_FUNC();
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::LocationDialog dialogFile;
    dialogFile.setTitle("Choose an vtk file to load an image");
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.addFilter("Vtk","*.vtk");

    ::fwData::location::SingleFile::sptr  result;
    result= ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath = result->getPath();
        m_fsImgPath = result->getPath();
        m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void ImageReaderService::starting() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    // This method does nothing
}

//------------------------------------------------------------------------------

void ImageReaderService::stopping() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    // This method does nothing
}

//------------------------------------------------------------------------------

void ImageReaderService::info( std::ostream &_sstream )
{
    _sstream << "ImageReaderService::info (ToDo :))";
}

//------------------------------------------------------------------------------

void ImageReaderService::updating() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    if( m_bServiceIsConfigured )
    {
        // Retrieve dataStruct associated with this service
        ::fwData::Image::sptr pImage = this->getObject< ::fwData::Image >() ;
        assert(pImage);

        // Read new image path and update image. If the reading process is a success, we notify all listeners that image has been modified.

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);
        if ( loadImage( m_fsImgPath, pImage ) )
        {
            notificationOfDBUpdate();
        }
        cursor.setDefaultCursor();
    }
}

//------------------------------------------------------------------------------

bool ImageReaderService::loadImage( const ::boost::filesystem::path vtkFile, ::fwData::Image::sptr _pImg )
{
    SLM_TRACE_FUNC();

    bool ok = true;

    // Use a reader of vtkIO library to read a vtk image
    ::vtkIO::ImageReader myReader;
    // Set the image (already created, but empty) that will be modifed
    myReader.setObject(_pImg);
    // Set the file system path
    myReader.setFile(vtkFile);

    try
    {
        // Create a progress bar and attach it to reader
        fwGui::ProgressDialog progressMeterGUI("Loading Image ");
        myReader.addHandler( progressMeterGUI );
        // Launch reading process
        myReader.read();

    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during loading : " << e.what();

        ::fwGui::MessageDialog messageBox;
        messageBox.setTitle("Warning");
        messageBox.setMessage( ss.str() );
        messageBox.setIcon(::fwGui::IMessageDialog::WARNING);
        messageBox.addButton(::fwGui::IMessageDialog::OK);
        messageBox.show();

        ok = false;
    }
    catch( ... )
    {
        std::stringstream ss;
        ss << "Warning during loading.";

        ::fwGui::MessageDialog messageBox;
        messageBox.setTitle("Warning");
        messageBox.setMessage( ss.str() );
        messageBox.setIcon(::fwGui::IMessageDialog::WARNING);
        messageBox.addButton(::fwGui::IMessageDialog::OK);
        messageBox.show();

        ok = false;
    }

    return ok;
}

//------------------------------------------------------------------------------

void ImageReaderService::updating( ::boost::shared_ptr< const ::fwServices::ObjectMsg > _msg ) throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    // This method does nothing
}

//------------------------------------------------------------------------------

void ImageReaderService::notificationOfDBUpdate()
{
    SLM_TRACE_FUNC();
    ::fwData::Image::sptr pImage = this->getObject< ::fwData::Image >();
    assert( pImage );

    // Creation of an image message to say that image is an new image ( or all fields are modified (old version of msg ) )
    ::fwComEd::ImageMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::ImageMsg::NEW_IMAGE ) ;
    msg->addEvent( ::fwComEd::ImageMsg::BUFFER ) ;
    msg->addEvent( ::fwComEd::ImageMsg::REGION ) ;
    msg->addEvent( ::fwComEd::ImageMsg::SPACING ) ;
    msg->addEvent( ::fwComEd::ImageMsg::PIXELTYPE ) ;

    // Notify message to all service listeners
    ::fwServices::IEditionService::notify(this->getSptr(), pImage, msg);
}

//------------------------------------------------------------------------------


} // namespace ioVtk
