/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/registry/macros.hpp"
#include "fwData/Exception.hpp"

#include "fwData/Material.hpp"

fwDataRegisterMacro( ::fwData::Material );

namespace fwData
{

Material::Material(::fwData::Object::Key key) :
    m_shadingMode(MODE_PHONG),
    m_representationMode(MODE_SURFACE),
    m_optionsMode(MODE_STANDARD),
    m_ambient( Color::New() ),
    m_diffuse( Color::New() )
{
}

//------------------------------------------------------------------------------

Material::~Material()
{
}

//------------------------------------------------------------------------------

void Material::shallowCopy(const Object::csptr &_source )
{
    Material::csptr other = Material::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );

    m_ambient = other->m_ambient;
    m_diffuse = other->m_diffuse;

    m_shadingMode = other->m_shadingMode;
    m_representationMode = other->m_representationMode;
    m_optionsMode = other->m_optionsMode;
}

//------------------------------------------------------------------------------

void Material::deepCopy(const Object::csptr &_source )
{
    Material::csptr other = Material::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source );

    m_ambient->deepCopy( other->m_ambient );
    m_diffuse->deepCopy( other->m_diffuse );

    m_shadingMode = other->m_shadingMode;
    m_representationMode = other->m_representationMode;
    m_optionsMode = other->m_optionsMode;
}

//------------------------------------------------------------------------------

Color::sptr Material::ambient() const
{
    return m_ambient ;
}

//------------------------------------------------------------------------------

Color::sptr Material::diffuse() const
{
    return m_diffuse ;
}

//------------------------------------------------------------------------------

void Material::setAmbient(Color::sptr ambient)
{
    m_ambient = ambient;
}

//------------------------------------------------------------------------------

void Material::setDiffuse(Color::sptr diffuse)
{
    m_diffuse = diffuse;
}

//------------------------------------------------------------------------------

}
