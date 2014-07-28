/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>
#include <fwComEd/CompositeMsg.hpp>
#include <fwComEd/ImageMsg.hpp>
#include <fwComEd/Dictionary.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>

#include <fwData/Image.hpp>
#include <fwData/TransferFunction.hpp>
#include <fwData/Color.hpp>
#include <fwData/String.hpp>

#include <fwVtkIO/vtk.hpp>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkImageActor.h>
#include <vtkImageBlend.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkImageMapper3D.h>

#include "visuVTKAdaptor/ImageSlice.hpp"


fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::ImageSlice, ::fwData::Composite ) ;

namespace visuVTKAdaptor
{


//------------------------------------------------------------------------------

ImageSlice::ImageSlice() throw()
{
    SLM_TRACE_FUNC();
    m_imageActor = vtkImageActor::New();

    m_imageSource = NULL;

    m_planeOutlinePolyData = vtkPolyData::New();
    m_planeOutlineMapper   = vtkPolyDataMapper::New();
    m_planeOutlineActor    = vtkActor::New();

    m_interpolation = true;

    m_actorOpacity = 1.0;
    m_useImageTF = false;

    // Manage events
    //this->addNewHandledEvent( ::fwComEd::ImageMsg::BUFFER              );
    //this->addNewHandledEvent( ::fwComEd::ImageMsg::NEW_IMAGE           );
    //this->addNewHandledEvent( ::fwComEd::ImageMsg::SLICE_INDEX         );
    //this->addNewHandledEvent( ::fwComEd::ImageMsg::CHANGE_SLICE_TYPE   );
    //this->addNewHandledEvent( "ACTOR_TRANSPARENCY");
}

//------------------------------------------------------------------------------

ImageSlice::~ImageSlice() throw()
{
    SLM_TRACE_FUNC();
    m_imageActor->Delete();
    m_imageActor = NULL;

    m_planeOutlineActor->Delete();
    m_planeOutlineActor = NULL;

    m_planeOutlineMapper->Delete();
    m_planeOutlineMapper = NULL;

    m_planeOutlinePolyData->Delete();
    m_planeOutlinePolyData = NULL;
}

//------------------------------------------------------------------------------

void ImageSlice::doStart() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->addToRenderer(m_imageActor);
    this->addToRenderer(m_planeOutlineActor);
    this->addToPicker(m_imageActor);

    m_connection = this->getCtrlImage()->signal(::fwData::Object::s_OBJECT_MODIFIED_SIG)->connect(
            this->slot(::fwServices::IService::s_RECEIVE_SLOT));
    this->doUpdate();
}

//------------------------------------------------------------------------------

void ImageSlice::doStop() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    if (!m_connection.expired())
    {
        m_connection.disconnect();
    }
    this->removeFromPicker(m_imageActor);
    this->removeAllPropFromRenderer();
}

//------------------------------------------------------------------------------

void ImageSlice::doSwap() throw(fwTools::Failed)
{
    if (!m_connection.expired())
    {
        m_connection.disconnect();
    }
    m_connection = this->getCtrlImage()->signal(::fwData::Object::s_OBJECT_MODIFIED_SIG)->connect(
            this->slot(::fwServices::IService::s_RECEIVE_SLOT));
    this->doUpdate();
}

//------------------------------------------------------------------------------

::fwData::Image::sptr ImageSlice::getCtrlImage()
{
    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >();
    if (m_ctrlImage.expired())
    {
        if (!m_ctrlImageId.empty())
        {
            m_ctrlImage.reset();
            ::fwData::Composite::iterator it = (*composite).find(m_ctrlImageId);
            if (it != (*composite).end())
            {
                m_ctrlImage = ::fwData::Image::dynamicCast((*it).second);
            }
        }
    }
    SLM_ASSERT("Null control image", !m_ctrlImage.expired());
    return m_ctrlImage.lock();
}

//------------------------------------------------------------------------------

void ImageSlice::doUpdate() throw(::fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getCtrlImage();

    bool imageIsValid = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity( image );
    if (imageIsValid)
    {
        this->buildPipeline();
        this->updateImage(image);
        this->updateSliceIndex(image);
        this->updateOutline();
    }
}

//------------------------------------------------------------------------------

void ImageSlice::doReceive(::fwServices::ObjectMsg::csptr msg) throw(::fwTools::Failed)
{
    ::fwData::Image::sptr image = m_ctrlImage.lock();
    bool imageIsValid = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity( image );

    if ( msg->hasEvent( ::fwComEd::CompositeMsg::CHANGED_KEYS )
         || msg->hasEvent( ::fwComEd::CompositeMsg::ADDED_KEYS )
         || msg->hasEvent( ::fwComEd::CompositeMsg::REMOVED_KEYS )
         )
    {
        if (m_ctrlImage.expired() || m_ctrlImage.lock() != this->getCtrlImage())
        {
            if (!m_connection.expired())
            {
                m_connection.disconnect();
            }
            m_connection = m_ctrlImage.lock()->signal(::fwData::Object::s_OBJECT_MODIFIED_SIG)->connect(
                                            this->slot(::fwServices::IService::s_RECEIVE_SLOT));
        }
        this->doUpdate();
    }

    if (imageIsValid)
    {
        if ( msg->hasEvent( ::fwComEd::ImageMsg::BUFFER ) || ( msg->hasEvent( ::fwComEd::ImageMsg::NEW_IMAGE )) )
        {
            doUpdate();
        }

        if ( msg->hasEvent( ::fwComEd::ImageMsg::SLICE_INDEX ) )
        {
            ::fwComEd::ImageMsg::dynamicConstCast(msg)->getSliceIndex( m_axialIndex, m_frontalIndex, m_sagittalIndex);
            this->updateSliceIndex(image);
            this->updateOutline();
        }

        if ( msg->hasEvent( ::fwComEd::ImageMsg::CHANGE_SLICE_TYPE ) && imageIsValid)
        {
            ::fwData::Object::csptr cObjInfo = msg->getDataInfo( ::fwComEd::ImageMsg::CHANGE_SLICE_TYPE );
            ::fwData::Object::sptr objInfo = ::boost::const_pointer_cast< ::fwData::Object > ( cObjInfo );
            ::fwData::Composite::sptr info = ::fwData::Composite::dynamicCast ( objInfo );

            int fromSliceType = ::fwData::Integer::dynamicCast( info->getContainer()["fromSliceType"] )->value();
            int toSliceType =   ::fwData::Integer::dynamicCast( info->getContainer()["toSliceType"] )->value();

            if( toSliceType == static_cast<int>(m_orientation) )
            {
                setOrientation( static_cast< Orientation >( fromSliceType ));
                this->doUpdate();
            }
            else if(fromSliceType == static_cast<int>(m_orientation))
            {
                setOrientation( static_cast< Orientation >( toSliceType ));
                this->doUpdate();
            }
        }
        if ( msg->hasEvent( "ACTOR_OPACITY" ) )
        {
            ::fwData::Integer::csptr opacity =
                    ::fwData::Integer::dynamicConstCast( msg->getDataInfo( "ACTOR_OPACITY" )) ;

            double opacityDouble = static_cast<double>(opacity->value()) / 100.;
            this->setActorOpacity(opacityDouble);
            this->doUpdate();
        }
    }
}

//------------------------------------------------------------------------------

void ImageSlice::configuring() throw(fwTools::Failed)
{

    assert(m_configuration->getName() == "config");
    this->setRenderId( m_configuration->getAttributeValue("renderer") );
    this->setPickerId( m_configuration->getAttributeValue("picker") );
    if(m_configuration->hasAttribute("sliceIndex"))
    {
         std::string  orientation = m_configuration->getAttributeValue("sliceIndex");
         if(orientation == "axial" )
         {
             m_orientation = Z_AXIS;
         }
         else if(orientation == "frontal" )
         {
             m_orientation = Y_AXIS;
         }
         else if(orientation == "sagittal" )
         {
             m_orientation = X_AXIS;
         }
    }
    if(m_configuration->hasAttribute("transform") )
    {
        this->setTransformId( m_configuration->getAttributeValue("transform") );
    }
    if(m_configuration->hasAttribute("ctrlimage") )
    {
        this->setCtrlImageId( m_configuration->getAttributeValue("ctrlimage") );
    }
    if(m_configuration->hasAttribute("vtkimagesource") )
    {
        this->setVtkImageSourceId( m_configuration->getAttributeValue("vtkimagesource") );
    }
    if (m_configuration->hasAttribute("interpolation"))
    {
        this->setInterpolation(!(m_configuration->getAttributeValue("interpolation") == "off"));
    }
    if(m_configuration->hasAttribute("actorOpacity") )
    {
        this->setActorOpacity(::boost::lexical_cast<double>(m_configuration->getAttributeValue("actorOpacity")));
    }
}

//------------------------------------------------------------------------------

void ImageSlice::updateImage( ::fwData::Image::sptr image  )
{
    SLM_ASSERT("Null control image", !m_ctrlImage.expired());
    this->updateImageInfos(image);
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void ImageSlice::updateSliceIndex( ::fwData::Image::sptr image )
{
    unsigned int axialIndex    = m_axialIndex->value();
    unsigned int frontalIndex  = m_frontalIndex->value();
    unsigned int sagittalIndex = m_sagittalIndex->value();

    const unsigned int pos[3] = { sagittalIndex, frontalIndex, axialIndex  };
    //int pos[3];
    //pos[2]= axialIndex;
    //pos[1]= frontalIndex;
    //pos[0]= sagittalIndex;

    this->setSlice( pos[ m_orientation], image );
}

//------------------------------------------------------------------------------

void ImageSlice::setSlice( int slice, ::fwData::Image::sptr image  )
{
    SLM_TRACE_FUNC();
    int extent[6];
    std::fill(  extent, extent+6, 0);
    extent[1] = static_cast<int>(image->getSize()[0]-1);
    extent[3] = static_cast<int>(image->getSize()[1]-1);
    extent[5] = static_cast<int>(image->getSize()[2]-1);
    extent[2*m_orientation]=slice;
    extent[2*m_orientation+1]=slice;

    OSLM_TRACE("DisplayExtent : " << " X min: " << extent[0] << " X max: " << extent[1] <<
                " Y min: " << extent[2] << " Y max: " << extent[3] <<
                " Z min: " << extent[4] << " Z max: " << extent[5]
                );

    m_imageActor->SetDisplayExtent( extent );

    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void ImageSlice::buildPipeline( )
{
    SLM_TRACE_FUNC();

    if (!m_imageSourceId.empty())
    {
        m_imageSource = this->getVtkObject(m_imageSourceId);
    }

    vtkImageAlgorithm *algorithm  = vtkImageAlgorithm::SafeDownCast(m_imageSource);
    vtkImageData      *imageData  = vtkImageData::SafeDownCast(m_imageSource);
    //vtkImageBlend     *imageBlend = vtkImageBlend::SafeDownCast(m_imageSource);

    SLM_ASSERT("Invalid vtk image source", algorithm||imageData );
    if (algorithm)
    {
        SLM_TRACE("Input is a vtkImageAlgorithm");
        m_imageActor->GetMapper()->SetInputConnection(algorithm->GetOutputPort());
        //if (imageBlend)
        //{
            //imageBlend->SetBlendModeToCompound();
            //imageBlend->SetCompoundThreshold(0);
        //}
    }
    else if (imageData)
    {
        SLM_TRACE("Input is a vtkImageData");
        m_imageActor->SetInputData(imageData);
    }

    if(!this->getTransformId().empty())
    {
        m_imageActor->SetUserTransform(this->getTransform());
    }

    m_imageActor->SetInterpolate(m_interpolation);
    m_imageActor->SetOpacity(m_actorOpacity);
    
    this->buildOutline();
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------
void ImageSlice::buildOutline()
{
    SLM_TRACE_FUNC();
    vtkPoints* points   = vtkPoints::New(VTK_DOUBLE);
    points->SetNumberOfPoints(4);
    int i;
    for (i = 0; i < 4; i++)
    {
        points->SetPoint(i,0.0,0.0,0.0);
    }

    vtkCellArray *cells = vtkCellArray::New();
    cells->Allocate(cells->EstimateSize(4,2));
    vtkIdType pts[2];
    pts[0] = 3; pts[1] = 2;       // top edge
    cells->InsertNextCell(2,pts);
    pts[0] = 0; pts[1] = 1;       // bottom edge
    cells->InsertNextCell(2,pts);
    pts[0] = 0; pts[1] = 3;       // left edge
    cells->InsertNextCell(2,pts);
    pts[0] = 1; pts[1] = 2;       // right edge
    cells->InsertNextCell(2,pts);

    m_planeOutlinePolyData->SetPoints(points);
    points->Delete();
    points = NULL;
    m_planeOutlinePolyData->SetLines(cells);
    cells->Delete();
    cells = NULL;

    m_planeOutlineMapper = vtkPolyDataMapper::New();
    m_planeOutlineMapper->SetInputData( m_planeOutlinePolyData );
    m_planeOutlineMapper->SetResolveCoincidentTopologyToPolygonOffset();
    m_planeOutlineActor->SetMapper(m_planeOutlineMapper);
    m_planeOutlineActor->PickableOff();
    if(!this->getTransformId().empty())
    {
        m_planeOutlineActor->SetUserTransform(this->getTransform());
    }
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------


void ImageSlice::updateOutline()
{
    SLM_TRACE_FUNC();
    static const int indexZ[12] = { 0,2,4, 1,2,4,  1,3,4 ,0,3,4 };
    static const int indexY[12] = { 0,2,4, 1,2,4,  1,2,5 ,0,2,5 };
    static const int indexX[12] = { 0,2,4, 0,2,5,  0,3,5 ,0,3,4 };
    static const int *indexSet[3] = { indexX, indexY, indexZ  };
    static double colors[3][3] = { {0.,0.,1.} , {0.,1.,0.}, {1.,0.,0.} };

    double *extent = m_imageActor->GetBounds();
    vtkPoints* points = m_planeOutlinePolyData->GetPoints();


    const int *index = indexSet[ m_orientation ];
    for ( int i=0; i < 4; ++i)
    {
        double pt[3];
        pt[0] = extent[ *(index++) ];
        pt[1] = extent[ *(index++) ];
        pt[2] = extent[ *(index++) ];
        points->SetPoint(i,pt);
    }

    points->GetData()->Modified();
    m_planeOutlinePolyData->Modified();

    m_planeOutlineActor->GetProperty()->SetColor( colors[m_orientation]);
    this->setVtkPipelineModified();
}


} //namespace visuVTKAdaptor
