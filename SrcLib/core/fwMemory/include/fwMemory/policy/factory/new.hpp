/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWMEMORY_POLICY_FACTORY_NEW_HPP__
#define __FWMEMORY_POLICY_FACTORY_NEW_HPP__

#include <string>

#include <boost/make_shared.hpp>

#include <fwTools/macros.hpp>

#include "fwMemory/config.hpp"
#include "fwMemory/policy/registry/detail.hpp"

namespace fwMemory
{

class GuiBaseObject;

namespace policy
{
namespace factory
{

template<class CLASSNAME > SPTR( CLASSNAME )  New();


FWMEMORY_API SPTR( ::fwMemory::IPolicy ) New( const ::fwMemory::policy::registry::KeyType & classname );

template<class CLASSNAME > SPTR( CLASSNAME )  New()
{
    SPTR(CLASSNAME) policy = ::boost::make_shared< CLASSNAME >();
    return policy;
}

} // namespace factory
} // namespace policy
} // namespace fwMemory

#endif /* __FWMEMORY_POLICY_FACTORY_NEW_HPP__ */


