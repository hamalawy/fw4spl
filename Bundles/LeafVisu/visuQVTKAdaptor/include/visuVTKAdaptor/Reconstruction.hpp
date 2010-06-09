/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_RECONSTRUCTION_HPP_
#define _VISUQVTKADAPTOR_RECONSTRUCTION_HPP_

#include <fwServices/ObjectMsg.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/MeshFactory.hpp"

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API Reconstruction: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Reconstruction)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;
    VISUQVTKADAPTOR_API Reconstruction() throw();
    VISUQVTKADAPTOR_API virtual ~Reconstruction() throw();
    VISUQVTKADAPTOR_API void setClippingPlanes(::fwRenderVTK::VtkRenderService::VtkObjectIdType id){ m_clippingPlanesId = id ; }
    VISUQVTKADAPTOR_API void setSharpEdgeAngle(double angle){ m_sharpEdgeAngle = angle; }
    VISUQVTKADAPTOR_API void setForceHide(bool hide);

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API virtual void doUpdate( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);


    void createMeshService();

    ::fwRenderVTK::VtkRenderService::VtkObjectIdType m_clippingPlanesId;

    ::fwRenderVTK::IVtkAdaptorService::wptr m_meshService;

    double      m_sharpEdgeAngle;
};




} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_RECONSTRUCTION_HPP_
