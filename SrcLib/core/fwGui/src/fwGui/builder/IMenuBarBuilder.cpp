/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

/**
 * @file fwGui/IMenuBarBuilder.cpp
 * @brief This file defines the implementation of the interface class  for the menubar builder.
 *
 * @author IRCAD (Research and Development Team).
 * @date 2009-2010
 */

#include <boost/foreach.hpp>

#include "fwGui/builder/IMenuBarBuilder.hpp"

namespace fwGui
{
namespace builder
{

const IMenuBarBuilder::RegistryKeyType IMenuBarBuilder::REGISTRY_KEY = "::fwGui::MenuBarBuilder";

//-----------------------------------------------------------------------------

IMenuBarBuilder::IMenuBarBuilder()
{}

//-----------------------------------------------------------------------------

IMenuBarBuilder::~IMenuBarBuilder()
{}

//-----------------------------------------------------------------------------

void IMenuBarBuilder::initialize( ::fwRuntime::ConfigurationElement::sptr configuration)
{

}

//-----------------------------------------------------------------------------

::fwGui::container::fwMenuBar::sptr IMenuBarBuilder::getMenuBar()
{
    return this->m_menuBar;
}

//-----------------------------------------------------------------------------

} // namespace builder
} // namespace fwGui



