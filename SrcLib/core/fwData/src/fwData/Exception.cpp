/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/Exception.hpp"

namespace fwData
{

Exception::Exception(const std::string &message) throw()
    : ::fwCore::Exception(message)
{
}


Exception::~Exception() throw()
{
}



} // namespace fwData
