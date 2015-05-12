/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_BOXWIDGET_HPP__
#define __VISUVTKADAPTOR_BOXWIDGET_HPP__

#include <fwRenderVTK/IVtkAdaptorService.hpp>
#include "visuVTKAdaptor/config.hpp"

class vtkTransform;
class vtkBoxWidget2;


namespace visuVTKAdaptor
{

class Transform;

// BoxWidget
class VISUVTKADAPTOR_CLASS_API BoxWidget : public ::fwRenderVTK::IVtkAdaptorService
{
public:

    fwCoreServiceClassDefinitionsMacro( (BoxWidget)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API BoxWidget() throw();
    VISUVTKADAPTOR_API virtual ~BoxWidget() throw();

    VISUVTKADAPTOR_API void updateFromVtk();

protected:

    VISUVTKADAPTOR_API virtual void configuring() throw( ::fwTools::Failed );
    VISUVTKADAPTOR_API virtual void doStart()     throw( ::fwTools::Failed );
    VISUVTKADAPTOR_API virtual void doStop()      throw( ::fwTools::Failed );
    VISUVTKADAPTOR_API virtual void doSwap()      throw( ::fwTools::Failed );
    VISUVTKADAPTOR_API virtual void doUpdate()    throw( ::fwTools::Failed );
    VISUVTKADAPTOR_API virtual void doReceive( ::fwServices::ObjectMsg::csptr msg ) throw( ::fwTools::Failed );

    vtkTransform* m_transform;
    WPTR(::visuVTKAdaptor::Transform) m_transformService;

private:

    ::vtkBoxWidget2* m_vtkBoxWidget;
    vtkCommand* m_boxWidgetCommand;

    double m_scaleFactor;
    bool m_enableScaling;
};

} // namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_BOXWIDGET_HPP__
