/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fstream>

#include <fwRuntime/EConfigurationElement.hpp>
#include <fwRuntime/profile/Profile.hpp>

#include <fwTools/System.hpp>
#include <fwTools/Type.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/AppConfigManager.hpp>
#include <fwServices/registry/AppConfig.hpp>

#include <fwData/Image.hpp>


#include <fwDataTools/Image.hpp>

#include <fwDataCamp/visitor/CompareObjects.hpp>

#include <fwComEd/helper/Image.hpp>

#include <fwMedData/ImageSeries.hpp>

#include <fwTest/generator/Image.hpp>
#include <fwTest/helper/compare.hpp>
#include <fwTest/Data.hpp>

#include "ImageReaderWriterTest.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::ioVTK::ut::ImageReaderWriterTest );

static const double epsilon = 0.00001;

namespace ioVTK
{
namespace ut
{

//------------------------------------------------------------------------------

void ImageReaderWriterTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void ImageReaderWriterTest::tearDown()
{
    // Clean up after the test run.
}


::fwRuntime::EConfigurationElement::sptr getIOConfiguration(const ::boost::filesystem::path &file)
{
    ::fwRuntime::EConfigurationElement::sptr readerSrvCfg = ::fwRuntime::EConfigurationElement::New("service");
    ::fwRuntime::EConfigurationElement::sptr readerCfg = ::fwRuntime::EConfigurationElement::New("file");
    readerCfg->setValue(file.string());
    readerSrvCfg->addConfigurationElement(readerCfg);

    return readerSrvCfg;
}

//------------------------------------------------------------------------------

void ImageReaderWriterTest::testVtkImageReader()
{

    const ::boost::filesystem::path file = ::fwTest::Data::dir() / "fw4spl/image/vtk/img.vtk";

    ::fwData::Image::sptr image = ::fwData::Image::New();

    // Data expected
    const size_t dim = 3;
    ::fwData::Image::SpacingType spacingExpected(dim);
    spacingExpected[0] = 1.732;
    spacingExpected[1] = 1.732;
    spacingExpected[2] = 3.2;

    ::fwData::Image::OriginType originExpected(dim);
    originExpected[0] = 34.64;
    originExpected[1] = 86.6;
    originExpected[2] = 56;

    ::fwData::Image::SizeType sizeExpected(dim);
    sizeExpected[0] = 230;
    sizeExpected[1] = 170;
    sizeExpected[2] = 58;

    this->runImageSrv("::io::IReader","::ioVTK::ImageReaderService",getIOConfiguration(file), image);

    // Data read.
    ::fwData::Image::SpacingType spacingRead = image->getSpacing();
    ::fwData::Image::SpacingType originRead = image->getOrigin();
    ::fwData::Image::SizeType sizeRead = image->getSize();

    CPPUNIT_ASSERT_EQUAL(spacingExpected.size(), spacingRead.size() );
    CPPUNIT_ASSERT_EQUAL(originExpected.size(), originRead.size() );
    CPPUNIT_ASSERT_EQUAL(sizeExpected.size(), sizeRead.size() );

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on x", spacingExpected[0], spacingRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on y", spacingExpected[1], spacingRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on z", spacingExpected[2], spacingRead[2], epsilon);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on x", originExpected[0], originRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on y", originExpected[1], originRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on z", originExpected[2], originRead[2], epsilon);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on x", sizeExpected[0], sizeRead[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on y", sizeExpected[1], sizeRead[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on z", sizeExpected[2], sizeRead[2]);

}

//------------------------------------------------------------------------------

void ImageReaderWriterTest::testVtiImageReader()
{
    const ::boost::filesystem::path file = ::fwTest::Data::dir() /"fw4spl/image/vti/BostonTeapot.vti";

    ::fwData::Image::sptr image = ::fwData::Image::New();
    this->runImageSrv("::io::IReader","::ioVTK::ImageReaderService",getIOConfiguration(file), image);

    // Data expected
    const size_t dim = 3;
    ::fwData::Image::SpacingType spacingExpected(dim);
    spacingExpected[0] = 1.0;
    spacingExpected[1] = 1.0;
    spacingExpected[2] = 1.0;

    ::fwData::Image::OriginType originExpected(dim);
    originExpected[0] = 1.1;
    originExpected[1] = 2.2;
    originExpected[2] = 3.3;

    ::fwData::Image::SizeType sizeExpected(dim);
    sizeExpected[0] = 256;
    sizeExpected[1] = 256;
    sizeExpected[2] = 178;

    ::fwTools::Type expectedType("int8"); // MHD File image type : MET_CHAR

    // Data read.
    ::fwData::Image::SpacingType spacingRead = image->getSpacing();
    ::fwData::Image::SpacingType originRead = image->getOrigin();
    ::fwData::Image::SizeType sizeRead = image->getSize();


    CPPUNIT_ASSERT_EQUAL(spacingExpected.size(), spacingRead.size() );
    CPPUNIT_ASSERT_EQUAL(originExpected.size(), originRead.size() );
    CPPUNIT_ASSERT_EQUAL(sizeExpected.size(), sizeRead.size() );

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on x", spacingExpected[0], spacingRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on y", spacingExpected[1], spacingRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on z", spacingExpected[2], spacingRead[2], epsilon);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on x", originExpected[0], originRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on y", originExpected[1], originRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on z", originExpected[2], originRead[2], epsilon);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on x", sizeExpected[0], sizeRead[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on y", sizeExpected[1], sizeRead[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on z", sizeExpected[2], sizeRead[2]);

    CPPUNIT_ASSERT_EQUAL( expectedType, image->getType());
}

//------------------------------------------------------------------------------
void ImageReaderWriterTest::testMhdImageReader()
{

    const ::boost::filesystem::path file = ::fwTest::Data::dir() / "fw4spl/image/mhd/BostonTeapot.mhd";

    ::fwData::Image::sptr image = ::fwData::Image::New();
    this->runImageSrv("::io::IReader","::ioVTK::ImageReaderService",getIOConfiguration(file), image);

    // Data expected
    const size_t dim = 3;
    ::fwData::Image::SpacingType spacingExpected(dim);
    spacingExpected[0] = 1.0;
    spacingExpected[1] = 1.0;
    spacingExpected[2] = 1.0;

    ::fwData::Image::OriginType originExpected(dim);
    originExpected[0] = 1.1;
    originExpected[1] = 2.2;
    originExpected[2] = 3.3;

    ::fwData::Image::SizeType sizeExpected(dim);
    sizeExpected[0] = 256;
    sizeExpected[1] = 256;
    sizeExpected[2] = 178;

    ::fwTools::Type expectedType("int8"); // MHD File image type : MET_CHAR

    // Data read.
    ::fwData::Image::SpacingType spacingRead = image->getSpacing();
    ::fwData::Image::SpacingType originRead = image->getOrigin();
    ::fwData::Image::SizeType sizeRead = image->getSize();



    CPPUNIT_ASSERT_EQUAL(spacingExpected.size(), spacingRead.size() );
    CPPUNIT_ASSERT_EQUAL(originExpected.size(), originRead.size() );
    CPPUNIT_ASSERT_EQUAL(sizeExpected.size(), sizeRead.size() );

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on x", spacingExpected[0], spacingRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on y", spacingExpected[1], spacingRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on z", spacingExpected[2], spacingRead[2], epsilon);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on x", originExpected[0], originRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on y", originExpected[1], originRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on z", originExpected[2], originRead[2], epsilon);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on x", sizeExpected[0], sizeRead[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on y", sizeExpected[1], sizeRead[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on z", sizeExpected[2], sizeRead[2]);

    CPPUNIT_ASSERT_EQUAL( expectedType, image->getType());

}
//------------------------------------------------------------------------------

void ImageReaderWriterTest::testImageReaderExtension()
{
    const ::boost::filesystem::path file = ::fwTools::System::getTemporaryFolder() / "img.xxx";

    std::ofstream ofile;
    ofile.open(file.string().c_str());
    ofile.close();

    ::fwData::Image::sptr image = ::fwData::Image::New();

    {
        const std::string srvtype("::io::IReader");
        const std::string srvname("::ioVTK::ImageReaderService");


        ::fwServices::IService::sptr srv;
        srv = ::fwServices::registry::ServiceFactory::getDefault()->create( srvtype, srvname );

        CPPUNIT_ASSERT_MESSAGE(std::string("Failed to create service ") + srvname, srv);

        ::fwServices::OSR::registerService( image , srv );

        CPPUNIT_ASSERT_NO_THROW( srv->setConfiguration(getIOConfiguration(file)) );
        CPPUNIT_ASSERT_NO_THROW( srv->configure() );
        CPPUNIT_ASSERT_NO_THROW( srv->start() );
        CPPUNIT_ASSERT_THROW( srv->update(), ::fwTools::Failed);
        CPPUNIT_ASSERT_NO_THROW( srv->stop() );
        ::fwServices::OSR::unregisterService( srv );
    }
    ::boost::filesystem::remove(file);

}

//------------------------------------------------------------------------------
void ImageReaderWriterTest::testVtkImageWriter()
{
    // Data to write
    const size_t dim = 3;
    ::fwTools::Type type("uint8");
    ::fwData::Image::SizeType sizeExpected(dim);
    sizeExpected[0] = 10;
    sizeExpected[1] = 20;
    sizeExpected[2] = 30;
    ::fwData::Image::SpacingType spacingExpected(dim);
    spacingExpected[0] = 0.24;
    spacingExpected[1] = 1.07;
    spacingExpected[2] = 2.21;
    ::fwData::Image::OriginType originExpected(dim);
    originExpected[0] = -05.6;
    originExpected[1] = 15.16;
    originExpected[2] = 11.11;

    ::fwData::Image::sptr image = ::fwData::Image::New();
    ::fwTest::generator::Image::generateImage(image, sizeExpected, spacingExpected, originExpected, type);

    // Write to vtk image.
    const ::boost::filesystem::path file = ::fwTools::System::getTemporaryFolder() / "temporaryFile.vtk";

    this->runImageSrv("::io::IWriter","::ioVTK::ImageWriterService",getIOConfiguration(file), image);


    // Read image from disk
    ::fwData::Image::sptr imageFromDisk = ::fwData::Image::New();
    this->runImageSrv("::io::IReader","::ioVTK::ImageReaderService",getIOConfiguration(file), imageFromDisk);

    ::boost::filesystem::remove(file);

    // Data read
    ::fwData::Image::SpacingType spacingRead = imageFromDisk->getSpacing();
    ::fwData::Image::SpacingType originRead = imageFromDisk->getOrigin();
    ::fwData::Image::SizeType sizeRead = imageFromDisk->getSize();


    CPPUNIT_ASSERT_EQUAL(spacingExpected.size(), spacingRead.size() );
    CPPUNIT_ASSERT_EQUAL(originExpected.size(), originRead.size() );
    CPPUNIT_ASSERT_EQUAL(sizeExpected.size(), sizeRead.size() );

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on x", spacingExpected[0], spacingRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on y", spacingExpected[1], spacingRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on z", spacingExpected[2], spacingRead[2], epsilon);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on x", originExpected[0], originRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on y", originExpected[1], originRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on z", originExpected[2], originRead[2], epsilon);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on x", sizeExpected[0], sizeRead[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on y", sizeExpected[1], sizeRead[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on z", sizeExpected[2], sizeRead[2]);

    ::fwComEd::helper::Image imageHelper(image);
    ::fwComEd::helper::Image imageFromDiskHelper(imageFromDisk);

    char *ptrOnGeneratedImage = static_cast<char*>(imageHelper.getBuffer());
    char *ptrOnReadImage = static_cast<char*>(imageFromDiskHelper.getBuffer());

    CPPUNIT_ASSERT_EQUAL( image->getType(), imageFromDisk->getType() );
    CPPUNIT_ASSERT( std::equal(ptrOnGeneratedImage, ptrOnGeneratedImage + image->getSizeInBytes(), ptrOnReadImage) );

}

//------------------------------------------------------------------------------

void ImageReaderWriterTest::testVtkImageSeriesWriter()
{
    ::fwTools::Type type = ::fwTools::Type::create< float >();
    ::fwData::Image::sptr image = ::fwData::Image::New();
    ::fwTest::generator::Image::generateRandomImage(image, type);

    ::fwMedData::ImageSeries::sptr imageSeries = ::fwMedData::ImageSeries::New();
    imageSeries->setImage(image);

    const ::boost::filesystem::path file = ::fwTools::System::getTemporaryFolder() / "imageSeries.vtk";

    // Write image series
    this->runImageSrv("::io::IWriter", "::ioVTK::SImageSeriesWriter", getIOConfiguration(file), imageSeries);

    // Read image series
    ::fwData::Image::sptr newImage = ::fwData::Image::New();
    this->runImageSrv("::io::IReader","::ioVTK::ImageReaderService", getIOConfiguration(file), newImage);

    ::fwTest::helper::ExcludeSetType exclude;
    exclude.insert("window_center");
    exclude.insert("window_width");

    CPPUNIT_ASSERT(::fwTest::helper::compare(image, newImage, exclude));
}

//------------------------------------------------------------------------------

void ImageReaderWriterTest::testVtiImageWriter()
{
    // Data to write
    const size_t dim = 3;
    ::fwTools::Type type("uint8");
    ::fwData::Image::SizeType sizeExpected(dim);
    sizeExpected[0] = 10;
    sizeExpected[1] = 20;
    sizeExpected[2] = 30;
    ::fwData::Image::SpacingType spacingExpected(dim);
    spacingExpected[0] = 0.24;
    spacingExpected[1] = 1.07;
    spacingExpected[2] = 2.21;
    ::fwData::Image::OriginType originExpected(dim);
    originExpected[0] = -05.6;
    originExpected[1] = 15.16;
    originExpected[2] = 11.11;

    ::fwData::Image::sptr image = ::fwData::Image::New();
    ::fwTest::generator::Image::generateImage(image, sizeExpected, spacingExpected, originExpected, type);

    // Write to vtk image.
    const ::boost::filesystem::path file = ::fwTools::System::getTemporaryFolder() / "temporaryFile.vti";

    this->runImageSrv("::io::IWriter","::ioVTK::ImageWriterService",getIOConfiguration(file), image);


    // Read image from disk
    ::fwData::Image::sptr imageFromDisk = ::fwData::Image::New();
    this->runImageSrv("::io::IReader","::ioVTK::ImageReaderService",getIOConfiguration(file), imageFromDisk);

    // Data read
    ::fwData::Image::SpacingType spacingRead = imageFromDisk->getSpacing();
    ::fwData::Image::SpacingType originRead = imageFromDisk->getOrigin();
    ::fwData::Image::SizeType sizeRead = imageFromDisk->getSize();


    CPPUNIT_ASSERT_EQUAL(spacingExpected.size(), spacingRead.size() );
    CPPUNIT_ASSERT_EQUAL(originExpected.size(), originRead.size() );
    CPPUNIT_ASSERT_EQUAL(sizeExpected.size(), sizeRead.size() );

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on x", spacingExpected[0], spacingRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on y", spacingExpected[1], spacingRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on z", spacingExpected[2], spacingRead[2], epsilon);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on x", originExpected[0], originRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on y", originExpected[1], originRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on z", originExpected[2], originRead[2], epsilon);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on x", sizeExpected[0], sizeRead[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on y", sizeExpected[1], sizeRead[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on z", sizeExpected[2], sizeRead[2]);

    ::fwComEd::helper::Image imageHelper(image);
    ::fwComEd::helper::Image imageFromDiskHelper(imageFromDisk);
    char *ptrOnGeneratedImage = static_cast<char*>(imageHelper.getBuffer());
    char *ptrOnReadImage = static_cast<char*>(imageFromDiskHelper.getBuffer());

    CPPUNIT_ASSERT_EQUAL( image->getType(), imageFromDisk->getType());
    CPPUNIT_ASSERT( std::equal(ptrOnGeneratedImage, ptrOnGeneratedImage + image->getSizeInBytes(), ptrOnReadImage) );


}

//------------------------------------------------------------------------------

void ImageReaderWriterTest::testMhdImageWriter()
{
    // Data to write
    const size_t dim = 3;
    ::fwTools::Type type("uint8");
    ::fwData::Image::SizeType sizeExpected(dim);
    sizeExpected[0] = 10;
    sizeExpected[1] = 20;
    sizeExpected[2] = 30;
    ::fwData::Image::SpacingType spacingExpected(dim);
    spacingExpected[0] = 0.24;
    spacingExpected[1] = 1.07;
    spacingExpected[2] = 2.21;
    ::fwData::Image::OriginType originExpected(dim);
    originExpected[0] = -05.6;
    originExpected[1] = 15.16;
    originExpected[2] = 11.11;

    ::fwData::Image::sptr image = ::fwData::Image::New();
    ::fwTest::generator::Image::generateImage(image, sizeExpected, spacingExpected, originExpected, type);

    // Write to vtk image.
    const ::boost::filesystem::path file = ::fwTools::System::getTemporaryFolder()/ "temporaryFile.mhd";

    this->runImageSrv("::io::IWriter","::ioVTK::ImageWriterService",getIOConfiguration(file), image);

    // Read image from disk
    ::fwData::Image::sptr imageFromDisk = ::fwData::Image::New();
    this->runImageSrv("::io::IReader","::ioVTK::ImageReaderService",getIOConfiguration(file), imageFromDisk);

    // Data read
    ::fwData::Image::SpacingType spacingRead = imageFromDisk->getSpacing();
    ::fwData::Image::SpacingType originRead = imageFromDisk->getOrigin();
    ::fwData::Image::SizeType sizeRead = imageFromDisk->getSize();


    CPPUNIT_ASSERT_EQUAL(spacingExpected.size(), spacingRead.size() );
    CPPUNIT_ASSERT_EQUAL(originExpected.size(), originRead.size() );
    CPPUNIT_ASSERT_EQUAL(sizeExpected.size(), sizeRead.size() );

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on x", spacingExpected[0], spacingRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on y", spacingExpected[1], spacingRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect spacing on z", spacingExpected[2], spacingRead[2], epsilon);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on x", originExpected[0], originRead[0], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on y", originExpected[1], originRead[1], epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Incorrect origin on z", originExpected[2], originRead[2], epsilon);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on x", sizeExpected[0], sizeRead[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on y", sizeExpected[1], sizeRead[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size on z", sizeExpected[2], sizeRead[2]);


    ::fwComEd::helper::Image imageHelper(image);
    ::fwComEd::helper::Image imageFromDiskHelper(imageFromDisk);
    char *ptrOnGeneratedImage = static_cast<char*>(imageHelper.getBuffer());
    char *ptrOnReadImage = static_cast<char*>(imageFromDiskHelper.getBuffer());

    CPPUNIT_ASSERT_EQUAL( image->getType(), imageFromDisk->getType());
    CPPUNIT_ASSERT( std::equal(ptrOnGeneratedImage, ptrOnGeneratedImage + image->getSizeInBytes(), ptrOnReadImage) );

}

//------------------------------------------------------------------------------

void ImageReaderWriterTest::testImageWriterExtension()
{
    // Data to write
    const size_t dim = 3;
    ::fwTools::Type type("uint8");
    ::fwData::Image::SizeType sizeExpected(dim);
    sizeExpected[0] = 10;
    sizeExpected[1] = 20;
    sizeExpected[2] = 30;
    ::fwData::Image::SpacingType spacingExpected(dim);
    spacingExpected[0] = 0.24;
    spacingExpected[1] = 1.07;
    spacingExpected[2] = 2.21;
    ::fwData::Image::OriginType originExpected(dim);
    originExpected[0] = -05.6;
    originExpected[1] = 15.16;
    originExpected[2] = 11.11;

    ::fwData::Image::sptr image = ::fwData::Image::New();
    ::fwTest::generator::Image::generateImage(image, sizeExpected, spacingExpected, originExpected, type);

    // Write to vtk image.
    const ::boost::filesystem::path file = ::fwTools::System::getTemporaryFolder()/ "temporaryFile.xxx";


    {
        const std::string srvtype("::io::IWriter");
        const std::string srvname("::ioVTK::ImageWriterService");


        ::fwServices::IService::sptr srv;
        srv = ::fwServices::registry::ServiceFactory::getDefault()->create( srvtype, srvname );

        CPPUNIT_ASSERT_MESSAGE(std::string("Failed to create service ") + srvname, srv);

        ::fwServices::OSR::registerService( image , srv );

        CPPUNIT_ASSERT_NO_THROW( srv->setConfiguration(getIOConfiguration(file)) );
        CPPUNIT_ASSERT_NO_THROW( srv->configure() );
        CPPUNIT_ASSERT_NO_THROW( srv->start() );
        CPPUNIT_ASSERT_THROW( srv->update(), ::fwTools::Failed);
        CPPUNIT_ASSERT_NO_THROW( srv->stop() );
        ::fwServices::OSR::unregisterService( srv );
    }
}

//------------------------------------------------------------------------------

void ImageReaderWriterTest::runImageSrv(
        const std::string &srvtype,
        const std::string &srvname,
        const SPTR(::fwRuntime::EConfigurationElement)& cfg,
        const SPTR(::fwData::Object)& image)
{

    ::fwServices::IService::sptr srv;
    srv = ::fwServices::registry::ServiceFactory::getDefault()->create( srvtype, srvname );

    CPPUNIT_ASSERT_MESSAGE(std::string("Failed to create service ") + srvname, srv);

    ::fwServices::OSR::registerService( image , srv );

    CPPUNIT_ASSERT_NO_THROW( srv->setConfiguration(cfg) );
    CPPUNIT_ASSERT_NO_THROW( srv->configure() );
    CPPUNIT_ASSERT_NO_THROW( srv->start() );
    CPPUNIT_ASSERT_NO_THROW( srv->update() );
    CPPUNIT_ASSERT_NO_THROW( srv->stop() );
    ::fwServices::OSR::unregisterService( srv );
}

//------------------------------------------------------------------------------

} //namespace ut
} //namespace ioVTK

