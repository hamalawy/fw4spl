/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_PLANE_HPP_
#define _VISUQVTKADAPTOR_PLANE_HPP_

#include <fwServices/ObjectMsg.hpp>
#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

class vtkPlaneSource;
class vtkPlaneCollection;
class vtkPlane;
class vtkObject;

namespace fwData
{
    class Plane;
}

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API Plane: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Plane)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API Plane() throw();

    VISUQVTKADAPTOR_API virtual ~Plane() throw();

    VISUQVTKADAPTOR_API void setVtkPlaneCollection( vtkObject * col );

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API virtual void doUpdate( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    void selectPlane(bool select);

private:

    vtkPlaneSource* m_vtkPlane;
    vtkActor *m_actorPlan;
    fwData::Plane::wptr m_pPlane;

    vtkPlane* m_vtkImplicitPlane;
    vtkPlaneCollection* m_vtkPlaneCollection;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_PLANE_HPP_
