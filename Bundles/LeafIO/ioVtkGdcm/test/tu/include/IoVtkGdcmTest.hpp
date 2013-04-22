/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATATOOLS_TEST_TU_IOVTKGDCMREADER_HPP_
#define _FWDATATOOLS_TEST_TU_IOVTKGDCMREADER_HPP_

#include <cppunit/extensions/HelperMacros.h>

namespace ioVtkGdcm
{
namespace ut
{

/**
 * @brief Test the reading of Dicom file with vtkGdcm.
 */
class IoVtkGdcmTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( IoVtkGdcmTest );
    CPPUNIT_TEST( testReader );
    CPPUNIT_TEST( testWriter );
    CPPUNIT_TEST( imageSeriesWriterTest );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    void testReader();
    void testWriter();
    void imageSeriesWriterTest();

};

} //namespace ut
} //namespace ioVtkGdcm

#endif //_FWDATATOOLS_TEST_TU_IOVTKGDCMREADER_HPP_
