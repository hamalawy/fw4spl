/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWXML_UT_OBJECTTEST_HPP_
#define _FWXML_UT_OBJECTTEST_HPP_

#include <cppunit/extensions/HelperMacros.h>

#include <fwData/Object.hpp>

namespace fwXML
{
namespace ut
{

class ObjectTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( ObjectTest );
    CPPUNIT_TEST( colorTest );
    CPPUNIT_TEST( materialTest );
    CPPUNIT_TEST( arrayTest );
    CPPUNIT_TEST( meshTest );
    CPPUNIT_TEST( imageTest );
    CPPUNIT_TEST( reconstructionTest );
    CPPUNIT_TEST( acquisitionTest );
    CPPUNIT_TEST( transferFunctionTest );
    CPPUNIT_TEST( studyTest );
    CPPUNIT_TEST( patientTest );
    CPPUNIT_TEST( patientDBTest );
    CPPUNIT_TEST( structureTraitsTest );
    CPPUNIT_TEST( ROITraitsTest );
    CPPUNIT_TEST( reconstructionTraitsTest );
    CPPUNIT_TEST( structureTraitsDictionaryTest );
    CPPUNIT_TEST( fieldSerializationTest );
    CPPUNIT_TEST( imageFieldSerializationTest );
    CPPUNIT_TEST( resectionDBSerializationTest );
    CPPUNIT_TEST( resectionDBWithSafeResectionSerializationTest );
    CPPUNIT_TEST( dictionarySerializationTest );
    CPPUNIT_TEST( processObjectSerializationTest );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    static ::fwData::Object::sptr serialize(std::string fileName, ::fwData::Object::sptr obj1);

    void materialTest();
    void colorTest();
    void arrayTest();
    void meshTest();
    void imageTest();
    void reconstructionTest();
    void acquisitionTest();
    void triangularMeshTest();
    void transferFunctionTest();
    void studyTest();
    void patientTest();
    void patientDBTest();
    void structureTraitsTest();
    void ROITraitsTest();
    void reconstructionTraitsTest();
    void structureTraitsDictionaryTest();
    void fieldSerializationTest();
    void imageFieldSerializationTest();
    void resectionDBSerializationTest();
    void resectionDBWithSafeResectionSerializationTest();
    void dictionarySerializationTest();
    void processObjectSerializationTest();
};

} //namespace ut
} //namespace fwXML

#endif // _FWXML_UT_OBJECTTEST_HPP_
