/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>
#include <fwData/registry/macros.hpp>
#include <fwData/Exception.hpp>

#include "scene2D/data/Viewport.hpp"

namespace scene2D
{
namespace data
{

fwDataRegisterMacro( ::scene2D::data::Viewport );

//-----------------------------------------------------------------------------

Viewport::Viewport(::fwData::Object::Key key)
: m_x (200),
  m_y (200),
  m_width (400),
  m_height (400)
{}

//-----------------------------------------------------------------------------

Viewport::~Viewport()
{
}

//------------------------------------------------------------------------------

void Viewport::shallowCopy(const ::fwData::Object::csptr &_source )
{
    Viewport::csptr other = Viewport::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );

    m_x = other->m_x;
    m_y = other->m_y;
    m_width = other->m_width;
    m_height = other->m_height;
}

//------------------------------------------------------------------------------

void Viewport::cachedDeepCopy(const ::fwData::Object::csptr &_source, DeepCopyCacheType &cache )
{
    Viewport::csptr other = Viewport::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source, cache );

    m_x = other->m_x;
    m_y = other->m_y;
    m_width = other->m_width;
    m_height = other->m_height;
}

//------------------------------------------------------------------------------

float Viewport::getX()
{
    return m_x;
}

//-----------------------------------------------------------------------------

void Viewport::setX (float _x)
{
    m_x = _x;
}

//-----------------------------------------------------------------------------

float Viewport::getY()
{
    return m_y;
}

//-----------------------------------------------------------------------------

void Viewport::setY (float _y)
{
    m_y = _y;
}

//-----------------------------------------------------------------------------

float Viewport::getWidth()
{
    return m_width;
}

//-----------------------------------------------------------------------------

void Viewport::setWidth (float _width)
{
    m_width = _width;
}

//-----------------------------------------------------------------------------

float Viewport::getHeight()
{
    return m_height;
}

//-----------------------------------------------------------------------------

void Viewport::setHeight (float _height)
{
    m_height = _height;
}

} // namespace data
} // namespace scene2D



