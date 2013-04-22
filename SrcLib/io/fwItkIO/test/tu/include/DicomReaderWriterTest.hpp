/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWITKIO_UT_DICOMREADERWRITERTEST_HPP_
#define _FWITKIO_UT_DICOMREADERWRITERTEST_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <fwServices/macros.hpp>
#include <fwRuntime/EConfigurationElement.hpp>

#include <fwData/Image.hpp>

namespace fwItkIO
{
namespace ut
{

class DicomReaderWriterTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( DicomReaderWriterTest );
    CPPUNIT_TEST( testWriteImage );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();
    // fonctions de tests
    void testWriteImage();

private :

    void writeImage( ::fwData::Image::sptr image );

    std::stringstream m_gdcmTraceStream;
};

} //namespace ut
} //namespace fwItkIO

#endif // _FWITKIO_UT_DICOMREADERWRITERTEST_HPP_
