/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_POINT_HPP_
#define _VISUQVTKADAPTOR_POINT_HPP_


#include <fwServices/ObjectMsg.hpp>
#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

class vtkHandleWidget;
class vtkHandleRepresentation;
class vtkCommand;

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API Point: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Point)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API Point() throw();
    VISUQVTKADAPTOR_API virtual ~Point() throw();
    VISUQVTKADAPTOR_API void setColor(double red, double green, double blue, double alpha = 1.0);
    VISUQVTKADAPTOR_API void setSelectedColor(double red, double green, double blue, double alpha = 1.0);

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API virtual void doUpdate( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

protected:
    vtkHandleWidget         * m_handle;
    vtkHandleRepresentation * m_representation;
    vtkCommand              * m_pointUpdateCommand;

};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_POINT_HPP_
