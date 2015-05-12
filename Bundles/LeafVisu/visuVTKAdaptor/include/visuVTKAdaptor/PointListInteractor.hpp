/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_POINTLISTINTERACTOR_HPP__
#define __VISUVTKADAPTOR_POINTLISTINTERACTOR_HPP__

#include <vector>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/MeshFactory.hpp"

class vtkCommand;

namespace visuVTKAdaptor
{

class VISUVTKADAPTOR_CLASS_API PointListInteractor : public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (PointListInteractor)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API PointListInteractor() throw();

    VISUVTKADAPTOR_API virtual ~PointListInteractor() throw();


    VISUVTKADAPTOR_API void resetPointList();
    VISUVTKADAPTOR_API void addPoint(const double &x, const double &y, const double &z);

protected:

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    void doReceive(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed)
    {
    }



    vtkCommand *m_interactionCommand;
    float m_priority;

};




} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_POINTLISTINTERACTOR_HPP__
