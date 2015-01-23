/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/assign/std/vector.hpp>

#include <fwDataCamp/Version.hpp>
#include <fwMedData/ModelSeries.hpp>
#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/SeriesDB.hpp>

#include <fwAtomConversion/convert.hpp>

#include <fwAtoms/Sequence.hpp>
#include <fwDataCamp/getObject.hpp>

#include "fwMedDataCamp/SeriesDBTest.hpp"
#include "DataCampHelper.hpp"

using namespace ::boost::assign;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwMedDataCamp::ut::SeriesDBTest );

namespace fwMedDataCamp
{
namespace ut
{

void SeriesDBTest::setUp()
{
    // Set up context before running a test.
    //Force link with fwDataCamp
    const int version = ::fwDataCamp::Version::s_CURRENT_VERSION;
}

void SeriesDBTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void SeriesDBTest::propertiesTest()
{
    const ::DataCampHelper::PropertiesNameType dataProperties = list_of("fields")
                                                                       ("values");

    ::fwMedData::SeriesDB::sptr obj = ::fwMedData::SeriesDB::New();
    ::fwMedData::SeriesDB::ContainerType vectSeries;
    vectSeries.push_back(::fwMedData::ImageSeries::New());
    vectSeries.push_back(::fwMedData::ImageSeries::New());
    vectSeries.push_back(::fwMedData::ModelSeries::New());
    obj->setContainer(vectSeries);

    ::DataCampHelper::visitProperties(obj->getClassname(), dataProperties);
    ::DataCampHelper::compareObjectPropertyValue(obj, "@values.0", vectSeries[0]);
    ::DataCampHelper::compareObjectPropertyValue(obj, "@values.1", vectSeries[1]);
    ::DataCampHelper::compareObjectPropertyValue(obj, "@values.2", vectSeries[2]);

    ::fwAtoms::Object::sptr metaObject = ::fwAtomConversion::convert(obj);
    ::fwAtoms::Object::AttributesType attrs = metaObject->getAttributes();

    CPPUNIT_ASSERT_MESSAGE("Attributes values not found in SeriesDB atom",
                           attrs.find("values") != attrs.end());

    ::fwAtoms::Base::sptr baseAtom = metaObject->getAttribute("values");
    CPPUNIT_ASSERT_MESSAGE("Bad Atom SeriesDB conversion", baseAtom->isSequence());

    ::fwAtoms::Sequence::sptr seqAtom = ::fwAtoms::Sequence::dynamicCast(baseAtom);
    CPPUNIT_ASSERT_MESSAGE("Sequence dynamicCast failed", seqAtom);
    CPPUNIT_ASSERT_EQUAL( vectSeries.size(), seqAtom->getValue().size());
}

//------------------------------------------------------------------------------

} //namespace ut
} //namespace fwMedDataCamp
