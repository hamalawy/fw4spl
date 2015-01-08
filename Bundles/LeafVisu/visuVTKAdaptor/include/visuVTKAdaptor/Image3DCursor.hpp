/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUVTKADAPTOR_IMAGE3DCURSOR_HPP_
#define _VISUVTKADAPTOR_IMAGE3DCURSOR_HPP_

#include <vtkSmartPointer.h>

#include <fwData/TransferFunction.hpp>
#include <fwComEd/helper/MedicalImageAdaptor.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

class vtkCommand;
class vtkActor;
class vtkPolyData;
class vtkPolyDataMapper;

namespace visuVTKAdaptor
{

/**
* @brief Add a 3D spatial marker represented by a sphere
*/
class VISUVTKADAPTOR_CLASS_API Image3DCursor: public  ::fwComEd::helper::MedicalImageAdaptor,public ::fwRenderVTK::IVtkAdaptorService
{

public:
    fwCoreServiceClassDefinitionsMacro ( (Image3DCursor)(::fwRenderVTK::IVtkAdaptorService) ) ;

    VISUVTKADAPTOR_API Image3DCursor() throw();

    VISUVTKADAPTOR_API virtual ~Image3DCursor() throw();

    VISUVTKADAPTOR_API void updateCursorPosition( double world[3] );

    VISUVTKADAPTOR_API void setVisibility( bool visibility );

protected:

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doReceive(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    void buildPolyData(float radius = 1.0);

    double m_priority;

    vtkSmartPointer<vtkPolyData>       m_cursorPolyData;
    vtkSmartPointer<vtkPolyDataMapper> m_cursorMapper;
    vtkSmartPointer<vtkActor>          m_cursorActor;



};




} //namespace visuVTKAdaptor

#endif // _VISUVTKADAPTOR_IMAGE3DCURSOR_HPP_
