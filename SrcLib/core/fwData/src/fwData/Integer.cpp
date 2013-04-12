/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/registry/macros.hpp"
#include "fwData/Exception.hpp"

#include "fwData/Object.hpp"
#include "fwData/GenericField.hpp"
#include "fwData/Integer.hpp"


fwDataRegisterMacro( ::fwData::Integer );

namespace fwData
{
//------------------------------------------------------------------------------

Integer::Integer(::fwData::Object::Key key ) throw()
{}

//------------------------------------------------------------------------------

Integer::~Integer() throw()
{}

//------------------------------------------------------------------------------

void Integer::shallowCopy(const Object::csptr &_source )
{
    Integer::csptr other = Integer::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );
    m_value = other->m_value;
}

//------------------------------------------------------------------------------

void Integer::deepCopy(const Object::csptr &_source )
{
    Integer::csptr other = Integer::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source );
    m_value = other->m_value;
}

} // namespace fwData
