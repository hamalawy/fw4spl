/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <fwTools/dateAndTime.hpp>
#include <fwTools/System.hpp>

#include <fwRuntime/EConfigurationElement.hpp>

#include <fwDataCamp/visitor/CompareObjects.hpp>

#include <fwData/Object.hpp>
#include <fwData/Patient.hpp>
#include <fwData/PatientDB.hpp>
#include <fwData/Study.hpp>

#include <fwMedData/ImageSeries.hpp>

#include <fwTest/Data.hpp>
#include <fwTest/generator/Image.hpp>

#include <fwDataTools/Image.hpp>

#include <fwServices/registry/ServiceFactory.hpp>
#include <fwServices/registry/ObjectService.hpp>


#include "IoItkTest.hpp"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::ioITK::ut::IoItkTest );

namespace ioITK
{
namespace ut
{

//-----------------------------------------------------------------------------

void compare(::fwData::Object::sptr objRef, ::fwData::Object::sptr objComp)
{
    ::fwDataCamp::visitor::CompareObjects visitor;
    visitor.compare(objRef, objComp);
    SPTR(::fwDataCamp::visitor::CompareObjects::PropsMapType) props = visitor.getDifferences();
    BOOST_FOREACH( ::fwDataCamp::visitor::CompareObjects::PropsMapType::value_type prop, (*props) )
    {
        OSLM_ERROR( "new object difference found : " << prop.first << " '" << prop.second << "'" );
    }
    CPPUNIT_ASSERT_MESSAGE("Object Not equal" , props->size() == 0 );
}

//------------------------------------------------------------------------------

void IoItkTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void IoItkTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void IoItkTest::executeService(
        const SPTR(::fwData::Object)& obj,
        const std::string& srvType,
        const std::string& srvImpl,
        const SPTR(::fwRuntime::EConfigurationElement)& cfg )
{
    ::fwServices::IService::sptr srv
        = ::fwServices::registry::ServiceFactory::getDefault()->create(srvType, srvImpl);

    CPPUNIT_ASSERT(srv);
    ::fwServices::OSR::registerService( obj , srv );
    srv->setConfiguration(cfg);
    CPPUNIT_ASSERT_NO_THROW(srv->configure());
    CPPUNIT_ASSERT_NO_THROW(srv->start());
    CPPUNIT_ASSERT_NO_THROW(srv->update());
    CPPUNIT_ASSERT_NO_THROW(srv->stop());
    ::fwServices::OSR::unregisterService( srv );
}

//------------------------------------------------------------------------------

void IoItkTest::testImageSeriesWriterJPG()
{
    // Create image series
    ::fwData::Image::sptr image = ::fwData::Image::New();
    ::fwTest::generator::Image::generateRandomImage(image, ::fwTools::Type::create("int16"));

    ::fwMedData::ImageSeries::sptr imageSeries = ::fwMedData::ImageSeries::New();
    imageSeries->setImage(image);

    // Create path
    const ::boost::filesystem::path path = "imageSeriesJPG";
    ::boost::filesystem::create_directories(path);

    // Create Config
    ::fwRuntime::EConfigurationElement::NewSptr srvCfg("service");
    ::fwRuntime::EConfigurationElement::NewSptr folderCfg("folder");
    folderCfg->setValue(path.string());
    srvCfg->addConfigurationElement(folderCfg);

    // Create and execute service
    this->executeService(imageSeries, "::io::IWriter", "::ioITK::SJpgImageSeriesWriter", srvCfg);

    // Remove path
    ::boost::filesystem::remove_all( path.string() );
}


//------------------------------------------------------------------------------

void IoItkTest::testImageWriterJPG()
{
    // Create Image
    ::fwData::Image::sptr image = ::fwData::Image::New();
    ::fwTest::generator::Image::generateRandomImage(image, ::fwTools::Type::create("int16"));

    // Create path
    const ::boost::filesystem::path path = "imageJPG";
    ::boost::filesystem::create_directories( path );

    // Create Config
    ::fwRuntime::EConfigurationElement::NewSptr srvCfg("service");
    ::fwRuntime::EConfigurationElement::NewSptr folderCfg("folder");
    folderCfg->setValue(path.string());
    srvCfg->addConfigurationElement(folderCfg);

    // Create and execute service
    this->executeService( image, "::io::IWriter", "::ioITK::JpgImageWriterService", srvCfg );

    // Remove path
    ::boost::filesystem::remove_all( path.string() );
}

//------------------------------------------------------------------------------

void IoItkTest::testPatientDBReaderJPG()
{
    // create a Patient
    ::boost::filesystem::path pathJPGDir = ::fwTest::Data::dir() / "fw4spl/image/jpg";
    ::fwData::PatientDB::NewSptr patientDB;

    // Create Config
    ::fwRuntime::EConfigurationElement::NewSptr srvCfg("service");
    ::fwRuntime::EConfigurationElement::NewSptr folderCfg("folder");
    folderCfg->setValue(pathJPGDir.string());
    srvCfg->addConfigurationElement(folderCfg);

    // Create and execute service
    this->executeService( patientDB, "::io::IReader", "::ioITK::JpgPatientDBReaderService", srvCfg );

    CPPUNIT_ASSERT_EQUAL(size_t(3),patientDB->getNumberOfPatients());
    ::fwData::Image::sptr image
        = patientDB->getPatients().front()->getStudies().front()->getAcquisitions().front()->getImage();
    CPPUNIT_ASSERT_EQUAL(size_t(3),image->getNumberOfDimensions());
    CPPUNIT_ASSERT_EQUAL(::fwData::Image::SizeType::value_type(512), image->getSize()[0]);
    CPPUNIT_ASSERT_EQUAL(::fwData::Image::SizeType::value_type(256), image->getSize()[1]);
    CPPUNIT_ASSERT_EQUAL(::fwData::Image::SizeType::value_type(1), image->getSize()[2]);
}

//------------------------------------------------------------------------------

double tolerance(double num)
{
    return std::floor(num * 100. + .5) / 100.;
}

void IoItkTest::testSaveLoadInr()
{
    ::fwData::Image::sptr image = ::fwData::Image::New();
    ::fwTest::generator::Image::generateRandomImage(image, ::fwTools::Type::create("int16"));

    // inr only support image origin (0,0,0)
    ::fwData::Image::OriginType origin(3,0);
    image->setOrigin(origin);

    // save image in inr
    const ::boost::filesystem::path PATH = "imageInrTest/image.inr.gz";
    ::boost::filesystem::create_directories( PATH.parent_path() );

    // Create Config
    ::fwRuntime::EConfigurationElement::NewSptr srvCfg("service");
    ::fwRuntime::EConfigurationElement::NewSptr fileCfg("file");
    fileCfg->setValue(PATH.string());
    srvCfg->addConfigurationElement(fileCfg);

    // Create and execute service
    this->executeService( image, "::io::IWriter", "::ioITK::InrImageWriterService", srvCfg );

    // load Image
    ::fwData::Image::sptr image2 = ::fwData::Image::New();
    this->executeService( image2, "::io::IReader", "::ioITK::InrImageReaderService", srvCfg );

    ::boost::filesystem::remove_all( PATH.parent_path().string() );

    ::fwData::Image::SpacingType spacing = image2->getSpacing();
    std::transform (spacing.begin(), spacing.end(), spacing.begin(), tolerance);
    image2->setSpacing(spacing);

    // check Image
    compare(image, image2);
}

//------------------------------------------------------------------------------

void IoItkTest::testLoadInr()
{
    // path on inr
    ::boost::filesystem::path file1 = ::fwTest::Data::dir() / "fw4spl/image/inr/image.inr.gz";
    ::boost::filesystem::path file2 = ::fwTest::Data::dir() / "fw4spl/image/inr/skin.inr.gz";
    ::boost::filesystem::path folder = ::fwTest::Data::dir() / "fw4spl/image/inr";



    // Create Config 1
    ::fwRuntime::EConfigurationElement::NewSptr srvCfg("service");
    ::fwRuntime::EConfigurationElement::NewSptr fileCfg("file");
    fileCfg->setValue(file1.string());
    srvCfg->addConfigurationElement(fileCfg);

    // Create and execute service
    ::fwData::PatientDB::NewSptr patientDB;
    this->executeService( patientDB, "::io::IReader", "::ioITK::InrPatientDBReaderService", srvCfg );
    CPPUNIT_ASSERT_EQUAL(size_t(1), patientDB->getNumberOfPatients());



    // Create Config 2
    ::fwRuntime::EConfigurationElement::NewSptr srvCfg2("service");
    ::fwRuntime::EConfigurationElement::NewSptr fileCfg2a("file");
    fileCfg2a->setValue(file1.string());
    srvCfg2->addConfigurationElement(fileCfg2a);
    ::fwRuntime::EConfigurationElement::NewSptr fileCfg2b("file");
    fileCfg2b->setValue(file2.string());
    srvCfg2->addConfigurationElement(fileCfg2b);

    // Create and execute service
    ::fwData::PatientDB::NewSptr patientDB2;
    this->executeService( patientDB2, "::io::IReader", "::ioITK::InrPatientDBReaderService", srvCfg2 );
    CPPUNIT_ASSERT_EQUAL(size_t(2),patientDB2->getNumberOfPatients());



    // Create Config 3
    ::fwRuntime::EConfigurationElement::NewSptr srvCfg3("service");
    ::fwRuntime::EConfigurationElement::NewSptr folderCfg3("folder");
    folderCfg3->setValue(folder.string());
    srvCfg3->addConfigurationElement(folderCfg3);

    // Create and execute service
    ::fwData::PatientDB::NewSptr patientDB3;
    this->executeService( patientDB3, "::io::IReader", "::ioITK::InrPatientDBReaderService", srvCfg3 );
    CPPUNIT_ASSERT_EQUAL(size_t(2),patientDB3->getNumberOfPatients());
}

//------------------------------------------------------------------------------

} //namespace ut
} //namespace ioITK

