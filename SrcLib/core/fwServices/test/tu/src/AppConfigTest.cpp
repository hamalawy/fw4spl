/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/registry/AppConfig.hpp>
#include <fwData/String.hpp>
#include <fwTest/helper/Thread.hpp>

#include "AppConfigTest.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwServices::ut::AppConfigTest );

namespace fwServices
{
namespace ut
{

void AppConfigTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void AppConfigTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void AppConfigTest::parametersConfigTest()
{
    ::fwServices::registry::AppConfig::sptr currentAppConfig = ::fwServices::registry::AppConfig::getDefault();

    const std::string configId(::fwServices::registry::AppConfig::getUniqueIdentifier());
    const ::fwServices::registry::AppInfo::ConfigType configType(::fwServices::registry::AppInfo::PARAMETERS);
    const std::string group("parametersGroup");
    const std::string desc("Descritpion");
    ::fwServices::registry::AppInfo::ParamatersType parameters;

    parameters["TEST_IMAGE"] = "";
    parameters["UID_SERVICE2"] = "myTestService2";
    parameters["GENERIC_UID"] = "";

    ::fwRuntime::ConfigurationElement::csptr config = this->buildParametersConfig();

    currentAppConfig->addAppInfo(configId, configType, group, desc, parameters, config);

    std::vector< std::string > allCconfigs = currentAppConfig->getAllConfigs();
    CPPUNIT_ASSERT( !allCconfigs.empty());

    std::vector< std::string > configs = currentAppConfig->getConfigsFromGroup(group);
    CPPUNIT_ASSERT(!configs.empty());

    ::fwServices::registry::AppConfig::FieldAdaptorType replaceFields;
    replaceFields["TEST_IMAGE"] = "objectUUID";
    const std::string uniqueIdentifier = ::fwServices::registry::AppConfig::getUniqueIdentifier();
    replaceFields["GENERIC_UID"] = uniqueIdentifier;

    ::fwRuntime::ConfigurationElement::csptr configEltAdaptedConst;
    configEltAdaptedConst = currentAppConfig->getAdaptedTemplateConfig(configId, replaceFields);

    ::fwRuntime::ConfigurationElement::sptr configEltAdapted;
    configEltAdapted =  ::fwRuntime::ConfigurationElement::constCast(configEltAdaptedConst);

    std::string uid = configEltAdapted->getAttributeValue("uid");
    CPPUNIT_ASSERT_EQUAL(std::string("objectUUID"), uid );

    std::vector< ::fwRuntime::ConfigurationElement::sptr >  servicesCfg = configEltAdapted->find("service");

    std::string serviceUid1 = servicesCfg.at(0)->getAttributeValue("uid");
    CPPUNIT_ASSERT_EQUAL(uniqueIdentifier+"_myTestService1", serviceUid1);

    std::string serviceUid2 = servicesCfg.at(1)->getAttributeValue("uid");
    CPPUNIT_ASSERT_EQUAL( std::string("myTestService2"), serviceUid2);

}

//-----------------------------------------------------------------------------

void AppConfigTest::concurentAccessToAppConfigTest()
{
    const unsigned int nbThreads = 20;
    std::vector< SPTR(::fwTest::helper::Thread) > threads;

    for (int i=0 ; i<nbThreads ; ++i)
    {
        SPTR(::fwTest::helper::Thread) thread;
        thread = ::boost::shared_ptr< ::fwTest::helper::Thread >(
                new ::fwTest::helper::Thread(::boost::bind(&AppConfigTest::parametersConfigTest, this)));
        threads.push_back(thread);
    }

    for (int i=0 ; i<nbThreads ; ++i)
    {
        std::stringstream str;
        str << "thread " << i;
        CPPUNIT_ASSERT_MESSAGE(str.str(), threads[i]->timedJoin(1000));
    }

    ::fwServices::registry::AppConfig::getDefault()->clearRegistry();
    std::vector< std::string > allCconfigs = ::fwServices::registry::AppConfig::getDefault()->getAllConfigs();
    CPPUNIT_ASSERT(allCconfigs.empty());

}

//------------------------------------------------------------------------------

::fwRuntime::ConfigurationElement::sptr AppConfigTest::buildParametersConfig()
{
    // Configuration on fwTools::Object which uid is objectUUID
    ::boost::shared_ptr< ::fwRuntime::EConfigurationElement > cfg ( new ::fwRuntime::EConfigurationElement("object")) ;
    cfg->setAttributeValue( "uid" , "${TEST_IMAGE}") ;
    cfg->setAttributeValue( "type" , "::fwData::Image") ;

    // Object's service A
    ::boost::shared_ptr< ::fwRuntime::EConfigurationElement > serviceA = cfg->addConfigurationElement("service");
    serviceA->setAttributeValue( "uid" , "${GENERIC_UID}_myTestService1" ) ;
    serviceA->setAttributeValue( "type" , "::fwComEd::ut::TestService" ) ;
    serviceA->setAttributeValue( "impl" , "::fwComEd::ut::TestServiceImplementationImage" ) ;
    serviceA->setAttributeValue( "autoConnect" , "no" ) ;

    // Object's service B
    ::boost::shared_ptr< ::fwRuntime::EConfigurationElement > serviceB = cfg->addConfigurationElement("service");
    serviceB->setAttributeValue( "uid" , "${UID_SERVICE2}" ) ;
    serviceB->setAttributeValue( "type" , "::fwComEd::ut::TestService" ) ;
    serviceB->setAttributeValue( "impl" , "::fwComEd::ut::TestServiceImplementationImage" ) ;
    serviceB->setAttributeValue( "autoConnect" , "no" ) ;

    // Start method from object's services
    ::boost::shared_ptr< ::fwRuntime::EConfigurationElement > startA = cfg->addConfigurationElement("start");
    startA->setAttributeValue( "uid" , "myTestService1" ) ;
    ::boost::shared_ptr< ::fwRuntime::EConfigurationElement > startB = cfg->addConfigurationElement("start");
    startB->setAttributeValue( "uid" , "myTestService2" ) ;

    // Update method from object's services
    ::boost::shared_ptr< ::fwRuntime::EConfigurationElement > updateA = cfg->addConfigurationElement("update");
    updateA->setAttributeValue( "uid" , "myTestService1" ) ;

    return cfg ;
}

//------------------------------------------------------------------------------

} //namespace ut
} //namespace fwServices
