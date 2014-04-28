/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem/operations.hpp>

#include <fwRuntime/EConfigurationElement.hpp>
#include <fwRuntime/profile/Profile.hpp>

#include <fwTools/dateAndTime.hpp>
#include <fwTools/System.hpp>

#include <fwDataCamp/visitor/CompareObjects.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/AppConfigManager.hpp>
#include <fwServices/registry/AppConfig.hpp>

#include <fwMedData/SeriesDB.hpp>
#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/Patient.hpp>
#include <fwMedData/Study.hpp>

#include <fwTest/generator/SeriesDB.hpp>
#include <fwTest/generator/Image.hpp>
#include <fwTest/helper/compare.hpp>
#include <fwTest/Data.hpp>

#include "IoVtkGdcmTest.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::ioVtkGdcm::ut::IoVtkGdcmTest );


namespace ioVtkGdcm
{
namespace ut
{

//------------------------------------------------------------------------------

void IoVtkGdcmTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void IoVtkGdcmTest::tearDown()
{
    // Clean up after the test run.
}

//-----------------------------------------------------------------------------

void IoVtkGdcmTest::readerDicomTest( std::string srvImpl )
{
    ::boost::filesystem::path dicomDataPath(::fwTest::Data::dir() / "fw4spl/Patient/Dicom/image_281433");

    ::fwMedData::SeriesDB::sptr seriesDB = ::fwMedData::SeriesDB::New();
    ::fwRuntime::EConfigurationElement::sptr readerCfg = ::fwRuntime::EConfigurationElement::New("service");
    ::fwRuntime::EConfigurationElement::sptr folderCfg = ::fwRuntime::EConfigurationElement::New("folder");
    folderCfg->setValue(dicomDataPath.string());
    readerCfg->addConfigurationElement(folderCfg);

    ::fwServices::IService::sptr srv =
            ::fwServices::registry::ServiceFactory::getDefault()->create( "::io::IReader", srvImpl );
    CPPUNIT_ASSERT(srv);

    ::fwServices::OSR::registerService( seriesDB, srv );

    srv->setConfiguration( readerCfg );
    srv->configure();
    srv->start();
    srv->update();
    srv->stop();
    ::fwServices::OSR::unregisterService( srv );

    // Patient expected
    const std::string nameExpected("anonymous^anonymous");
    const std::string sexExpected("F");

    //Info image expected.
    const size_t imgDimensionExpected   = 3;
    double imgSpacingX = 0.667969;
    double imgSpacingY = 0.667969;
    double imgSpacingZ = 1.5;

    ::fwData::Image::OriginType imgOriginExpected (3,0);

    size_t imgSizeX_Expected =  512;
    size_t imgSizeY_Expected =  512;
    size_t imgSizeZ_Expected =  166;
    // int imgSize = imgSizeX_Expected*imgSizeY_Expected*imgSizeZ_Expected;

    const double imgWindowCenter = 50;
    const double imgWindowWidth = 500;
    ::fwTools::Type imgPixelType = ::fwTools::Type::create<signed int>();

    CPPUNIT_ASSERT_EQUAL(size_t(1), seriesDB->getContainer().size());

    ::fwMedData::Series::sptr series = seriesDB->getContainer()[0];

    ::fwMedData::Patient::sptr patient = series->getPatient();
    ::fwMedData::Study::sptr study = series->getStudy();

    CPPUNIT_ASSERT_EQUAL(nameExpected, patient->getName());
    CPPUNIT_ASSERT_EQUAL(sexExpected, patient->getSex());

    ::fwMedData::ImageSeries::sptr imgSeries = ::fwMedData::ImageSeries::dynamicCast(series);
    CPPUNIT_ASSERT(imgSeries);
    ::fwData::Image::csptr fisrtImage = imgSeries->getImage();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on image dimension.",
            fisrtImage->getNumberOfDimensions(),
            imgDimensionExpected);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on origin on X ",
            static_cast< ::fwData::Image::OriginType::value_type > (fisrtImage->getOrigin()[0]),
            imgOriginExpected[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on origin on Y ",
            static_cast< ::fwData::Image::OriginType::value_type > (fisrtImage->getOrigin()[1]),
            imgOriginExpected[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on origin on Z ",
            static_cast< ::fwData::Image::OriginType::value_type > (fisrtImage->getOrigin()[2]),
            imgOriginExpected[2]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on window center ",
            fisrtImage->getWindowCenter(),
            imgWindowCenter);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on window width ",
            fisrtImage->getWindowWidth(),
            imgWindowWidth);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on size x ",
            static_cast< ::fwData::Image::SizeType::value_type > (fisrtImage->getSize()[0]),
            static_cast< ::fwData::Image::SizeType::value_type > (imgSizeX_Expected));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on size y ",
            static_cast< ::fwData::Image::SizeType::value_type > (fisrtImage->getSize()[1]),
            static_cast< ::fwData::Image::SizeType::value_type > (imgSizeY_Expected));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on size z ",
            static_cast< ::fwData::Image::SizeType::value_type > (fisrtImage->getSize()[2]),
            static_cast< ::fwData::Image::SizeType::value_type > (imgSizeZ_Expected));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on spacing x ",
            static_cast< ::fwData::Image::SpacingType::value_type > (fisrtImage->getSpacing()[0]),
            imgSpacingX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on spacing y ",
            static_cast< ::fwData::Image::SpacingType::value_type > (fisrtImage->getSpacing()[1]),
            imgSpacingY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed on spacing z ",
            static_cast< ::fwData::Image::SpacingType::value_type > (fisrtImage->getSpacing()[2]),
            imgSpacingZ);
}

//------------------------------------------------------------------------------

void IoVtkGdcmTest::seriesDBLazyReaderTest()
{
    this->readerDicomTest("::ioVtkGdcm::SSeriesDBLazyReader");
}

//------------------------------------------------------------------------------

void IoVtkGdcmTest::seriesDBReaderTest()
{
    this->readerDicomTest("::ioVtkGdcm::SSeriesDBReader");
}

//------------------------------------------------------------------------------

void IoVtkGdcmTest::imageSeriesWriterTest()
{
    const ::boost::filesystem::path PATH = ::fwTools::System::getTemporaryFolder() / "DicomWriterTest";

    ::boost::filesystem::create_directories( PATH );

    ::fwRuntime::EConfigurationElement::sptr srvConfig = ::fwRuntime::EConfigurationElement::New("service");
    ::fwRuntime::EConfigurationElement::sptr folderCfg = ::fwRuntime::EConfigurationElement::New("folder");
    folderCfg->setValue(PATH.string());
    srvConfig->addConfigurationElement(folderCfg);

    ::fwTest::generator::Image::initRand();
    ::fwMedData::ImageSeries::sptr imgSeries;
    imgSeries = ::fwTest::generator::SeriesDB::createImageSeries();

    ::fwServices::IService::sptr writerSrv = ::fwServices::registry::ServiceFactory::getDefault()->create( "::io::IWriter", "::ioVtkGdcm::SImageSeriesWriter" );
    CPPUNIT_ASSERT(writerSrv);

    ::fwServices::OSR::registerService( imgSeries, writerSrv );

    writerSrv->setConfiguration( srvConfig );
    writerSrv->configure();
    writerSrv->start();
    writerSrv->update();
    writerSrv->stop();
    ::fwServices::OSR::unregisterService( writerSrv );


    // Load Dicom from disk
    ::fwMedData::SeriesDB::sptr seriesDB = ::fwMedData::SeriesDB::New();

    ::fwServices::IService::sptr readerSrv = ::fwServices::registry::ServiceFactory::getDefault()->create( "::io::IReader", "::ioVtkGdcm::SSeriesDBReader" );
    CPPUNIT_ASSERT(readerSrv);

    ::fwServices::OSR::registerService( seriesDB , readerSrv );

    readerSrv->setConfiguration( srvConfig ); // use same config as writer
    readerSrv->configure();
    readerSrv->start();
    readerSrv->update();
    readerSrv->stop();
    ::fwServices::OSR::unregisterService( readerSrv );

    // Clean the written data
    ::boost::filesystem::remove_all( PATH.string() );

    // check series
    CPPUNIT_ASSERT_EQUAL(size_t(1), seriesDB->getContainer().size());

    CPPUNIT_ASSERT(::fwTest::helper::compare(imgSeries, seriesDB->getContainer().front()));
}

//------------------------------------------------------------------------------

} //namespace ut
} //namespace ioVtkGdcm
