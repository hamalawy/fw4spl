/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <set>

#include <boost/foreach.hpp>

#include "fwServices/ComChannelService.hpp"
#include "fwServices/IEditionService.hpp"
#include "fwServices/macros.hpp"
#include "fwServices/ObjectMsg.hpp"
#include "fwServices/registry/ObjectService.hpp"
#include "fwServices/op/Com.hpp"
#include "fwServices/op/Get.hpp"

#include "fwServices/GlobalEventManager.hpp"

namespace fwServices
{

namespace
{

//-----------------------------------------------------------------------------

/**
 * @brief   Defines a STL compatible predicate that test if a given weak pointer
 *          has expired or not.
 */
struct HasExpired
{
    const bool operator() ( ::fwServices::ComChannelService::wptr observer ) const
    {
        return observer.expired();
    }
};

//-----------------------------------------------------------------------------

/**
 * @brief   Defines a predicate STL compatible that test the equality of weak pointers to
 *          a given shared pointer.
 */
struct IsEqual
{
    IsEqual( ::fwServices::ComChannelService::sptr observer )
    : m_observer( observer )
    {}

    const bool operator() ( ::fwServices::ComChannelService::wptr observer ) const
    {
        return observer.lock() == m_observer;
    }

private:

    ::fwServices::ComChannelService::sptr m_observer;
};

//-----------------------------------------------------------------------------

/**
 * @brief   Defines a predicate STL compatible that test priority order between two Com Channel Service.
 */

bool Less (::fwServices::ComChannelService::wptr observer1,  ::fwServices::ComChannelService::wptr observer2 )
{
    bool result = false;
    if(!observer1.expired())
    {
        if(!observer2.expired())
        {
            // Both wpstr are valid
            result =
                    ( observer1.lock()->getPriority() > observer2.lock()->getPriority() ) ||
                    ( observer1.lock()->getPriority() == observer2.lock()->getPriority() && observer1.lock().get() > observer2.lock().get() );
        }
        else
        {
            // Normally it never happens because expired comchannel is remove on the IEditionService::notify function
            SLM_FATAL("comChannel expired.");
            // observer1 is valid and obseerver2 is expire.
            result = true;
        }
    }
    else
    {
        if(!observer2.expired())
        {
            // Normally it never happens because expired comchannel is remove on the IEditionService::notify function
            SLM_FATAL("comChannel expired.");
            // Both observer1 and observer1 are expired.
            result = false;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------

/**
 * @brief   Defines a predicate STL compatible that test priority order between two Com Channel Service in a set container.
 */
struct SetLess
{
    bool operator()(::fwServices::ComChannelService::wptr observer1,  ::fwServices::ComChannelService::wptr observer2 )
    {
        return Less(observer1,observer2);
    }
};

//-----------------------------------------------------------------------------

} // end namespace

//-----------------------------------------------------------------------------

void IEditionService::notify(
        ::fwServices::IService::sptr _pSource,
        ::fwData::Object::sptr _pSubject,
        ::fwServices::ObjectMsg::sptr _pMsg,
        ::fwServices::ComChannelService::MsgOptionsType options )
{

    _pMsg->setSource(_pSource);
    _pMsg->setSubject(_pSubject);
    _pMsg->timeModified();

    GlobalEventManager::getDefault()->notify( _pMsg, options );
}

//-----------------------------------------------------------------------------

IEditionService::IEditionService()
{}

//-----------------------------------------------------------------------------

IEditionService::~IEditionService() throw()
{}

//-----------------------------------------------------------------------------

//@todo remove throw specifications
void IEditionService::attach( ::fwServices::ComChannelService::sptr observer) throw()
{
    // Pre-condition : This observer must exist and must not be already registered.
    SLM_ASSERT("The ComChannelService given in parameter is a null shared pointer.", observer != 0 );
    SLM_ASSERT("ACH : The ComChannelService, given in parameter, is already registered", !isAttached( observer ) );

    // Get the service observer
    ::fwServices::IService::sptr service = observer->getDest();

    m_globalObservers.push_back( observer );
    // sort the list to put the new element at the right place.
    m_globalObservers.sort( Less );
}

//-----------------------------------------------------------------------------

//@todo remove throw specifications
void IEditionService::detach( ::fwServices::ComChannelService::sptr observer ) throw()
{
    // Post-condition
    SLM_ASSERT("ACH : The ComChannelService, given in parameter, is not attached", isAttached( observer ) );

    // Get the service observer
    IService::sptr service = observer->getDest();

    m_globalObservers.remove_if( IsEqual(observer) );
}

//-----------------------------------------------------------------------------

//@todo remove throw specifications
const bool IEditionService::isAttached( ::fwServices::ComChannelService::sptr observer ) const throw()
{
    bool isAttached = false;

    // Get the service observer
    ::fwServices::IService::sptr service = observer->getDest();

    isAttached = findObserver( m_globalObservers, observer ) != m_globalObservers.end();

    return isAttached;
}

//-----------------------------------------------------------------------------

//@todo remove throw specifications
IEditionService::ObserverContainer::iterator IEditionService::findObserver( ObserverContainer & _observers, ::fwServices::ComChannelService::sptr observer ) throw()
{
    return std::find_if( _observers.begin(), _observers.end(), IsEqual(observer) );
}

//-----------------------------------------------------------------------------

//@todo remove throw specifications
IEditionService::ObserverContainer::const_iterator IEditionService::findObserver( const ObserverContainer & _observers, ::fwServices::ComChannelService::sptr observer ) const throw()
{
    return std::find_if( _observers.begin(), _observers.end(), IsEqual(observer) );
}

//-----------------------------------------------------------------------------

bool IEditionService::hasExpiredObserver()
{
    bool hasExpiredObserver = false;

    ObserverContainer::const_iterator iterOnObserver;

    iterOnObserver = std::find_if( m_globalObservers.begin(), m_globalObservers.end(), HasExpired() );

    if ( iterOnObserver != m_globalObservers.end() )
    {
        hasExpiredObserver = true;
    }
    else
    {
        for (   Event2ObserversContainer::iterator iter = m_event2SpecificObservers.begin();
                !hasExpiredObserver && iter != m_event2SpecificObservers.end();
                ++iter )
        {
            ObserverContainer & specificObservers = iter->second;
            iterOnObserver = std::find_if( specificObservers.begin(), specificObservers.end(), HasExpired() );
            if ( iterOnObserver != specificObservers.end() )
            {
                hasExpiredObserver = true;
            }
        }
    }

    return hasExpiredObserver;
}

//-----------------------------------------------------------------------------

void IEditionService::notify( ::fwServices::ObjectMsg::csptr eventMessage, ::fwServices::ComChannelService::MsgOptionsType options )
{
    SLM_ASSERT( "ACH : we have expired observer in the observer list", !hasExpiredObserver() );

    typedef std::set< ::fwServices::ComChannelService::wptr, SetLess > NotifiedOberverContainer;
    NotifiedOberverContainer notifiedObservers;

    // Insert global observers
    OSLM_DEBUG( "m_globalObservers.size() == " << m_globalObservers.size() );
    BOOST_FOREACH(::fwServices::ComChannelService::wptr comChannel, m_globalObservers)
    {
        notifiedObservers.insert( comChannel );
    }

    std::vector< std::string > eventIds = eventMessage->getEventIds();
    BOOST_FOREACH(std::string eventId, eventIds)
    {
        Event2ObserversContainer::iterator itOnSpecificObservers = m_event2SpecificObservers.find( eventId );
        if ( itOnSpecificObservers != m_event2SpecificObservers.end() )
        {
            ObserverContainer & specificObservers = itOnSpecificObservers->second;
            BOOST_FOREACH(::fwServices::ComChannelService::wptr comChannel, specificObservers)
            {
                notifiedObservers.insert( comChannel );
            }
        }
    }

    // Notifies messages
    // We work on copy of the observer collection because observers could be attached
    // or detached while forwarding the event and this would invalidate iterators
    // or lead to inconsistent notifications.
    OSLM_DEBUG( "notifiedObservers.size() == " << notifiedObservers.size() );
    for(    NotifiedOberverContainer::iterator iter = notifiedObservers.begin();
            iter != notifiedObservers.end();
            ++iter )
    {
        if( ! iter->expired() )
        {
            (*iter).lock()->sendMessage( eventMessage, options ) ;
        }
    }
}

//-----------------------------------------------------------------------------

size_t IEditionService::getNbObservers() const
{
    size_t nbObservers = m_globalObservers.size();
    BOOST_FOREACH(Event2ObserversContainer::value_type eventObservers, m_event2SpecificObservers)
    {
        ObserverContainer & specificObservers = eventObservers.second;
        nbObservers += specificObservers.size();
    }
    return nbObservers;
}

//-----------------------------------------------------------------------------

}
