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
#include <vtkSmartPointer.h>

#include <fwData/Image.hpp>

#include <fwComEd/ImageMsg.hpp>
#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/IEditionService.hpp>

#include <vtkIO/vtk.hpp>

#include "vtkSimpleNegato/RendererService.hpp"

//-----------------------------------------------------------------------------

fwServicesRegisterMacro( ::fwRender::IRender , ::vtkSimpleNegato::RendererService , ::fwData::Image) ;

//-----------------------------------------------------------------------------

namespace vtkSimpleNegato
{

//-----------------------------------------------------------------------------

RendererService::RendererService() throw()
    : m_render( 0 ), m_bPipelineIsInit(false)
{
    SLM_TRACE_FUNC();
    this->addNewHandledEvent(::fwComEd::ImageMsg::NEW_IMAGE );
    this->addNewHandledEvent(::fwComEd::ImageMsg::BUFFER );
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
    this->initialize();
}

//-----------------------------------------------------------------------------

void RendererService::starting() throw(fwTools::Failed)
{
    this->create();

    m_interactorManager = ::fwRenderVTK::IVtkRenderWindowInteractorManager::createManager();
    m_interactorManager->installInteractor( this->getContainer() );

    m_bPipelineIsInit = false;

    // Renderer
    m_render = vtkRenderer::New();
    m_interactorManager->getInteractor()->GetRenderWindow()->AddRenderer(m_render);

}

//-----------------------------------------------------------------------------

void RendererService::stopping() throw(fwTools::Failed)
{
    if( m_render == 0 ) return;

    if(m_bPipelineIsInit)
    {
        m_negatoSagittal->Delete();
        m_negatoFrontal->Delete();
        m_negatoAxial->Delete();
        m_outline->Delete();
    }

    SLM_ASSERT("m_render not instanced", m_render);
    m_render->Delete();
    m_render = 0;

    m_interactorManager->uninstallInteractor();
    m_interactorManager.reset();

//    this->getContainer()->clean();
    this->destroy();
}

//-----------------------------------------------------------------------------

void RendererService::updating() throw(fwTools::Failed)
{

//    m_interactorManager->getInteractor()->Render();
    refresh();
}

//-----------------------------------------------------------------------------

void RendererService::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{
    // If message is a ImageMsg
    ::fwComEd::ImageMsg::csptr pImageMsg = ::fwComEd::ImageMsg::dynamicConstCast( _msg ) ;
    if (pImageMsg)
    {
        if( pImageMsg->hasEvent( ::fwComEd::ImageMsg::NEW_IMAGE ) || pImageMsg->hasEvent( ::fwComEd::ImageMsg::BUFFER ))
        {
            refresh();
        }
    }
}

//-----------------------------------------------------------------------------

void RendererService::refresh()
{
    ::fwData::Image::sptr img = this->getObject< ::fwData::Image >();
    bool imageIsValid = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity( img );
    if(imageIsValid )
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


        //
        unsigned int axialIndex = img->getSize()[2]/2;
        unsigned int frontalIndex = img->getSize()[1]/2;
        unsigned int sagittalIndex = img->getSize()[0]/2;

        m_negatoAxial->SetSliceIndex( axialIndex );
        m_negatoFrontal->SetSliceIndex( frontalIndex );
        m_negatoSagittal->SetSliceIndex( sagittalIndex );
        m_interactorManager->getInteractor()->Render();
    }
}

//-----------------------------------------------------------------------------

void RendererService::initVTKPipeline()
{
    vtkSmartPointer< vtkImageData > vtk_img = vtkSmartPointer< vtkImageData >::New();
    ::vtkIO::toVTKImage( this->getObject< ::fwData::Image >(), vtk_img);

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
    m_negatoSagittal->SetInteractor( m_interactorManager->getInteractor() );
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
    m_negatoFrontal->SetInteractor( m_interactorManager->getInteractor() );
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
    m_negatoAxial->SetInteractor( m_interactorManager->getInteractor() );
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
    m_render->ResetCamera();

    picker->Delete();
    outlineActor->Delete();
    outlineMapper->Delete();
}

//-----------------------------------------------------------------------------

void RendererService::updateVTKPipeline()
{
    assert(this->getObject< ::fwData::Image >());
    vtkSmartPointer< vtkImageData > vtk_img = vtkSmartPointer< vtkImageData >::New();
    ::vtkIO::toVTKImage( this->getObject< ::fwData::Image >(), vtk_img);

    m_outline->SetInput(vtk_img);
    m_negatoSagittal->SetInput(vtk_img);
    m_negatoFrontal->SetInput(vtk_img);
    m_negatoAxial->SetInput(vtk_img);
}

//-----------------------------------------------------------------------------

}
