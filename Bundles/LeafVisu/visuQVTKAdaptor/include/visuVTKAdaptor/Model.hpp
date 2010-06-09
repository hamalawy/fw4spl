/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_MODEL_HPP_
#define _VISUQVTKADAPTOR_MODEL_HPP_

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/MeshFactory.hpp"

#include "visuVTKAdaptor/config.hpp"

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API Model: public ::fwRenderVTK::IVtkAdaptorService, protected MeshFactory
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Model)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API Model() throw();

    VISUQVTKADAPTOR_API virtual ~Model() throw();

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_MODEL_HPP_
