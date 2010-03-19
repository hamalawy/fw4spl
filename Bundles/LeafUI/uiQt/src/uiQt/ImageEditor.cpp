/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QMainWindow>
#include <QDockWidget>


#include <fwComEd/LocationMsg.hpp>
#include <fwData/location/SingleFile.hpp>  

#include <fwServices/Base.hpp>

#include "uiQt/ImageEditor.hpp"

REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiQt::ImageEditor , ::fwData::location::SingleFile) ;

   
namespace uiQt
{

ImageEditor::ImageEditor() throw()
{
    SLM_TRACE_FUNC();
   
    addNewHandledEvent( ::fwComEd::LocationMsg::LOCATION_IS_MODIFIED );
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
   
   ::guiQt::editor::IEditor::starting();
   std::cout<<"Servie UUID : "<<this->getUUID()<<"\n";

   //container = m_globalUIDToQtContainer[this->getUUID()];
   container = m_globalUIDToQtContainer.find(this->getUUID())->second;

   // important le setParent
   imageLabel = new QLabel();
   imageLabel->setParent(container);
   
 //  imageLabel->resize(container->height(), container->width());
   imageLabel->setMinimumHeight(container->height());
   imageLabel->setMinimumWidth(container->width());

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
      imageLabel->setScaledContents(true);
      imageLabel->setAlignment(Qt::AlignHCenter);
    //  imageLabel->show();  

}

//-----------------------------------------------------------------------------

void ImageEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{
  
  
  ::fwComEd::LocationMsg::csptr fileMsg =  ::fwComEd::LocationMsg::dynamicConstCast( _msg );
  
    if ( fileMsg && fileMsg->hasEvent( ::fwComEd::LocationMsg::LOCATION_IS_MODIFIED ) )
    {
        this->updating();
    }
   
}

//-----------------------------------------------------------------------------

}
