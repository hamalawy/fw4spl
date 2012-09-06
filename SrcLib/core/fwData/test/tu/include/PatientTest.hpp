/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_UT_PATIENTTEST_HPP__
#define __FWDATA_UT_PATIENTTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwData
{
namespace ut
{

class PatientTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( PatientTest );
    CPPUNIT_TEST( methodes1 );
    CPPUNIT_TEST( methodes2 );
    CPPUNIT_TEST( addStudy );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    void methodes1();
    void methodes2();
    void addStudy() ;
};
} //namespace ut
} //namespace fwData
#endif //__FWDATA_UT_PATIENTTEST_HPP__
