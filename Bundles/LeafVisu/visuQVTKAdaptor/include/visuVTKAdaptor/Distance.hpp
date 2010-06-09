/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_DISTANCE_HPP_
#define _VISUQVTKADAPTOR_DISTANCE_HPP_

#include <vector>

#include <fwData/Point.hpp>
#include <fwData/Color.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/MeshFactory.hpp"

class vtkDistanceRepresentation2D;
class vtkActor;
class vtkLineSource;

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API Distance: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Distance)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API Distance() throw();

    VISUQVTKADAPTOR_API virtual ~Distance() throw();

    /// set Distance Axis color AND alpha
    VISUQVTKADAPTOR_API void setAxisColor( ::fwData::Color::sptr newColor) throw();

protected:

    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);
    VISUQVTKADAPTOR_API void doStart();
    VISUQVTKADAPTOR_API void doStop();


    vtkDistanceRepresentation2D * m_distanceRepresentation;
//  typedef std::vector < ::fwRenderVTK::IVtkAdaptorService::wptr > ServiceVector;
//  ServiceVector m_services;

    vtkActor          *m_lineActor;
//      vtkPolyDataMapper *LineMapper;
    vtkLineSource     *m_lineSource;

    ::fwData::Point::wptr m_point1;
    ::fwData::Point::wptr m_point2;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_DISTANCE_HPP_
