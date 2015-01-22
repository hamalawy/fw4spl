/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VTKSIMPLEMESH_SSIMPLEMESHDEFORMATION_HPP__
#define __VTKSIMPLEMESH_SSIMPLEMESHDEFORMATION_HPP__

#include "vtkSimpleMesh/config.hpp"

#include <fwData/Mesh.hpp>

#include <fwThread/Timer.hpp>

#include <fwServices/ObjectMsg.hpp>
#include <fwServices/IController.hpp>



namespace vtkSimpleMesh
{

/**
 * @brief   Service used to deform mesh
 * @class   SSimpleMeshDeformation
 * 
 * @date    2012.
 */
class VTKSIMPLEMESH_CLASS_API SSimpleMeshDeformation : public ::fwServices::IController
{
public :

    fwCoreServiceClassDefinitionsMacro ( (SSimpleMeshDeformation)(::fwServices::IController) ) ;

    VTKSIMPLEMESH_API static const ::fwCom::Slots::SlotKeyType s_START_DEFORMATION_SLOT;
    typedef ::fwCom::Slot<void()> StartDeformationSlotType;

    VTKSIMPLEMESH_API static const ::fwCom::Slots::SlotKeyType s_STOP_DEFORMATION_SLOT;
    typedef ::fwCom::Slot<void()> StopDeformationSlotType;

    /// Constructor
    VTKSIMPLEMESH_API SSimpleMeshDeformation() throw() ;

    /// Destructor
    VTKSIMPLEMESH_API virtual ~SSimpleMeshDeformation() throw() ;

protected :

    /// Starting method. Initialize timer.
    VTKSIMPLEMESH_API virtual void starting() throw(fwTools::Failed);


    /// Configuring method.
    VTKSIMPLEMESH_API virtual void configuring() throw(::fwTools::Failed);


    /// Stopping method.
    VTKSIMPLEMESH_API virtual void stopping() throw(fwTools::Failed);

    /// Updating method. Deforms the mesh
    VTKSIMPLEMESH_API virtual void updating() throw(fwTools::Failed);

    /// Receiving.
    VTKSIMPLEMESH_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /// Starts mesh deformation
    VTKSIMPLEMESH_API void startDeformation();

    /// Stops mesh deformation
    VTKSIMPLEMESH_API void stopDeformation();

private:

    /// Slot to call startDeformation method
    StartDeformationSlotType::sptr m_slotStartDeformation;

    /// Slot to call stopDeformation method
    StopDeformationSlotType::sptr m_slotStopDeformation;

    /// Helper to copy quicly meshes ( only points, normals and point colors )
    void copyMesh( const ::fwData::Mesh::sptr & src, const ::fwData::Mesh::sptr & dest ) const;

    /// Copy associated mesh to m_mesh and initialize m_transformMesh
    void initMeshBackup();

    /// Helper to compute mesh deformation
    void computeDeformation( const ::fwData::Mesh::sptr & refMesh, const ::fwData::Mesh::sptr & transformMesh );

    /// Helper to compute mesh deformation
    void computeDeformation(
            const ::fwData::Mesh::sptr & refMesh,
            const ::fwData::Mesh::sptr & transformMesh,
            float deformationPercent );

    ::fwThread::Timer::sptr m_timer;

    /// Hight resolution timer to log information about computing function time
    ::fwCore::HiResTimer m_hiRestimer;

    /// Transformed version of original mesh
    ::fwData::Mesh::sptr m_transformMesh;

    /// Store original mesh
    ::fwData::Mesh::sptr m_mesh;

    /// Current increment for deformation
    float m_currentIncrement;

    /// Current deformation in percent
    float m_currentDeformation;
};

}

#endif /* __VTKSIMPLEMESH_SSIMPLEMESHDEFORMATION_HPP__ */
