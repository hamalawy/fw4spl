/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_PLANEINTERACTOR_HPP_
#define _VISUQVTKADAPTOR_PLANEINTERACTOR_HPP_

#include <fwData/TransfertFunction.hpp>
#include <fwComEd/helper/MedicalImageAdaptor.hpp>
#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"


class vtkCommand;

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API PlaneInteractor: public ::fwComEd::helper::MedicalImageAdaptor,public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (PlaneInteractor)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API PlaneInteractor() throw();
    VISUQVTKADAPTOR_API virtual ~PlaneInteractor() throw();
    VISUQVTKADAPTOR_API void switchPlaneNormal();
    VISUQVTKADAPTOR_API void pushPlane(double factor);
    VISUQVTKADAPTOR_API void deselectPlane();

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    vtkCommand *m_vtkObserver;
    double m_priority;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_PLANEINTERACTOR_HPP_
