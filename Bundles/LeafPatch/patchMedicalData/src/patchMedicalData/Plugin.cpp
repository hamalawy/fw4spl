/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include <fwStructuralPatch/PatchLoader.hpp>

#include <fwMDSemanticPatch/PatchLoader.hpp>

#include "patchMedicalData/Plugin.hpp"

namespace patchMedicalData
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::patchMedicalData::Plugin");

Plugin::~Plugin() throw()
{}

void Plugin::start() throw(::fwRuntime::RuntimeException)
{
    //Hack: force link with fwStructuralPatch
    ::fwStructuralPatch::PatchLoader::loadPatches();

    /* Force link with fwMDSemanticPatch and compute version/link tables
     * for fwMDSemanticPatch data.
     */
    ::fwMDSemanticPatch::PatchLoader::loadPatches();
}

void Plugin::stop() throw()
{}

} // namespace patchMedicalData
