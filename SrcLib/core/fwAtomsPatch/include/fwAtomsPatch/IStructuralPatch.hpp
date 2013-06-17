/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWATOMSPATCH_ISTRUCTURALPATCH_HPP__
#define __FWATOMSPATCH_ISTRUCTURALPATCH_HPP__

#include <string>

#include <fwCore/base.hpp>

#include "fwAtomsPatch/helper/Object.hpp"
#include "fwAtomsPatch/IPatch.hpp"
#include "fwAtomsPatch/config.hpp"


namespace fwAtoms
{
class Object;
}

namespace fwAtomsPatch
{

/**
 * @class IStructuralPatch
 * @brief Base class of all structural patches
 */
class FWATOMSPATCH_CLASS_API IStructuralPatch : public IPatch
{

public:
    fwCoreNonInstanciableClassDefinitionsMacro((IStructuralPatch));
    fwCoreAllowSharedFromThis();

    ///Constructor
    FWATOMSPATCH_API IStructuralPatch();

    ///Copy constructor
    FWATOMSPATCH_API IStructuralPatch( const IStructuralPatch &cpy );

    ///Destructor
    FWATOMSPATCH_API virtual ~IStructuralPatch();

    /**
     * @brief Applies the patch to the specified object
     *
     * @param previous object from which modifications will be applied
     * @param current object receiving modifications
     * @param newVersions map containing already patched sub objects
     *
     * @pre previous and current objects must have the same classname and version.
     */
    FWATOMSPATCH_API virtual void apply(const SPTR(::fwAtoms::Object)& previous,
                                        const SPTR(::fwAtoms::Object)& current,
                                        ::fwAtomsPatch::IPatch::NewVersionsType& newVersions);

    /// Returns the target classname of the object
    FWATOMSPATCH_API virtual const std::string& getTargetClassname() const;

    /// Returns the target version of the object
    FWATOMSPATCH_API virtual const std::string& getTargetVersion() const;

protected:

    /// Updates the version of the patched object
    FWATOMSPATCH_API void updateVersion(SPTR(::fwAtoms::Object) current);

    /// Target type
    std::string m_targetClassname;

    /// Target version
    std::string m_targetVersion;
};


} // namespace fwAtomsPatch

#endif /* __FWATOMSPATCH_ISTRUCTURALPATCH_HPP__ */
