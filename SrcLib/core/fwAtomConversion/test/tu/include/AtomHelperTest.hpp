/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWATOMCONVERSION_UT_METAHELPERTEST_HPP__
#define __FWATOMCONVERSION_UT_METAHELPERTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwAtomConversion
{
namespace ut
{

class AtomHelperTest :  public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( AtomHelperTest );
    CPPUNIT_TEST( dataToMetaTest );
    CPPUNIT_TEST( materialConversionTest );
    CPPUNIT_TEST( patientConversionTest );
    CPPUNIT_TEST( graphConversionTest );
    CPPUNIT_TEST_SUITE_END();

public:

    // interface
    void setUp();
    void tearDown();

    void dataToMetaTest();
    void materialConversionTest();
    void patientConversionTest();

    //// Test fwData::Graph conversion, UUID management, and object field conversion
    void graphConversionTest();

};


}  // namespace ut
}  // namespace fwAtomConversion



#endif // __FWATOMCONVERSION_UT_METAHELPERTEST_HPP__
