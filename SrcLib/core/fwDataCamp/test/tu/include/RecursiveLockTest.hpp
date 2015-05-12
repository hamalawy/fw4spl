/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATACAMP_UT_RECURSIVELOCKTEST_HPP__
#define __FWDATACAMP_UT_RECURSIVELOCKTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwDataCamp
{
namespace ut
{

class RecursiveLockTest :  public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( RecursiveLockTest );
CPPUNIT_TEST( lockTest );
CPPUNIT_TEST_SUITE_END();

public:

    // interface
    void setUp();
    void tearDown();

    void lockTest();

};

}  // namespace ut
}  // namespace fwDataCamp

#endif // __FWDATACAMP_UT_RECURSIVELOCKTEST_HPP__

