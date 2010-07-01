/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/EConfigurationElement.hpp>

#include <fwServices/ComChannelService.hpp>
#include <fwServices/helper.hpp>

#include <fwComEd/Dictionary.hpp>

#include <fwData/ProcessObject.hpp>
#include <fwData/Composite.hpp>
#include <fwData/Image.hpp>
#include <fwData/Float.hpp>
#include <fwTools/Factory.hpp>

#include "proc/IProc.hpp"


namespace proc
{
/**
 * Start recursively services
 * @note should probably be generalized
 */
void recursiveStart(::fwTools::Object::sptr obj)
{
	std::vector< ::fwServices::IService::sptr > services = ::fwServices::getRegisteredServices(obj);
	std::vector< ::fwServices::IService::sptr >::iterator iter = services.begin();
	for( ; iter != services.end() ; ++iter)
	{
		(*iter)->start();
		recursiveStart((*iter));
	}
}
/**
 * Stop recursively services
 * @note should probably be generalized
 */
void recursiveStop(::fwTools::Object::sptr obj)
{
	std::vector< ::fwServices::IService::sptr > services = ::fwServices::getRegisteredServices(obj);
	std::vector< ::fwServices::IService::sptr >::iterator iter = services.begin();
	for( ; iter != services.end() ; ++iter)
	{
		recursiveStop((*iter));
		(*iter)->stop();
	}
}

IProc::IProc() throw()
{
}
IProc::~IProc() throw()
{
}
void IProc::starting() throw(::fwTools::Failed)
{
	this->doStart();
	recursiveStart(this->getSptr());
	for(AttributeContainer::iterator iter = m_localAttributes.begin(); iter != m_localAttributes.end() ; ++iter)
	{
		OSLM_TRACE("Local Attribute "  << iter->first );
	}
	for(MappingContainer::iterator iter = m_localToExportedAttributes.begin(); iter != m_localToExportedAttributes.end() ; ++iter)
	{
		OSLM_TRACE("Local Attribute "  << iter->first << " exported as " << iter->second);
	}
	for(DynamicAttributeIdentifierContainer::iterator iter = m_dynamicAttributeIdentifiers.begin(); iter != m_dynamicAttributeIdentifiers.end() ; ++iter)
	{
		OSLM_TRACE("Dynamic attribute "  << *iter);
	}

	for(DynamicAttributeIdentifierContainer::iterator iter = m_dynamicAttributeIdentifiers.begin(); iter != m_dynamicAttributeIdentifiers.end() ; ++iter)
	{
			::fwServices::registerCommunicationChannel(this->getAttribute(*iter), this->getSptr() )->start();
	}

}
void IProc::stopping() throw(::fwTools::Failed)
{
	recursiveStop(this->getSptr());
	this->doStop();
}
void IProc::updating() throw(::fwTools::Failed)
{
	this->doUpdate();
}
void IProc::updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _mssg ) throw(::fwTools::Failed)
{
	this->doUpdate(_mssg);
}

void IProc::configuring()  throw(::fwTools::Failed)
{
	for( ::fwRuntime::ConfigurationElementContainer::Iterator iter = m_configuration->begin() ; iter != m_configuration->end();++iter)
	{
		if( (*iter)->getName() == "object" )
		{
			OSLM_ASSERT("Config format must respect <object id=...	type=... />", (*iter)->hasAttribute("id") && (*iter)->hasAttribute("type") ) ;
			std::string id = (*iter)->getAttributeValue("id");
			OSLM_ASSERT("Check that your id value corresponds to the addAttribute identifier", m_localAttributes.find( id ) != m_localAttributes.end() );
			std::string type = (*iter)->getAttributeValue("type");
			OSLM_ASSERT("Check that your type value corresponds to the addAttribute type", type == (m_localAttributes[id])->getClassname() );

			//Create object and configure it using the general mechanism
			::fwTools::Object::sptr obj = ::fwServices::New( (*iter) );

			//Replace the corresponding local attribute by this newly configured.
			m_localAttributes[id] = obj;

			//Check whether export to this->getObject() is requested
			if( (*iter)->hasAttribute("export") )
			{
				std::string exportId = (*iter)->getAttributeValue("export");
				this->exportAttribute(id,exportId);
			}
		}
	}
	///Invoke specific configuration
	this->doConfigure();
}
::fwTools::Object::sptr IProc::getAttribute(std::string _name)
{
	//If only local
	if(  m_localAttributes.find(_name) != m_localAttributes.end() )
	{
		::fwTools::Object::sptr obj = m_localAttributes[_name];
		assert(obj);
		return obj;
	}
	//If exported : access depends on the object to which this service is attached to
	else
	{
		assert(  m_localToExportedAttributes.find(_name) != m_localToExportedAttributes.end() );
		std::string exportId = m_localToExportedAttributes[_name];
		if( ::fwData::Composite::dynamicCast( this->getObject() ) )
		{
			::fwTools::Object::wptr obj = this->getObject< ::fwData::Composite >()->getRefMap()[exportId];
			assert(!obj.expired());
			return obj.lock();
		}
		else if( ::fwData::ProcessObject::dynamicCast( this->getObject() ) )
		{
			OSLM_ASSERT("Implement me",false);
		}
		else
		{
			assert( this->getObject()->getField(exportId) );
			::fwTools::Object::wptr obj = this->getObject()->getFieldSingleElement< ::fwTools::Object >(m_localToExportedAttributes[_name]) ;
			assert(!obj.expired());
			return obj.lock();
		}
	}

}
void IProc::exportAttribute(std::string _localId, std::string _exportId)
{
	///Precondition
	assert(  m_localAttributes.find(_localId) != m_localAttributes.end() );
	assert(  m_localToExportedAttributes.find(_localId) == m_localToExportedAttributes.end() );

	//Update the mapping
	m_localToExportedAttributes[_localId] = _exportId ;
	//Get the object to export
	::fwTools::Object::sptr localObject = m_localAttributes[_localId];

	//Affect it to object to which the service (this) is attached
	if( ::fwData::Composite::dynamicCast( this->getObject() ) )
	{
		this->exportForComposite(_exportId,localObject);
	}
	else if( ::fwData::ProcessObject::dynamicCast( this->getObject() ) )
	{
		this->exportForProcessObject(_exportId,localObject);
	}
	else
	{
		this->exportForObject(_exportId,localObject);
	}

	//Update the map of local attributes: localObject can be erased if an exported object was already available
	m_localAttributes.erase(_localId);

	///Postcondition
	assert(  m_localToExportedAttributes.find(_localId) != m_localToExportedAttributes.end() );
	assert(  m_localAttributes.find(_localId) == m_localAttributes.end() );
}

void IProc::exportForComposite(std::string _exportId , ::fwTools::Object::sptr _obj)
{
	::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >();
	if( composite->getRefMap().find(_exportId) == composite->getRefMap().end() )
	{
		::fwData::Object::sptr data = ::fwData::Object::dynamicCast( _obj ) ;
		composite->getRefMap()[_exportId] = data ;
		OSLM_TRACE( "Add field/attrib this->getObject() " << _exportId ) ;
	}
	else
	{
		OSLM_ASSERT("Already defined field/attrib this->getObject() : BUT with a different type -> check you type element in config" , composite->getRefMap()[_exportId]->getClassname() == _obj->getClassname() );
		OSLM_TRACE( "Already defined field/attrib this->getObject() " << _exportId  ) ;
	}
}

void IProc::exportForObject(std::string _exportId , ::fwTools::Object::sptr _obj)
{
	if( !this->getObject()->getField( _exportId ) )
	{
		this->getObject()->setFieldSingleElement( _exportId ,  _obj ) ;
		OSLM_TRACE( "Add field/attrib this->getObject() " << _exportId ) ;
	}
	else
	{
		OSLM_ASSERT("Already defined field/attrib this->getObject() : BUT with a different type -> check you type element in config" , this->getObject()->getFieldSingleElement< ::fwTools::Object >(_exportId)->getClassname() == _obj->getClassname() );
		OSLM_TRACE( "Already defined field/attrib this->getObject() " << _exportId  ) ;
	}
}

void IProc::exportForProcessObject(std::string _exportId , ::fwTools::Object::sptr _obj)
{
	OSLM_ASSERT("Implement me",false);
}

void IProc::setAttribute(std::string _name, ::fwTools::Object::sptr obj)
{
	//If only local
	if(  m_localAttributes.find(_name) != m_localAttributes.end() )
	{
		m_localAttributes[_name] = obj;
	}
	//If exported : access depends on the object to which this service is attached to
	else
	{
		assert(  m_localToExportedAttributes.find(_name) != m_localToExportedAttributes.end() );
		std::string exportId = m_localToExportedAttributes[_name];
		if( ::fwData::Composite::dynamicCast( this->getObject() ) )
		{
			this->getObject< ::fwData::Composite >()->getRefMap()[exportId] = ::fwData::Object::dynamicCast(obj);
		}
		else if( ::fwData::ProcessObject::dynamicCast( this->getObject() ) )
		{
			OSLM_ASSERT("Implement me",false);
		}
		else
		{
			assert( this->getObject()->getField(exportId) );
			this->getObject()->setFieldSingleElement(m_localToExportedAttributes[_name],obj) ;
		}
	}

}


void IProc::printInfo()
{
	OSLM_TRACE("Local dynamic attributes:" << std::endl);
	for( std::map< std::string , ::fwTools::Object::sptr >::iterator iter = m_localAttributes.begin() ; iter != m_localAttributes.end() ; ++iter )
	{
		OSLM_TRACE( iter->first << " of type " << iter->second->getClassname() << std::endl ) ;
	}
	for( std::map< std::string , std::string >::iterator iter = m_localToExportedAttributes.begin() ; iter != m_localToExportedAttributes.end() ; ++iter )
	{
		OSLM_TRACE( iter->first << " export as " << iter->second << std::endl ) ;
	}

}

}

