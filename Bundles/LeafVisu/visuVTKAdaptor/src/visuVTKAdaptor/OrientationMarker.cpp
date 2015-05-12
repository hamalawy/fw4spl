/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <fwServices/macros.hpp>

#include <vtkCubeSource.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>



#include "visuVTKAdaptor/OrientationMarker.hpp"



fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::OrientationMarker, ::fwData::Object );

namespace visuVTKAdaptor
{

OrientationMarker::OrientationMarker() : m_hAlign("left")
{
}

//-----------------------------------------------------------------------------

void OrientationMarker::doStart() throw(fwTools::Failed)
{
    vtkSmartPointer< vtkGenericDataObjectReader > reader = vtkSmartPointer< vtkGenericDataObjectReader >::New();
    reader->SetFileName("Bundles/visuVTKAdaptor_0-1/human.vtk");
    reader->Update();
    vtkDataObject *obj = reader->GetOutput();
    vtkPolyData* mesh  = vtkPolyData::SafeDownCast(obj);

    if(!obj)
    {
        SLM_WARN("Orientation marker load failed ");
    }

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(mesh);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
    widget->SetOrientationMarker( actor );
    widget->SetInteractor( this->getInteractor() );

    if(m_hAlign == "left")
    {
        widget->SetViewport( 0.0, 0.0, 0.1, 0.1 );
    }
    else if(m_hAlign == "right")
    {
        widget->SetViewport( 0.9, 0.0, 1, 0.1 );
    }

    widget->SetEnabled( 1 );
    widget->InteractiveOff();
    this->setVtkPipelineModified();
}

//-----------------------------------------------------------------------------

void OrientationMarker::doStop() throw(fwTools::Failed)
{
    this->removeAllPropFromRenderer();
}

//-----------------------------------------------------------------------------

void OrientationMarker::configuring() throw(fwTools::Failed)
{
    if(m_configuration->hasAttribute("hAlign"))
    {
        m_hAlign = m_configuration->getAttributeValue("hAlign");
        SLM_ASSERT("'hAlign' value must be 'left', 'center' or 'right'",
                   m_hAlign == "left"
                   || m_hAlign == "right"
                   );
    }
}


} //namespace visuVTKAdaptor
