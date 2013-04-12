/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/log/SpyLogger.hpp>
#include <fwTools/UUID.hpp>

#include <fwData/Array.hpp>
#include <fwData/Mesh.hpp>
#include <fwData/Image.hpp>
#include <fwData/Float.hpp>
#include <fwData/Integer.hpp>
#include <fwData/Boolean.hpp>
#include <fwData/String.hpp>
#include <fwData/Color.hpp>
#include <fwData/Vector.hpp>
#include <fwData/Material.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwData/Acquisition.hpp>
#include <fwData/Study.hpp>
#include <fwData/Patient.hpp>
#include <fwData/PatientDB.hpp>
#include <fwData/TransformationMatrix3D.hpp>
#include <fwData/Point.hpp>
#include <fwData/PointList.hpp>
#include <fwData/Composite.hpp>
#include <fwData/TransferFunction.hpp>
#include <fwData/Graph.hpp>

#include <fwDataTools/Patient.hpp>
#include <fwDataTools/MeshGenerator.hpp>
#include <fwDataTools/Image.hpp>

#include <fwCamp/macros.hpp>
#include <fwCamp/Mapper/ValueMapper.hpp>
#include <fwCamp/UserObject.hpp>

#include <fwDataCamp/visitor/CompareObjects.hpp>

#include <fwAtoms/Object.hpp>
#include <fwAtoms/Map.hpp>
#include <fwAtoms/Sequence.hpp>

#include <fwTest/Exception.hpp>

#include <fwAtomConversion/convert.hpp>
#include <fwAtomConversion/DataVisitor.hpp>
#include <fwAtomConversion/exception/DataFactoryNotFound.hpp>
#include <fwAtomConversion/exception/DuplicatedDataUUID.hpp>
#include <fwAtomConversion/exception/ConversionNotManaged.hpp>

#include "ConversionTest.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION( ::fwAtomConversion::ut::ConversionTest );

namespace fwAtomConversion
{
namespace ut
{

static ::fwTest::Exception fwTestException(""); // force link with fwTest

//-----------------------------------------------------------------------------

// Set up context before running a test.
void ConversionTest::setUp()
{};

//-----------------------------------------------------------------------------

void ConversionTest::tearDown()
{
    // Clean up after the test run.
}

//-----------------------------------------------------------------------------

void ConversionTest::dataToAtomTest()
{
    ::fwData::Patient::sptr patient = ::fwData::Patient::New();
    ::fwDataTools::Patient::generatePatient(patient, 2, 2, 2);

    const ::fwData::Object::sptr VALUES[] = {
            ::fwData::Integer::New(1337),
            ::fwData::Float::New(),
            ::fwData::String::New(),
            ::fwData::Boolean::New(),
            ::fwData::Vector::New(),
            ::fwData::Color::New(1.4f, 0.9f, 1.1f, 1.67f),
            ::fwData::Array::New(),
            ::fwData::Image::New(),
            ::fwData::Mesh::New(),
            ::fwData::Material::New(),
            ::fwData::Reconstruction::New(),
            ::fwData::Acquisition::New(),
            ::fwData::Study::New(),
            ::fwData::Composite::New(),
            ::fwData::Patient::New(),
            ::fwData::Point::New(),
            ::fwData::PointList::New(),
            ::fwData::TransformationMatrix3D::New(),
            ::fwData::TransferFunction::New(),
            patient,
            ::fwData::Graph::New(),
    };

    ::fwAtoms::Object::sptr atom;

    BOOST_FOREACH ( fwData::Object::sptr  object, VALUES )
    {
        atom = ::fwAtomConversion::convert(object);

        const ::camp::Class& metaClass = ::camp::classByName(object->getClassname());

        //Type test
        CPPUNIT_ASSERT_EQUAL( object->getClassname(),
                              atom->getMetaInfo( ::fwAtomConversion::DataVisitor::CLASSNAME_METAINFO ) );

         //Test attribute type
        BOOST_FOREACH( ::fwAtoms::Object::AttributesType::value_type elem, atom->getAttributes() )
        {
            std::string classname = atom->getMetaInfo( ::fwAtomConversion::DataVisitor::CLASSNAME_METAINFO );
            if ( ! (  classname == "::fwData::Graph" &&
                    elem.first == "connections" ) )
            {
                int type = metaClass.property(elem.first).type();
                std::string attribute = metaClass.property(elem.first).name();
                switch(type)
                {
                case camp::stringType:
                    CPPUNIT_ASSERT(elem.second->isString());
                    CPPUNIT_ASSERT(elem.second->isValue());
                    break;
                case camp::realType :
                case camp::intType :
                    CPPUNIT_ASSERT(elem.second->isNumeric());
                    CPPUNIT_ASSERT(elem.second->isValue());
                    break;
                case camp::boolType :
                    CPPUNIT_ASSERT(elem.second->isBoolean());
                    CPPUNIT_ASSERT(elem.second->isValue());
                    break;
                case camp::userType:
                    if( ( ( classname == "::fwData::Mesh" ) && ( attribute == "cell_colors" ) ) ||
                            ( ( classname == "::fwData::Mesh" ) && ( attribute == "cell_normals" ) ) ||
                            ( ( classname == "::fwData::Mesh" ) && ( attribute == "point_colors" ) ) ||
                            ( ( classname == "::fwData::Mesh" ) && ( attribute == "point_normals" ) ) ||
                            ( ( classname == "::fwData::Reconstruction" ) && ( attribute == "image" ) ) ||
                            ( ( classname == "::fwData::Reconstruction" ) && ( attribute == "mesh" ) ) ||
                            ( ( classname == "::fwData::Acquisition" ) && ( attribute == "image" ) ) ||
                            ( ( classname == "::fwData::Acquisition" ) && ( attribute == "struct_anat" ) ) )
                    {
                        CPPUNIT_ASSERT(!elem.second);
                    }
                    else
                    {
                        CPPUNIT_ASSERT(elem.second->isObject() || elem.second->isBlob());
                    }
                    break;
                case camp::mappingType:
                    CPPUNIT_ASSERT(elem.second->isMapping());
                    break;
                case camp::enumType:
                    CPPUNIT_ASSERT(elem.second->isString());
                    CPPUNIT_ASSERT(elem.second->isValue());
                    break;
                case camp::arrayType:
                    CPPUNIT_ASSERT(elem.second->isSequence());
                    break;
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------

void ConversionTest::materialConversionTest()
{
    ::fwData::Color::sptr color = ::fwData::Color::New(0.2f, 1.2f, 1.3f, 0.9f);
    ::fwData::Material::sptr material = ::fwData::Material::New();
    material->setAmbient(color);

    // Create Atom
    ::fwData::Material::sptr materialTmp = ::fwData::Material::New();
    materialTmp->deepCopy( material );
    ::fwAtoms::Object::sptr atom = ::fwAtomConversion::convert( materialTmp );
    materialTmp.reset();

    // Create Data from Atom
    ::fwData::Object::sptr materialRes = ::fwAtomConversion::convert(atom);
    ::fwData::Material::sptr materialResultat = ::fwData::Material::dynamicCast(materialRes);

    bool materialComparison = ::fwDataTools::Patient::compareMaterial(material, materialResultat, std::string("material"));
    CPPUNIT_ASSERT_MESSAGE("Material Not equal" , materialComparison);
}

//-----------------------------------------------------------------------------

void ConversionTest::patientConversionTest()
{
    ::fwData::Patient::sptr patient = ::fwData::Patient::New();
    ::fwDataTools::Patient::generatePatient(patient, 1, 1, 1);

    // Creates Atom
    ::fwData::Patient::sptr patientTmp = ::fwData::Patient::New();
    patientTmp->deepCopy( patient );

    ::fwAtoms::Object::sptr atom = ::fwAtomConversion::convert( patientTmp );

    // Manages buffer owners
    ::fwData::Acquisition::sptr acq = patientTmp->getStudies()[0]->getAcquisitions()[0];
    ::fwData::Image::sptr image =  acq->getImage();
    ::fwData::Reconstruction::sptr rec =  acq->getReconstructions()[0];
    image->getDataArray()->setIsBufferOwner(false);
    rec->getImage()->getDataArray()->setIsBufferOwner(false);
    rec->getMesh()->getPointsArray()->setIsBufferOwner(false);
    rec->getMesh()->getPointColorsArray()->setIsBufferOwner(false);
    rec->getMesh()->getPointNormalsArray()->setIsBufferOwner(false);
    rec->getMesh()->getCellDataArray()->setIsBufferOwner(false);
    rec->getMesh()->getCellTypesArray()->setIsBufferOwner(false);
    rec->getMesh()->getCellDataOffsetsArray()->setIsBufferOwner(false);
    rec->getMesh()->getCellColorsArray()->setIsBufferOwner(false);
    rec->getMesh()->getCellNormalsArray()->setIsBufferOwner(false);
    rec.reset();
    image.reset();
    acq.reset();
    patientTmp.reset();

    // Creates Data from Atom
    ::fwData::Object::sptr patientObject = ::fwAtomConversion::convert(atom);
    ::fwData::Patient::sptr patientResultat = ::fwData::Patient::dynamicCast(patientObject);

    using namespace ::fwDataCamp::visitor;
    CompareObjects visitor;
    visitor.compare(patient, patientResultat);
    SPTR(CompareObjects::PropsMapType) props = visitor.getDifferences();
    BOOST_FOREACH( CompareObjects::PropsMapType::value_type prop, (*props) )
    {
        OSLM_ERROR( "new object difference found : " << prop.first << " " << prop.second );
    }
    CPPUNIT_ASSERT_MESSAGE("Patient Not equal" , props->size() == 0 );

    bool patientComparison = ::fwDataTools::Patient::comparePatient(patient, patientResultat);
    CPPUNIT_ASSERT_MESSAGE("Patient Not equal" , patientComparison);
}

//-----------------------------------------------------------------------------

void ConversionTest::graphConversionTest()
{
    ::fwAtoms::Object::sptr atom;
    ::fwTools::UUID::UUIDType gID,n1ID,n2ID,n3ID,e12ID,e23ID;
    {
        ::fwData::Graph::sptr g = ::fwData::Graph::New();
        gID = ::fwTools::UUID::get(g);
        ::fwData::Node::sptr  n1 = ::fwData::Node::New();
        n1ID = ::fwTools::UUID::get(n1);
        ::fwData::Node::sptr  n2 = ::fwData::Node::New();
        n2ID = ::fwTools::UUID::get(n2);
        ::fwData::Node::sptr  n3 = ::fwData::Node::New();
        n3ID = ::fwTools::UUID::get(n3);
        ::fwData::Edge::sptr  e12 = ::fwData::Edge::New();
        e12ID = ::fwTools::UUID::get(e12);
        ::fwData::Edge::sptr  e23 = ::fwData::Edge::New();
        e23ID = ::fwTools::UUID::get(e23);

        // build graph
        g->addNode(n1);
        g->addNode(n2);
        g->addNode(n3);

        n1->addOutputPort( ::fwData::Port::New() );
        n2->addInputPort( ::fwData::Port::New() );
        n2->addOutputPort( ::fwData::Port::New() );
        n3->addInputPort( ::fwData::Port::New() );

        e12->setIdentifiers("IDNOTdefined","IDNOTdefined");
        e23->setIdentifiers("IDNOTdefined","IDNOTdefined");

        g->addEdge(e12,n1,n2);
        g->addEdge(e23,n2,n3);

        // Test field on edge
        e12->setField("infoTest",::fwData::String::New("valueInfoTest"));

        // Create Atom
        atom = ::fwAtomConversion::convert( g );
    }

   // Create Data from Atom
   ::fwData::Graph::sptr newGraph = ::fwData::Graph::dynamicCast( ::fwAtomConversion::convert(atom) );

   // nodes
   ::fwData::Node::sptr n1, n2, n3;

   // Test nodes
   const ::fwData::Graph::NodeContainer & nodes = newGraph->getCRefNodes();
   CPPUNIT_ASSERT_EQUAL_MESSAGE("Graph nodes size" ,  (size_t)3, nodes.size() );
   BOOST_FOREACH( ::fwData::Node::sptr node, nodes )
   {
       ::fwTools::UUID::UUIDType nodeID = ::fwTools::UUID::get(node);
       CPPUNIT_ASSERT_MESSAGE("Test node uuid" ,  nodeID == n1ID || nodeID == n2ID || nodeID == n3ID );
       if (  nodeID == n1ID ) { n1 = node; }
       else if  (  nodeID == n2ID ) { n2 = node; }
       else if  (  nodeID == n3ID ) { n3 = node; }

   }
   CPPUNIT_ASSERT_MESSAGE("Test node n1" , n1 );
   CPPUNIT_ASSERT_MESSAGE("Test node n2" , n2 );
   CPPUNIT_ASSERT_MESSAGE("Test node n3" , n3 );

   // Test edges
   const ::fwData::Graph::ConnectionContainer & connections = newGraph->getCRefConnections();
   CPPUNIT_ASSERT_EQUAL_MESSAGE("Graph connections size" ,  (size_t)2, connections.size() );
   BOOST_FOREACH( ::fwData::Graph::ConnectionContainer::value_type elem , connections )
   {
       ::fwTools::UUID::UUIDType edgeID = ::fwTools::UUID::get(elem.first);
       CPPUNIT_ASSERT_MESSAGE("Test edge uuid" ,  edgeID == e12ID || edgeID == e23ID );
       if ( edgeID == e12ID )
       {
           CPPUNIT_ASSERT( newGraph->getSourceNode( elem.first ) == n1 );
           CPPUNIT_ASSERT( newGraph->getDestinationNode( elem.first ) == n2 );

           // test field
           CPPUNIT_ASSERT( elem.first->getField("infoTest") );
           CPPUNIT_ASSERT( elem.first->getField< ::fwData::String >("infoTest")->value() == "valueInfoTest" );
       }
       else
       {
           CPPUNIT_ASSERT( newGraph->getSourceNode( elem.first ) == n2 );
           CPPUNIT_ASSERT( newGraph->getDestinationNode( elem.first ) == n3 );
       }
   }
}

//-----------------------------------------------------------------------------

void ConversionTest::tfConversionTest()
{
    ::fwAtoms::Object::sptr atom;
    {
        ::fwData::TransferFunction::sptr tf = ::fwData::TransferFunction::New();
        tf->addTFColor(0.56, ::fwData::TransferFunction::TFColor(0.5, 0.56, 0.9, 1.0));
        tf->addTFColor(25.89, ::fwData::TransferFunction::TFColor(0.8, 0.05, 0.99, 0.2));

        // Create Atom
        atom = ::fwAtomConversion::convert( tf );
    }
    ::fwData::TransferFunction::sptr newTF =
            ::fwData::TransferFunction::dynamicCast( ::fwAtomConversion::convert(atom) );
    ::fwData::TransferFunction::TFColor color1 = newTF->getTFColor(0.56);
    ::fwData::TransferFunction::TFColor color2 = newTF->getTFColor(25.89);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,  color1.r, 0.000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.56, color1.g, 0.000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9,  color1.b, 0.000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,  color1.a, 0.000001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8,  color2.r, 0.000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.05, color2.g, 0.000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.99, color2.b, 0.000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2,  color2.a, 0.000001);
}

//-----------------------------------------------------------------------------

void ConversionTest::objectMultiReferencedTest()
{
    ::fwAtoms::Object::sptr atom;
    {
        ::fwData::Composite::sptr composite = ::fwData::Composite::New();
        ::fwData::String::sptr data = ::fwData::String::New();
        composite->getContainer()["key1"] = data;
        composite->getContainer()["key2"] = data;

        // Create Atom
        atom = ::fwAtomConversion::convert( composite );
    }

    // Create Data from Atom
    ::fwData::Composite::sptr newComposite = ::fwData::Composite::dynamicCast( ::fwAtomConversion::convert(atom) );
    ::fwData::Composite::ContainerType & dataMap = newComposite->getContainer();
    CPPUNIT_ASSERT( dataMap.find("key1") != dataMap.end() );
    CPPUNIT_ASSERT( dataMap.find("key2") != dataMap.end() );
    CPPUNIT_ASSERT( dataMap["key1"] );
    CPPUNIT_ASSERT( dataMap["key2"] );
    CPPUNIT_ASSERT( dataMap["key2"] == dataMap["key1"] );
}

//-----------------------------------------------------------------------------

void ConversionTest::recursiveObjectTest()
{
    ::fwAtoms::Object::sptr atom;
    ::fwTools::UUID::UUIDType compositeID;
    {
        ::fwData::Composite::sptr composite = ::fwData::Composite::New();
        compositeID = ::fwTools::UUID::get( composite );
        composite->getContainer()["key"] = composite;
        // Create Atom
        atom = ::fwAtomConversion::convert( composite );

        // Hack, remove composite to destroy composite
        composite->getContainer().erase("key");
    }

    CPPUNIT_ASSERT( ! ::fwTools::UUID::exist( compositeID ) );

    // Create Data from Atom
    ::fwData::Composite::sptr newComposite = ::fwData::Composite::dynamicCast( ::fwAtomConversion::convert(atom) );
    ::fwData::Composite::ContainerType & dataMap = newComposite->getContainer();
    CPPUNIT_ASSERT( dataMap.find("key") != dataMap.end() );
    CPPUNIT_ASSERT( newComposite == dataMap["key"] );
}

//-----------------------------------------------------------------------------

void ConversionTest::dataFactoryNotFoundExceptionTest()
{
    ::fwAtoms::Object::sptr atom;
    {
        ::fwData::Composite::sptr composite = ::fwData::Composite::New();
        ::fwData::String::sptr data = ::fwData::String::New();
        composite->getContainer()["key"] = data;

        // Create Atom
        atom = ::fwAtomConversion::convert( composite );
    }

    {
        ::fwAtoms::Map::sptr map = ::fwAtoms::Map::dynamicCast( atom->getAttribute("values") );
        ::fwAtoms::Object::sptr obj = ::fwAtoms::Object::dynamicCast( (*map)["key"] );
        obj->eraseMetaInfo( ::fwAtomConversion::DataVisitor::CLASSNAME_METAINFO );
        obj->setMetaInfo( ::fwAtomConversion::DataVisitor::CLASSNAME_METAINFO, "CHANGE::CLASNAME" );
    }

    CPPUNIT_ASSERT_THROW( ::fwAtomConversion::convert(atom),
                          ::fwAtomConversion::exception::DataFactoryNotFound );

}

//-----------------------------------------------------------------------------

void ConversionTest::uuidExceptionTest()
{
    // Create data
    ::fwData::Composite::sptr composite = ::fwData::Composite::New();
    ::fwData::String::sptr data = ::fwData::String::New();
    composite->getContainer()["key"] = data;

    // Create Atom
    ::fwAtoms::Object::sptr atom = ::fwAtomConversion::convert( composite );

    CPPUNIT_ASSERT_THROW( ::fwAtomConversion::convert(atom),
            ::fwAtomConversion::exception::DuplicatedDataUUID );
}

//-----------------------------------------------------------------------------

class ClassNotCamped : public ::fwData::Object
{

public :

    fwCoreClassDefinitionsWithNFactoriesMacro( (ClassNotCamped)(::fwData::Object),
       ((::fwData::factory::New< ClassNotCamped > ,() )) );

    ClassNotCamped(::fwData::Object::Key key){};
    void deepCopy(const Object::csptr &_source ){};

};


void ConversionTest::campFactoryNotFoundExceptionTest()
{
    ClassNotCamped::sptr obj = ClassNotCamped::New();
    CPPUNIT_ASSERT(obj);
    CPPUNIT_ASSERT_THROW( ::fwAtomConversion::convert( obj ), ::camp::ClassNotFound );
}

//-----------------------------------------------------------------------------

void ConversionTest::nullPtrManagmentTest()
{
    // null shared ptr attribute managed
    {
        ::fwAtoms::Object::sptr atom;
        {
            ::fwData::Mesh::sptr mesh = ::fwData::Mesh::New();
            CPPUNIT_ASSERT( ! mesh->getPointColorsArray() );

            // Create Atom
            atom = ::fwAtomConversion::convert( mesh );
        }

        // null ptr must be also in atom
        CPPUNIT_ASSERT( atom->getAttributes().find("point_colors") != atom->getAttributes().end() );

        ::fwData::Mesh::sptr newMesh = ::fwData::Mesh::dynamicCast( ::fwAtomConversion::convert(atom) );
        CPPUNIT_ASSERT( newMesh );
        CPPUNIT_ASSERT( ! newMesh->getPointColorsArray() );
    }

    // null shared ptr in map is managed
    {
        ::fwAtoms::Object::sptr atom;
        {
            ::fwData::Composite::sptr composite = ::fwData::Composite::New();
            composite->getContainer()["key1"] = ::fwData::String::New();
            composite->getContainer()["key2"] = ::fwData::Object::sptr();

            // Create Atom
            atom = ::fwAtomConversion::convert( composite );
        }

        {
            ::fwAtoms::Map::sptr map = ::fwAtoms::Map::dynamicCast( atom->getAttribute("values") );
            CPPUNIT_ASSERT_EQUAL( (size_t)2, map->size() );
            CPPUNIT_ASSERT( map->find("key2") != map->end() );
            CPPUNIT_ASSERT( ! (*map)["key2"] );
        }

        ::fwData::Composite::sptr newComposite = ::fwData::Composite::dynamicCast( ::fwAtomConversion::convert(atom) );
        ::fwData::Composite::ContainerType & dataMap = newComposite->getContainer();
        CPPUNIT_ASSERT( newComposite );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, dataMap.size() );
        CPPUNIT_ASSERT( dataMap["key1"] );
        CPPUNIT_ASSERT( dataMap.find("key2") != dataMap.end() );
        CPPUNIT_ASSERT( ! dataMap["key2"] );
    }

    // null shared ptr in vector is managed
    {
        ::fwAtoms::Object::sptr atom;
        {
            ::fwData::Vector::sptr vector = ::fwData::Vector::New();
            vector->getContainer().push_back( ::fwData::String::New() );
            vector->getContainer().push_back( ::fwData::Object::sptr() );

            // Create Atom
            atom = ::fwAtomConversion::convert( vector );
        }

        {
            ::fwAtoms::Sequence::sptr seq = ::fwAtoms::Sequence::dynamicCast( atom->getAttribute("values") );
            CPPUNIT_ASSERT_EQUAL( (size_t)2, seq->size() );
            CPPUNIT_ASSERT( ! (*seq)[1] );
        }

        ::fwData::Vector::sptr newVector = ::fwData::Vector::dynamicCast( ::fwAtomConversion::convert(atom) );
        ::fwData::Vector::ContainerType & dataVec = newVector->getContainer();
        CPPUNIT_ASSERT( newVector );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, dataVec.size() );
        CPPUNIT_ASSERT( dataVec[0] );
        CPPUNIT_ASSERT( ! dataVec[1] );
    }
}


}  // namespace ut
}  // namespace fwAtomConversion

//-----------------------------------------------------------------------------

#define EMTPY_CLASS_API
fwCampAutoDeclareDataMacro((fwAtomConversion)(ut)(ClassNotManaged),EMTPY_CLASS_API);

namespace fwAtomConversion {
namespace ut {

class ClassNotManaged : public ::fwData::Object
{

public :

    fwCoreClassDefinitionsWithNFactoriesMacro( (ClassNotManaged)(::fwData::Object),
       ((::fwData::factory::New< ClassNotManaged > ,() )) );

    fwCampMakeFriendDataMacro((fwAtomConversion)(ut)(ClassNotManaged));

    ClassNotManaged(::fwData::Object::Key key)
    {
        m_values.insert( std::make_pair( ::fwData::String::New(), 0.2 ) );
    };

    void deepCopy(const Object::csptr &_source ){};

    std::map< ::fwData::String::sptr, double > m_values;

};

}  // namespace ut
}  // namespace fwAtomConversion


fwCampImplementDataMacro((fwAtomConversion)(ut)(ClassNotManaged))
{
    builder
        .tag("object_version", "1")
        .tag("lib_name", "fwAtomConversion")
        .base< ::fwData::Object>()
        .property("values", &::fwAtomConversion::ut::ClassNotManaged::m_values);
}

namespace fwAtomConversion {
namespace ut {

void ConversionTest::conversionNotManagedExceptionTest()
{
    // Test ConversionNotManaged throwing during data to atom conversion
    {
        localDeclarefwAtomConversionutClassNotManaged();

        ClassNotManaged::sptr classNotManaged = ClassNotManaged::New();

        CPPUNIT_ASSERT_THROW( ::fwAtomConversion::convert( classNotManaged ),
                              ::fwAtomConversion::exception::ConversionNotManaged );
    }

    // Test ConversionNotManaged throwing during atom to data conversion
    {
        ::fwAtoms::Object::sptr atomObj = ::fwAtoms::Object::New();
        atomObj->setMetaInfo( DataVisitor::CLASSNAME_METAINFO, "::fwData::Vector" );
        atomObj->setMetaInfo( DataVisitor::ID_METAINFO, ::fwTools::UUID::generateUUID() );

        CPPUNIT_ASSERT_THROW( ::fwAtomConversion::convert( atomObj ),
                              ::fwAtomConversion::exception::ConversionNotManaged );
    }

    // Test ConversionNotManaged throwing during atom to data conversion
    {
        ::fwAtoms::Object::sptr atomObj = ::fwAtoms::Object::New();
        atomObj->setMetaInfo( DataVisitor::CLASSNAME_METAINFO, "::fwData::Vector");
        atomObj->setMetaInfo( DataVisitor::ID_METAINFO, ::fwTools::UUID::generateUUID());

        ::fwAtoms::Map::sptr atomFields = ::fwAtoms::Map::New();
        atomObj->setAttribute("fields",atomFields);

        ::fwAtoms::Sequence::sptr atomSeq = ::fwAtoms::Sequence::New();
        atomObj->setAttribute("values",atomSeq);

        ::fwAtoms::Map::sptr atomBasicMap = ::fwAtoms::Map::New();
        atomSeq->push_back( atomBasicMap );

        CPPUNIT_ASSERT_THROW( ::fwAtomConversion::convert( atomObj ),
                              ::fwAtomConversion::exception::ConversionNotManaged );
    }
}

}  // namespace ut
}  // namespace fwAtomConversion


