/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef _PROC_IPROC_HXX_
#define _PROC_IPROC_HXX_

#include <fwData/ProcessObject.hpp>
#include <fwData/Composite.hpp>

namespace proc
{

template< class TYPE >
SPTR(TYPE) IProc::addAttribute(std::string _name)
{
	::fwTools::Object::sptr object = ::fwTools::Factory::New< TYPE >() ;
	assert( object );
	m_localAttributes[_name] = object;
	m_dynamicAttributeIdentifiers.push_back(_name);
	OSLM_TRACE( "Affecting to this (" << this->getClassname() << ") an attribute named " << _name << " of type " << object->className() ) ;
	return TYPE::dynamicCast( object ) ;
}

template< class TYPE >
SPTR(TYPE) IProc::getAttribute(std::string _name)
{
	//If only local
	if(  m_localAttributes.find(_name) != m_localAttributes.end() )
	{
		::fwTools::Object::sptr obj = m_localAttributes[_name];
		SPTR(TYPE) attribute = TYPE::dynamicCast( obj ) ;
		assert(attribute);
		return attribute;
	}
	//If exported : access depends on the object to which this service is attached to
	else
	{
		assert(  m_localToExportedAttributes.find(_name) != m_localToExportedAttributes.end() );
		std::string exportId = m_localToExportedAttributes[_name];
		if( ::fwData::Composite::dynamicCast( this->getObject() ) )
		{
			::fwTools::Object::wptr obj = this->getObject< ::fwData::Composite >()->getRefMap()[exportId];
			SPTR(TYPE) attribute = TYPE::dynamicCast( obj.lock() ) ;
			assert(attribute);
			return attribute;
		}
		else if( ::fwData::ProcessObject::dynamicCast( this->getObject() ) )
		{
			OSLM_ASSERT("Implement me",false);
		}
		else
		{
			assert( this->getObject()->getField(exportId) );
			SPTR(TYPE) attribute = this->getObject()->getFieldSingleElement< TYPE >(m_localToExportedAttributes[_name]) ;
			assert(attribute);
			return attribute;
		}
	}
}

} // namespace proc

#endif /* _PROC_IPROC_HXX_ */
