/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <fwCore/base.hpp>

#include <fwTools/fwID.hpp>

#include "fwServices/ComChannelService.hpp"
#include "fwServices/registry/ObjectService.hpp"
#include "fwServices/op/Get.hpp"
#include "fwServices/IService.hpp"
#include "fwServices/macros.hpp"
#include "fwServices/ObjectMsg.hpp"
#include "fwServices/IEditionService.hpp"
#include "fwServices/registry/ServiceFactory.hpp"


REGISTER_SERVICE( ::fwServices::ICommunication , ::fwServices::ComChannelService , ::fwTools::Object ) ;

namespace fwServices
{

ComChannelService::ComChannelService() : m_destUUID( std::pair< bool , std::string >(false , "") ), m_priority(0.5)
{}

//------------------------------------------------------------------------------

ComChannelService::~ComChannelService()
{}

//------------------------------------------------------------------------------

bool ComChannelService::isValid()
{
    return (!m_destination.expired() && !m_source.expired()  ) ;
}

//------------------------------------------------------------------------------

void ComChannelService::setSrc(IEditionService::sptr  _src)
{
    m_source = _src;
}

//------------------------------------------------------------------------------

void ComChannelService::configuring() throw( ::fwTools::Failed )
{
    SLM_ASSERT("Depreciated tag source", !m_configuration->findConfigurationElement("source"));

    if( m_configuration->findConfigurationElement("target") )
    {
        m_destUUID.first = true ;
        m_destUUID.second = m_configuration->findConfigurationElement("target")->getValue() ;
        OSLM_INFO("Destination = " << m_destUUID.second ) ;
    }

    if( m_configuration->findConfigurationElement("priority") )
    {
        m_priority = ::boost::lexical_cast<float>(m_configuration->findConfigurationElement("priority")->getValue());
        if( (m_priority<=1.0) && (m_priority>=0.0) )
        {
            OSLM_INFO("Priority = " << m_priority ) ;
        }
        else
        {
            OSLM_FATAL("Priority must be a float between 0.0 and 1.0, and you selected " << m_priority ) ;
        }
    }
}

//------------------------------------------------------------------------------

void ComChannelService::starting() throw(fwTools::Failed)
{
    SLM_ASSERT("No UID target", m_destUUID.first );
    OSLM_ASSERT("Unknown UID Objects : "<<m_destUUID.second, ::fwTools::fwID::exist( m_destUUID.second ) );
    m_destination = ::fwServices::IService::dynamicCast( ::fwTools::fwID::getObject( m_destUUID.second ) ) ;
    OSLM_DEBUG("Destination = " << m_destUUID.second << " found") ;

    SLM_ASSERT("intern data mismatch", m_destination.lock()->getID() == m_destUUID.second);

    if(m_source.expired())
    {
        if(::fwServices::OSR::has(this->getObject(), "::fwServices::IEditionService"))
        {
            m_source = ::fwServices::get< ::fwServices::IEditionService >( this->getObject() ) ;
        }
        else
        {
            m_source = ::fwServices::add< ::fwServices::IEditionService >( this->getObject(), "::fwServices::DefaultEditor" ) ;
        }
    }
    OSLM_ASSERT("ComChannelService object "<<this->getObject()->getID()<<"different of IEditionService object "<<m_source.lock()->getObject()->getID(),
           m_source.lock()->getObject() == this->getObject());
    OSLM_DEBUG("Source (IEditionService) = " << m_source.lock()->getID() << " found") ;

    OSLM_ASSERT("there are similar observations, dest= " <<
            m_destination.lock()->getID() << "\n" << ::fwServices::OSR::getRegistryInformation(),
            !this->hasSimilarObservation());

    // Assertion
    OSLM_ASSERT("Destination is expired for ComChannel "<<this->getID(), !m_destination.expired() ) ;
    OSLM_ASSERT("Source is expired for ComChannel "<<this->getID(), !m_source.expired() ) ;

    if( !m_source.lock()->isAttached( this->getSptr() ) )
    {
        std::stringstream msg ;
        this->info( msg ) ;
        OSLM_TRACE( "Starting ComChannelService : " << msg.str() << " with priority: " << m_priority);
        m_source.lock()->attach( this->getSptr() );
        if (! m_source.lock()->isStarted())
        {
            m_source.lock()->start() ;
        }
    }

    // Post condition
    OSLM_ASSERT("Source is expired for ComChannel "<<this->getID(), !m_source.expired() ) ;
    OSLM_ASSERT("Destination is expired for ComChannel "<<this->getID(), !m_destination.expired() );
}

//------------------------------------------------------------------------------

void ComChannelService::swapping() throw(fwTools::Failed)
{
    this->stopping();
    this->starting();
}

//------------------------------------------------------------------------------

void ComChannelService::stopping() throw(fwTools::Failed)
{
    OSLM_DEBUG( "Stopping ComChannelService : " << getInfo() );
    SLM_ASSERT( "Sorry, before stopping, source and dest must still exist in system.", ! m_destination.expired() && ! m_source.expired() );

    // Pre condition
    if( !m_source.expired() )
    {
        if( m_source.lock()->isAttached( this->getSptr() ) )
        {
            std::stringstream msg ;
            this->info( msg ) ;
            SLM_TRACE( "Stopping ComChannelService " + msg.str() ); // crash from spylog???
            m_source.lock()->detach( this->getSptr() );

            /// Remove IEditionService if it is the last comChannel
            int nbObservers = m_source.lock()->getNbObservers();
            if(nbObservers == 0)
            {
                if (m_source.lock()->isStarted())
                {
                    m_source.lock()->stop();
                }
                ::fwServices::OSR::unregisterService(m_source.lock());
            }
        }
        m_source.reset();
    }
}

//------------------------------------------------------------------------------

std::string ComChannelService::getInfo()
{
    std::stringstream sstr;

    std::string fwIdSrc = ( ! m_source.expired() ? m_source.lock()->getID() : "fwIdExpired" );
    std::string fwIdDest = ( ! m_destination.expired() ? m_destination.lock()->getID() : "fwIdExpired" );
    sstr << "Com Channel ( "<< this->getID() << " ) Src  fwId = " << fwIdSrc << ", Dest fwId = " << fwIdDest;

    return sstr.str();
}

//------------------------------------------------------------------------------

void ComChannelService::info(std::ostream &_sstream )
{
    if( ! this->isStopped() )
    {
        // Status
        std::string status ;
        if( !m_source.expired() )
        {
            if( m_source.lock()->isAttached( this->getSptr() ) )
            {
                status = "ON" ;
            }
            else
            {
                status = "OFF" ;
            }
        }

        // Update _sstream
        if(!m_source.expired() )
        {
            if( !this->m_associatedObject.expired() ) // FIXME expired object not authorized
            {
                ::fwTools::Object::sptr observedObject = m_source.lock()->getObject() ;
                _sstream << "ComChannelService (" << status << ") "<< " : SRC = " << observedObject.get() << " (" << observedObject->className() << ")";
            }
            else
            {
                _sstream << "ComChannelService (" << status << ") "<< " SRC not specified" ;
            }
        }

        if( !m_destination.expired() )
        {
            _sstream << " - DEST = " << m_destination.lock().get() << " (" << (m_destination.lock())->getClassname() << ")" << " Priority: " << m_priority;
        }
    }
    else
    {
        // Update _sstream
        if(!m_source.expired() )
        {
            if( !this->m_associatedObject.expired() ) // FIXME expired object not authorized
            {
                ::fwTools::Object::sptr observedObject = m_source.lock()->getObject() ;
                _sstream << "ComChannelService ( com is stopped ) "<< " : SRC = " << observedObject.get() << " (" << observedObject->className() << ")";
            }
            else
            {
                _sstream << "ComChannelService ( com is stopped ) "<< " SRC not specified" ;
            }
        }

        if( !m_destination.expired() )
        {
            _sstream << " - DEST = " << m_destination.lock().get() << " (" << (m_destination.lock())->getClassname() << ")" << " Priority: " << m_priority;
        }
    }
}

//------------------------------------------------------------------------------

void ComChannelService::setDest(IService::sptr _client)
{
    m_destination = _client ;
    m_destUUID.first = true ;
    m_destUUID.second = _client->getID() ;
}

//------------------------------------------------------------------------------

IService::sptr  ComChannelService::getDest()
{
    OSLM_ASSERT("Destination is expired for ComChannel "<<this->getID(), !m_destination.expired() ) ;
    return m_destination.lock();
}

//------------------------------------------------------------------------------

IEditionService::sptr ComChannelService::getSrc()
{
    OSLM_ASSERT("Source is expired for ComChannel "<<this->getID(), !m_source.expired() ) ;
    return m_source.lock();
}

//------------------------------------------------------------------------------

void ComChannelService::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{
    SLM_FATAL("method depreciated");
}

//------------------------------------------------------------------------------

bool ComChannelService::hasSimilarObservation()
{
    bool res = false;
    typedef std::vector< ::fwServices::ComChannelService::sptr > OContainerType;
    OContainerType obs = ::fwServices::OSR::getServices< ::fwServices::ComChannelService >() ;

    for( OContainerType::iterator iter = obs.begin() ; !res && iter != obs.end() ; ++iter )
    {
        if( (*iter).get() != this && (*iter)->isValid() )
        {
            res = ( (*iter)->getDest() == m_destination.lock() &&  (*iter)->getSrc() == m_source.lock() );
        }
    }
    return res;
}

//------------------------------------------------------------------------------

std::string ComChannelService::convertToLightString( std::string _initialString )
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

//------------------------------------------------------------------------------

std::string ComChannelService::getNotificationInformation( ::fwServices::ObjectMsg::csptr _msg )
{
    std::string sourceUUID = convertToLightString(_msg->getSubject().lock()->getID() );
    std::string destUUID = convertToLightString( m_destination.lock()->getID() );
    std::string objectUUID = convertToLightString( _msg->getID() );

    std::stringstream sstr;
    sstr << "MSG ( " << objectUUID << " ) : " << sourceUUID << " ===> " << destUUID << " Priority: " << m_priority;
    return sstr.str();
}

//------------------------------------------------------------------------------

void ComChannelService::sendMessage( ::fwServices::ObjectMsg::csptr _msg, ::fwServices::ComChannelService::MsgOptionsType options )
{
    SLM_ASSERT("ComChannel destination expired", m_destination.lock());
    if( m_destination.lock()->isStarted())
    {
        bool notifySource = options & ::fwServices::ComChannelService::NOTIFY_SOURCE;
        if( m_destination.lock() !=  _msg->getSource().lock() || notifySource )
        {
            SLM_INFO( getNotificationInformation(_msg) );
            m_destination.lock()->update(_msg ) ;
        }
    }
}

}
