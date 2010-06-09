/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_CUBE_HPP_
#define _VISUQVTKADAPTOR_CUBE_HPP_


#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

namespace visuVTKAdaptor
{
class VISUQVTKADAPTOR_CLASS_API Cube: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Cube)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

protected:
    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed){};
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed){};
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed){};
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed){};

};
} //namespace visuVTKAdaptor
#endif
