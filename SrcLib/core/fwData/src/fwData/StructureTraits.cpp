/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include "fwData/registry/macros.hpp"
#include "fwData/StructureTraits.hpp"

fwDataRegisterMacro( ::fwData::StructureTraits );

namespace fwData
{
//------------------------------------------------------------------------------

StructureTraits::StructureTraits(::fwData::Object::Key key) : m_anatomicRegion(""),
                                                              m_propertyCategory(""),
                                                              m_propertyType("")
{
    m_color = ::fwData::Color::New();
}

//------------------------------------------------------------------------------

StructureTraits::~StructureTraits ()
{}

//------------------------------------------------------------------------------

} // namespace fwData


