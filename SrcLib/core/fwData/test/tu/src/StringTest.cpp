/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <fwData/String.hpp>
#include "StringTest.hpp"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwData::ut::StringTest );

namespace fwData
{
namespace ut
{

void StringTest::setUp()
{
    // Set up context before running a test.
}

void StringTest::tearDown()
{
    // Clean up after the test run.
}

void StringTest::methode1()
{
    const std::string VALUES[]  = { "", "chaine_caractere" };

    BOOST_FOREACH ( std::string VALUE, VALUES )
    {
        ::fwData::String::sptr s0 = ::fwData::String::New();
        s0->value() = VALUE;
        ::fwData::String::sptr s1 = ::fwData::String::New( VALUE );

        CPPUNIT_ASSERT_EQUAL( VALUE , s0->value() );
        CPPUNIT_ASSERT_EQUAL( VALUE , s1->value() );
        CPPUNIT_ASSERT_EQUAL( VALUE , ::fwData::String::New( VALUE )->value() );
    }
}

} //namespace ut
} //namespace fwData
