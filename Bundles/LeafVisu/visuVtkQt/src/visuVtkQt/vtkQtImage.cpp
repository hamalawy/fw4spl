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

#include <fwData/Image.hpp>


#include "visuVtkQt/vtkQtImage.hpp"
#include <QLayout>
#include <QVBoxLayout>	


#include <QVTKWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTextSource.h>
#include <vtkVectorText.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

REGISTER_SERVICE( ::guiQt::editor::IEditor  , ::visuVtkQt::vtkQtImage, ::fwData::location::SingleFile);
//REGISTER_SERVICE( ::guiQt::editor::IEditor  , ::visuVtkQt::vtkQtImage, ::fwData::Image);  et dans plugin.xml

   
namespace visuVtkQt
{

vtkQtImage::vtkQtImage() throw()
{
    SLM_TRACE_FUNC();
//       addNewHandledEvent( ::fwComEd::LocationMsg::LOCATION_IS_MODIFIED );

}

//-----------------------------------------------------------------------------

vtkQtImage::~vtkQtImage() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void vtkQtImage::configuring() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void vtkQtImage::starting() throw(fwTools::Failed)
{
  QWidget *mainWidget =  m_globalUIDToQtContainer.find(this->getUUID())->second;

  
 QVTKWidget *widget = new QVTKWidget(mainWidget);
 widget->resize(mainWidget->width(), mainWidget->height());

 vtkTextSource *text = vtkTextSource::New();
  text->SetText("Hello World!");
  text->BackingOff();
 vtkVectorText *vectorText = vtkVectorText::New();
  vectorText->SetText("QVTKWidget");
 vtkPolyDataMapper *textMapper = vtkPolyDataMapper::New();
  textMapper->SetInput(text->GetOutput());
 vtkPolyDataMapper *vectorTextMapper = vtkPolyDataMapper::New();
  vectorTextMapper->SetInput(vectorText->GetOutput());
 vtkActor *textActor = vtkActor::New();
  textActor->SetMapper(textMapper);
 vtkActor *vectorTextActor = vtkActor::New();
  vectorTextActor->SetMapper(vectorTextMapper);
 vtkRenderer *renderer = vtkRenderer::New();
  renderer->SetBackground(0.4,0.6,0.8);
  renderer->AddActor(textActor);
  renderer->AddActor(vectorTextActor);
 vtkRenderWindow *renderWindow = vtkRenderWindow::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetStereoTypeToDresden();

 widget->SetRenderWindow(renderWindow);
//widget->show();


}

//-----------------------------------------------------------------------------

void vtkQtImage::stopping() throw(fwTools::Failed)
{
  ::guiQt::editor::IEditor::stopping();

}

//-----------------------------------------------------------------------------

void vtkQtImage::updating() throw(fwTools::Failed)
{

}

//-----------------------------------------------------------------------------

void vtkQtImage::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{
  
/*     
  ::fwComEd::LocationMsg::csptr fileMsg =  ::fwComEd::LocationMsg::dynamicConstCast( _msg );
  
    if ( fileMsg && fileMsg->hasEvent( ::fwComEd::LocationMsg::LOCATION_IS_MODIFIED ) )
    {
        this->updating();
    }
    */
}

//-----------------------------------------------------------------------------

}
