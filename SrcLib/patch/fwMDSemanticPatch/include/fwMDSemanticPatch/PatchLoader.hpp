/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWMDSEMANTICPATCH_PATCHLOADER_HPP__
#define __FWMDSEMANTICPATCH_PATCHLOADER_HPP__

#include "fwMDSemanticPatch/config.hpp"

namespace fwMDSemanticPatch
{

/// Loads patches by processing .versions and .graphlink files.
class PatchLoader
{
public:
    /// Computes VersionsManager's link and version tables.
    FWMDSEMANTICPATCH_API static void loadPatches();
};

} //namespace fwMDSemanticPatch

#endif /* __FWMDSEMANTICPATCH_PATCHLOADER_HPP__ */

