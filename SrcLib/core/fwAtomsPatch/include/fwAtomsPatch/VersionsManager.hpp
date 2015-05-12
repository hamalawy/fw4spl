/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWATOMSPATCH_VERSIONSMANAGER_HPP__
#define __FWATOMSPATCH_VERSIONSMANAGER_HPP__

#include <map>
#include <vector>
#include <string>

#include <boost/filesystem/path.hpp>

#include <fwCore/mt/types.hpp>

#include "fwAtomsPatch/VersionsGraph.hpp"
#include "fwAtomsPatch/config.hpp"

namespace fwAtomsPatch
{

/**
 * @class VersionsManager
 * @brief Maintains a table of object versions and a table of links between objects.
 *
 * Version and link tables are built from .versions and .graphlink files.
 */
class FWATOMSPATCH_CLASS_API VersionsManager
{

public:

    /**
     * @name Typedefs
     * @{ */
    typedef std::vector< ::boost::filesystem::path > ListPathType;
    typedef std::map< std::string, VersionsGraph::sptr > VersionsGraphMapType;
    /**  @} */

    FWATOMSPATCH_API VersionsManager();
    FWATOMSPATCH_API ~VersionsManager();

    /**
     * @brief Returns graph corresponding to given context.
     * @param context context of the requested VersionGraph.
     * @note This method is thread safe.
     */
    FWATOMSPATCH_API VersionsGraph::sptr getGraph(const std::string& context);

    /**
     * @brief Generates a .versions file with currently known data versions.
     * @param filePath path of .versions file to write.
     * @param context context of the VersionGraph.
     * @param versionName name of the version to write.
     * @todo Ignoring file in ignored library.
     */
    FWATOMSPATCH_API static void generateNewFile(const ::boost::filesystem::path& filePath, const std::string& context,
                                                 const std::string& versionName);

    /**
     * @brief Builds version table.
     * @param dirPath directory to search for .versions files
     * @note This method is thread safe.
     */
    FWATOMSPATCH_API void buildVersionTable(const std::string& dirPath);

    /**
     * @brief Builds link table.
     * @param dirPath directory to search for .graphlink files
     * @note This method is thread safe.
     */
    FWATOMSPATCH_API void buildLinkTable(const std::string& dirPath);

    /// Returns the default instance of VersionsManager
    static SPTR(VersionsManager) getDefault() {
        return s_default;
    }

private:


    /**
     * @brief Loads a file containing object versions.
     * @param filePath path to a .versions file
     */
    FWATOMSPATCH_API static ::fwAtomsPatch::VersionDescriptor getVersion(
        const ::boost::filesystem::path& filePath);

    /**
     * @brief Loads a file containing links between objects versions.
     * @param filePath path to a .graphlink file
     */
    FWATOMSPATCH_API static ::fwAtomsPatch::LinkDescriptor getLink(
        const ::boost::filesystem::path& filePath);

    /**
     * @brief Generates the versions graph.
     * @note This method is NOT thread safe.
     */
    FWATOMSPATCH_API void generateVersionsGraph();

    /// Mutex to protect m_versionTable access;
    ::fwCore::mt::ReadWriteMutex m_versionMutex;

    /// Mutex to protect m_linkTable access;
    ::fwCore::mt::ReadWriteMutex m_linkMutex;

    /// Mutex to protect m_versionsGraphMap access;
    ::fwCore::mt::ReadWriteMutex m_graphMutex;

    /// Version table
    ListPathType m_versionTable;

    /// Link table
    ListPathType m_linkTable;

    /// Versions graphs
    VersionsGraphMapType m_versionsGraphMap;

    /// Default instance of Versions Manager
    FWATOMSPATCH_API static SPTR(VersionsManager) s_default;
};

} // fwAtomsPatch

#endif /* __FWATOMSPATCH_VERSIONSMANAGER_HPP__ */
