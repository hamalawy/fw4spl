/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#ifndef _FWXML_SERIALIZER_HPP_
#define _FWXML_SERIALIZER_HPP_

#include <boost/filesystem/path.hpp>

#include <libxml/tree.h>

#include <fwData/Object.hpp>
#include <fwTools/Failed.hpp>
#include <fwTools/ProgressAdviser.hpp>

#include <fwMemory/IPolicy.hpp>

#include "fwXML/config.hpp"
#include "fwXML/policy/ISplitPolicy.hpp"
#include "fwXML/policy/IPathPolicy.hpp"


namespace fwXML
{

/*
 * @brief (De)serialize object
 * @author IRCAD (Research and Development Team).
 */


class FWXML_CLASS_API Serializer : public ::fwTools::ProgressAdviser
{
    friend class XMLPatch;
public:

    FWXML_API Serializer();


    FWXML_API virtual ~Serializer();

    /// set/get rootFolder for serialization
    static ::boost::filesystem::path &rootFolder() { return m_rootFolder;}

    /// serialize the given object in the given rootFolder(), saving schema is changeable
    FWXML_API void serialize(::fwData::Object::sptr object,bool saveSchema = true) throw (::fwTools::Failed);

    /// deserialize to the given object, UPDATE rootFolde static ivar
    FWXML_API ::fwData::Object::sptr deSerialize(boost::filesystem::path filePath, bool validateWithSchema = true) throw (::fwTools::Failed);

    /// allow to change Path policy : by default use "ClassName-PTRADDR.xml"
    FWXML_API void setPathPolicy( IPathPolicy::sptr newPathPolicy);

    /// allow to change split policy : by default always split
    FWXML_API void setSplitPolicy( ISplitPolicy::sptr newSplitPolicy);

    // TODO move to XMLTranslatorHelper
    /**
     * @brief helper (factory which create a fwTools::Object form
     * \li xmlNodePtr->name for class
     * \li xmlNodePtr child for fwTools::Object information
     * @param loadExtraXML : if true extra XML information (exple image buffer are also loaded)
     */
    ::fwData::Object::sptr ObjectsFromXml( xmlNodePtr xmlNode );

protected :

    std::string m_processedXMLFile;

    FWXML_API static std::string getIDFormXML( xmlNodePtr xmlNode );

    FWXML_API static std::string getClassNameFromXML( xmlNodePtr xmlNode);

    FWXML_API static std::string translateID( bool xmlid, bool generateNewUUID);

    /// here rootFolder is mandatory *static* because sub classes processing serialization must k,ow the basename of patient Folder
    FWXML_API static ::boost::filesystem::path m_rootFolder;

    /// Dump policy
    ::fwMemory::IPolicy::sptr m_oldPolicy;

};

} // namespace fwXML

#endif // _FWXML_SERIALIZER_HPP_
