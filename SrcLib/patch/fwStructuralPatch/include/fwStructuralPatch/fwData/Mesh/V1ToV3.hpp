/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSTRUCTURALPATCH_FWDATA_MESH_V1TOV3_HPP__
#define __FWSTRUCTURALPATCH_FWDATA_MESH_V1TOV3_HPP__

#include <fwAtomsPatch/IStructuralPatch.hpp>

#include "fwStructuralPatch/config.hpp"

namespace fwStructuralPatch
{

namespace fwData
{

namespace Mesh
{

/// Structural patch to convert a fwData::Mesh from version '1' to version '3'.
class FWSTRUCTURALPATCH_CLASS_API V1ToV3 : public ::fwAtomsPatch::IStructuralPatch
{
public:
    fwCoreClassDefinitionsWithFactoryMacro(
        (V1ToV3)(::fwStructuralPatch::fwData::Mesh::V1ToV3), (()), new V1ToV3);

    /// Constructor
    FWSTRUCTURALPATCH_API V1ToV3();

    /// Destructor
    FWSTRUCTURALPATCH_API ~V1ToV3();

    /// Copy constructor
    FWSTRUCTURALPATCH_API V1ToV3( const V1ToV3 &cpy );

    /**
     * @brief Applies patch
     *
     * Added new map of additional array.
     */
    FWSTRUCTURALPATCH_API virtual void apply(
        const ::fwAtoms::Object::sptr& previous,
        const ::fwAtoms::Object::sptr& current,
        ::fwAtomsPatch::IPatch::NewVersionsType& newVersions);

};


} // namespace Mesh

} // namespace fwData

} // namespace fwStructuralPatch

#endif /* __FWSTRUCTURALPATCH_FWDATA_MESH_V1TOV3_HPP__ */


