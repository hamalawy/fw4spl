/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include "qvtkSimpleMesh/Plugin.hpp"

namespace qvtkSimpleMesh
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::qvtkSimpleMesh::Plugin");

Plugin::~Plugin() throw()
{}

void Plugin::start() throw(::fwRuntime::RuntimeException)
{}

void Plugin::stop() throw()
{}

} // namespace vtkSimpleMesh
