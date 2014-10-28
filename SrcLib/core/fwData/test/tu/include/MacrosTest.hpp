/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_UT_MACROSTEST_HPP__
#define __FWDATA_UT_MACROSTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwData
{
namespace ut
{

class MacrosTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( MacrosTest );
    CPPUNIT_TEST( getterSetterTest );
    CPPUNIT_TEST_SUITE_END();

public:

    void setUp();
    void tearDown();

    void getterSetterTest();
};
} //namespace ut
} //namespace fwData

#endif //__FWDATA_UT_MACROSTEST_HPP__
