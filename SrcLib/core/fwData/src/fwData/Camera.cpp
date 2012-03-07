/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>
#include "fwData/registry/macros.hpp"

#include "fwData/Video.hpp"

#include "fwData/Camera.hpp"

REGISTER_DATA( ::fwData::Camera );
namespace fwData
{
//------------------------------------------------------------------------------

Camera::Camera()
{
    SLM_TRACE_FUNC();
    m_extrinsicMatrix = ::fwData::TransformationMatrix3D::New();
    m_intrinsicMatrix = ::fwData::TransformationMatrix3D::New();
    uint32HL def;
    def.Low = 0;
    def.High = 0;
    m_cameraGUID = std::make_pair(false, def);
    m_dataAvailable = false ;
}

//------------------------------------------------------------------------------

Camera::~Camera() throw()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

bool &Camera::dataAvailable()
{
    return m_dataAvailable;
}

//------------------------------------------------------------------------------

void Camera::setExtrinsicCameraTransformation( ::fwData::TransformationMatrix3D::sptr  transMat)
{
    m_extrinsicMatrix = transMat;
}

//------------------------------------------------------------------------------

void Camera::setIntrinsicCameraTransformation( ::fwData::TransformationMatrix3D::sptr  transMat)
{
    m_intrinsicMatrix = transMat;
}

//------------------------------------------------------------------------------

void Camera::setCameraGUID(uint32HL _cameraGUID){
    m_cameraGUID.first = true;
    m_cameraGUID.second = _cameraGUID;
}

} // namespace fwData
