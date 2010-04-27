/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWRENDERVTK_INTERACTORSTYLE2DFORNEGATO_HPP_
#define _FWRENDERVTK_INTERACTORSTYLE2DFORNEGATO_HPP_

#include <vtkImagePlaneWidget.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "fwRenderVTK/config.hpp"


class Renderer;

class FWRENDERVTK_CLASS_API InteractorStyle2DForNegato : public vtkInteractorStyleTrackballCamera
{

public:

    FWRENDERVTK_API static InteractorStyle2DForNegato *New();

    vtkTypeRevisionMacro(InteractorStyle2DForNegato, vtkInteractorStyle);

    FWRENDERVTK_API virtual void OnMiddleButtonDown();

    FWRENDERVTK_API virtual void OnChar();

    FWRENDERVTK_API virtual void OnLeftButtonDown();
    FWRENDERVTK_API virtual void OnMouseMove();
    FWRENDERVTK_API virtual void OnMouseWheelBackward();
    FWRENDERVTK_API virtual void OnMouseWheelForward();
    FWRENDERVTK_API virtual void OnRightButtonDown();

    FWRENDERVTK_API virtual void Pan();
    FWRENDERVTK_API virtual void Dolly();

protected:

    FWRENDERVTK_API InteractorStyle2DForNegato();
    FWRENDERVTK_API ~InteractorStyle2DForNegato();

    FWRENDERVTK_API virtual void Dolly(double factor);
private:

    InteractorStyle2DForNegato(const InteractorStyle2DForNegato&);  // Not implemented.

    void operator=(const InteractorStyle2DForNegato&);  // Not implemented.

    double m_newPickPoint[4];
    double m_oldPickPoint[4];
};

#endif // _FWRENDERVTK_INTERACTORSTYLE3DFORNEGATO_HPP_
