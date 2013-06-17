/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fstream>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/exceptions.hpp>

#include <camp/class.hpp>

#include <fwCore/spyLog.hpp>

#include "fwAtomsPatch/VersionsManager.hpp"
#include "fwAtomsPatch/exceptions/MissingInformation.hpp"
#include "fwAtomsPatch/exceptions/BadExtension.hpp"
#include "fwAtomsPatch/types.hpp"



namespace fwAtomsPatch {

std::string getValue(const ::boost::property_tree::ptree& node, const std::string& name,
                     const ::boost::filesystem::path& filePath )
{
    std::string value;
    try
    {
        value = node.get< std::string >(name);
    }
    catch( ::boost::property_tree::ptree_bad_path & )
    {
        FW_RAISE_EXCEPTION(::fwAtomsPatch::exceptions::MissingInformation(
                               name + " information are missing in '"+ filePath.string() +"'."));
    }

    return value;
}

SPTR(VersionsManager) VersionsManager::s_default = ::boost::make_shared<VersionsManager>();

// ----------------------------------------------------------------------------

VersionsManager::VersionsManager()
{
}

// ----------------------------------------------------------------------------

VersionsManager::~VersionsManager()
{
}

// ----------------------------------------------------------------------------

void VersionsManager::buildVersionTable(const std::string& dirPath)
{
    ::fwCore::mt::WriteLock lock(m_versionMutex);
    for (::boost::filesystem::recursive_directory_iterator end, dir(dirPath); dir != end; ++dir)
    {
        if(  !::boost::filesystem::is_directory(*dir)
          && ::boost::filesystem::extension(*dir) == ".versions")
        {
            m_versionTable.push_back((*dir).path());
        }
    }
}

// ----------------------------------------------------------------------------

void VersionsManager::buildLinkTable(const std::string& dirPath)
{
    ::fwCore::mt::WriteLock lock(m_linkMutex);
    for ( ::boost::filesystem::recursive_directory_iterator end, dir(dirPath);
        dir != end; ++dir )
    {
        if(  !::boost::filesystem::is_directory(*dir)
          && ::boost::filesystem::extension(*dir) == ".graphlink")
        {
            m_linkTable.push_back((*dir).path());
        }
    }
}

// ----------------------------------------------------------------------------

void VersionsManager::generateNewFile(const ::boost::filesystem::path& filePath,
                                      const std::string& context, const std::string& versionName)
{
    FW_RAISE_EXCEPTION_IF( ::fwAtomsPatch::exceptions::BadExtension(".versions file required"),
            filePath.extension() != ".versions");

    namespace pt = ::boost::property_tree;
    std::size_t classCount = ::camp::classCount();
    pt::ptree root;

    root.put("context", context);
    root.put("version_name", versionName);

    pt::ptree versions;
    for (int i = 0; i < classCount; ++i)
    {
        const ::camp::Class& metaclass = ::camp::classByIndex(i);
        const std::string& className = metaclass.name();

        if (metaclass.hasTag(::fwAtomsPatch::s_OBJ_VERSION))
        {
            const ::camp::Value& value = metaclass.tag(::fwAtomsPatch::s_OBJ_VERSION);
            versions.put(className, value.to<std::string>());
        }
    }

    root.push_back( pt::ptree::value_type("versions", pt::ptree(versions)));
    pt::json_parser::write_json(filePath.string(), root);
}

// ----------------------------------------------------------------------------

::fwAtomsPatch::VersionDescriptor VersionsManager::getVersion(const ::boost::filesystem::path& filePath)
{
    FW_RAISE_EXCEPTION_IF( ::fwAtomsPatch::exceptions::BadExtension(".versions file required"),
            filePath.extension() != ".versions");

    namespace pt = ::boost::property_tree;
    pt::ptree root;

    ::fwAtomsPatch::VersionDescriptor::VersionsType versionids;

    std::ifstream file(filePath.string().c_str());
    std::istream input(file.rdbuf());
    pt::json_parser::read_json(input, root);
    const std::string& context = getValue(root, "context", filePath);
    const std::string& versionName = getValue(root, "version_name", filePath);

    BOOST_FOREACH(pt::ptree::value_type &node, root.get_child("versions"))
    {
        versionids[node.first] = std::string(node.second.data().c_str());
    }

    file.close();

    return ::fwAtomsPatch::VersionDescriptor(context, versionName, versionids);
}

// ----------------------------------------------------------------------------

::fwAtomsPatch::LinkDescriptor VersionsManager::getLink(const ::boost::filesystem::path& filePath)
{
    FW_RAISE_EXCEPTION_IF( ::fwAtomsPatch::exceptions::BadExtension(".graphlink file required"),
            filePath.extension() != ".graphlink");

    namespace pt = ::boost::property_tree;
    typedef std::vector< std::pair< std::string, std::string > > LinkType;

    LinkType link;
    ::fwAtomsPatch::LinkDescriptor::LinksType links;
    pt::ptree root;

    std::ifstream file(filePath.string().c_str());
    std::istream input(file.rdbuf());
    pt::json_parser::read_json(input, root);

    const std::string& context = getValue(root, "context", filePath);
    const std::string& originVersion = getValue(root, "origin_version", filePath);
    const std::string& targetVersion = getValue(root, "target_version", filePath);

    const std::string& patcher = root.get("patcher", "DefaultPatcher");

    BOOST_FOREACH(pt::ptree::value_type &child, root.get_child("links"))
    {
        BOOST_FOREACH(pt::ptree::value_type &node, (child.second).get_child(""))
        {
            link.push_back(std::make_pair(node.first, node.second.data()));
        }

        FW_RAISE_EXCEPTION_IF(::fwAtomsPatch::exceptions::MissingInformation(
                        "A link should contain an origin version and a target version."), link.size() != 2);

        links[link[0]] = link[1];

        // Clear the vector
        LinkType().swap(link);
    }

    file.close();

    return ::fwAtomsPatch::LinkDescriptor(context, originVersion, targetVersion, patcher, links);
}

// ----------------------------------------------------------------------------

void VersionsManager::generateVersionsGraph()
{
    {
        ::fwCore::mt::ReadLock versionLock(m_versionMutex);
        //For every versions
        BOOST_FOREACH(VersionsManager::ListPathType::value_type elt, m_versionTable)
        {
            ::fwAtomsPatch::VersionDescriptor version = VersionsManager::getVersion(elt);

            if(m_versionsGraphMap.find(version.getContext()) == m_versionsGraphMap.end())
            {
                m_versionsGraphMap[version.getContext()] = VersionsGraph::New();
            }

            m_versionsGraphMap[version.getContext()]->addNode(version);
        }
    }

    {
        ::fwCore::mt::ReadLock linkLock(m_linkMutex);
        //For every links
        BOOST_FOREACH(VersionsManager::ListPathType::value_type elt, m_linkTable)
        {
            ::fwAtomsPatch::LinkDescriptor link = VersionsManager::getLink(elt);

            OSLM_ASSERT("There is no graph created for the context \"" << link.getContext() << "\".",
                        m_versionsGraphMap.find(link.getContext()) != m_versionsGraphMap.end());

            m_versionsGraphMap[link.getContext()]->addEdge(link);
        }
    }
}

// ----------------------------------------------------------------------------

VersionsGraph::sptr VersionsManager::getGraph(const std::string& context)
{
    VersionsGraph::sptr vg;

    ::fwCore::mt::ReadToWriteLock lock(m_graphMutex);
    if (m_versionsGraphMap.empty())
    {
        ::fwCore::mt::UpgradeToWriteLock writeLock(lock);
        VersionsManager::generateVersionsGraph();
    }

    VersionsGraphMapType::iterator elem = m_versionsGraphMap.find(context);
    if (elem != m_versionsGraphMap.end())
    {
        vg = elem->second;
    }

    return vg;
}

} // namespace fwAtomsPatch

