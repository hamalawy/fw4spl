/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#ifndef _VISUQVTKADAPTOR_PLANESELECTOR_HPP_
#define _VISUQVTKADAPTOR_PLANESELECTOR_HPP_

#include <fwData/Object.hpp>
#include <fwData/PlaneList.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API PlaneSelector: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (PlaneSelector)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API PlaneSelector() throw();

    VISUQVTKADAPTOR_API virtual ~PlaneSelector() throw();

    VISUQVTKADAPTOR_API void selectObject( ::fwData::Object::sptr object );

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    ::fwData::Object::wptr m_currentObject;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_PLANESELECTOR_HPP_
