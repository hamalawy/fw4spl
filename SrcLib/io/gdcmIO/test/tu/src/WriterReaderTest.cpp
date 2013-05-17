/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <fwTools/System.hpp>

#include <fwDataCamp/visitor/CompareObjects.hpp>

#include <fwData/Object.hpp>
#include <fwData/Image.hpp>
#include <fwData/Material.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwData/PointList.hpp>
#include <fwData/String.hpp>
#include <fwData/Vector.hpp>
#include <fwData/Boolean.hpp>
#include <fwData/Mesh.hpp>

#include <fwMedData/SeriesDB.hpp>
#include <fwMedData/Series.hpp>
#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/ModelSeries.hpp>

#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>
#include <fwComEd/Dictionary.hpp>

#include <fwTest/generator/SeriesDB.hpp>
#include <fwTest/generator/Image.hpp>
#include <fwTest/generator/Object.hpp>
#include <fwTest/helper/compare.hpp>

#include <gdcmIO/writer/Series.hpp>
#include <gdcmIO/writer/SeriesDB.hpp>
#include <gdcmIO/reader/SeriesDB.hpp>

#include "WriterReaderTest.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::gdcmIO::ut::WriterReaderTest );

namespace gdcmIO
{
namespace ut
{

//-----------------------------------------------------------------------------

double tolerance(double num)
{
    return std::floor(num * 1000. + .5) / 1000;
}

void roundSpacing(::fwData::Image::sptr image)
{
    ::fwData::Image::SpacingType spacing = image->getSpacing();
    std::transform (spacing.begin(), spacing.end(), spacing.begin(), tolerance);
    image->setSpacing(spacing);
}

//------------------------------------------------------------------------------

void WriterReaderTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void WriterReaderTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void WriterReaderTest::writeReadImageSeriesTest()
{
    ::fwTest::generator::Image::initRand();
    ::fwMedData::ImageSeries::sptr imgSeries;
    imgSeries = ::fwTest::generator::SeriesDB::createImageSeries();

    const ::boost::filesystem::path PATH = ::fwTools::System::getTemporaryFolder() / "dicomTest";

    ::boost::filesystem::create_directories( PATH );

    ::gdcmIO::writer::Series::sptr writer = ::gdcmIO::writer::Series::New();
    writer->setObject(imgSeries);
    writer->setFolder(PATH);
    CPPUNIT_ASSERT_NO_THROW(writer->write());

    // load ImageSeries
    ::fwMedData::SeriesDB::sptr sdb = ::fwMedData::SeriesDB::New();
    ::gdcmIO::reader::SeriesDB::sptr reader = ::gdcmIO::reader::SeriesDB::New();
    reader->setObject(sdb);
    reader->setFolder(PATH);

    CPPUNIT_ASSERT_NO_THROW(reader->read());

    ::boost::filesystem::remove_all( PATH );

    // check series
    CPPUNIT_ASSERT_EQUAL(size_t(1), sdb->getContainer().size());

    ::fwMedData::Series::sptr series = sdb->getContainer().front();
    ::fwMedData::ImageSeries::sptr imgseries = ::fwMedData::ImageSeries::dynamicCast(series);
    ::fwData::Image::sptr image = imgseries->getImage();
    roundSpacing(image);

    CPPUNIT_ASSERT(::fwTest::helper::compare(imgSeries, sdb->getContainer().front()));
}
//------------------------------------------------------------------------------

void WriterReaderTest::writeReadSeriesDBTest()
{
    ::fwTest::generator::Image::initRand();
    ::fwMedData::SeriesDB::sptr seriesDB;
    seriesDB = this->createSeriesDB();

    const ::boost::filesystem::path PATH = ::fwTools::System::getTemporaryFolder() / "dicomTest";

    ::boost::filesystem::create_directories( PATH );

    ::gdcmIO::writer::SeriesDB::sptr writer = ::gdcmIO::writer::SeriesDB::New();
    writer->setObject(seriesDB);
    writer->setFolder(PATH);
    CPPUNIT_ASSERT_NO_THROW(writer->write());

    // load ImageSeries
    ::fwMedData::SeriesDB::sptr sdb = ::fwMedData::SeriesDB::New();
    ::gdcmIO::reader::SeriesDB::sptr reader = ::gdcmIO::reader::SeriesDB::New();
    reader->setObject(sdb);
    reader->setFolder(PATH);

    CPPUNIT_ASSERT_NO_THROW(reader->read());

    ::boost::filesystem::remove_all( PATH );

    // check series
    CPPUNIT_ASSERT(::fwTest::helper::compare(seriesDB, sdb));
}

//------------------------------------------------------------------------------

::fwMedData::SeriesDB::sptr WriterReaderTest::createSeriesDB()
{
    //create SeriesDB
    ::fwMedData::SeriesDB::sptr sdb = ::fwMedData::SeriesDB::New();
    ::fwMedData::ImageSeries::sptr imgSeries = ::fwTest::generator::SeriesDB::createImageSeries();
    ::fwMedData::ModelSeries::sptr modelSeries = ::fwTest::generator::SeriesDB::createModelSeries(1);

    sdb->getContainer().push_back(imgSeries);
    sdb->getContainer().push_back(modelSeries);

    ::fwData::Image::sptr image = imgSeries->getImage();

    // Add landmarks
    ::fwComEd::fieldHelper::MedicalImageHelpers::checkLandmarks(image);
    ::fwData::PointList::sptr landmarks = image->getField< ::fwData::PointList >( ::fwComEd::Dictionary::m_imageLandmarksId);
    ::fwData::Image::SpacingType spacing = image->getSpacing();
    ::fwData::Image::OriginType origin = image->getOrigin();
    ::fwData::Point::sptr point = ::fwData::Point::New(2.6f + static_cast<float>(origin[0]),
                                                       1.2f + static_cast<float>(origin[1]),
                                                       4.5f + static_cast<float>(origin[2]));
    point->setField( ::fwComEd::Dictionary::m_labelId , ::fwData::String::New("Label1") );
    landmarks->getRefPoints().push_back(point);
    ::fwData::Point::sptr point2 = ::fwData::Point::New(1.2f + static_cast<float>(origin[0]),
                                                        2.4f + static_cast<float>(origin[1]),
                                                        0.3f + static_cast<float>(origin[2]));
    point2->setField( ::fwComEd::Dictionary::m_labelId , ::fwData::String::New("Label2") );
    landmarks->getRefPoints().push_back(point2);
    ::fwData::Image::SizeType size = image->getSize();
    ::fwData::Point::sptr point3 = ::fwData::Point::New(1.2f + static_cast<float>(origin[0]),
                                                        2.4f + static_cast<float>(origin[1]),
                                                        static_cast<float>((size[2]-1) * spacing[2] + origin[2]));
    point3->setField( ::fwComEd::Dictionary::m_labelId , ::fwData::String::New("toto") );
    landmarks->getRefPoints().push_back(point3);

    // Add distance
    ::fwData::PointList::sptr pl = ::fwData::PointList::New();
    ::fwData::Point::sptr pt1 = ::fwData::Point::New(0., 0., 0.);
    ::fwData::Point::sptr pt2 = ::fwData::Point::New(static_cast<float>((size[0]-1) * spacing[0]),
                                                     static_cast<float>((size[1]-1) * spacing[1]),
                                                     static_cast<float>((size[2]-1) * spacing[2]));
    pl->getRefPoints().push_back( pt1 );
    pl->getRefPoints().push_back( pt2 );

    ::fwData::Vector::sptr vectDist;
    vectDist = image->setDefaultField< ::fwData::Vector >(::fwComEd::Dictionary::m_imageDistancesId, ::fwData::Vector::New());
    vectDist->getContainer().push_back(pl);

    image->setField("ShowLandmarks", ::fwData::Boolean::New(true));
    image->setField("ShowDistances", ::fwData::Boolean::New(true));

    // Update Reconstruction
    ::fwData::Reconstruction::sptr rec = modelSeries->getReconstructionDB().front();
    ::fwData::Mesh::sptr mesh = rec->getMesh();
    mesh->setPointColorsArray(::fwData::Array::sptr());
    mesh->setCellColorsArray(::fwData::Array::sptr());
    mesh->setCellNormalsArray(::fwData::Array::sptr());

    // gdcm only manage ambient color in reconstruction
    ::fwData::Material::sptr material = ::fwData::Material::New();
    ::fwData::Color::sptr color = ::fwTest::generator::Object::randomizeColor();
    material->setAmbient(color);
    rec->setMaterial(material);
    rec->setImage(::fwData::Image::sptr()); // not managed

    modelSeries->setField("ShowReconstructions", ::fwData::Boolean::New(true));

    return sdb;
}

//------------------------------------------------------------------------------

} // namespace ut
} // namespace gdcmIO
