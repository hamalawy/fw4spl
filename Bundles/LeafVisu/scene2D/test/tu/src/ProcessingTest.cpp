
#include <fwRuntime/EConfigurationElement.hpp>

#include <fwTest/Exception.hpp>

#include <fwData/Image.hpp>
#include <fwData/Histogram.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>

#include <fwComEd/helper/Array.hpp>

#include "ProcessingTest.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::scene2D::ut::ProcessingTest );

namespace scene2D
{
namespace ut
{

static ::fwTest::Exception fwTestException(""); // force link with fwTest

//------------------------------------------------------------------------------

void ProcessingTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void ProcessingTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void ProcessingTest::histogramTest()
{
    ::fwServices::registry::ActiveWorkers::sptr activeWorkers = ::fwServices::registry::ActiveWorkers::getDefault();
    activeWorkers->initRegistry();

    typedef signed short ImageType;
    const int sizeX = 50;
    const int sizeY = 50;
    const int sizeZ = 50;
    const int imageSize = sizeX * sizeY * sizeZ;

    std::string implementation = "::scene2D::processing::SComputeHistogram";

    // Configure data hirearchy
    ::fwData::Image::sptr image = ::fwData::Image::New();
    ::fwData::Histogram::sptr histogram = ::fwData::Histogram::New();

    // Create image.
    image->allocate(sizeX, sizeY,  sizeZ, ::fwTools::Type("int16"));
    ::fwData::Array::sptr array = image->getDataArray();
    ::fwComEd::helper::Array arrayHelper(array);

    ImageType* itr = arrayHelper.begin< ImageType >();
    ImageType* itrEnd = arrayHelper.end< ImageType >();

    int count=0;
    for(; itr!= itrEnd; ++itr)
    {
        if(count < imageSize/4)
        {
            *itr =10;
        }
        else if( count < imageSize/2)
        {
            *itr =20;
        }
        else if( count < 3*imageSize/4)
        {
            *itr =30;
        }
        else if( count < imageSize)
        {
            *itr =40;
        }
        ++count;
    }

    // Create service.
    ::fwServices::IService::sptr srv;
    srv = ::fwServices::registry::ServiceFactory::getDefault()->create( implementation );
    CPPUNIT_ASSERT(srv);

    // Register service.
    ::fwServices::OSR::registerService( image , srv );

    ::fwRuntime::EConfigurationElement::NewSptr srvCfg("service");

    ::fwRuntime::EConfigurationElement::NewSptr histogramIdCfg("histogramId");
    histogramIdCfg->setValue(histogram->getID());
    srvCfg->addConfigurationElement(histogramIdCfg);

    ::fwRuntime::EConfigurationElement::NewSptr binsWidthCfg("binsWidth");
    binsWidthCfg->setValue("1.0");
    srvCfg->addConfigurationElement(binsWidthCfg);

    srv->setConfiguration(srvCfg);
    srv->configure();
    srv->start().wait();
    srv->stop().wait();
    ::fwServices::OSR::unregisterService(srv);

    ::fwData::Histogram::fwHistogramValues values = histogram->getValues();
    CPPUNIT_ASSERT_EQUAL((size_t) 40-10+1, values.size());

    CPPUNIT_ASSERT_EQUAL((float) 10, histogram->getMinValue());

    CPPUNIT_ASSERT_EQUAL((float) 40, histogram->getMaxValue());

    CPPUNIT_ASSERT_EQUAL((long) imageSize/4, histogram->getNbPixels(10,11));
    CPPUNIT_ASSERT_EQUAL((long) imageSize/4, histogram->getNbPixels(20,21));
    CPPUNIT_ASSERT_EQUAL((long) imageSize/4, histogram->getNbPixels(30,31));
    CPPUNIT_ASSERT_EQUAL((long) imageSize/4, histogram->getNbPixels(40,41));
}

//------------------------------------------------------------------------------

} // namespace ut
} // namespace scene2D
