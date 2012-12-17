/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwData/Resection.hpp>
#include <fwData/Reconstruction.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <fwComEd/ResectionMsg.hpp>

#include "visuVTKAdaptor/Reconstruction.hpp"
#include "visuVTKAdaptor/Resection.hpp"


fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::Resection, ::fwData::Resection ) ;

namespace visuVTKAdaptor
{


Resection::Resection() throw()
{
    m_clippingPlanes = "";
    m_sharpEdgeAngle = 50;
    m_autoResetCamera = true;
    //addNewHandledEvent( ::fwComEd::ResectionMsg::VISIBILITY );
    //addNewHandledEvent( ::fwComEd::ResectionMsg::MODIFIED );
}

//------------------------------------------------------------------------------

Resection::~Resection() throw()
{

}

//------------------------------------------------------------------------------

void Resection::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    assert(m_configuration->getName() == "config");
    this->setPickerId( m_configuration->getAttributeValue("picker") );
    this->setRenderId( m_configuration->getAttributeValue("renderer") );

    this->setClippingPlanes( m_configuration->getAttributeValue("clippingplanes") );

    std::string sSharpEdgeAngle(m_configuration->getAttributeValue("sharpangle"));
    if ( !sSharpEdgeAngle.empty() )
    {
        double sharpEdgeAngle = ::boost::lexical_cast< double > (sSharpEdgeAngle);
        this->setSharpEdgeAngle( sharpEdgeAngle );
    }

    if(m_configuration->hasAttribute("transform") )
    {
        this->setTransformId( m_configuration->getAttributeValue("transform") );
    }

    if (m_configuration->hasAttribute("autoresetcamera") )
    {
        std::string autoresetcamera = m_configuration->getAttributeValue("autoresetcamera");
        m_autoResetCamera = (autoresetcamera == "yes");
    }
}

//------------------------------------------------------------------------------

void Resection::doStart() throw(fwTools::Failed)
{
    this->doUpdate();
}

//------------------------------------------------------------------------------

void Resection::doUpdate() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    doStop();

    ::fwData::Resection::sptr resec = this->getObject< ::fwData::Resection >();

    if (resec->getIsVisible())
    {
        std::vector< ::fwData::Reconstruction::sptr >::iterator iterRes;
        std::vector< ::fwData::Reconstruction::sptr > vReconst;
        bool resectionIsValid = resec->getIsValid();
        if(resec->getIsSafePart() || resectionIsValid)
        {
            vReconst = resec->getRefOutputs();
        }
        else
        {
            vReconst = resec->getRefInputs();
        }
        for (iterRes = vReconst.begin(); iterRes != vReconst.end(); ++iterRes)
        {
            ::fwRenderVTK::IVtkAdaptorService::sptr service =
                    ::fwServices::add< ::fwRenderVTK::IVtkAdaptorService >
            ( *iterRes, "::visuVTKAdaptor::Reconstruction" );
            SLM_ASSERT("service not instanced", service);

            service->setTransformId( this->getTransformId() );
            service->setRenderId( this->getRenderId() );
            service->setPickerId( this->getPickerId() );
            service->setRenderService(this->getRenderService());
            ::visuVTKAdaptor::Reconstruction::sptr reconstAdaptor = ::visuVTKAdaptor::Reconstruction::dynamicCast(service);
            if(!resectionIsValid)
            {
                reconstAdaptor->setClippingPlanes( m_clippingPlanes );
            }
            reconstAdaptor->setSharpEdgeAngle( m_sharpEdgeAngle );
            reconstAdaptor->setAutoResetCamera(m_autoResetCamera);
            service->start();

            this->registerService(service);
        }
    }
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void Resection::doSwap() throw(fwTools::Failed)
{
    this->doUpdate();
}

//------------------------------------------------------------------------------

void Resection::doStop() throw(fwTools::Failed)
{
    this->unregisterServices();
}

//------------------------------------------------------------------------------

void Resection::doUpdate( ::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed)
{
    ::fwComEd::ResectionMsg::csptr pResectionMsg = ::fwComEd::ResectionMsg::dynamicConstCast( msg ) ;
    if ( pResectionMsg)
    {
        if ( pResectionMsg->hasEvent(::fwComEd::ResectionMsg::VISIBILITY) )
        {
            this->doUpdate();
        }
        if ( pResectionMsg->hasEvent(::fwComEd::ResectionMsg::MODIFIED) )
        {
            this->doUpdate();
        }
    }
}




} //namespace visuVTKAdaptor
