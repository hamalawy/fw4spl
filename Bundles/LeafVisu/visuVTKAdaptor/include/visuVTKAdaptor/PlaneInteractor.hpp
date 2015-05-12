/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_PLANEINTERACTOR_HPP__
#define __VISUVTKADAPTOR_PLANEINTERACTOR_HPP__

#include <fwData/TransferFunction.hpp>
#include <fwComEd/helper/MedicalImageAdaptor.hpp>
#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"


class vtkCommand;

namespace visuVTKAdaptor
{

class VISUVTKADAPTOR_CLASS_API PlaneInteractor : public ::fwComEd::helper::MedicalImageAdaptor,
                                                 public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (PlaneInteractor)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API PlaneInteractor() throw();

    VISUVTKADAPTOR_API virtual ~PlaneInteractor() throw();

    VISUVTKADAPTOR_API void switchPlaneNormal();

    VISUVTKADAPTOR_API void pushPlane(double factor);

    VISUVTKADAPTOR_API void deselectPlane();

protected:

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doReceive(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    vtkCommand *m_vtkObserver;
    double m_priority;
};




} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_PLANEINTERACTOR_HPP__
