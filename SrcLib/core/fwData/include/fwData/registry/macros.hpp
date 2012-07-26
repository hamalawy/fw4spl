/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_REGISTRY_MACRO_HPP__
#define __FWDATA_REGISTRY_MACRO_HPP__

#include "fwData/registry/detail.hpp"

namespace fwData
{
namespace registry
{

#define fwDataRegisterMacro( classname )                                  \
    static ::fwData::Object::Registrar< classname > s__factory__record__ ## __LINE__ ;

} // end namespace registry
} // end namespace fwData

#endif /*__FWDATA_REGISTRY_MACRO_HPP__*/
