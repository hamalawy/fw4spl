#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>

#include <fwTools/UUID.hpp>
#include <fwMemory/BufferManager.hpp>

#include <fwAtoms/Base.hpp>
#include <fwAtoms/Blob.hpp>
#include <fwAtoms/Boolean.hpp>
#include <fwAtoms/Map.hpp>
#include <fwAtoms/Numeric.hpp>
#include <fwAtoms/Object.hpp>
#include <fwAtoms/Sequence.hpp>
#include <fwAtoms/String.hpp>

#include <fwMemory/BufferManager.hpp>

#include <fwZip/IWriteArchive.hpp>

#include "fwAtomsBoostIO/Writer.hpp"



namespace fwAtomsBoostIO
{

const std::string Writer::s_VERSION = "1";
const std::string Writer::s_ATOMS_VERSION_KEY = "atoms_version";
const std::string Writer::s_WRITER_VERSION_KEY = "writer_version";

//-----------------------------------------------------------------------------

struct AtomVisitor
{

typedef std::map< ::fwAtoms::Base::sptr, ::boost::property_tree::ptree > PropTreeCacheType;

PropTreeCacheType m_cache;
::fwZip::IWriteArchive::sptr m_archive;
const std::string m_dirPrefix;


AtomVisitor( ::fwZip::IWriteArchive::sptr archive, const std::string &dirPrefix ) :
    m_archive(archive), m_dirPrefix(dirPrefix)
{}

//-----------------------------------------------------------------------------

PropTreeCacheType::mapped_type hitCache(const PropTreeCacheType::key_type &atom) const
{
    PropTreeCacheType::const_iterator iter = m_cache.find(atom);
    if(iter != m_cache.end())
    {
        return iter->second;
    }
    return PropTreeCacheType::mapped_type();
}

//-----------------------------------------------------------------------------

void cache(const PropTreeCacheType::key_type &atom, const std::string &ptpath)
{
    ::boost::property_tree::ptree ref;
    ref.put("ref", ptpath );
    m_cache.insert( PropTreeCacheType::value_type( atom, ref ) );
}

//-----------------------------------------------------------------------------

::boost::property_tree::ptree visit(const ::fwAtoms::Boolean::sptr &atom, const std::string &ptpath)
{
    ::boost::property_tree::ptree pt;
    this->cache(atom, ptpath);
    pt.put("boolean.value", atom->getValue()?"true":"false");
    return pt;
}

//-----------------------------------------------------------------------------

::boost::property_tree::ptree visit(const ::fwAtoms::Numeric::sptr &atom, const std::string &ptpath)
{
    ::boost::property_tree::ptree pt;
    this->cache(atom, ptpath);
    pt.put("numeric.value", atom->getString());
    return pt;
}

//-----------------------------------------------------------------------------

::boost::property_tree::ptree visit(const ::fwAtoms::String::sptr &atom, const std::string &ptpath)
{
    ::boost::property_tree::ptree pt;
    this->cache(atom, ptpath);
    pt.put("string.value", atom->getString());
    return pt;
}

//-----------------------------------------------------------------------------

::boost::property_tree::ptree visit(const ::fwAtoms::Map::sptr &atom, const std::string &ptpath)
{
    ::boost::property_tree::ptree pt;
    ::boost::property_tree::ptree map;
    this->cache(atom, ptpath);
    std::string path = ptpath + (ptpath.empty()?"":".") + "map";
    unsigned long long count = 0;
    BOOST_FOREACH(const ::fwAtoms::Map::MapType::value_type& elt, atom->getValue())
    {
        const std::string nodeName = "item_" + ::boost::lexical_cast< std::string >(count++);
        ::boost::property_tree::ptree mapChild;
        mapChild.put("key", elt.first);
        mapChild.add_child("value", this->visit(elt.second, path + "." + nodeName + ".value"));

        map.add_child(nodeName, mapChild);
    }
    pt.add_child("map", map);
    return pt;
}

//-----------------------------------------------------------------------------

::boost::property_tree::ptree visit(const ::fwAtoms::Sequence::sptr &atom, const std::string &ptpath)
{
    ::boost::property_tree::ptree pt;
    ::boost::property_tree::ptree seq;
    this->cache(atom, ptpath);
    std::string path = ptpath + (ptpath.empty()?"":".") + "sequence";

    unsigned long long count = 0;
    BOOST_FOREACH( const ::fwAtoms::Sequence::SequenceType::value_type& elt, atom->getValue())
    {
        const std::string nodeName = ::boost::lexical_cast< std::string >(count++);
        seq.add_child(nodeName, this->visit(elt, path + "." + nodeName));
    }
    pt.add_child("sequence", seq);
    return pt;
}

//-----------------------------------------------------------------------------

::boost::property_tree::ptree visit(const ::fwAtoms::Object::sptr &atom, const std::string &ptpath)
{
    ::boost::property_tree::ptree pt;
    ::boost::property_tree::ptree object;
    this->cache(atom, ptpath);
    std::string path = ptpath + (ptpath.empty()?"":".") + "object";

    const ::fwAtoms::Object::MetaInfosType& metaInfos = atom->getMetaInfos();
    ::boost::property_tree::ptree metaInfosPt;
    unsigned long long count = 0;
    BOOST_FOREACH(const ::fwAtoms::Object::MetaInfosType::value_type& info, metaInfos)
    {
        const std::string nodeName = "item_" + ::boost::lexical_cast< std::string >(count++);
        ::boost::property_tree::ptree item;
        item.put("key", info.first);
        item.put("value", info.second);
        metaInfosPt.push_back(::boost::property_tree::ptree::value_type(nodeName, item));
    }
    object.add_child("meta_infos", metaInfosPt);

    const ::fwAtoms::Object::AttributesType& attributes = atom->getAttributes();
    ::boost::property_tree::ptree attributesPt;
    BOOST_FOREACH(const ::fwAtoms::Object::AttributesType::value_type attr, attributes)
    {
        ::boost::property_tree::ptree childAttributes = this->visit(attr.second, path + ".attributes." + attr.first);
        attributesPt.add_child(attr.first, childAttributes);
    }
    object.add_child("attributes", attributesPt);

    pt.add_child("object", object);

    return pt;
}

//-----------------------------------------------------------------------------

::boost::property_tree::ptree visit(const ::fwAtoms::Blob::sptr &atom, const std::string &ptpath)
{
    ::boost::property_tree::ptree pt;
    this->cache(atom, ptpath);
    std::string path = ptpath + (ptpath.empty()?"":".") + "blob";

    std::string bufType = "raw";
    pt.put("blob.buffer_type", bufType);

    ::fwMemory::BufferObject::sptr buffObj = atom->getBufferObject();
    if (!buffObj || buffObj->getSize() == 0)
    {
        pt.put("blob.buffer_size", 0);
    }
    else
    {
        ::boost::filesystem::path bufFile = m_dirPrefix;
        size_t buffSize = buffObj->getSize();

        const ::fwMemory::BufferManager::StreamInfo streamInfo = buffObj->getStreamInfo();
        const ::boost::filesystem::path dumpedFile = streamInfo.fsFile;
        const ::fwMemory::FileFormatType& format = streamInfo.format;

        bufFile /= ::fwTools::UUID::generateUUID() + ".raw";

        if ( !dumpedFile.empty() && (format & ::fwMemory::RAW) )
        {
            m_archive->putFile(dumpedFile, bufFile);
        }
        else
        {
            SPTR(std::istream) is = streamInfo.stream;
            SLM_ASSERT("no istream", is);

            SPTR(std::ostream) os = m_archive->createFile(bufFile);
            *os << is->rdbuf();
        }

        pt.put("blob.buffer_size", buffSize);
        pt.put("blob.buffer", bufFile.generic_string());
    }

    return pt;
}

//-----------------------------------------------------------------------------

::boost::property_tree::ptree visit(const ::fwAtoms::Base::sptr &atom, std::string ptpath = "")
{
    ::boost::property_tree::ptree pt;
    ::boost::property_tree::ptree ref;

    if (!atom)
    {
        return pt;
    }

    ref = this->hitCache(atom);
    if( !ref.empty() )
    {
        return ref;
    }

    switch(atom->type())
    {
    case ::fwAtoms::Base::BOOLEAN :
        pt = this->visit(::fwAtoms::Boolean::dynamicCast(atom), ptpath);
        break;
    case ::fwAtoms::Base::NUMERIC :
        pt = this->visit(::fwAtoms::Numeric::dynamicCast(atom), ptpath);
        break;
    case ::fwAtoms::Base::STRING :
        pt = this->visit(::fwAtoms::String::dynamicCast(atom), ptpath);
        break;
    case ::fwAtoms::Base::OBJECT :
        pt = this->visit(::fwAtoms::Object::dynamicCast(atom), ptpath);
        break;
    case ::fwAtoms::Base::SEQUENCE :
        pt = this->visit(::fwAtoms::Sequence::dynamicCast(atom), ptpath);
        break;
    case ::fwAtoms::Base::MAP :
        pt = this->visit(::fwAtoms::Map::dynamicCast(atom), ptpath);
        break;
    case ::fwAtoms::Base::BLOB :
        pt = this->visit(::fwAtoms::Blob::dynamicCast(atom), ptpath);
        break;
    default:
        SLM_ASSERT("You shall not pass", 0);
        break;
    }

    return pt;
}

};

//-----------------------------------------------------------------------------

void Writer::write( ::fwZip::IWriteArchive::sptr archive,
                    const ::boost::filesystem::path& rootFilename,
                    FormatType format ) const
{
    ::boost::property_tree::ptree root;
    AtomVisitor visitor(archive, rootFilename.stem().string() + "-" + ((format==JSON)?"json":"xml") );
    root = visitor.visit(m_atom);

    ::boost::property_tree::ptree versions;
    versions.put(s_ATOMS_VERSION_KEY, ::fwAtoms::Base::s_VERSION);
    versions.put(s_WRITER_VERSION_KEY, Writer::s_VERSION);

    root.add_child("versions", versions);

    SPTR(std::ostream) os = archive->createFile(rootFilename);
    switch(format)
    {
    case JSON:
        ::boost::property_tree::json_parser::write_json(*os, root, false);
        break;
    case XML:
    {
        ::boost::property_tree::xml_writer_settings<char> settings(' ', 4);
        ::boost::property_tree::xml_parser::write_xml(*os, root, settings);
        break;
    }
    default:
        SLM_ASSERT("You shall not pass", 0);
        break;
    }
}



}
