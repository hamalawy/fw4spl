/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <fwRuntime/operations.hpp>

#include <fwData/Object.hpp>

#include "fwServices/Base.hpp"
#include "fwServices/registry/ObjectService.hpp"
#include "fwServices/registry/ServiceConfig.hpp"
#include "fwServices/registry/Proxy.hpp"
#include "fwServices/registry/ActiveWorkers.hpp"
#include "fwServices/AppConfigManager.hpp"

namespace fwServices
{
// Private methods

::fwData::Object::sptr AppConfigManager::getNewObject(
        ConfigAttribute type,
        ConfigAttribute uid,
        ConfigAttribute id)
{
    // Building object structure
    SPTR(::fwRuntime::Extension) ext = ::fwRuntime::findExtension(::boost::get<0>(type));
    if (ext)
    {
        std::string className = ::fwCore::getClassname< ::fwData::Object >();
        SLM_ASSERT("Extension and classname are different.",
                   ext->getPoint() == className);

        // Start dll to retrieve proxy and register object
        ext->getBundle()->start();
    }

    ::fwData::Object::sptr obj;
    obj = ::fwData::factory::New(::boost::get<0>(type));
    OSLM_ASSERT("Factory failed to build object : " <<  ::boost::get<0>(type), obj);

    if (::boost::get<1>(uid))
    {
#ifndef COM_LOG
        OSLM_ASSERT("Object already has an UID.", !obj->hasID());
#endif

        OSLM_FATAL_IF("UID " << ::boost::get<0>(uid) << " already exists.",
                      ::fwTools::fwID::exist(::boost::get<0>(uid)));
        obj->setID(::boost::get<0>(uid));
    }

    if (::boost::get<1>(id))
    {
        obj->setName(::boost::get<0>(id));
    }
    return obj;
}

// ------------------------------------------------------------------------

::fwData::Object::sptr AppConfigManager::getNewObject(
        ConfigAttribute type,
        const std::string& uid,
        ConfigAttribute id)
{
    return this->getNewObject(type, ConfigAttribute(uid, true), id);
}

// ------------------------------------------------------------------------

::fwData::Object::sptr AppConfigManager::getRefObject(
        ConfigAttribute type,
        const std::string& uid,
        ConfigAttribute id)
{
    OSLM_ASSERT("Object with UID \"" << uid << "\" doesn't exist.", ::fwTools::fwID::exist(uid));
    ::fwData::Object::sptr obj;
    obj = ::fwData::Object::dynamicCast(::fwTools::fwID::getObject(uid));

    if (::boost::get<1>(type))
    {
        OSLM_ASSERT("Object with UID \"" << uid
                << "\" has a different type (\""
                << obj->getClassname() << "\").",
                ::boost::get<0>(type) == obj->getClassname());
    }
    if (::boost::get<1>(id))
    {
        OSLM_ASSERT("Object with UID \"" << uid
                << "\" has a different id (\""
                << obj->getName() << "\").",
                ::boost::get<0>(id) == obj->getName());
    }
    return obj;
}

// ------------------------------------------------------------------------

::fwServices::IService::sptr AppConfigManager::getNewService(
        ConfigAttribute type,
        ConfigAttribute uid,
        ConfigAttribute implType)
{
    ::fwServices::registry::ServiceFactory::sptr srvFactory = ::fwServices::registry::ServiceFactory::getDefault();

    ::fwServices::IService::sptr srv;
    std::string typestr = ::boost::get<0>(type);
    if(typestr.empty())
    {
        srv = srvFactory->create(::boost::get<0>(implType));
    }
    else
    {
        srv = srvFactory->create(typestr, ::boost::get<0>(implType));
    }

#ifndef COM_LOG
    OSLM_ASSERT("Service already has an UID.", !srv->hasID());
#endif

    OSLM_FATAL_IF("UID " << ::boost::get<0>(uid) << " already exists.", ::fwTools::fwID::exist(::boost::get<0>(uid)));
    if (::boost::get<1>(uid))
    {
        srv->setID(::boost::get<0>(uid));
    }

    return srv;
}

// ------------------------------------------------------------------------

void AppConfigManager::autoSigSlotConnection(
        ::fwData::Object::sptr obj,
        ::fwServices::IService::sptr srv,
        ConfigAttribute priority)
{
    m_connections->connect( obj, srv, srv->getObjSrvConnections() );

//    // Add priority for the new comChannel, default is 0.5
//    if (::boost::get<1>(priority))
//    {
//        double priorityValue = ::boost::lexical_cast<double>(::boost::get<0>(priority));
//        if (priorityValue < 0.0)
//        {
//            priorityValue = 0.0;
//        }
//        else if (priorityValue > 1.0)
//        {
//            priorityValue = 1.0;
//        }
//        comChannel->setPriority(priorityValue);
//    }
}

// ------------------------------------------------------------------------

void AppConfigManager::startConnections()
{
    /// Connections are already started
}

// ------------------------------------------------------------------------

void AppConfigManager::stopConnections()
{
    m_connections->disconnect();
}

// ------------------------------------------------------------------------

void AppConfigManager::stopStartedServices()
{
    std::vector< ::fwServices::IService::SharedFutureType > futures;

    BOOST_REVERSE_FOREACH(::fwServices::IService::wptr w_srv, m_startedSrv)
    {
        SLM_ASSERT("Service expired.", !w_srv.expired());

        ::fwServices::IService::sptr srv = w_srv.lock();
        OSLM_ASSERT("Service " << srv->getID() << " already stopped.", !srv->isStopped());
        futures.push_back(srv->stop());
    }
    m_startedSrv.clear();
    ::boost::wait_for_all(futures.begin(), futures.end());
}

// ------------------------------------------------------------------------

void AppConfigManager::destroyCreatedServices()
{
    BOOST_REVERSE_FOREACH(::fwServices::IService::wptr w_srv, m_createdSrv)
    {
        SLM_ASSERT("Service expired.", !w_srv.expired());

        ::fwServices::IService::sptr srv = w_srv.lock();
        OSLM_ASSERT("Service " << srv->getID() << " must be stopped before destruction.", srv->isStopped());
        ::fwServices::OSR::unregisterService(srv);
    }
    m_createdSrv.clear();
}

// ------------------------------------------------------------------------

void AppConfigManager::processStartItems()
{
    std::vector< ::fwServices::IService::SharedFutureType > futures;

    BOOST_FOREACH(::fwRuntime::ConfigurationElement::csptr elem, m_cfgElem->getElements())
    {
        if (elem->getName() == "start")
        {
            SLM_ERROR_IF("\"type\" attribute on \"start\" node is deprecated. Use UID instead.",
                        elem->hasAttribute("type"));

            // Uid
            SLM_ASSERT("Missing attribute \"uid\".", elem->hasAttribute("uid"));
            std::string uid = elem->getExistingAttributeValue("uid");

            OSLM_ASSERT("Service with UID \"" << uid << "\" doesn't exist.", ::fwTools::fwID::exist(uid));

            ::fwServices::IService::sptr srv = ::fwServices::get(uid);

            OSLM_ASSERT("No service registered with UID \"" << uid << "\".", srv);

            futures.push_back(srv->start());
            m_startedSrv.push_back(srv);
        }
    }
    ::boost::wait_for_all(futures.begin(), futures.end());
}

// ------------------------------------------------------------------------

void AppConfigManager::processUpdateItems()
{
    std::vector< ::fwServices::IService::SharedFutureType > futures;

    BOOST_FOREACH(::fwRuntime::ConfigurationElement::csptr elem, m_cfgElem->getElements())
    {
        if (elem->getName() == "update")
        {
            // Type
            if (elem->hasAttribute("type"))
            {
                std::string type = elem->getExistingAttributeValue("type");

                std::vector<fwServices::IService::sptr> servicesToUpdate = ::fwServices::OSR::getServices(type);

                OSLM_ASSERT("No services of type \"" << type << "\" found.", !servicesToUpdate.empty());

                BOOST_FOREACH(::fwServices::IService::sptr srv, servicesToUpdate)
                {
                    futures.push_back(srv->update());
                }
            }
            else
            {
                SLM_ASSERT("Missing attribute \"uid\" or \"type\".", elem->hasAttribute("uid"));
                std::string uid = elem->getExistingAttributeValue("uid");

                OSLM_ASSERT("Service with UID \"" << uid << "\" doesn't exist.", ::fwTools::fwID::exist(uid));

                futures.push_back(::fwServices::get(uid)->update());
            }
        }
    }

    ::boost::wait_for_all(futures.begin(), futures.end());
}

// ------------------------------------------------------------------------

::fwData::Object::sptr AppConfigManager::createObject()
{
    OSLM_ASSERT("Bad configuration element.", m_cfgElem->getName() == "object");

    // Get attributes

    // Uid
    ConfigAttribute uid("", false);
    if (m_cfgElem->hasAttribute("uid"))
    {
        ::boost::get<0>(uid) = m_cfgElem->getExistingAttributeValue("uid");
        SLM_ASSERT("\"uid\" attribute is empty.", !::boost::get<0>(uid).empty());
        ::boost::get<1>(uid) = true;
    }

    // Id
    ConfigAttribute id("", false);
    if (m_cfgElem->hasAttribute("id"))
    {
        ::boost::get<0>(id) = m_cfgElem->getExistingAttributeValue("id");
        SLM_ASSERT("\"id\" attribute is empty.", !::boost::get<0>(id).empty());
        ::boost::get<1>(id) = true;
    }

    // Type
    ConfigAttribute type("", false);
    if (m_cfgElem->hasAttribute("type"))
    {
        ::boost::get<0>(type) = m_cfgElem->getExistingAttributeValue("type");
        SLM_ASSERT("\"type\" attribute is empty.", !::boost::get<0>(type).empty());
        SLM_ASSERT("\"type\" must be a rooted namespace.",
                ::boost::get<0>(type).substr(0, 2) == "::");
        ::boost::get<1>(type) = true;
    }

    // Buildmode
    ConfigAttribute buildMode("", false);
    if (m_cfgElem->hasAttribute("src"))
    {
        ::boost::get<0>(buildMode) = m_cfgElem->getExistingAttributeValue("src");
        SLM_ASSERT("\"src\" attribute is empty.", !::boost::get<0>(buildMode).empty());

        SLM_ASSERT("Unhandled build mode (bad \"src\" attribute). "
                "Must be \"new\" or \"ref\".",
                ::boost::get<0>(buildMode) == "ref" ||
                ::boost::get<0>(buildMode) == "src");
        ::boost::get<1>(buildMode) = true;
    }

    // Creation of a new object
    ::fwData::Object::sptr obj;

    // Create new or get the referenced object
    if (::boost::get<1>(buildMode) && ::boost::get<0>(buildMode) == "ref")
    {
        SLM_ASSERT("Missing attribute \"uid\".", ::boost::get<1>(uid));
        obj = getRefObject(type, ::boost::get<0>(uid), id);
    }
    else
    {
        obj = getNewObject(type, uid, id);
    }

    // Get the object parser associated with the object type
    ::fwServices::registry::ServiceFactory::sptr srvFactory = ::fwServices::registry::ServiceFactory::getDefault();

    std::string srvImpl = srvFactory->getDefaultImplementationIdFromObjectAndType(
                          obj->getClassname(), "::fwServices::IXMLParser");

    ::fwServices::IService::sptr srv =  srvFactory->create("::fwServices::IXMLParser", srvImpl);
    m_objectParser = ::fwServices::IXMLParser::dynamicCast(srv);
    m_objectParser->setObjectConfig(m_cfgElem);
    m_objectParser->createConfig(obj);
    return obj;
}

// ------------------------------------------------------------------------

void AppConfigManager::createServices()
{
    BOOST_FOREACH(::fwRuntime::ConfigurationElement::csptr elem,  m_cfgElem->getElements())
    {
        if (elem->getName() == "service")
        {
            this->bindService(elem);
        }
        else if (elem->getName() == "serviceList")
        {
            this->createServices(elem);
        }
    }
}

// ------------------------------------------------------------------------

void AppConfigManager::createServices(::fwRuntime::ConfigurationElement::csptr cfgElem)
{
    BOOST_FOREACH(::fwRuntime::ConfigurationElement::csptr elem, cfgElem->getElements())
    {
        if (elem->getName() == "service")
        {
            this->bindService(elem);
        }
        else if (elem->getName() == "serviceList")
        {
            this->createServices(elem);
        }
    }
}

// ------------------------------------------------------------------------

void AppConfigManager::bindService(::fwRuntime::ConfigurationElement::csptr srvElem)
{
    SLM_ASSERT("Configuration element is not a \"service\" node.", srvElem->getName() == "service");

    // Get attributes

    // Uid
    ConfigAttribute uid("", false);
    if (srvElem->hasAttribute("uid"))
    {
        ::boost::get<0>(uid) = srvElem->getExistingAttributeValue("uid");
        OSLM_ASSERT("\"uid\" attribute is empty.", !::boost::get<0>(uid).empty());
        ::boost::get<1>(uid) = true;
    }

    // Type
    std::string type = srvElem->getAttributeValue("type");

    // Implementation
    ConfigAttribute implType("", false);
    if (srvElem->hasAttribute("implementation"))
    {
        ::boost::get<0>(implType) = srvElem->getExistingAttributeValue("implementation");
        SLM_ASSERT("\"implementation\" attribute is empty.", !::boost::get<0>(implType).empty());

        ::boost::get<1>(implType) = true;
    }

    // Type
    std::string workerKey = srvElem->getAttributeValue("worker");

    // Config
    ConfigAttribute config("", false);
    if (srvElem->hasAttribute("config"))
    {
        ::boost::get<0>(config) = srvElem->getExistingAttributeValue("config");
        SLM_ASSERT("\"config\" attribute is empty.", !::boost::get<0>(config).empty());

        ::boost::get<1>(config) = true;
    }

    // AutoComChannel
    SLM_ASSERT("Missing attribute \"autoComChannel\".", srvElem->hasAttribute("autoComChannel"));
    std::string autoComChannel = srvElem->getExistingAttributeValue("autoComChannel");
    SLM_ASSERT("\"autoComChannel\" attribute must be either \"yes\" or \"no\".",
               autoComChannel == "yes" || autoComChannel == "no");

    // Priority
    ConfigAttribute priority("", false);
    if (srvElem->hasAttribute("priority"))
    {
        ::boost::get<0>(priority) = srvElem->getExistingAttributeValue("priority");
        SLM_ASSERT("\"priority\" attribute is empty.", !::boost::get<0>(priority).empty());

        ::boost::get<1>(priority) = true;
    }

    // TODO: have a default implementation of service
    if (!::boost::get<1>(implType))
    {
        OSLM_FATAL("Requesting default implementation for service of type "  << type << " is not implemented yet.");
    }

    // Create and bind service
    ::fwServices::IService::sptr srv = this->getNewService(type, uid, implType);
    ::fwServices::OSR::registerService(m_configuredObject, srv);
    m_createdSrv.push_back(srv);

    if (!workerKey.empty())
    {
        ::fwServices::registry::ActiveWorkers::sptr activeWorkers = ::fwServices::registry::ActiveWorkers::getDefault();
        ::fwThread::Worker::sptr worker;
        worker = activeWorkers->getWorker(workerKey);
        if (!worker)
        {
            worker = ::fwThread::Worker::New();
            activeWorkers->addWorker(workerKey, worker);
        }
        srv->setWorker(worker);
    }

    // Get service configuration
    ::fwRuntime::ConfigurationElement::csptr cfgElem = srvElem;

    ::fwServices::registry::ServiceConfig::sptr srvCfgFactory = ::fwServices::registry::ServiceConfig::getDefault();
    if (::boost::get<1>(config))
    {
        cfgElem = srvCfgFactory->getServiceConfig(::boost::get<0>(config), ::boost::get<0>(implType));
    }

    // Configure
    SLM_ASSERT("ConstCast failed.", ::fwRuntime::ConfigurationElement::constCast(cfgElem));
    srv->setConfiguration(::fwRuntime::ConfigurationElement::constCast(cfgElem));
    srv->configure();

    // Communication channel
    if (autoComChannel == "yes")
    {
        this->autoSigSlotConnection(m_configuredObject, srv, priority);
    }

    // Check if user did not bind a service to another service
    BOOST_FOREACH(::fwRuntime::ConfigurationElement::csptr elem, cfgElem->getElements())
    {
        SLM_ASSERT("Cannot bind a service to another service.",
                elem->getName() != "service" &&
                elem->getName() != "serviceList");
    }
}

// Constructors / Destructors
// ------------------------------------------------------------------------

AppConfigManager::AppConfigManager() : m_state(STATE_DESTROYED), m_connections( helper::SigSlotConnection::New() )
{}

// ------------------------------------------------------------------------

AppConfigManager::~AppConfigManager()
{
    SLM_ASSERT("Manager must be stopped before destruction.", m_state == STATE_DESTROYED);
}

// Public methods
// ------------------------------------------------------------------------

void AppConfigManager::create()
{
    SLM_ASSERT("Manager already running.", m_state == STATE_DESTROYED);
    m_configuredObject = this->createObject();
    this->createServices();
    this->createConnections();

    m_state = STATE_CREATED;
}

// ------------------------------------------------------------------------

void AppConfigManager::start()
{
    SLM_ASSERT("Manager must be created first.", m_state == STATE_CREATED || m_state == STATE_STOPPED);

    this->processStartItems() ;
    m_objectParser->startConfig();
    this->startConnections();

    m_state = STATE_STARTED;
}

// ------------------------------------------------------------------------

void AppConfigManager::update()
{
    this->processUpdateItems();
    m_objectParser->updateConfig();
}

// ------------------------------------------------------------------------

void AppConfigManager::stop()
{
    SLM_ASSERT("Manager is not started, cannot stop.", m_state == STATE_STARTED);

    this->destroyProxies();
    this->stopConnections();
    m_objectParser->stopConfig();
    this->stopStartedServices();

    OSLM_INFO("Parsing OSR after stopping the config :" << std::endl
              << ::fwServices::OSR::getRegistryInformation());
    m_state = STATE_STOPPED;
}

// ------------------------------------------------------------------------

void AppConfigManager::destroy()
{
    SLM_ASSERT("Manager is not stopped, cannot destroy.", m_state == STATE_STOPPED || m_state == STATE_CREATED);

    m_objectParser->destroyConfig();
    this->destroyCreatedServices();

    OSLM_INFO("Parsing OSR after destroying the config :" << std::endl
            << ::fwServices::OSR::getRegistryInformation());

    m_objectParser.reset();
    m_cfgElem.reset();
    m_configuredObject.reset();

    m_state = STATE_DESTROYED;
}

// ------------------------------------------------------------------------

void AppConfigManager::launch()
{
    this->create();
    this->start();
    this->update();
}

// ------------------------------------------------------------------------

void AppConfigManager::stopAndDestroy()
{
    this->stop();
    this->destroy();
}

// ------------------------------------------------------------------------

void AppConfigManager::createConnections()
{
    BOOST_FOREACH(::fwRuntime::ConfigurationElement::csptr elem,  m_cfgElem->getElements())
    {
        if (elem->getName() == "connect")
        {
            this->createConnection(elem);
        }
        else if (elem->getName() == "proxy")
        {
            this->createProxy(elem);
        }
    }
}

// ------------------------------------------------------------------------

void AppConfigManager::createConnection(::fwRuntime::ConfigurationElement::csptr connectionCfg)
{
    typedef std::pair< std::string, ::fwCom::Signals::SignalKeyType > SignalInfoType;
    typedef std::pair< std::string, ::fwCom::Slots::SlotKeyType > SlotInfoType;
    typedef std::vector< SlotInfoType > SlotInfoContainerType;

    SignalInfoType signalInfo;
    SlotInfoContainerType slotInfos;

    ::boost::regex re("(.*)/(.*)");
    ::boost::smatch match;
    std::string src, uid, key;

    BOOST_FOREACH(::fwRuntime::ConfigurationElement::csptr elem,  connectionCfg->getElements())
    {
        src = elem->getValue();
        if( ::boost::regex_match(src, match, re) )
        {
            OSLM_ASSERT("Wrong value for attribute src: "<<src, match.size() >= 3);
            uid.assign(match[1].first, match[1].second);
            key.assign(match[2].first, match[2].second);

            OSLM_ASSERT(src << " configuration is not correct for "<< elem->getName() ,
                        !uid.empty() && !key.empty());

            if (elem->getName() == "signal")
            {
                SLM_ASSERT("There must be only one signal by connection",
                           signalInfo.first.empty() && signalInfo.second.empty());
                signalInfo = std::make_pair(uid, key);
            }
            else if (elem->getName() == "slot")
            {
                slotInfos.push_back( std::make_pair(uid, key) );
            }
        }
    }

    ::fwTools::Object::sptr obj = ::fwTools::fwID::getObject(signalInfo.first);
    ::fwCom::HasSignals::sptr hasSignals = ::boost::dynamic_pointer_cast< ::fwCom::HasSignals >(obj);


    BOOST_FOREACH(SlotInfoType slotInfo,  slotInfos)
    {
        ::fwTools::Object::sptr obj = ::fwTools::fwID::getObject(slotInfo.first);
        ::fwCom::HasSlots::sptr hasSlots = ::boost::dynamic_pointer_cast< ::fwCom::HasSlots >(obj);

        m_connections->connect(hasSignals, signalInfo.second, hasSlots, slotInfo.second);
    }
}

// ------------------------------------------------------------------------

void AppConfigManager::createProxy(::fwRuntime::ConfigurationElement::csptr config)
{
    ::boost::regex re("(.*)/(.*)");
    ::boost::smatch match;
    std::string src, uid, key;

    ::fwServices::registry::Proxy::sptr proxy = ::fwServices::registry::Proxy::getDefault();

    SLM_ASSERT("Missing 'channel' attribute", config->hasAttribute("channel"));
    std::string channel = config->getAttributeValue("channel");
    ProxyConnections proxyCnt(channel);

    BOOST_FOREACH(::fwRuntime::ConfigurationElement::csptr elem,  config->getElements())
    {
        src = elem->getValue();
        if( ::boost::regex_match(src, match, re) )
        {
            OSLM_ASSERT("Wrong value for attribute src: "<<src, match.size() >= 3);
            uid.assign(match[1].first, match[1].second);
            key.assign(match[2].first, match[2].second);

            OSLM_ASSERT(src << " configuration is not correct for "<< elem->getName() ,
                        !uid.empty() && !key.empty());

            ::fwTools::Object::sptr obj = ::fwTools::fwID::getObject(uid);

            if (elem->getName() == "signal")
            {
                ::fwCom::HasSignals::sptr hasSignals = ::boost::dynamic_pointer_cast< ::fwCom::HasSignals >(obj);
                ::fwCom::SignalBase::sptr sig = hasSignals->signal(key);
                proxy->connect(channel, sig);
                proxyCnt.addSignalConnection(uid, key);
            }
            else if (elem->getName() == "slot")
            {
                ::fwCom::HasSlots::sptr hasSlots = ::boost::dynamic_pointer_cast< ::fwCom::HasSlots >(obj);
                ::fwCom::SlotBase::sptr slot = hasSlots->slot(key);
                proxy->connect(channel, slot);
                proxyCnt.addSlotConnection(uid, key);
            }
        }
    }
    m_vectProxyCtns.push_back(proxyCnt);
}

// ------------------------------------------------------------------------

void AppConfigManager::destroyProxies()
{
    ::fwServices::registry::Proxy::sptr proxy = ::fwServices::registry::Proxy::getDefault();
    BOOST_FOREACH(ProxyConnectionsVectType::value_type proxyConnections,  m_vectProxyCtns)
    {
        BOOST_FOREACH(ProxyConnections::ProxyEltType signalElt, proxyConnections.m_signals)
        {
            ::fwTools::Object::sptr obj = ::fwTools::fwID::getObject(signalElt.first);
            ::fwCom::HasSignals::sptr hasSignals = ::boost::dynamic_pointer_cast< ::fwCom::HasSignals >(obj);
            ::fwCom::SignalBase::sptr sig = hasSignals->signal(signalElt.second);
            proxy->disconnect(proxyConnections.m_channel, sig);
        }
        BOOST_FOREACH(ProxyConnections::ProxyEltType slotElt, proxyConnections.m_slots)
        {
            ::fwTools::Object::sptr obj = ::fwTools::fwID::getObject(slotElt.first);
            ::fwCom::HasSlots::sptr hasSlots = ::boost::dynamic_pointer_cast< ::fwCom::HasSlots >(obj);
            ::fwCom::SlotBase::sptr slot = hasSlots->slot(slotElt.second);
            proxy->disconnect(proxyConnections.m_channel, slot);
        }
    }
    m_vectProxyCtns.clear();
}

}
