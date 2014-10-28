/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

/**
 * @file fwGui/builder/IContainerBuilder.cpp
 * @brief This file defines the implementation of the interface class  for the menubar builder.
 *
 * 
 * @date 2009-2010
 */

#include <boost/foreach.hpp>

#include "fwGui/builder/IContainerBuilder.hpp"

namespace fwGui
{
namespace builder
{

const IContainerBuilder::RegistryKeyType IContainerBuilder::REGISTRY_KEY = "::fwGui::ContainerBuilder";

//-----------------------------------------------------------------------------

IContainerBuilder::IContainerBuilder()
{}

//-----------------------------------------------------------------------------

IContainerBuilder::~IContainerBuilder()
{}

//-----------------------------------------------------------------------------

void IContainerBuilder::initialize( ::fwRuntime::ConfigurationElement::sptr configuration)
{}

//-----------------------------------------------------------------------------

::fwGui::container::fwContainer::sptr IContainerBuilder::getContainer()
{
    return this->m_container;
}

//-----------------------------------------------------------------------------

} // namespace builder
} // namespace fwGui



