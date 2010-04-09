/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <boost/filesystem/operations.hpp>

#include <fwCore/base.hpp>

#include <fwData/Image.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwComEd/ImageMsg.hpp>

#include <io/IReader.hpp>

#include <vtkIO/ImageReader.hpp>

#include "ioQVTK/ImageReaderService.hpp"

#include <iostream>
#include <QFileDialog>
#include <QString>

REGISTER_SERVICE( ::io::IReader , ::ioQVTK::ImageReaderService , ::fwData::Image );


namespace ioQVTK
{

//------------------------------------------------------------------------------

// Register a new reader of ::data::Image

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
    
       std::cout<<"\n\nQVTK Configuring() \n";

    
    // Test if in the service configuration the tag filename is defined. If it is defined, the image path is initialized and we tag the service as configured.
  /*  if( m_configuration->findConfigurationElement("filename") )
    {	
        std::string filename = m_configuration->findConfigurationElement("filename")->getExistingAttributeValue("id") ;
	std::cout<<" QVTK Configuring() ---> FileName : "<<filename<<"\n";
        m_fsImgPath = ::boost::filesystem::path( filename) ;
	
	if(is_regular_file(m_fsImgPath))
	   std::cout<<"\n  FILE OK \n";
	else
	   std::cout<<"\n  SDVSDVSDVSDV \n";

        m_bServiceIsConfigured = ::boost::filesystem::exists(m_fsImgPath);
        OSLM_TRACE("Filename found in service configuration : img path = " << filename ) ;
    }  */
   if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getExistingAttributeValue("id") ;
        m_fsImgPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = ::boost::filesystem::exists(m_fsImgPath);
        OSLM_TRACE("Filename found in service configuration : img path = " << filename ) ;
    }
}


void ImageReaderService::configureWithIHM()
{
    SLM_TRACE_FUNC();
  
       std::cout<<"\n\nQVTK WithIHM() \n";
    
    
    QString file = QFileDialog::getOpenFileName(0,QObject::tr("Open File"), QDir::currentPath(), QObject::tr("Images (*.vtk *.VTK )"));
    std::cout<<"PATH_FILE : "<<file.toStdString()<<"\n";
    
    
    // If the user choose an vtk file, the image path is initialized and we tag the service as configured.
    if( file.isEmpty() == false )
    {
        m_fsImgPath = ::boost::filesystem::path( file.toStdString(), ::boost::filesystem::native );
        m_bServiceIsConfigured = true;
      //  _sDefaultPath = wxConvertMB2WX( m_fsImgPath.branch_path().string().c_str() );
    }
}



//------------------------------------------------------------------------------

void ImageReaderService::starting() throw ( ::fwTools::Failed )
{
     std::cout<<"\n\nQVTK STARTING() \n";

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



void ImageReaderService::updating() throw ( ::fwTools::Failed )
{
      SLM_TRACE_FUNC();
  
 
    if( !m_bServiceIsConfigured )
    {
       configureWithIHM();
    }
    if( m_bServiceIsConfigured )
    {
      ::fwData::Image::sptr pImage = this->getObject< ::fwData::Image >() ;
        assert(pImage);
      if ( loadImage( m_fsImgPath, pImage ) )
      {
	   notificationOfDBUpdate();
      }
    }
    else
      	   std::cout<<"\n\n NOT CONFIGURED \n";

      
}


//------------------------------------------------------------------------------
/*
void ImageReaderService::updating() throw ( ::fwTools::Failed )
{
      SLM_TRACE_FUNC();
  
   std::cout<<"\n\nQVTK Updating() \n";

    if( m_bServiceIsConfigured )
    {

        // Retrieve dataStruct associated with this service
        ::fwData::Image::sptr pImage = this->getObject< ::fwData::Image >() ;
        assert(pImage);
      if ( loadImage( m_fsImgPath, pImage ) )
      {
	   std::cout<<"\n\n           QVTK NOTIFY \n";

	   notificationOfDBUpdate();
      }
    }
    else
    {
       configureWithIHM();
       ::fwData::Image::sptr pImage = this->getObject< ::fwData::Image >() ;
        assert(pImage);
      if ( loadImage( m_fsImgPath, pImage ) )
      {
	   std::cout<<"\n\n           QVTK NOTIFY \n";

	   notificationOfDBUpdate();
      }
    }
  
}
*/
//------------------------------------------------------------------------------

bool ImageReaderService::loadImage( const ::boost::filesystem::path vtkFile, ::fwData::Image::sptr _pImg )
{
    SLM_TRACE_FUNC();

       std::cout<<"\n\nQVTK LOADING() \n";

    
    bool ok = true;

    // Use a reader of vtkIO library to read a vtk image
    ::vtkIO::ImageReader myReader;
    // Set the image (already created, but empty) that will be modifed
    myReader.setObject(_pImg);
    // Set the file system path
    std::cout<<" QVTK LOADING()-->setFile() \n";
    myReader.setFile(vtkFile);
    std::cout<<" QVTK LOADING()-->read() \n";
    myReader.read();

     std::cout<<" QVTK LOADING()-->return \n";


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
