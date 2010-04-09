/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef QVTKSIMPLEMESH_RENDERER_SERVICE_HPP_
#define QVTKSIMPLEMESH_RENDERER_SERVICE_HPP_


#include  <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include  <vtkImagePlaneWidget.h>
#include  <vtkOutlineFilter.h>
#include <QVTKWidget.h>


#include <vtkCommand.h>

#include <fwQtRender/IRender.hpp>

#include <fwServices/ObjectMsg.hpp>

#include <fwData/Image.hpp>

#include "qvtkSimpleMesh/config.hpp"

// VTK
class vtkRenderer;
class vtkPolyDataNormals;

namespace qvtkSimpleMesh
{

/**
 * @brief   Renderer service.
 * @class   RendererService.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * Service rendering a ::fwData::TriangularMesh using VTK.
 *
 * Service registered details : \n
 * REGISTER_SERVICE( ::fwRender::IRender , ::vtkSimpleMesh::RendererService , ::fwData::TriangularMesh)
 */
class  RendererService : public fwQtRender::IRender
{
public :

    fwCoreServiceClassDefinitionsMacro ( (RendererService)(::fwQtRender::IRender::Baseclass) ) ;

    /**
    * @brief    Constructor
    */
     RendererService() throw() ;

    /**
    * @brief    Destructor
    */
     virtual ~RendererService() throw() ;

    /**
    * @brief VTK event managing of the VTK Camera position.
    *
    * This method is used to update the VTK camera position.
    */
    void updateCamPosition();


protected :

    /**
    * @brief Starting method.
    *
    * This method is used to initialize the service.
    * Initialize VTK renderer and wxWidget containers
    */
     virtual void starting() throw(fwTools::Failed);


    /**
    * @brief Configuring method.
    *
    * XML configuration sample:
    * @verbatim
    <service implementation="::vtkSimpleMesh::RendererService" type="::fwRender::IRender" autoComChannel="yes" >
            <masterSlaveRelation>master</masterSlaveRelation>
            <win guiContainerId="900"/>
    </service>
    @endverbatim
    * This method is used to configure the service.
    */
     virtual void configuring() throw(::fwTools::Failed);


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



    /**
    * @brief Contains the mesh, and allows to compute normals.
    */
    vtkPolyDataNormals* m_normals;

    /**
    * @brief the m_bPipelineIsInit value is \b true
    * if the pipeline is initialized.
    */
    bool m_bPipelineIsInit;

    vtkCommand* m_loc;

    bool m_isCamMaster;

};

}

#endif /* VTKSIMPLEMESH_RENDERER_SERVICE_HPP_ */
