/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_IMAGETEXT_HPP_
#define _VISUQVTKADAPTOR_IMAGETEXT_HPP_


#include <string>

#include <fwRenderVTK/IVtkAdaptorService.hpp>
#include <fwComEd/helper/MedicalImageAdaptor.hpp>

#include "visuVTKAdaptor/Text.hpp"
#include "visuVTKAdaptor/config.hpp"

class vtkTextActor;

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API ImageText: public Text, public ::fwComEd::helper::MedicalImageAdaptor
{

public:

    fwCoreServiceClassDefinitionsMacro ( (ImageText)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API ImageText() throw();

    VISUQVTKADAPTOR_API virtual ~ImageText() throw();

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);

};
} //namespace visuVTKAdaptor

#endif //_VISUQVTKADAPTOR_IMAGETEXT_HPP_
