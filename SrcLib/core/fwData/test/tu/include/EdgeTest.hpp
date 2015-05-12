/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_UT_EDGETEST_HPP__
#define __FWDATA_UT_EDGETEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwData
{
namespace ut
{

class EdgeTest : public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( EdgeTest );
CPPUNIT_TEST( methode1 );
CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    void methode1();
};

} //namespace ut
} //namespace fwData
#endif //__FWDATA_UT_EDGETEST_HPP__
