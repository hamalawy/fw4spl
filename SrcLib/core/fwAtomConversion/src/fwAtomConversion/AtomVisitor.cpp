/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwAtomConversion/AtomVisitor.hpp"
#include "fwAtomConversion/DataVisitor.hpp"
#include "fwAtomConversion/AtomToDataMappingVisitor.hpp"
#include "fwAtomConversion/custom/Mapper.hpp"

namespace fwAtomConversion
{

::fwData::Object::sptr AtomVisitor::convert( ::fwAtoms::Object::sptr atomObj, DataCacheType & cache )
{
    ::fwData::Object::sptr data;

    AtomVisitor::DataCacheType::iterator elem = cache.find( atomObj->getId() );

    if ( elem == cache.end() )
    {
        SPTR(custom::Mapper) mapper = custom::factory::New( atomObj->getMetaInfo( DataVisitor::CLASSNAME_METAINFO ) );
        if ( mapper )
        {
            data = mapper->convert( atomObj, cache );
        }
        else
        {
            ::fwAtomConversion::AtomVisitor visitor ( atomObj, cache );
            visitor.visit();
            data = visitor.getDataObject();
        }
    }
    else // already analysed
    {
        data = elem->second;
    }

    return data;
}

AtomVisitor::AtomVisitor( ::fwAtoms::Object::sptr atomObj, DataCacheType & cache )
: m_atomObj ( atomObj ),
  m_cache ( cache )
{}

AtomVisitor::~AtomVisitor()
{}

void AtomVisitor::visit()
{
    this->processMetaInfos( m_atomObj->getMetaInfos() );
    this->processAttributes( m_atomObj->getAttributes() );
}

void AtomVisitor::processMetaInfos( const ::fwAtoms::Object::MetaInfosType & metaInfos )
{
    const DataVisitor::ClassnameType & classname = metaInfos.find( DataVisitor::CLASSNAME_METAINFO )->second;
    ::fwTools::UUID::UUIDType uuid = m_atomObj->getId();
    m_dataObj = ::fwData::factory::New(classname);

    bool uuidIsSetted = ::fwTools::UUID::set(m_dataObj, uuid);
    SLM_FATAL_IF("Sorry it is impossible to set uuid on new object because it already exists.",! uuidIsSetted );

    m_cache[uuid] = m_dataObj;
}

void AtomVisitor::processAttributes( const ::fwAtoms::Object::AttributesType & attributes )
{
    const camp::Class& metaclass = ::camp::classByName( m_dataObj->getClassname() );
    ::fwAtomConversion::AtomToDataMappingVisitor visitor ( m_dataObj, m_atomObj, m_cache );
    metaclass.visit(visitor);
}

::fwData::Object::sptr AtomVisitor::getDataObject() const
{
    return m_dataObj;
}

} // end namespace fwAtomConversion
