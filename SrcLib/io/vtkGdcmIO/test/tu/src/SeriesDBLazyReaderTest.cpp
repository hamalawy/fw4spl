/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#if (SPYLOG_LEVEL >= 4 )
#include <fwCore/HiResTimer.hpp>
#endif

#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/SeriesDB.hpp>

#include <fwComEd/helper/Image.hpp>

#include <fwTest/Data.hpp>
#include <fwTest/DicomReaderTest.hpp>

#include <vtkGdcmIO/SeriesDBLazyReader.hpp>

#include "SeriesDBLazyReaderTest.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( ::vtkGdcmIO::ut::SeriesDBLazyReaderTest );

namespace vtkGdcmIO
{
namespace ut
{

//------------------------------------------------------------------------------

void SeriesDBLazyReaderTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void SeriesDBLazyReaderTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void SeriesDBLazyReaderTest::readSeriesDBTest()
{
    ::fwMedData::SeriesDB::sptr seriesDB = ::fwMedData::SeriesDB::New();

    const ::boost::filesystem::path path = ::fwTest::Data::dir() / "fw4spl/Patient/Dicom/ACHGenou";

    ::vtkGdcmIO::SeriesDBLazyReader::sptr reader = ::vtkGdcmIO::SeriesDBLazyReader::New();
    reader->setObject(seriesDB);

    reader->setFolder(path);

    CPPUNIT_ASSERT_NO_THROW(reader->read());

    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), seriesDB->size());
    ::fwMedData::ImageSeries::sptr series = ::fwMedData::ImageSeries::dynamicCast(seriesDB->front());

    CPPUNIT_ASSERT( ::fwTest::DicomReaderTest::checkSeriesACHGenou( series ) );

#if (SPYLOG_LEVEL >= 4 ) // Log level info
    ::fwCore::HiResTimer timer;
    timer.start();
#endif

    ::fwComEd::helper::Image locker ( series->getImage() );

    #if (SPYLOG_LEVEL >= 4 )
    timer.stop();
    OSLM_INFO( "Time in to read data : " << timer.getElapsedTimeInMilliSec() );
#endif

}

//------------------------------------------------------------------------------

} // namespace ut

} // namespace vtkGdcmIO

