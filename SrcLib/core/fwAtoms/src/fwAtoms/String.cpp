/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwAtoms/String.hpp"

#include <string>
#include <boost/any.hpp>

fwCampImplementMacro((fwAtoms)(String))
{
    builder
        .tag("object_version", "1")
        .tag("lib_name", "fwAtoms")
        .base< ::fwAtoms::Base>()
        .property("value", &::fwAtoms::String::getString, &::fwAtoms::String::setString);
}

namespace fwAtoms
{

String::String(const std::string& value) : m_value(value)
{}

String::sptr String::New(std::string value)
{
    String::sptr valueSptr(new String(value));
    return valueSptr;

}
//------------------------------------------------------------------------------

Base::sptr String::clone()
{
    return String::New(m_value);
}

}
