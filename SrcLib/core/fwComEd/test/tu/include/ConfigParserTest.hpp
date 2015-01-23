/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWCOMED_TU_CONFIGPARSERTEST_HPP_
#define _FWCOMED_TU_CONFIGPARSERTEST_HPP_

#include <cppunit/extensions/HelperMacros.h>

#include <fwCore/base.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/ObjectMsg.hpp>

#include <fwComEd/CompositeMsg.hpp>
#include <fwComEd/ImageMsg.hpp>

#include <fwRuntime/EConfigurationElement.hpp>


namespace fwComEd
{
namespace ut
{

/**
 * @brief   Test ProcessObject building from a ConfigurationElement and composite message.
 * @see     ICompositeParser
 * @see     CompositeEditor
 * @see     CompositeMsg
 */
class ConfigParserTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( ConfigParserTest );
    CPPUNIT_TEST( testObjectCreationWithConfig );
    CPPUNIT_TEST( testBuildComposite );
    CPPUNIT_TEST_SUITE_END();

public:

    void setUp();
    void tearDown();

    /// test object with services creation from a configuration
    void testObjectCreationWithConfig();

    /// Test the composite building
    void testBuildComposite();

private:
    /// Create a configurationElement to build an object
    ::fwRuntime::ConfigurationElement::sptr buildObjectConfig();

    /// Create a configurationElement to build the composite
    ::fwRuntime::ConfigurationElement::sptr buildCompositeConfig();
};

} //namespace ut
} //namespace fwComEd

#endif // _FWCOMED_TU_CONFIGPARSERTEST_HPP_
