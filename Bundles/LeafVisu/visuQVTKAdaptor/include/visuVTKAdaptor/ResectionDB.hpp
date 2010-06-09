/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_RESECTIONDB_HPP_
#define _VISUQVTKADAPTOR_RESECTIONDB_HPP_

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/MeshFactory.hpp"

namespace visuVTKAdaptor
{



class VISUQVTKADAPTOR_CLASS_API ResectionDB: public ::fwRenderVTK::IVtkAdaptorService
{

public:
    fwCoreServiceClassDefinitionsMacro ( (ResectionDB)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API ResectionDB() throw();
    VISUQVTKADAPTOR_API virtual ~ResectionDB() throw();
    VISUQVTKADAPTOR_API void setClippingPlanes(::fwRenderVTK::VtkRenderService::VtkObjectIdType id){ m_clippingPlanes = id ; }
    VISUQVTKADAPTOR_API void setSharpEdgeAngle(double angle){ m_sharpEdgeAngle = angle; }

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    // manage only "ShowReconstructions" Field
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

private:
    ::fwRenderVTK::VtkRenderService::VtkObjectIdType m_clippingPlanes;
    double m_sharpEdgeAngle;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_RESECTIONDB_HPP_
