/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <iostream>

#include <boost/regex.hpp>

#include <fwTools/ClassRegistrar.hpp>

#include "fwServices/ObjectMsg.hpp"
#include "fwServices/IService.hpp"

REGISTER_BINDING_BYCLASSNAME( ::fwTools::Object, ::fwServices::ObjectMsg, ::fwServices::ObjectMsg );

namespace fwServices
{

std::string ObjectMsg::NEW_OBJECT     = "ObjectMsg::NEW_OBJECT";
std::string ObjectMsg::UPDATED_OBJECT = "ObjectMsg::UPDATED_OBJECT";
std::string ObjectMsg::DELETE_OBJECT  = "ObjectMsg::DELETE_OBJECT";

std::string ObjectMsg::ADDED_FIELDS   = "ADDED_FIELDS";
std::string ObjectMsg::REMOVED_FIELDS = "REMOVED_FIELDS";
std::string ObjectMsg::CHANGED_FIELDS = "CHANGED_FIELDS";

//-----------------------------------------------------------------------------

ObjectMsg::ObjectMsg() : m_hasCallback (false)
{}

//-----------------------------------------------------------------------------

ObjectMsg::~ObjectMsg()
{
    if ( m_hasCallback )
    {
        m_callback();
    }
}

//-----------------------------------------------------------------------------

void ObjectMsg::addEvent( std::string _eventId , ::fwData::Object::csptr _pDataInfo )
{
    m_eventId2DataInfo[ _eventId ] = _pDataInfo;
}

//-----------------------------------------------------------------------------

bool ObjectMsg::hasEvent( std::string _eventId ) const
{
    return m_eventId2DataInfo.find( _eventId ) != m_eventId2DataInfo.end() ;
}

//-----------------------------------------------------------------------------

std::vector< std::string > ObjectMsg::getEventIds() const
{
    std::vector< std::string > result ;
    std::map< std::string , ::fwData::Object::csptr >::const_iterator iter ;
    for( iter = m_eventId2DataInfo.begin() ; iter != m_eventId2DataInfo.end() ; ++iter )
    {
        result.push_back( iter->first ) ;
    }
    return result ;
}

//-----------------------------------------------------------------------------

::fwData::Object::csptr ObjectMsg::getDataInfo( std::string _eventId ) const
{
    return m_eventId2DataInfo.find( _eventId )->second;
}

//-----------------------------------------------------------------------------

std::string ObjectMsg::convertToLightString( std::string _initialString )
{
    std::string lightString = _initialString;

    ::boost::regex toolsRegex ("::fwTools::(.*)");
    ::boost::regex dataRegex ("::fwData::(.*)");
    const std::string machine_format("\\1");

    if ( ::boost::regex_match( _initialString, toolsRegex ) )
    {
        lightString = ::boost::regex_replace( _initialString, toolsRegex, machine_format, boost::match_default | boost::format_sed );
    }
    else if ( ::boost::regex_match( _initialString, dataRegex ) )
    {
        lightString = ::boost::regex_replace( _initialString, dataRegex, machine_format, boost::match_default | boost::format_sed );
    }

    return lightString;
}

//-----------------------------------------------------------------------------

std::string ObjectMsg::getGeneralInfo() const
{
    ::fwServices::IService::sptr source = m_source.lock();
    std::string msgUUID    = convertToLightString( const_cast< ObjectMsg * >(this)->getID() );

    std::string sourceUUID = convertToLightString( source? source->getID():"[source died]" );
    std::string destUUID   = convertToLightString( m_subject.expired()?"[subject died]":m_subject.lock()->getID());

    std::stringstream eventstream;
    for(    std::map< std::string, ::fwData::Object::csptr >::const_iterator itEvent2Data = m_eventId2DataInfo.begin();
            itEvent2Data != m_eventId2DataInfo.end();
            ++itEvent2Data )
    {
        eventstream << "| " << itEvent2Data->first << " |";
    }


    std::stringstream sstream;
    sstream << msgUUID << " | " << sourceUUID << " ===> " << destUUID << eventstream.str();

    return sstream.str();
}

//-----------------------------------------------------------------------------

std::ostream & operator<<(std::ostream & _ostream, const ObjectMsg& _message)
{
    _message.info( _ostream ) ;
    return _ostream ;
}

//-----------------------------------------------------------------------------

void ObjectMsg::info(std::ostream &_sstream ) const
{
    _sstream << getGeneralInfo();
}

//-----------------------------------------------------------------------------

void ObjectMsg::setSource(::fwServices::IService::wptr _source)
{
    m_source = _source;
}

//-----------------------------------------------------------------------------

::fwServices::IService::wptr ObjectMsg::getSource() const
{
    return m_source;
}

//-----------------------------------------------------------------------------

void ObjectMsg::setSubject( ::fwData::Object::wptr _subject)
{
    m_subject = _subject;
}

//-----------------------------------------------------------------------------

::fwData::Object::wptr ObjectMsg::getSubject() const
{
    return m_subject;
}

//-----------------------------------------------------------------------------

void ObjectMsg::setMessageCallback( MessageCallbackType callback )
{
    m_callback = callback;
    m_hasCallback = true;
}

//-------------------------------------------------------------------------

void ObjectMsg::appendAddedField( const FieldNameType &fieldName, ::fwData::Object::sptr object )
{
    if( ! this->hasEvent( ADDED_FIELDS ) )
    {
        this->addEvent( ADDED_FIELDS );
    }

    SLM_ASSERT("This field is already registered", m_addedFields.find(fieldName) == m_addedFields.end() );

    m_addedFields[ fieldName ] = object;
}

//-----------------------------------------------------------------------------

const ObjectMsg::ModifiedFieldsContainerType & ObjectMsg::getAddedFields() const
{
    return m_addedFields;
}

//-----------------------------------------------------------------------------

void ObjectMsg::appendRemovedField( const FieldNameType &fieldName, ::fwData::Object::sptr object )
{
    if( ! this->hasEvent( REMOVED_FIELDS ) )
    {
        this->addEvent( REMOVED_FIELDS );
    }

    SLM_ASSERT("This field is already registered", m_removedFields.find(fieldName) == m_removedFields.end() );

    m_removedFields[ fieldName ] = object;
}

//-----------------------------------------------------------------------------

const ObjectMsg::ModifiedFieldsContainerType& ObjectMsg::getRemovedFields() const
{
    return m_removedFields;
}

//-----------------------------------------------------------------------------

void ObjectMsg::appendChangedField( const FieldNameType &fieldName, ::fwData::Object::sptr oldObject, ::fwData::Object::sptr newObject )
{
    if( ! this->hasEvent( CHANGED_FIELDS ) )
    {
        this->addEvent( CHANGED_FIELDS );
    }

    SLM_ASSERT("This field is already registered", m_oldChangedFields.find(fieldName) == m_oldChangedFields.end() );

    m_oldChangedFields[ fieldName ] = oldObject;
    m_newChangedFields[ fieldName ] = newObject;
}

//-----------------------------------------------------------------------------

const ObjectMsg::ModifiedFieldsContainerType& ObjectMsg::getOldChangedFields() const
{
    return m_oldChangedFields;
}

//-----------------------------------------------------------------------------

const ObjectMsg::ModifiedFieldsContainerType& ObjectMsg::getNewChangedFields() const
{
    return m_newChangedFields;
}

//------------------------------------------------------------------------------

} // namespace fwServices

