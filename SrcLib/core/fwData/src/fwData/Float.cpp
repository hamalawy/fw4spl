/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/registry/macros.hpp"
#include "fwData/Exception.hpp"

#include "fwData/Object.hpp"
#include "fwData/GenericField.hpp"
#include "fwData/Float.hpp"

fwDataRegisterMacro( ::fwData::Float );

namespace fwData
{
//------------------------------------------------------------------------------

Float::Float( ) throw()
{
}


//------------------------------------------------------------------------------

Float::Float( ::fwData::Object::Key key ) throw()
{
}

//------------------------------------------------------------------------------

Float::~Float() throw()
{
}

//------------------------------------------------------------------------------
void Float::shallowCopy(const Object::csptr &_source )
{
    Float::csptr other = Float::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );
    m_value = other->m_value;
}

//------------------------------------------------------------------------------

void Float::cachedDeepCopy(const Object::csptr &_source, DeepCopyCacheType &cache)
{
    Float::csptr other = Float::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source, cache );
    m_value = other->m_value;
}


} // namespace fwData
