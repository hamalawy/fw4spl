/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>

#include <fwData/Image.hpp>

#include <fwComEd/ImageMsg.hpp>

#include <fwServices/Base.hpp>

#include <QVTKWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTextSource.h>
#include <vtkVectorText.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

#include <vtkIO/vtk.hpp>

#include "qvtkSimpleNegato/RendererService.hpp"

REGISTER_SERVICE( ::fwQtRender::IRender , ::qvtkSimpleNegato::RendererService , ::fwData::Image) ;


namespace qvtkSimpleNegato
{

RendererService::RendererService() throw()
    : m_render( 0 ), m_bPipelineIsInit(false)
{
    SLM_TRACE_FUNC();
    
    addNewHandledEvent( ::fwComEd::ImageMsg::NEW_IMAGE );
    addNewHandledEvent( ::fwComEd::ImageMsg::BUFFER );
    addNewHandledEvent( ::fwComEd::ImageMsg::REGION  );
    addNewHandledEvent( ::fwComEd::ImageMsg::SPACING );
    addNewHandledEvent( ::fwComEd::ImageMsg::PIXELTYPE );

}

//-----------------------------------------------------------------------------

RendererService::~RendererService() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void RendererService::configuring() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void RendererService::starting() throw(fwTools::Failed)
{
  std::cout<<"\n\n qvtkSimpleNegato ===> STARTING() \n";
  // this->initRender();

    m_bPipelineIsInit = false;

    QWidget *mainWidget = m_globalUIDToQtContainer.find(this->getUUID())->second;
    widget = new QVTKWidget(mainWidget);
    
    QVBoxLayout *layout = new QVBoxLayout();

    widget->resize(mainWidget->width(), mainWidget->height());
    widget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    
    layout->addWidget(widget);

    mainWidget->setLayout(layout);

    m_render = vtkRenderer::New();
   // renderWindow = vtkRenderWindow::New();
    renderWindow = widget->GetRenderWindow();
    renderWindow->AddRenderer(m_render);

 //   widget->SetRenderWindow(renderWindow);

 
}

//-----------------------------------------------------------------------------

void RendererService::stopping() throw(fwTools::Failed)
{/*
    if( m_render == 0 ) return;

    if( m_wxmanager == 0 ) return;

    if(m_bPipelineIsInit)
    {
        m_negatoSagittal->Delete();
        m_negatoFrontal->Delete();
        m_negatoAxial->Delete();
        m_outline->Delete();
    }

    assert( m_interactor );
    m_interactor->Delete();
    m_interactor = 0;

    assert( m_wxmanager );
    m_wxmanager->UnInit();
    delete m_wxmanager;
    m_wxmanager = 0;

    m_container->DestroyChildren() ;

    assert( m_render );
    m_render->Delete();
    m_render = 0;

    this->stopRender();*/
}

//-----------------------------------------------------------------------------

void RendererService::updating() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

//    renderWindow->Render();
}

//-----------------------------------------------------------------------------

void RendererService::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{
      std::cout<<"\n\n qvtkSimpleNegato ===> UPDATING(ARGS) \n";

    // If message is a ImageMsg
    ::fwComEd::ImageMsg::csptr pImageMsg = ::fwComEd::ImageMsg::dynamicConstCast( _msg ) ;
    if (pImageMsg)
    {
        if( pImageMsg->hasEvent( ::fwComEd::ImageMsg::NEW_IMAGE ) || pImageMsg->hasEvent( ::fwComEd::ImageMsg::BUFFER ))
        {
            if(!m_bPipelineIsInit)
            {
                initVTKPipeline();
                m_bPipelineIsInit = true;
            }
            else
            {
                updateVTKPipeline();
            }

            ::fwData::Image::sptr img = this->getObject< ::fwData::Image >();

            //
            unsigned int axialIndex = img->getSize()[2]/2;
            unsigned int frontalIndex = img->getSize()[1]/2;
            unsigned int sagittalIndex = img->getSize()[0]/2;

            m_negatoAxial->SetSliceIndex( axialIndex );
            m_negatoFrontal->SetSliceIndex( frontalIndex );
            m_negatoSagittal->SetSliceIndex( sagittalIndex );
	    
	    renderWindow->Render();
           // m_interactor->Render();
        }
    }
}

//-----------------------------------------------------------------------------

void RendererService::initVTKPipeline()
{
    SLM_TRACE_FUNC();
    
    vtkImageData* vtk_img = ::vtkIO::toVTKImage( this->getObject< ::fwData::Image >());
    
    m_outline = vtkOutlineFilter::New();
    m_outline->SetInput(vtk_img);

    vtkPolyDataMapper* outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(m_outline->GetOutputPort());

    vtkActor* outlineActor =  vtkActor::New();
    outlineActor->SetMapper( outlineMapper);

    vtkCellPicker* picker = vtkCellPicker::New();
    picker->SetTolerance(0.005);

    //assign default props to the ipw's texture plane actor
    

    m_negatoSagittal = vtkImagePlaneWidget::New();
//    m_negatoSagittal->SetInteractor( m_interactor);

    SLM_TRACE("Interactor Sagittal");
    m_negatoSagittal->SetInteractor(widget->GetRenderWindow()->GetInteractor());
    m_negatoSagittal->SetKeyPressActivationValue('x');
    m_negatoSagittal->SetPicker(picker);
    m_negatoSagittal->GetPlaneProperty()->SetColor(1,0,0);
    m_negatoSagittal->TextureInterpolateOn();
    m_negatoSagittal->SetInput(vtk_img);
    m_negatoSagittal->SetPlaneOrientationToXAxes();
    m_negatoSagittal->DisplayTextOn();
    m_negatoSagittal->On();
    m_negatoSagittal->InteractionOn();

    m_negatoFrontal = vtkImagePlaneWidget::New();
    //m_negatoFrontal->SetInteractor( m_interactor);
    SLM_TRACE("Interactor Sagittal");
    m_negatoFrontal->SetInteractor(widget->GetRenderWindow()->GetInteractor());
    m_negatoFrontal->SetKeyPressActivationValue('y');
    m_negatoFrontal->SetPicker(picker);
    m_negatoFrontal->GetPlaneProperty()->SetColor(0,1,0);
    m_negatoFrontal->TextureInterpolateOn();
    m_negatoFrontal->SetInput(vtk_img);
    m_negatoFrontal->SetPlaneOrientationToYAxes();
    m_negatoFrontal->SetLookupTable( m_negatoSagittal->GetLookupTable());
    m_negatoFrontal->DisplayTextOn();
    m_negatoFrontal->UpdatePlacement();
    m_negatoFrontal->On();

    m_negatoAxial = vtkImagePlaneWidget::New();
    //m_negatoAxial->SetInteractor( m_interactor);
    m_negatoAxial->SetInteractor(widget->GetRenderWindow()->GetInteractor());
    m_negatoAxial->SetKeyPressActivationValue('z');
    m_negatoAxial->SetPicker(picker);
    m_negatoAxial->GetPlaneProperty()->SetColor(0,0,1);
    m_negatoAxial->TextureInterpolateOn();
    m_negatoAxial->SetInput(vtk_img);
    m_negatoAxial->SetPlaneOrientationToZAxes();
    m_negatoAxial->SetLookupTable( m_negatoSagittal->GetLookupTable());
    m_negatoAxial->DisplayTextOn();
    m_negatoAxial->On();

    // Add the actors
    m_render->AddActor( outlineActor);

    // Repaint and resize window
   // m_wxmanager->Update();
    m_render->ResetCamera();

    picker->Delete();
    outlineActor->Delete();
    outlineMapper->Delete();
}

//-----------------------------------------------------------------------------

void RendererService::updateVTKPipeline()
{
    assert(this->getObject< ::fwData::Image >());
    vtkImageData* vtk_img = ::vtkIO::toVTKImage( this->getObject< ::fwData::Image >());

    m_outline->SetInput(vtk_img);
    m_negatoSagittal->SetInput(vtk_img);
    m_negatoFrontal->SetInput(vtk_img);
    m_negatoAxial->SetInput(vtk_img);
}

}
