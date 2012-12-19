/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <vtkCommand.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataNormals.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkTransform.h>

#include <fwCore/HiResTimer.hpp>

#include <fwData/Mesh.hpp>
#include <fwData/mt/ObjectReadLock.hpp>

#include <fwComEd/CameraMsg.hpp>
#include <fwComEd/MeshMsg.hpp>

#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>
#include <fwCom/Signals.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>

#include <vtkIO/helper/Mesh.hpp>
#include <vtkIO/vtk.hpp>

#include "vtkSimpleMesh/RendererService.hpp"

fwServicesRegisterMacro( ::fwRender::IRender , ::vtkSimpleMesh::RendererService , ::fwData::Mesh );


namespace vtkSimpleMesh
{

const ::fwCom::Slots::SlotKeyType RendererService::s_UPDATE_CAM_POSITION_SLOT = "updateCamPosition";
const ::fwCom::Signals::SignalKeyType RendererService::s_CAM_UPDATED_SIG = "camUpdated";

class vtkLocalCommand : public vtkCommand
{
public:

    vtkLocalCommand(::vtkSimpleMesh::RendererService* _service)
    {
        m_service = _service;
        this->m_isMousePressed = false;
    }
    void Execute(vtkObject* _caller, unsigned long _event, void* _obj)
    {
        if (_event == vtkCommand::StartInteractionEvent )
        {
            this->m_isMousePressed = true;
        }
        else if (_event == vtkCommand::EndInteractionEvent )
        {
            this->m_isMousePressed = false;
        }
        else if ( (_event == vtkCommand::ModifiedEvent && this->m_isMousePressed)
                || _event == vtkCommand::MouseWheelBackwardEvent || _event == vtkCommand::MouseWheelForwardEvent)
        {
            m_service->notifyCamPositionUpdated();
        }
    }
private:
    ::vtkSimpleMesh::RendererService* m_service;
    bool m_isMousePressed;
};

RendererService::RendererService() throw()
: m_render( 0 ), m_bPipelineIsInit(false)
{
    //this->addNewHandledEvent( ::fwComEd::MeshMsg::NEW_MESH );
    //this->addNewHandledEvent( ::fwComEd::CameraMsg::CAMERA_MOVING );


    m_slotUpdateCamPosition   = ::fwCom::newSlot( &RendererService::updateCamPosition, this ) ;
    ::fwCom::HasSlots::m_slots( s_UPDATE_CAM_POSITION_SLOT   , m_slotUpdateCamPosition );

    m_sigCamUpdated = CamUpdatedSignalType::New();
#ifdef COM_LOG
    m_sigCamUpdated->setID( s_CAM_UPDATED_SIG );
#endif
    // Register
    ::fwCom::HasSignals::m_signals( s_CAM_UPDATED_SIG,  m_sigCamUpdated);

    this->setWorker( ::fwServices::registry::ActiveWorkers::getDefault()->
                                 getWorker( ::fwServices::registry::ActiveWorkers::s_DEFAULT_WORKER ) );
}

//-----------------------------------------------------------------------------

RendererService::~RendererService() throw()
{
}

//-----------------------------------------------------------------------------

void RendererService::starting() throw(fwTools::Failed)
{
    this->create();

    m_interactorManager = ::fwRenderVTK::IVtkRenderWindowInteractorManager::createManager();
    m_interactorManager->installInteractor( this->getContainer() );

    m_bPipelineIsInit = false;

    // Renderer
    m_render = vtkRenderer::New();
    m_interactorManager->getInteractor()->GetRenderWindow()->AddRenderer(m_render);

    bool meshIsLoaded;
    {
        ::fwData::Mesh::sptr mesh = this->getObject< ::fwData::Mesh >();
        ::fwData::mt::ObjectReadLock lock(mesh);
        meshIsLoaded = mesh->getNumberOfPoints() > 0;
    }

    if ( meshIsLoaded )
    {
        this->initVTKPipeline();
        m_bPipelineIsInit = true;
    }
}

//-----------------------------------------------------------------------------

void RendererService::configuring() throw(::fwTools::Failed)
{
    this->initialize();
}

//-----------------------------------------------------------------------------

void RendererService::stopping() throw(fwTools::Failed)
{
    if( m_render == 0 ) return;

    m_interactorManager->getInteractor()->RemoveObserver(m_loc);

    m_interactorManager->uninstallInteractor();
    m_interactorManager.reset();

    SLM_ASSERT("m_render not instanced", m_render);
    m_render->Delete();
    m_render = 0;

    this->destroy();
}

//-----------------------------------------------------------------------------

void RendererService::updating() throw(fwTools::Failed)
{
    m_interactorManager->getInteractor()->Render();
}

//-----------------------------------------------------------------------------

void RendererService::receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{
    ::fwComEd::MeshMsg::csptr meshMsg = ::fwComEd::MeshMsg::dynamicConstCast(_msg);
    if ( meshMsg && meshMsg->hasEvent( ::fwComEd::MeshMsg::NEW_MESH ) )
    {
        if(!m_bPipelineIsInit)
        {
            this->initVTKPipeline();
            m_bPipelineIsInit = true;
        }
        else
        {
            m_vtkPolyData = vtkSmartPointer<vtkPolyData>::New();
            ::fwData::Mesh::sptr mesh = this->getObject< ::fwData::Mesh >();
            {
                ::fwData::mt::ObjectReadLock lock(mesh);
                ::vtkIO::helper::Mesh::toVTKMesh( mesh, m_vtkPolyData );
            }
            m_mapper->SetInput(m_vtkPolyData);
        }
        m_interactorManager->getInteractor()->Render();
    }
    else if ( meshMsg && meshMsg->hasEvent( ::fwComEd::MeshMsg::VERTEX_MODIFIED ) )
    {
        m_hiResTimer.reset();
        m_hiResTimer.start();
        this->updateVTKPipeline(false);
        m_hiResTimer.stop();
        OSLM_INFO("Vertex updating time (milli sec) = " << m_hiResTimer.getElapsedTimeInMilliSec());

        m_hiResTimer.reset();
        m_hiResTimer.start();
        m_interactorManager->getInteractor()->Render();
        m_hiResTimer.stop();
        OSLM_INFO("Render time (milli sec) = " << m_hiResTimer.getElapsedTimeInMilliSec());
    }

}

//-----------------------------------------------------------------------------

void RendererService::initVTKPipeline()
{
    ::fwData::Mesh::sptr mesh = this->getObject< ::fwData::Mesh >();
    m_vtkPolyData = vtkSmartPointer<vtkPolyData>::New();

    {
        ::fwData::mt::ObjectReadLock lock(mesh);
        ::vtkIO::helper::Mesh::toVTKMesh( mesh, m_vtkPolyData );
    }

    m_mapper = vtkPolyDataMapper::New();
    m_mapper->ImmediateModeRenderingOn();
    m_mapper->SetInput(m_vtkPolyData);

    vtkActor* actor =  vtkActor::New();
    actor->SetMapper(m_mapper);

    // Add the actors
    m_render->AddActor( actor);

    m_interactorManager->getInteractor()->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());
    m_loc = new vtkLocalCommand(this);

    m_interactorManager->getInteractor()->AddObserver(vtkCommand::AnyEvent, m_loc);


    // Repaint and resize window
    m_render->ResetCamera();
}

//-----------------------------------------------------------------------------

void RendererService::updateVTKPipeline(bool resetCamera)
{
    ::fwData::Mesh::sptr mesh = this->getObject< ::fwData::Mesh >();

    {
        ::fwData::mt::ObjectReadLock lock(mesh);
        ::vtkIO::helper::Mesh::updatePolyDataPoints(m_vtkPolyData, mesh);
        ::vtkIO::helper::Mesh::updatePolyDataPointNormals(m_vtkPolyData, mesh);
        ::vtkIO::helper::Mesh::updatePolyDataPointColor(m_vtkPolyData, mesh);
        ::vtkIO::helper::Mesh::updatePolyDataCellNormals(m_vtkPolyData, mesh);
    }

    if (resetCamera)
    {
        m_render->ResetCamera();
    }

}

//-----------------------------------------------------------------------------

void RendererService::notifyCamPositionUpdated()
{
    vtkCamera* camera = m_render->GetActiveCamera();

    const double *position = camera->GetPosition();
    const double *focal = camera->GetFocalPoint();
    const double *viewUp = camera->GetViewUp();

    fwServicesBlockAndNotifyMacro( this->getLightID(), m_sigCamUpdated,
                                   (position, focal, viewUp),
                                   m_slotUpdateCamPosition );
}

//-----------------------------------------------------------------------------

void RendererService::updateCamPosition(const double positionValue[3], const double focalValue[3], const double viewUpValue[3])
{
    vtkCamera* camera = m_render->GetActiveCamera();

    camera->SetPosition(positionValue);
    camera->SetFocalPoint(focalValue);
    camera->SetViewUp(viewUpValue);
    camera->SetClippingRange(0.1, 1000000);

    m_interactorManager->getInteractor()->Render();
}

}
