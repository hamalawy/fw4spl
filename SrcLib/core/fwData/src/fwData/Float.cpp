/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/registry/macros.hpp"

#include "fwData/Object.hpp"
#include "fwData/GenericField.hpp"
#include "fwData/Float.hpp"

fwDataRegisterMacro( ::fwData::Float );

namespace fwData
{
//------------------------------------------------------------------------------

Float::Float( ) throw()
{}


//------------------------------------------------------------------------------

Float::Float( ::fwData::Object::Key key ) throw()
{}

//------------------------------------------------------------------------------

Float::~Float() throw()
{}

//------------------------------------------------------------------------------
void Float::shallowCopy( Float::csptr _source )
{
    this->fieldShallowCopy( _source );
    m_value = _source->m_value;
}

//------------------------------------------------------------------------------

void Float::deepCopy( Float::csptr _source )
{
    this->fieldDeepCopy( _source );
    m_value = _source->m_value;
}


} // namespace fwData
