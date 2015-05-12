/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSERVICES_UT_SERVICETEST_HPP__
#define __FWSERVICES_UT_SERVICETEST_HPP__

#include <cppunit/extensions/HelperMacros.h>
#include <fwRuntime/EConfigurationElement.hpp>

namespace fwServices
{
namespace ut
{

/**
 * @brief Test many methods to create service (with uuid, with configuration,...).
 * And test service methods (start, stop and update).
 */
class ServiceTest : public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( ServiceTest );
CPPUNIT_TEST( testServiceCreation );
CPPUNIT_TEST( testServiceCreationWithTemplateMethods );
CPPUNIT_TEST( testServiceCreationWithUUID );
CPPUNIT_TEST( testStartStopUpdate );
CPPUNIT_TEST( testCommunication );
CPPUNIT_TEST_SUITE_END();


public:
    // interface
    void setUp();
    void tearDown();


    // fonctions de tests

    /// Test add and remove service
    void testServiceCreation();

    /// Test add and remove service using template methods
    void testServiceCreationWithTemplateMethods();

    /// Test add and remove service with UUID
    void testServiceCreationWithUUID();

    /// test start/stop/update service
    void testStartStopUpdate();

    /// test sending/receiving message
    void testCommunication();

private:
    // create a configurationElement to build a service
    ::fwRuntime::EConfigurationElement::sptr buildServiceConfig();

    // create a configurationElement to build an object with services
    ::fwRuntime::ConfigurationElement::sptr buildObjectConfig();
};

} //namespace ut
} //namespace fwServices

#endif //__FWSERVICES_UT_SERVICETEST_HPP__

