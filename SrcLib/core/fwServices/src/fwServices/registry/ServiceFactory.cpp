/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <vector>

#include <fwTools/IClassFactory.hpp>
#include <fwTools/TBKClassFactory.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>
#include <fwRuntime/profile/Profile.hpp>

#include "fwServices/registry/ServiceFactory.hpp"


namespace fwServices
{
namespace registry
{

//-----------------------------------------------------------------------------

ServiceFactory::sptr ServiceFactory::getDefault()
{
    SLM_TRACE_FUNC();
    static ServiceFactory::sptr m_instance = ServiceFactory::New();
    return m_instance;
}

//-----------------------------------------------------------------------------

ServiceFactory::~ServiceFactory()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ServiceFactory::parseBundleInformation()
{
    SrvRegContainer bundleInfoMap;

    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;
    typedef ::boost::shared_ptr< ::fwRuntime::Extension > ExtensionType;

    std::vector< ExtensionType >  extElements;
    extElements = ::fwRuntime::getAllExtensionsForPoint("::fwServices::registry::ServiceFactory");
    BOOST_FOREACH(ExtensionType extElt , extElements)
    {
        std::vector< ConfigurationType > cfgEltVec = extElt->getElements();
        SLM_ASSERT("extension element MUST have 3 or 4 elements", cfgEltVec.size() == 3 || cfgEltVec.size() == 4);
        std::string type = "";
        std::string service ="";
        std::string object= "";
        std::string desc= "";

        BOOST_FOREACH(ConfigurationType cfgElt, cfgEltVec)
        {
            std::string elt = cfgElt->getName();
            if(elt == "type")
            {
                type = cfgElt->getValue();
            }
            else if(elt == "service")
            {
                service = cfgElt->getValue();
            }
            else if(elt == "object")
            {
                object = cfgElt->getValue();
            }
            else if(elt == "desc")
            {
                desc = cfgElt->getValue();
            }
            else
            {
                SLM_FATAL("Unknown element !");
            }
        }
        SLM_ASSERT("Missing type element.", !type.empty());
        SLM_ASSERT("Missing service element.", !service.empty());
        SLM_ASSERT("Missing object element.", !object.empty());

        ServiceFactoryInfo::NewSptr info;
        info->serviceType = type;
        info->objectImpl = object;
        info->desc = desc;

        info->bundle = cfgEltVec[0]->getBundle();
        SLM_ASSERT("Bundle not find.", info->bundle );

        bundleInfoMap[ service] = info;
    }
    // Verify object
    BOOST_FOREACH(SrvRegContainer::value_type bundle, bundleInfoMap)
    {
        if ( bundle.second->objectImpl.empty() )
        {
            OSLM_WARN("Service " << bundle.first << " of type  "
                    << bundle.second->serviceType << " in bundle " << bundle.second->bundle->getIdentifier()
                    << " has not object defined in plugin.xml ( declaration missing in dataReg ? ). Else is a service of service (sorry but not managed in this version )");

        }
    }

#if SPYLOG_LEVEL >= 5
    //Print information
    this->printInfoMap( bundleInfoMap );
#endif

    // Merge data info
    BOOST_FOREACH(SrvRegContainer::value_type bundle, bundleInfoMap)
    {
        SrvRegContainer::iterator iter = m_srvImplTosrvInfo.find( bundle.first );

        if ( iter != m_srvImplTosrvInfo.end() )
        {
            OSLM_DEBUG("Have already information (from register maccro) about this service ( " << bundle.first << " )." );
            ServiceFactoryInfo::sptr info = iter->second;
            ServiceFactoryInfo::sptr infoBundle = bundle.second;
            SLM_ASSERT("Try to add bundle, but bundle exists.", ! info->bundle );
            SLM_ASSERT("Try to add bundle, but this srv is already register and has not the same srv type.", infoBundle->serviceType == info->serviceType );
            SLM_ASSERT("Try to add bundle, but this srv (" << bundle.first << ")is already register and has not the same oimpl. ( "<< infoBundle->objectImpl <<" != "<< info->objectImpl <<" )", infoBundle->objectImpl == info->objectImpl );
            info->bundle =  infoBundle->bundle;
            info->desc =  infoBundle->desc;
        }
        else
        {
            m_srvImplTosrvInfo[ bundle.first ] = bundle.second;
        }

    }

#if SPYLOG_LEVEL >= 5
    //Print information
    this->printInfoMap( m_srvImplTosrvInfo );
#endif
    this->checkServicesNotDeclaredInPluginXml();
}

//-----------------------------------------------------------------------------

IService::sptr ServiceFactory::create( const std::string & _srvType, const std::string & _srvImpl )
{
    IService::sptr service;
    SrvRegContainer::iterator iter = m_srvImplTosrvInfo.find( _srvImpl );
    OSLM_ASSERT("Sorry don't find in ServiceFactory the service called " << _srvImpl , iter != m_srvImplTosrvInfo.end() );
    ServiceFactoryInfo::sptr info = iter->second;

    OSLM_DEBUG("SR create a new service ( classname = " << _srvImpl << " )");
    OSLM_ASSERT("Sorry, type of service must correspond. "<< _srvType << " != " << info->serviceType, _srvType == info->serviceType);
    if ( info->factory )
    {
        service = info->factory->create();
    }
    else
    {
        OSLM_ASSERT( "Sorry a bundle must declare the factory "<< _srvImpl <<". Service declaration is missing (or misspelled) in a bundle plugin ?", info->bundle );
        OSLM_ASSERT( "Sorry bundle is already load ( " << info->bundle->getIdentifier() << " ) , factory "<< _srvImpl << " is still missing. Service declaration is missing (or misspelled) in a .cpp file ?", ! info->bundle->isStarted() );

        info->bundle->start();
        ::fwRuntime::profile::getCurrentProfile()->setup();
        SLM_ASSERT( "Sorry after bundle loading ( " << info->bundle->getIdentifier() << " ) , factory "<< _srvImpl << " is still missing. Service declaration is missing (or misspelled) in a .cpp file ?", info->factory );
        service = info->factory->create();
    }

    this->checkServicesNotDeclaredInPluginXml();
    return service;
}

//------------------------------------------------------------------------------

void ServiceFactory::addFactory( ::fwTools::IClassFactory::sptr _factory,
        const std::string & simpl,
        const std::string & stype,
        const std::string & oimpl)
{
    OSLM_DEBUG( "New service registering : "
            << " simpl =" << simpl
            << " stype=" << stype
            << " oimpl=" << oimpl
    );

    SrvRegContainer::iterator iter = m_srvImplTosrvInfo.find( simpl );

    if ( iter != m_srvImplTosrvInfo.end() )
    {
        OSLM_DEBUG("Have already information about this service ( " << simpl << " )." );
        ServiceFactoryInfo::sptr info = iter->second;
        OSLM_ASSERT("Try to add factory, but this srv ( " << simpl << " ) has already a registered factory.", ! info->factory );
        OSLM_ASSERT("Try to add factory, but this srv ( " << simpl << " ) is already register and has not the same srv type. ( " << stype << " != " << info->serviceType <<" )", stype == info->serviceType );
        OSLM_ASSERT("Try to add factory, but this srv ( " << simpl << " ) is already register and has not the same oimpl. ( " << oimpl << " != " << info->objectImpl <<" )", oimpl == info->objectImpl );
        info->factory =  ::boost::dynamic_pointer_cast< ::fwTools::TBKClassFactory< ::fwServices::IService, std::string > >( _factory );
    }
    else
    {
        OSLM_DEBUG("Add new service factory in registry ( " << simpl << " )." );
        ServiceFactoryInfo::NewSptr info;
        info->serviceType = stype;
        info->objectImpl = oimpl;
        info->factory =  ::boost::dynamic_pointer_cast< ::fwTools::TBKClassFactory< ::fwServices::IService, std::string > >( _factory );
        m_srvImplTosrvInfo[simpl] = info;
    }
}

//-----------------------------------------------------------------------------

ServiceFactory::ServiceFactory()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ServiceFactory::printInfoMap( const SrvRegContainer & src )
{
    //Print information
    BOOST_FOREACH(SrvRegContainer::value_type srvReg, src)
    {
        OSLM_DEBUG(" Service name = " << srvReg.first );
        OSLM_DEBUG("  - type   = " << srvReg.second->serviceType );
        OSLM_DEBUG("  - object = " << srvReg.second->objectImpl);

        OSLM_DEBUG_IF("  - bundle = " <<  srvReg.second->bundle->getIdentifier(), srvReg.second->bundle );
        OSLM_DEBUG_IF("  - bundle = ( no bundle registered )", ! srvReg.second->bundle );

        OSLM_DEBUG_IF("  - name after creation = " <<  srvReg.second->factory->create()->getClassname(), srvReg.second->factory );
        OSLM_DEBUG_IF("  - name after creation = ( no factory registered )", ! srvReg.second->factory );
    }
}

//-----------------------------------------------------------------------------

void ServiceFactory::checkServicesNotDeclaredInPluginXml()
{
    //Print information
    BOOST_FOREACH(SrvRegContainer::value_type srvReg, m_srvImplTosrvInfo)
    {
        if ( ! srvReg.second->bundle )
        {
            OSLM_WARN("Service " << srvReg.first << " is not declared/found in a plugin.xml." );
        }
    }
}

//-----------------------------------------------------------------------------

void ServiceFactory::clearFactory()
{
    m_srvImplTosrvInfo.clear();
}

//-----------------------------------------------------------------------------

std::vector< std::string > ServiceFactory::getImplementationIdFromObjectAndType( std::string object, std::string type )
{
    std::vector< std::string > serviceImpl;

    BOOST_FOREACH(SrvRegContainer::value_type srv, m_srvImplTosrvInfo)
    {
        ServiceFactoryInfo::sptr srvInfo = srv.second;
        if(srvInfo->serviceType == type && (srvInfo->objectImpl == object || srvInfo->objectImpl == "::fwData::Object") )
        {
            serviceImpl.push_back(srv.first);
        }
    }
    return serviceImpl;
}

//-----------------------------------------------------------------------------

std::string ServiceFactory::getDefaultImplementationIdFromObjectAndType( std::string object, std::string type )
{
    SLM_ASSERT("Sorry, this case is not managed ", object != "::fwData::Object" );

    std::string serviceImpl = "";
    bool genericImplIsFound = false;
    bool specificImplIsFound = false;

    BOOST_FOREACH( SrvRegContainer::value_type srv, m_srvImplTosrvInfo )
    {
        ServiceFactoryInfo::sptr srvInfo = srv.second;
        if ( srvInfo->serviceType == type )
        {
            if ( srvInfo->objectImpl == object )
            {
                OSLM_ASSERT("Sorry, method has already found a specific ("<< serviceImpl <<" != " << srv.first << ") service for the object (" << srvInfo->objectImpl << ").", ! specificImplIsFound );
                specificImplIsFound = true;
                serviceImpl = srv.first;
            }
            else if ( srvInfo->objectImpl == "::fwData::Object" )
            {
                OSLM_ASSERT("Sorry, method has already found a generic service for the object (" << srvInfo->objectImpl << ").", ! genericImplIsFound );
                genericImplIsFound = true;
                if ( ! specificImplIsFound )
                {
                    serviceImpl = srv.first;
                }
            }
        }
    }

    OSLM_ASSERT("Sorry, default implementation is not found for this type of service "<<type, ! serviceImpl.empty() );

    return serviceImpl;
}

//-----------------------------------------------------------------------------

std::string ServiceFactory::getServiceDescription(std::string srvImpl)
{
    std::string srvDescription;
    SLM_ASSERT("The service " << srvImpl << " is not found.", m_srvImplTosrvInfo.find(srvImpl)!= m_srvImplTosrvInfo.end());
    srvDescription = m_srvImplTosrvInfo[srvImpl]->desc;
    return srvDescription;
}

//-----------------------------------------------------------------------------

bool ServiceFactory::checkServiceValidity(const std::string & objectClassName, const std::string & srvImpl)
{
    bool isValid = true;
    isValid &= (m_srvImplTosrvInfo.find(srvImpl)!= m_srvImplTosrvInfo.end());
    if (isValid)
    {
        ServiceFactoryInfo::sptr srvInfo = m_srvImplTosrvInfo[srvImpl];
        isValid &= (srvInfo->objectImpl == "::fwData::Object" || srvInfo->objectImpl == objectClassName);
    }
    return isValid;
}

//-----------------------------------------------------------------------------

bool ServiceFactory::support(const std::string & object, const std::string & srvType, const std::string & srvImpl)
{
    bool isSupported = true;
    isSupported &= (m_srvImplTosrvInfo.find(srvImpl)!= m_srvImplTosrvInfo.end());
    if (isSupported)
    {
        ServiceFactoryInfo::sptr srvInfo = m_srvImplTosrvInfo[srvImpl];
        isSupported &= (srvInfo->objectImpl == "::fwData::Object" || srvInfo->objectImpl == object);
        isSupported &= (srvInfo->serviceType == srvType);
    }
    return isSupported;
}

//-----------------------------------------------------------------------------

bool ServiceFactory::support(const std::string & object, const std::string & srvType)
{
    bool isSupported = false;
    SupportMapType::key_type key(object, srvType);
    SupportMapType::iterator iter = m_supportMap.find( key );
    if (iter != m_supportMap.end())
    {
        isSupported = iter->second;
    }
    else
    {
        BOOST_FOREACH(SrvRegContainer::value_type srv, m_srvImplTosrvInfo)
        {
            ServiceFactoryInfo::sptr srvInfo = srv.second;
            if(srvInfo->serviceType == srvType && (srvInfo->objectImpl == object || srvInfo->objectImpl == "::fwData::Object") )
            {
                isSupported = true;
                break;
            }
        }
        m_supportMap[key] = isSupported;
    }
    return isSupported;
}

//-----------------------------------------------------------------------------


} // namespace registry
} // namespace fwServices

