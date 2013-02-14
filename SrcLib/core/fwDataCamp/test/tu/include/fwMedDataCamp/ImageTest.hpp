/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWMEDDATACAMP_UT_IMAGETEST_HPP__
#define __FWMEDDATACAMP_UT_IMAGETEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwMedDataCamp
{
namespace ut
{

class ImageTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( ImageTest );
    CPPUNIT_TEST( propertiesTest );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    void propertiesTest();

};

} //namespace ut
} //namespace fwMedDataCamp

#endif // __FWMEDDATACAMP_UT_IMAGETEST_HPP__
