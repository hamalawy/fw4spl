/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_ACQUISITION_HPP_
#define _VISUQVTKADAPTOR_ACQUISITION_HPP_

#include <vector>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

namespace visuVTKAdaptor
{
class VISUQVTKADAPTOR_CLASS_API Acquisition: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Acquisition)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API Acquisition() throw();

    VISUQVTKADAPTOR_API virtual ~Acquisition() throw();

    VISUQVTKADAPTOR_API void setClippingPlanes(::fwRenderVTK::VtkRenderService::VtkObjectIdType id){ m_clippingPlanes = id ; }

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    // manage only "ShowReconstructions" Field
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    ::fwRenderVTK::VtkRenderService::VtkObjectIdType m_clippingPlanes;
};
} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_ACQUISITION_HPP_
