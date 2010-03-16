/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QMessageBox>
#include <fwComEd/SingleFileMsg.hpp>
#include <fwData/location/SingleFile.hpp>  

#include <fwServices/Base.hpp>

#include "uiQt/ImageEditor.hpp"

REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiQt::ImageEditor , ::fwData::location::SingleFile) ;

   
namespace uiQt
{

ImageEditor::ImageEditor() throw()
{
    SLM_TRACE_FUNC();
   
    addNewHandledEvent( ::fwComEd::SingleFileMsg::NEW_SINGLE_FILE );
}

//-----------------------------------------------------------------------------

ImageEditor::~ImageEditor() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ImageEditor::configuring() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ImageEditor::starting() throw(fwTools::Failed)
{
    QWidget* container;
    QFrame *f;
   ::guiQt::editor::IEditor::starting();
   std::cout<<"Servie UUID : "<<this->getUUID()<<"\n";

  container = m_globalUIDToQtContainer[this->getUUID()];
  
  /*
  QList< QFrame*> frame =  container->findChildren<QFrame *>();
  
  if(container!=0)
   std::cout<<" \n =====> CONTAINER OK \n";
  else
   std::cout<<" \n =====> CONTAINER NUL \n";
  
  
  
  if(!frame.isEmpty())
  {
     std::cout<<" \n =====>FRAME : List non vide \n";
     f = frame.first();
  }
   else
   {
    // SLM_FATAL(" F empty ");
   }
   */

 // imageLabel = new QLabel(frame);
   imageLabel = new QLabel(container);

  (container->layout())->addWidget(imageLabel);
}

//-----------------------------------------------------------------------------

void ImageEditor::stopping() throw(fwTools::Failed)
{
  ::guiQt::editor::IEditor::stopping();

}

//-----------------------------------------------------------------------------

void ImageEditor::updating() throw(fwTools::Failed)
{
  // Recuperation de l'objet
    ::fwData::location::SingleFile::sptr file = this->getObject < ::fwData::location::SingleFile >();
    OSLM_INFO("FileMsg : " << file->getPath());
    
    image = new QImage(file->getPath().string().c_str());
    
     if (image->isNull()) 
     {
	//QMessageBox::information(this, QObject::tr("Image Viewer"),QObject::tr("Cannot load %1.").arg(file->getPath().string()));
        return;
      }
      imageLabel->setPixmap(QPixmap::fromImage(*image));
      imageLabel->show();  

}

//-----------------------------------------------------------------------------

void ImageEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{
  
  
  ::fwComEd::SingleFileMsg::csptr fileMsg =  ::fwComEd::SingleFileMsg::dynamicConstCast( _msg );
  
    if ( fileMsg && fileMsg->hasEvent( ::fwComEd::SingleFileMsg::NEW_SINGLE_FILE ) )
    {
        this->updating();
    }
   
}

//-----------------------------------------------------------------------------

}
