/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <vector>
#include <string>
#include <set>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/functional/hash.hpp>

#include <fwRuntime/EConfigurationElement.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/AppConfigManager.hpp>
#include <fwServices/registry/AppConfig.hpp>

#include <fwMedData/ModelSeries.hpp>
#include <fwMedData/SeriesDB.hpp>

#include <fwData/Reconstruction.hpp>
#include <fwData/Mesh.hpp>
#include <fwData/Array.hpp>

#include <fwTools/BufferObject.hpp>
#include <fwTools/System.hpp>

#include <fwTest/generator/SeriesDB.hpp>

#include "ModelSeriesWriterTest.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::ioVTK::ut::ModelSeriesWriterTest );

namespace ioVTK
{
namespace ut
{

namespace fs = ::boost::filesystem;
typedef std::vector< std::string >  FileContainerType;

//------------------------------------------------------------------------------

void ModelSeriesWriterTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void ModelSeriesWriterTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void runModelSeriesSrv(
        const std::string& impl,
        const SPTR(::fwRuntime::EConfigurationElement)& cfg,
        const SPTR(::fwData::Object)& obj)
{
   ::fwServices::IService::sptr srv = ::fwServices::registry::ServiceFactory::getDefault()->create(impl);

    CPPUNIT_ASSERT_MESSAGE(std::string("Failed to create service ") + impl, srv);

    ::fwServices::OSR::registerService(obj, srv);

    CPPUNIT_ASSERT_NO_THROW( srv->setConfiguration( cfg ) );
    CPPUNIT_ASSERT_NO_THROW( srv->configure() );
    CPPUNIT_ASSERT_NO_THROW( srv->start() );
    CPPUNIT_ASSERT_NO_THROW( srv->update() );
    CPPUNIT_ASSERT_NO_THROW( srv->stop() );
    ::fwServices::OSR::unregisterService( srv );
}

//------------------------------------------------------------------------------

::fwRuntime::EConfigurationElement::sptr getIOCfgFromFolder(const fs::path& file)
{
    ::fwRuntime::EConfigurationElement::NewSptr srvCfg("service");
    ::fwRuntime::EConfigurationElement::NewSptr cfg("folder");
    cfg->setValue(file.string());
    srvCfg->addConfigurationElement(cfg);

    return srvCfg;
}

//------------------------------------------------------------------------------

::fwRuntime::EConfigurationElement::sptr getIOCfgFromFiles(const FileContainerType& files)
{
    ::fwRuntime::EConfigurationElement::NewSptr srvCfg("service");

    BOOST_FOREACH(std::string file, files)
    {
        ::fwRuntime::EConfigurationElement::NewSptr cfg("file");
        cfg->setValue(file);
        srvCfg->addConfigurationElement(cfg);
    }

    return srvCfg;
}

//------------------------------------------------------------------------------

size_t recHash(const ::fwData::Reconstruction::sptr &rec)
{
    ::fwData::Mesh::sptr mesh = rec->getMesh();

    ::fwData::Array::sptr points = mesh->getPointsArray();
    ::fwData::Array::sptr cellTypes = mesh->getCellTypesArray();
    ::fwData::Array::sptr cellData = mesh->getCellDataArray();
    ::fwData::Array::sptr cellDataOffsets = mesh->getCellDataOffsetsArray();

    std::string buf;

    ::fwTools::BufferObject::sptr bo;
    ::fwTools::BufferObject::Lock lock;

    bo = points->getBufferObject();
    lock = (bo->lock());
    buf.append(static_cast< char * >(lock.getBuffer()), bo->getSize());

    bo = cellTypes->getBufferObject();
    lock = (bo->lock());
    buf.append(static_cast< char * >(lock.getBuffer()), bo->getSize());

    bo = cellData->getBufferObject();
    lock = (bo->lock());
    buf.append(static_cast< char * >(lock.getBuffer()), bo->getSize());

    bo = cellDataOffsets->getBufferObject();
    lock = (bo->lock());
    buf.append(static_cast< char * >(lock.getBuffer()), bo->getSize());

    ::boost::hash<std::string> stringHash;
    return stringHash(buf);
}

//------------------------------------------------------------------------------

void ModelSeriesWriterTest::testWriteMeshes()
{
    ::fwMedData::ModelSeries::sptr modelSeries = ::fwTest::generator::SeriesDB::createModelSeries(5);

    const fs::path dir = ::fwTools::System::getTemporaryFolder() / "modelSeries";

    if( fs::exists(dir) )
    {
        CPPUNIT_ASSERT_MESSAGE(std::string("Directory ") + dir.string() + " must be empty",
                fs::is_empty(dir));
    }
    else
    {
        fs::create_directories(dir);
    }

    runModelSeriesSrv(
            "::ioVTK::SModelSeriesWriter",
            getIOCfgFromFolder(dir),
            modelSeries);

    ::fwMedData::SeriesDB::sptr seriesDB = ::fwMedData::SeriesDB::New();

    FileContainerType files;
    for(fs::directory_iterator it(dir); it != fs::directory_iterator(); ++it)
    {
        files.push_back(it->path().string());
    }

    CPPUNIT_ASSERT_EQUAL(modelSeries->getReconstructionDB().size(), files.size());

    runModelSeriesSrv(
            "::ioVTK::SSeriesDBReader",
            getIOCfgFromFiles(files),
            seriesDB);

    const ::fwMedData::SeriesDB::ContainerType& series = seriesDB->getContainer();
    CPPUNIT_ASSERT_EQUAL((size_t)1, series.size());

    ::fwMedData::ModelSeries::sptr readSeries = ::fwMedData::ModelSeries::dynamicCast(series[0]);
    CPPUNIT_ASSERT_MESSAGE("A ModelSeries was expected", readSeries);

    typedef ::fwMedData::ModelSeries::ReconstructionVectorType RecVecType;
    const RecVecType& readRecs = readSeries->getReconstructionDB();
    CPPUNIT_ASSERT_EQUAL(files.size(), readRecs.size());

    std::set< size_t > refHashes;
    std::set< size_t > readHashes;

    const RecVecType& refRecs = modelSeries->getReconstructionDB();
    RecVecType::const_iterator itRef = refRecs.begin();
    RecVecType::const_iterator itRead = readRecs.begin();

    for(; itRef != refRecs.end(); ++itRef, ++itRead)
    {
        refHashes.insert(recHash(*itRef));
        CPPUNIT_ASSERT_MESSAGE("No valid mesh found in reconstruction", (*itRef)->getMesh());
        readHashes.insert(recHash(*itRead));
        CPPUNIT_ASSERT_MESSAGE("No valid mesh found in read reconstruction", (*itRead)->getMesh());
    }

    CPPUNIT_ASSERT(refHashes == readHashes);
}

//------------------------------------------------------------------------------

void ModelSeriesWriterTest::testWriteReconstructions()
{
    ::fwMedData::ModelSeries::sptr modelSeries = ::fwTest::generator::SeriesDB::createModelSeries(5);

    const fs::path dir = ::fwTools::System::getTemporaryFolder() / "modelSeriesObj";

    if( fs::exists(dir) )
    {
        CPPUNIT_ASSERT_MESSAGE(std::string("Directory ") + dir.string() + " must be empty",
                fs::is_empty(dir));
    }
    else
    {
        fs::create_directories(dir);
    }

    runModelSeriesSrv(
            "::ioVTK::SModelSeriesObjWriter",
            getIOCfgFromFolder(dir),
            modelSeries);

    ::fwMedData::SeriesDB::sptr seriesDB = ::fwMedData::SeriesDB::New();

    FileContainerType files;
    for(fs::directory_iterator it(dir); it != fs::directory_iterator(); ++it)
    {
        files.push_back(it->path().string());
    }

    // Writer generates a .mtl file for each .obj file
    CPPUNIT_ASSERT_EQUAL(modelSeries->getReconstructionDB().size() * 2, files.size());
}

//------------------------------------------------------------------------------

} //namespace ut
} //namespace ioVTK

