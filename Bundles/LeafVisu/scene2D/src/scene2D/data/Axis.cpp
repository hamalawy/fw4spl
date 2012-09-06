/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwData/registry/macros.hpp>

#include "scene2D/data/Axis.hpp"

namespace scene2D
{
namespace data
{

fwDataRegisterMacro( ::scene2D::data::Axis );

Axis::Axis( ::fwData::Object::Key key )
: m_origin (0),
  m_scale (1),
  m_scaleType (LINEAR)
{}

Axis::~Axis()
{
}

float Axis::getOrigin()
{
    return m_origin;
}

void Axis::setOrigin (float _origin)
{
    m_origin = _origin;
}

float Axis::getScale()
{
    return m_scale;
}

void Axis::setScale (float _scale)
{
    m_scale = _scale;
}

std::string Axis::getScaleType()
{
    return (m_scaleType == LINEAR)?"LINEAR":"LOG";
}

void Axis::setScaleType (std::string _scaleType)
{
    if (_scaleType == "LINEAR")
    {
        m_scaleType = LINEAR;
    }
    else if (_scaleType == "LOG")
    {
        m_scaleType = LOG;
    }
}

} // namespace data
} // namespace scene2D



