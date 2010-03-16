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
#include "ioQt/ImageReaderService.hpp"

#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QApplication>
#include <QWidget>
#include <QFrame>
  
  
#include <iostream>

namespace ioQt
{

//------------------------------------------------------------------------------

// Register a new reader of ::data::Image
REGISTER_SERVICE( ::io::IReader , ::ioQt::ImageReaderService , ::fwData::Image );

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
     std::cout<<" configuring() \n";

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
   std::cout<<" configuringWITHIHM() \n";

  QWidget *mainWidget = qApp->activeWindow();
    
  QString fileName = QFileDialog::getOpenFileName(mainWidget,QObject::tr("Open File"), QDir::currentPath());
  std::cout<<"PATH_FILE : "<<fileName.toStdString()<<"\n";
  
  if (!fileName.isEmpty()) 
  {
    m_bServiceIsConfigured = true;

    QImage image(fileName);
    if (image.isNull()) 
    {
       std::cout<<" Image opening FAILED \n";
     // QMessageBox::information(this, tr("Image Viewer"),tr("Cannot load %1.").arg(fileName));
    }
    else
    {
      std::cout<<" Image opened \n";
    }

/*    QWidget *mainWidget = qApp->activeWindow();
    QFrame *frame = new QFrame(mainWidget);
    imageLabel = new QLabel(frame);
    imageLabel->setPixmap(QPixmap::fromImage(image));
    
    std::cout<<"Picture loaded \n";
    
     QPainter *painter = new QPainter(frame);
     painter->drawPixmap(0, 0, *imageLabel->pixmap());
frame->show();*/

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
    if(!m_bServiceIsConfigured)
    {
      configureWithIHM();
    }

}

//------------------------------------------------------------------------------
/*
bool ImageReaderService::loadImage( , ::fwData::Image::sptr _pImg )
{
    SLM_TRACE_FUNC();

    bool ok = true;

   

    return ok;
}
*/
//------------------------------------------------------------------------------

void ImageReaderService::updating( ::boost::shared_ptr< const ::fwServices::ObjectMsg > _msg ) throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    // This method does nothing
}

//------------------------------------------------------------------------------

void ImageReaderService::notificationOfDBUpdate()
{
   
}

//------------------------------------------------------------------------------


} // namespace ioVtk
