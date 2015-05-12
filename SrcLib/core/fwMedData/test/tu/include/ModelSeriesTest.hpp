/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWMEDDATA_UT_MODELSERIESTEST_HPP__
#define __FWMEDDATA_UT_MODELSERIESTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwMedData
{
namespace ut
{

class ModelSeriesTest : public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( ModelSeriesTest );
CPPUNIT_TEST(modelTest);
CPPUNIT_TEST(deepCopyTest);
CPPUNIT_TEST(shallowCopyTest);
CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    void modelTest();
    void deepCopyTest();
    void shallowCopyTest();

private:
    ModelSeries::sptr m_series;
};

} //namespace ut
} //namespace fwMedData

#endif //__FWMEDDATA_UT_MODELSERIESTEST_HPP__

