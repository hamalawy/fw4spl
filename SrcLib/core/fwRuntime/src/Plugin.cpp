/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwRuntime/Plugin.hpp"
#include "fwRuntime/ConfigurationElement.hpp"

namespace fwRuntime
{

//------------------------------------------------------------------------------

::boost::shared_ptr<Bundle> Plugin::getBundle() const throw()
{
    return m_bundle;
}

//------------------------------------------------------------------------------

void Plugin::setInitializationData(const ConfigurationElement::sptr) throw()
{}

//------------------------------------------------------------------------------

void Plugin::setBundle( ::boost::shared_ptr<Bundle> bundle) throw()
{
    m_bundle = bundle;
}

//------------------------------------------------------------------------------

} // namespace fwRuntime
