/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#include <fwServices/ComChannelService.hpp>
#include <fwServices/helper.hpp>
#include <fwData/Composite.hpp>

#include "ctrlField/IFieldSynchronizer.hpp"

namespace ctrlField
{

//-----------------------------------------------------------------------------

IFieldSynchronizer::IFieldSynchronizer() throw()
{}

//-----------------------------------------------------------------------------

IFieldSynchronizer::~IFieldSynchronizer() throw()
{}

//-----------------------------------------------------------------------------

void IFieldSynchronizer::starting() throw ( ::fwTools::Failed )
{
	for( std::vector< ::fwTools::Object::wptr >::iterator iter = m_objectsToSynchronize.begin() ; iter != m_objectsToSynchronize.end() ; ++iter )
	{
	       ::fwServices::IService::sptr          service;
	        ::fwServices::ComChannelService::sptr communicationChannelService;
	        service = ::fwServices::registerCommunicationChannel((*iter).lock() , this->getSptr() );
	        communicationChannelService = ::fwServices::ComChannelService::dynamicCast(service);

	        communicationChannelService->setPriority(1.0);
	        communicationChannelService->start();
	}
	this->doStart();
}

//-----------------------------------------------------------------------------

void IFieldSynchronizer::stopping() throw ( ::fwTools::Failed )
{
	for( std::vector< ::fwTools::Object::wptr >::iterator iter = m_objectsToSynchronize.begin() ; iter != m_objectsToSynchronize.end() ; ++iter )
	{
			::fwServices::unregisterCommunicationChannel((*iter).lock() , this->getSptr() );
	}

	this->doStop();
}

//-----------------------------------------------------------------------------

void IFieldSynchronizer::configuring() throw ( ::fwTools::Failed )
{
	OSLM_TRACE("Synchronisation of :" );
	for( ::fwRuntime::ConfigurationElementContainer::Iterator iter = m_configuration->begin() ; iter != m_configuration->end();++iter)
	{
		if( (*iter)->getName() == "object" )
		{
			OSLM_ASSERT("Config format must respect <object id=... />", (*iter)->hasAttribute("id") ) ;
			std::string id = (*iter)->getAttributeValue("id");

			//Affect it to object to which the service (this) is attached
			if( ::fwData::Composite::dynamicCast( this->getObject() ) )
			{
				::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >() ;
				OSLM_ASSERT("Identifier does not match an object of your composite" , composite->getRefMap().find(id) != composite->getRefMap().end() );
				::fwTools::Object::sptr object = ::fwTools::Object::dynamicCast(composite->getRefMap()[id]);
				assert( object );
				m_objectsToSynchronize.push_back( object );
				OSLM_TRACE( id << " of type " << object->getClassname() ) ;
			}
			else
			{
				OSLM_ASSERT("Identifier does not match a field of your object" , this->getObject()->getField( id ) );
				::fwTools::Object::sptr object = this->getObject()->getFieldSingleElement< ::fwTools::Object >(id);
				m_objectsToSynchronize.push_back( object );
				OSLM_TRACE( id << " of type " << object->getClassname() ) ;
			}

		}
	}

	this->doConfigure();
}

//-----------------------------------------------------------------------------

void IFieldSynchronizer::updating() throw ( ::fwTools::Failed )
{
	this->doUpdate();
}

//-----------------------------------------------------------------------------

void IFieldSynchronizer::updating( fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed )
{
	OSLM_TRACE( "message ") ;
	std::vector< ::fwTools::Object::wptr > objectsToUpdate;
	for( std::vector< ::fwTools::Object::wptr >::iterator iter = m_objectsToSynchronize.begin() ; iter != m_objectsToSynchronize.end() ; ++iter )
	{
		if( (*iter).lock().get() != _msg->getSubject().lock().get() )
		{
			objectsToUpdate.push_back( *iter );
		}
	}
	this->doUpdate(objectsToUpdate,_msg);
}



}
