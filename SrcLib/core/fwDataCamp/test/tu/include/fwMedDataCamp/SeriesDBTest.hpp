/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATACAMP_UT_FWMEDDATACAMP_SERIESDBTEST_HPP__
#define __FWDATACAMP_UT_FWMEDDATACAMP_SERIESDBTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwMedDataCamp
{
namespace ut
{

class SeriesDBTest : public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( SeriesDBTest );
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

#endif // __FWDATACAMP_UT_FWMEDDATACAMP_SERIESDBTEST_HPP__
