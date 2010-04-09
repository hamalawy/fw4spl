/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef QVTKSIMPLENEGATO_RENDERER_SERVICE_HPP_
#define QVTKSIMPLENEGATO_RENDERER_SERVICE_HPP_

#include  <vtkRenderer.h>
#include  <vtkImagePlaneWidget.h>
#include  <vtkOutlineFilter.h>


#include <fwQtRender/IRender.hpp>

#include "qvtkSimpleNegato/config.hpp"

namespace qvtkSimpleNegato
{

/**
* @brief    Renderer service.
* @class    RendererService.
* @author   IRCAD (Research and Development Team).
* @date 2009.
*
* Service rendering a ::fwData::Image using VTK.
*
* Service registered details : \n
* REGISTER_SERVICE( ::fwRender::IRender , ::vtkSimpleNegato::RendererService , ::fwData::Image)
*/
class  RendererService : public fwQtRender::IRender
{
public :
    fwCoreServiceClassDefinitionsMacro ( (RendererService)(::fwQtRender::IRender::Baseclass) );

    /**
    * @brief    Constructor
    */
     RendererService() throw() ;

    /**
    * @brief    Destructor
    */
     virtual ~RendererService() throw() ;


protected :

    /**
    * @brief Starting method.
    *
    * This method is used to initialize the service.
    * Initialize VTK renderer and wxWidget containers
    */
     virtual void starting() throw(fwTools::Failed);

    /**
    * @brief Stopping method.
    *
    * Destroy VTK renderer and wxWidget containers
    */
     virtual void stopping() throw(fwTools::Failed);

    /**
    * @brief Updating method.
    *
    * This method is used to update the service.
    * Make a render if necessary
    */
     virtual void updating() throw(fwTools::Failed);

    /**
    * @brief Configuring method.
    *
    * This method is used to configure the service.
    */
     virtual void configuring() throw(::fwTools::Failed);

    /**
    * @brief Updating method (react on data modifications).
    * @param[in] _msg ::fwServices::ObjectMsg::csptr.
    *
    * This method is used to update the service.
    */
     virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /// @brief vtk renderer
    vtkRenderer * m_render ;
    vtkRenderWindow *renderWindow;
    vtkRenderWindowInteractor *m_interactor;
    QVTKWidget *widget;

private :
    /// @brief required to facilitate resize of an empty vtk rendering window : why ?
//    wxAuiManager* m_wxmanager;
    /// @brief VTK Interactor window
  //  ::wxVTKRenderWindowInteractor* m_interactor;
  //    vtkRenderWindowInteractor *renderWindow;

    /**
    * @brief VTK pipeline initialization method.
    *
    * This method is used to initialize the VTK pipeline.
    */
    void initVTKPipeline();

    /**
    * @brief VTK pipeline updating method.
    *
    * This method is used to update the VTK pipeline.
    */
    void updateVTKPipeline();

    /// @brief image wireframe outline.
    vtkOutlineFilter* m_outline;

    /// @brief vtk widget used in the negatoscope axial plane.
    vtkImagePlaneWidget* m_negatoAxial;

    /// @brief vtk widget used in the negatoscope sagittal plane.
    vtkImagePlaneWidget* m_negatoSagittal;

    /// @brief vtk widget used in the negatoscope frontal plane.
    vtkImagePlaneWidget* m_negatoFrontal;

    /**
    * @brief the m_bPipelineIsInit value is \b true
    * if the pipeline is initialized.
    */
    bool m_bPipelineIsInit;
};

}


#endif /* VTKSIMPLENEGATO_RENDERER_SERVICE_HPP_ */
