/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_CAMERA_HPP_
#define _VISUQVTKADAPTOR_CAMERA_HPP_

#include <vector>
#include <fwRenderVTK/IVtkAdaptorService.hpp>
#include "visuVTKAdaptor/config.hpp"

namespace visuVTKAdaptor
{
class VISUQVTKADAPTOR_CLASS_API Camera: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Camera)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;
    VISUQVTKADAPTOR_API Camera() throw();
    VISUQVTKADAPTOR_API virtual ~Camera() throw();

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

private:

    bool initCameraParameters();
    bool bCam_init;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_CAMERA_HPP_
