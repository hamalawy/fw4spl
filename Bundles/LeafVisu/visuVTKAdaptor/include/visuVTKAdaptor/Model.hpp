/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUVTKADAPTOR_MODEL_HPP_
#define _VISUVTKADAPTOR_MODEL_HPP_

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/MeshFactory.hpp"

#include "visuVTKAdaptor/config.hpp"

namespace visuVTKAdaptor
{


class VISUVTKADAPTOR_CLASS_API Model: public ::fwRenderVTK::IVtkAdaptorService, protected MeshFactory
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Model)(::fwRenderVTK::IVtkAdaptorService) ) ;

    VISUVTKADAPTOR_API Model() throw();

    VISUVTKADAPTOR_API virtual ~Model() throw();

protected:

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

};




} //namespace visuVTKAdaptor

#endif // _VISUVTKADAPTOR_MODEL_HPP_
