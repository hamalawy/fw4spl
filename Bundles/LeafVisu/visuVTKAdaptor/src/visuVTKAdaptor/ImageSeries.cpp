/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <fwData/Reconstruction.hpp>
#include <fwData/Material.hpp>
#include <fwData/Boolean.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <fwMedData/ImageSeries.hpp>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

#include "visuVTKAdaptor/ImageSeries.hpp"



fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::ImageSeries, ::fwMedData::ImageSeries );

namespace visuVTKAdaptor
{


ImageSeries::ImageSeries() throw() :
        m_allowAlphaInTF(false),
        m_interpolation(false),
        m_3dModeEnabled ( ::boost::logic::indeterminate ),
        m_sliceMode(NegatoMPR::THREE_SLICES)

{}

//------------------------------------------------------------------------------

ImageSeries::~ImageSeries() throw()
{}

//------------------------------------------------------------------------------

void ImageSeries::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    assert(m_configuration->getName() == "config");
    this->setPickerId( m_configuration->getAttributeValue("picker") );
    this->setRenderId( m_configuration->getAttributeValue("renderer") );

    if (m_configuration->hasAttribute("mode"))
    {
        std::string value(m_configuration->getAttributeValue("mode"));
        std::transform(value.begin(), value.end(), value.begin(), tolower);
        OSLM_ASSERT("Sorry, bad value "<<value<<" for attribute mode.",
                    value == "3d" || value == "2d");
        this->set3dMode(value == "3d");
    }
    if (m_configuration->hasAttribute("slices"))
    {
        std::string value(m_configuration->getAttributeValue("slices"));
        std::transform(value.begin(), value.end(), value.begin(), tolower);

        if(value == "0")
        {
            this->setSliceMode(NegatoMPR::NO_SLICE);
        }
        else if(value == "1")
        {
            this->setSliceMode(NegatoMPR::ONE_SLICE);
        }
        else if(value == "3")
        {
            this->setSliceMode(NegatoMPR::THREE_SLICES);
        }
    }
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
    if(m_configuration->hasAttribute("tfalpha") )
    {
        this->setAllowAlphaInTF(m_configuration->getAttributeValue("tfalpha") == "yes");
    }
    if (m_configuration->hasAttribute("interpolation"))
    {
        this->setInterpolation(!(m_configuration->getAttributeValue("interpolation") == "off"));
    }
    if (m_configuration->hasAttribute("vtkimagesource"))
    {
        this->setVtkImageSourceId( m_configuration->getAttributeValue("vtkimagesource") );
    }
    this->parseTFConfig( m_configuration );
}

//------------------------------------------------------------------------------

void ImageSeries::doStart() throw(fwTools::Failed)
{
    this->doUpdate();
}

//------------------------------------------------------------------------------

void ImageSeries::doUpdate() throw(fwTools::Failed)
{
    ::fwMedData::ImageSeries::sptr series = this->getObject< ::fwMedData::ImageSeries >();

    doStop();

    ::fwData::Image::sptr image = series->getImage();

    ::fwRenderVTK::IVtkAdaptorService::sptr service =
            ::fwServices::add< ::fwRenderVTK::IVtkAdaptorService >( image, "::visuVTKAdaptor::NegatoMPR" );
    SLM_ASSERT("service not instanced", service);

    service->setTransformId( this->getTransformId() );
    service->setRenderId( this->getRenderId() );
    service->setPickerId( this->getPickerId() );
    service->setRenderService(this->getRenderService());
    service->setAutoRender( this->getAutoRender() );
    ::visuVTKAdaptor::NegatoMPR::sptr negato = ::visuVTKAdaptor::NegatoMPR::dynamicCast(service);
    negato->set3dMode(this->is3dModeEnabled());
    negato->setSliceMode(this->getSliceMode());
    negato->setOrientation(this->getOrientation());
    negato->setAllowAlphaInTF(m_allowAlphaInTF);
    negato->setInterpolation(m_interpolation);
    negato->setVtkImageSourceId(m_imageSourceId);
    negato->parseTFConfig(m_configuration);
    service->start();

    this->registerService(service);
}

//------------------------------------------------------------------------------

void ImageSeries::doSwap() throw(fwTools::Failed)
{
    this->doUpdate();
}

//------------------------------------------------------------------------------

void ImageSeries::doStop() throw(fwTools::Failed)
{
    this->unregisterServices();
}

//------------------------------------------------------------------------------

void ImageSeries::doReceive( ::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed)
{}

//------------------------------------------------------------------------------

void  ImageSeries::setSliceMode(NegatoMPR::SliceMode sliceMode)
{
    if(m_sliceMode != sliceMode)
    {
        m_sliceMode = sliceMode;
    }
}

//------------------------------------------------------------------------------

NegatoMPR::SliceMode ImageSeries::getSliceMode()
{
    return m_sliceMode;
}

//------------------------------------------------------------------------------

::boost::logic::tribool ImageSeries::is3dModeEnabled()
{
    return m_3dModeEnabled;
}

//------------------------------------------------------------------------------

void ImageSeries::set3dMode( bool enabled )
{
    m_3dModeEnabled = enabled;
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
