/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_POINTLABEL_HPP_
#define _VISUQVTKADAPTOR_POINTLABEL_HPP_

#include <string>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/Text.hpp"

class VISUQVTKADAPTOR_CLASS_API vtkTextActor;

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API PointLabel: public Text
{

public:
    fwCoreServiceClassDefinitionsMacro ( (PointLabel)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API PointLabel();

protected:

    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate( ::fwServices::ObjectMsg::csptr msg ) throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);

    void starting() throw(::fwTools::Failed);
    void stopping() throw(::fwTools::Failed);
};

} //namespace visuVTKAdaptor

#endif //  _VISUQVTKADAPTOR_POINTLABEL_HPP_

