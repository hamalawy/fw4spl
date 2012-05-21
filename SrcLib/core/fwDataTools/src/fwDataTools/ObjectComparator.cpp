/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <fwData/Color.hpp>
#include <fwData/StructureTraits.hpp>

#include <fwDataTools/MeshGenerator.hpp>
#include <fwDataTools/Image.hpp>
#include <fwDataTools/Patient.hpp>
#include <fwData/ProcessObject.hpp>
#include <fwData/ResectionDB.hpp>
#include <fwData/Dictionary.hpp>

#include "fwDataTools/Image.hpp"
#include "fwDataTools/ObjectComparator.hpp"

namespace fwDataTools
{

bool ObjectComparator::compareStructureTraitsDictionary(::fwData::StructureTraitsDictionary::sptr structureDico1, ::fwData::StructureTraitsDictionary::sptr structureDico2)
{
    SLM_ASSERT( "Null structureDico1 pointers", structureDico1);
    SLM_ASSERT( "Null structureDico2 pointers", structureDico2);

    bool compare = true;
    std::vector<std::string> structureTypes1 = structureDico1->getStructureTypeNames();
    std::vector<std::string> structureTypes2 = structureDico2->getStructureTypeNames();

    if(structureTypes1.size()== structureTypes2.size())
    {
        if(std::equal(structureTypes1.begin(), structureTypes1.end(), structureTypes2.begin()))
        {
            BOOST_FOREACH(std::string type, structureTypes1)
            {
                ::fwData::StructureTraits::sptr structure1 = structureDico1->getStructure(type);
                ::fwData::StructureTraits::sptr structure2 = structureDico2->getStructure(type);
                compare &= ObjectComparator::compareStructureTraits(structure1, structure2);
            }
        }
        else
        {
            compare &=false;
            OSLM_ERROR( "Structure type names are different" );
        }
    }
    else
    {
        compare &=false;
        OSLM_ERROR( "Structure type size are different " << structureTypes1.size() << " != " << structureTypes2.size() );
    }
    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareStructureTraits(::fwData::StructureTraits::sptr structure1, ::fwData::StructureTraits::sptr structure2)
{
    // check structure
    SLM_ASSERT( "Null structure1 pointers", structure1);
    SLM_ASSERT( "Null structure2 pointers", structure2);
    bool compare = true;

    compare &= structure1->getType() == structure2->getType();
    OSLM_ERROR_IF( "The structure type are different. ", structure1->getType() != structure2->getType());

    compare &= structure1->getClass() == structure2->getClass();
    OSLM_ERROR_IF( "The structure type are different. ", structure1->getClass() != structure2->getClass());
    compare &= structure1->getNativeExp() == structure2->getNativeExp();
    OSLM_ERROR_IF( "The native expression are different. ", structure1->getNativeExp() != structure2->getNativeExp());
    compare &= structure1->getNativeGeometricExp() == structure2->getNativeGeometricExp();
    OSLM_ERROR_IF( "The native geometric expression are different. ", structure1->getNativeGeometricExp() != structure2->getNativeGeometricExp());
    compare &= structure1->getAttachmentType() == structure2->getAttachmentType();
    OSLM_ERROR_IF( "The attachment type are different. ", structure1->getAttachmentType() != structure2->getAttachmentType());


    ::fwData::Color::sptr color1 = structure1->getColor();
    ::fwData::Color::sptr color2 = structure2->getColor();
    compare &= Patient::compareColor( color1, color2, 1/255.f, "Color : ");

    ::fwData::StructureTraits::CategoryContainer categories1 = structure1->getCategories();
    ::fwData::StructureTraits::CategoryContainer categories2 = structure2->getCategories();
    compare &= (categories1.size() == categories1.size());
    OSLM_ERROR_IF( "The categories size are different. ", categories1.size() != categories1.size());

    if(!std::equal(categories1.begin(), categories1.end(), categories2.begin()))
    {
        compare &= false;
    }
    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareROITraits(::fwData::ROITraits::sptr roi1, ::fwData::ROITraits::sptr roi2)
{
    SLM_ASSERT( "Null roi1 pointers", roi1);
    SLM_ASSERT( "Null roi2 pointers", roi2);

    bool compare = true;

    compare &=  (roi1->getIdentifier() == roi2->getIdentifier());
    OSLM_ERROR_IF("Node object are different.", roi1->getIdentifier() != roi2->getIdentifier());

    compare &=  (roi1->getEvaluatedExp() == roi2->getEvaluatedExp());
    OSLM_ERROR_IF("Node object are different.", roi1->getEvaluatedExp() != roi2->getEvaluatedExp());
    ObjectComparator::compareStructureTraits(roi1->getStructureTraits(), roi2->getStructureTraits());

    compare &=  ((roi1->getMaskOpNode() && roi2->getMaskOpNode()) || (!roi1->getMaskOpNode() && !roi2->getMaskOpNode()));
    if (roi1->getMaskOpNode() && roi2->getMaskOpNode())
    {
        compare &= ::fwDataTools::ObjectComparator::compareNode(roi1->getMaskOpNode(), roi2->getMaskOpNode());
    }
    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareReconstructionTraits(::fwData::ReconstructionTraits::sptr rec1, ::fwData::ReconstructionTraits::sptr rec2)
{
    SLM_ASSERT( "Null rec1 pointers",rec1);
    SLM_ASSERT( "Null rec2 pointers", rec2);

    bool compare = true;

    compare &= (rec1->getIdentifier() == rec2->getIdentifier());
    OSLM_ERROR_IF("Node object are different.",rec1->getIdentifier() != rec2->getIdentifier());

    compare &= ObjectComparator::compareStructureTraits(rec1->getStructureTraits(), rec2->getStructureTraits());

    bool result = ((rec1->getMaskOpNode() && rec2->getMaskOpNode()) || (!rec1->getMaskOpNode() && !rec2->getMaskOpNode()));
    compare &= result;
    OSLM_ERROR_IF("Node object are different.", !result);

    if (rec1->getMaskOpNode() && rec2->getMaskOpNode())
    {
        compare &= ::fwDataTools::ObjectComparator::compareNode(rec1->getMaskOpNode(), rec2->getMaskOpNode());
    }

    bool result2 = ((rec1->getMeshOpNode() && rec2->getMeshOpNode()) || (!rec1->getMeshOpNode() && !rec2->getMeshOpNode()));
    compare &= result2;
    OSLM_ERROR_IF("Node object are different.", !result2);

    if (rec1->getMeshOpNode() && rec2->getMeshOpNode())
    {
        compare &= ::fwDataTools::ObjectComparator::compareNode(rec1->getMeshOpNode(), rec2->getMeshOpNode());
    }
    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareNode(::fwData::Node::sptr node1, ::fwData::Node::sptr node2)
{
    SLM_ASSERT( "Null node1 pointers", node1);
    SLM_ASSERT( "Null node2 pointers", node2);

    bool compare = true;

    compare &= (node1->getObject() == node2->getObject());
    OSLM_ERROR_IF("Node object are different.",node1->getObject() != node2->getObject());

    ::fwData::Node::PortContainer inputs1 = node1->getInputPorts();
    ::fwData::Node::PortContainer inputs2 = node2->getInputPorts();

    compare &=  (inputs1.size() == inputs2.size());
    OSLM_ERROR_IF("Number of inputs are different.",inputs1.size() != inputs2.size());

    ::fwData::Node::PortContainer::iterator inputsIter1 = inputs1.begin();
    ::fwData::Node::PortContainer::iterator inputsIter2 = inputs2.begin();
    for (;inputsIter1 != inputs1.end(); ++inputsIter1, ++inputsIter2)
    {
        compare &=  ::fwDataTools::ObjectComparator::comparePort(*inputsIter1, *inputsIter2);
    }

    ::fwData::Node::PortContainer outputs1 = node1->getOutputPorts();
    ::fwData::Node::PortContainer outputs2 = node2->getOutputPorts();

    compare &= (outputs1.size() == outputs2.size());
    OSLM_ERROR_IF("Number of outputs are different.",outputs1.size() != outputs2.size());

    ::fwData::Node::PortContainer::iterator outputsIter1 = outputs1.begin();
    ::fwData::Node::PortContainer::iterator outputsIter2 = outputs2.begin();
    for (;outputsIter1 != outputs1.end(); ++outputsIter1, ++outputsIter2)
    {
        compare &=  ::fwDataTools::ObjectComparator::comparePort(*outputsIter1, *outputsIter2);
    }
    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::comparePort(::fwData::Port::sptr port1, ::fwData::Port::sptr port2)
{
    SLM_ASSERT( "Null port1 pointers", port1);
    SLM_ASSERT( "Null port2 pointers", port2);

    bool compare = true;

    compare &= (port1->getIdentifier() == port2->getIdentifier());
    OSLM_ERROR_IF("Port identifier are different.",port1->getIdentifier() != port2->getIdentifier());

    compare &= (port1->getType() ==  port2->getType());
    OSLM_ERROR_IF("Port type are different.",port1->getType() !=  port2->getType());

    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::comparePatientDB(::fwData::PatientDB::sptr patientDB1, ::fwData::PatientDB::sptr patientDB2)
{
    // check patientDB
    SLM_ASSERT( "Null patientDB1 pointers", patientDB1);
    SLM_ASSERT( "Null patientDB2 pointers",patientDB2);

    bool compare = true;
    compare &= patientDB1->getNumberOfPatients() == patientDB2->getNumberOfPatients();
    OSLM_ERROR_IF( "The number of patient are different. ", patientDB1->getNumberOfPatients() != patientDB2->getNumberOfPatients());

    ::fwData::PatientDB::PatientContainerType patientDB1List = patientDB1->getPatients();
    ::fwData::PatientDB::PatientContainerType::const_iterator iter1 = patientDB1List.begin();
    ::fwData::PatientDB::PatientContainerType::const_iterator iter2 = patientDB2->getPatients().begin();
    while( iter1 != patientDB1List.end ())
    {
        compare &= ::fwDataTools::Patient::comparePatient(*iter1, *iter2);
        iter1++;
        iter2++;
    }
    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareMaterial(::fwData::Material::sptr mat1, ::fwData::Material::sptr mat2)
{
    SLM_ASSERT( "Null mat1 pointers", mat1);
    SLM_ASSERT( "Null mat2 pointers", mat2);

    bool compare = true;

    compare &= (mat1->getShadingMode() ==  mat2->getShadingMode());
    OSLM_ERROR_IF( "Shading mode are different. ", mat1->getShadingMode() !=  mat2->getShadingMode());

    compare &= (mat1->getRepresentationMode() == mat2->getRepresentationMode());
    OSLM_ERROR_IF( "Representation mode are different. ", mat1->getRepresentationMode() != mat2->getRepresentationMode());

    compare &= (mat1->getOptionsMode() ==  mat2->getOptionsMode());
    OSLM_ERROR_IF( "Option mode are different.", mat1->getOptionsMode() !=  mat2->getOptionsMode());

    compare &= ::fwDataTools::Patient::compareColor(mat1->ambient(), mat2->ambient());
    compare &= ::fwDataTools::Patient::compareColor(mat1->diffuse(), mat2->diffuse());

    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareTransferFunction(::fwData::TransferFunction::sptr tf1, ::fwData::TransferFunction::sptr tf2)
{

    SLM_ASSERT( "Null tf1 pointers", tf1);
    SLM_ASSERT( "Null tf2 pointers", tf2);

    bool compare = true;

    compare &= (tf1->getName() == tf2->getName());
    OSLM_ERROR_IF( "Transfert funtion names are different.", tf1->getName() != tf2->getName());

    compare &= (tf1->getWindow() == tf2->getWindow());
    OSLM_ERROR_IF( "Window values are different.", tf1->getWindow() != tf2->getWindow());

    compare &= (tf1->getLevel() == tf2->getLevel());
    OSLM_ERROR_IF( "Level values are different.", tf1->getLevel() != tf2->getLevel());

    compare &= (tf1->getIsClamped() == tf2->getIsClamped());
    OSLM_ERROR_IF( "IsClamped values are different.", tf1->getIsClamped() != tf2->getIsClamped());

    compare &= (tf1->getInterpolationMode() == tf2->getInterpolationMode());
    OSLM_ERROR_IF( "InterpolationMode values are different.", tf1->getInterpolationMode() != tf2->getInterpolationMode());

    compare &= (tf1->getBackgroundColor() == tf2->getBackgroundColor());
    OSLM_ERROR_IF( "BackgroundColor values are different.", !(tf1->getBackgroundColor() == tf2->getBackgroundColor()) );

    compare &= (tf1->getTFData().size() == tf2->getTFData().size());
    OSLM_ERROR_IF( "Number of points in each TF are different.", tf1->getTFData().size() != tf2->getTFData().size());

    if( tf1->getTFData().size() > 0 && tf2->getTFData().size() > 0 )
    {
        compare &= (tf1->getMinMaxTFValues() == tf2->getMinMaxTFValues());
        OSLM_ERROR_IF( "Min max TF values are different.", tf1->getMinMaxTFValues() != tf2->getMinMaxTFValues());


        ::fwData::TransferFunction::TFDataType::const_iterator it1 = tf1->getTFData().begin();
        ::fwData::TransferFunction::TFDataType::const_iterator it2 = tf2->getTFData().begin();
        while ( it1 != tf1->getTFData().end() )
        {
            compare &= ( it1->first == it2->first );
            OSLM_ERROR_IF( "TFData key values are different.", it1->first != it2->first );

            compare &= ( it1->second == it2->second );
            OSLM_ERROR_IF( "TFData color values are different.", !(it1->second == it2->second) );

            it1++;
            it2++;
        }
    }

    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareMesh(::fwData::Mesh::sptr mesh1, ::fwData::Mesh::sptr mesh2)
{
    SLM_ASSERT( "Null mesh1 pointers", mesh1);
    SLM_ASSERT( "Null mesh2 pointers", mesh2);

    bool compare = true;
    compare &= (mesh1->getNumberOfPoints()== mesh2->getNumberOfPoints());
    OSLM_ERROR_IF( "Number of points are different.", mesh1->getNumberOfPoints()!= mesh2->getNumberOfPoints());

    compare &= (mesh1->getNumberOfCells() == mesh2->getNumberOfCells());
    OSLM_ERROR_IF( "Number of cells are different.", mesh1->getNumberOfCells() != mesh2->getNumberOfCells());

    compare &= (mesh1->getCellDataSize() == mesh2->getCellDataSize());
    OSLM_ERROR_IF( "Number of cell data are different.", mesh1->getCellDataSize() != mesh2->getCellDataSize());

    compare &= Image::compareArray(mesh1->getPointsArray(), mesh2->getPointsArray());
    compare &= Image::compareArray(mesh1->getCellTypesArray(), mesh2->getCellTypesArray());
    compare &= Image::compareArray(mesh1->getCellDataOffsetsArray(), mesh2->getCellDataOffsetsArray());
    compare &= Image::compareArray(mesh1->getCellDataArray(), mesh2->getCellDataArray());
    compare &= Image::compareArray(mesh1->getPointColorsArray(), mesh2->getPointColorsArray());
    compare &= Image::compareArray(mesh1->getCellColorsArray(), mesh2->getCellColorsArray());
    compare &= Image::compareArray(mesh1->getPointNormalsArray(), mesh2->getPointNormalsArray());
    compare &= Image::compareArray(mesh1->getCellNormalsArray(), mesh2->getCellNormalsArray());

    compare &= ObjectComparator::compareDataArrayMesh(mesh1, mesh2);
    return compare;
}

//------------------------------------------------------------------------------

bool  ObjectComparator::compareDataArrayMesh(::fwData::Mesh::sptr mesh1, ::fwData::Mesh::sptr mesh2)
{
    SLM_ASSERT( "Null mesh1 pointers", mesh1);
    SLM_ASSERT( "Null mesh2 pointers", mesh2);

    bool compare = true;
    compare &= mesh1->getDataArrayNames() == mesh2->getDataArrayNames();
    OSLM_ERROR_IF( "Mesh data array name are different.", mesh1->getDataArrayNames() != mesh2->getDataArrayNames());

    std::vector<std::string> vectNames = mesh1->getDataArrayNames();
    BOOST_FOREACH(std::string name, vectNames)
    {
        ::fwData::Array::sptr array1 = mesh1->getDataArray(name);
        ::fwData::Array::sptr array2 = mesh2->getDataArray(name);
        compare &= Image::compareArray(array1, array2);
    }
    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareDictionary(::fwData::Dictionary::sptr dico1, ::fwData::Dictionary::sptr dico2)
{
    SLM_ASSERT( "Null dico1 pointers", dico1);
    SLM_ASSERT( "Null dico2 pointers", dico2);

    bool compare = true;

    compare &= (dico1->getDictionaryOrgans().size() ==  dico2->getDictionaryOrgans().size());
    OSLM_ERROR_IF( "Number of dictionary are different.",
                   dico1->getDictionaryOrgans().size() !=  dico2->getDictionaryOrgans().size());

    for(::fwData::Dictionary::DictionaryOrganContainerType::const_iterator itr = dico1->getDictionaryOrgans().begin();
        itr != dico1->getDictionaryOrgans().end(); ++itr)
    {
        compare &= (dico2->hasDictionaryOrgan(itr->first) == true);

        OSLM_ERROR_IF( "dictionary hasn't the same name.",
                       dico2->hasDictionaryOrgan(itr->first) != true);
        ::fwData::DictionaryOrgan::sptr dico1Organ = itr->second;
        ::fwData::DictionaryOrgan::sptr dico2Organ = dico2->getDictionaryOrgan(itr->first);
    }
    return compare ;

}

//------------------------------------------------------------------------------
bool ObjectComparator::compareDictionaryOrgan(::fwData::DictionaryOrgan::sptr dicoOrgan1, ::fwData::DictionaryOrgan::sptr dicoOrgan2)
{
    SLM_ASSERT( "Null dicoOrgan1 pointers", dicoOrgan1);
    SLM_ASSERT( "Null dicoOrgan2 pointers", dicoOrgan2);

     bool compare = true;
     compare &= (dicoOrgan1->getCRefStructureType() == dicoOrgan2->getCRefStructureType());
     OSLM_ERROR_IF("Attribute Structuretype are different.",(dicoOrgan1->getCRefStructureType() != dicoOrgan2->getCRefStructureType()));
     compare &= (dicoOrgan1->getSALabel() == dicoOrgan2->getSALabel());
     OSLM_ERROR_IF("Attribute SALLabel are different.", dicoOrgan1->getSALabel() != dicoOrgan2->getSALabel());
     compare &= (dicoOrgan1->getIsMedicalStructure() == dicoOrgan2->getIsMedicalStructure());
     OSLM_ERROR_IF("Attribute IsMedicalStructure are different.", dicoOrgan1->getIsMedicalStructure() != dicoOrgan2->getIsMedicalStructure());
     compare &= (dicoOrgan1->getAOColor() == dicoOrgan2->getAOColor());
     OSLM_ERROR_IF("Attribute AOColor are different.", dicoOrgan1->getAOColor() != dicoOrgan2->getAOColor());
     compare &= (dicoOrgan1->getAvgVolume() == dicoOrgan2->getAvgVolume());
     OSLM_ERROR_IF("Attribute AvgVolume are different.", dicoOrgan1->getAvgVolume() != dicoOrgan2->getAvgVolume());
     compare &= (dicoOrgan1->getVolStdDeviation() == dicoOrgan2->getVolStdDeviation());
     OSLM_ERROR_IF("Attribute VolStdDeviation are different.", dicoOrgan1->getVolStdDeviation() != dicoOrgan2->getVolStdDeviation());
     compare &= (dicoOrgan1->getNbExams() == dicoOrgan2->getNbExams());
     OSLM_ERROR_IF("Attribute NbExams are different.", dicoOrgan1->getNbExams() != dicoOrgan2->getNbExams());
     compare &= (dicoOrgan1->getCRefPosition() == dicoOrgan2->getCRefPosition());
     OSLM_ERROR_IF("Attribute position are different.", dicoOrgan1->getCRefPosition() != dicoOrgan2->getCRefPosition());
     compare &= (dicoOrgan1->getCRefIconPath() == dicoOrgan2->getCRefIconPath());
     OSLM_ERROR_IF("Attribute IconPath are different.", dicoOrgan1->getCRefIconPath() != dicoOrgan2->getCRefIconPath());
     compare &= (dicoOrgan1->getRed() == dicoOrgan2->getRed());
     OSLM_ERROR_IF("Attribute red are different.", dicoOrgan1->getRed() != dicoOrgan2->getRed());
     compare &= (dicoOrgan1->getGreen() == dicoOrgan2->getGreen());
     OSLM_ERROR_IF("Attribute green are different.", dicoOrgan1->getGreen() != dicoOrgan2->getGreen());
     compare &= (dicoOrgan1->getBlue() == dicoOrgan2->getBlue());
     OSLM_ERROR_IF("Attribute blue are different.", dicoOrgan1->getBlue() != dicoOrgan2->getBlue());
     compare &= (dicoOrgan1->getAlpha() == dicoOrgan2->getAlpha());
     OSLM_ERROR_IF("Attribute alpha are different.", dicoOrgan1->getAlpha() != dicoOrgan2->getAlpha());
     compare &= (dicoOrgan1->getCRefTexturePath() ==  dicoOrgan2->getCRefTexturePath());
     OSLM_ERROR_IF("Attribute TexturePath are different.", dicoOrgan1->getCRefTexturePath() !=  dicoOrgan2->getCRefTexturePath());
     compare &= (dicoOrgan1->getAvgTriangleNb() == dicoOrgan2->getAvgTriangleNb());
     OSLM_ERROR_IF("Attribute AvgTrianleNb are different.", dicoOrgan1->getAvgTriangleNb() != dicoOrgan2->getAvgTriangleNb());
     compare &= (dicoOrgan1->getSegMin() == dicoOrgan2->getSegMin());
     OSLM_ERROR_IF("Attribute SegMin are different.", dicoOrgan1->getSegMin() != dicoOrgan2->getSegMin());
     compare &= (dicoOrgan1->getSegMax() == dicoOrgan2->getSegMax() );
     OSLM_ERROR_IF("Attribute SegMax are different.", dicoOrgan1->getSegMax() != dicoOrgan2->getSegMax());
     compare &= (dicoOrgan1->getCRefModality() == dicoOrgan2->getCRefModality());
     OSLM_ERROR_IF("Attribute Modality are different.", dicoOrgan1->getCRefModality() != dicoOrgan2->getCRefModality());
     compare &= (dicoOrgan1->getSurfaceElasticity() == dicoOrgan2->getSurfaceElasticity());
     OSLM_ERROR_IF("Attribute surface elasticity are different.", dicoOrgan1->getSurfaceElasticity() != dicoOrgan2->getSurfaceElasticity());
     compare &= (dicoOrgan1->getSurfaceRupture() == dicoOrgan2->getSurfaceRupture());
     OSLM_ERROR_IF("Attribute surface rupture are different.", dicoOrgan1->getSurfaceRupture() != dicoOrgan2->getSurfaceRupture());
     compare &= (dicoOrgan1->getInteriorRupture() == dicoOrgan2->getInteriorRupture());
     OSLM_ERROR_IF("Attribute InteriorRupture are different.", dicoOrgan1->getInteriorRupture() != dicoOrgan2->getInteriorRupture());
     compare &= (dicoOrgan1->getCRefComment() == dicoOrgan2->getCRefComment());
     OSLM_ERROR_IF("Attribute comment are different.", dicoOrgan1->getCRefComment() != dicoOrgan2->getCRefComment());
     compare &= (dicoOrgan1->getCRefBelongsTo().back() == dicoOrgan2->getCRefBelongsTo().back());
     OSLM_ERROR_IF("Attribute BelongTo  are different.", dicoOrgan1->getCRefBelongsTo().back() != dicoOrgan2->getCRefBelongsTo().back());
     return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareProcessObject(::fwData::ProcessObject::sptr po1, ::fwData::ProcessObject::sptr po2)
{
    SLM_ASSERT( "Null po1 pointers", po1);
    SLM_ASSERT( "Null po2 pointers", po2);

    bool compare = true;
    compare &= ( po1->getInputs().size() == po2->getInputs().size() );
    OSLM_ERROR_IF( "Number of inputs are different.",
                   po1->getInputs().size() != po2->getInputs().size());

    compare &= ( po1->getInputsParamNames().size()== po2->getInputsParamNames().size());
    OSLM_ERROR_IF( "Number of inputs param names are different.",
                   po1->getInputsParamNames().size()!= po2->getInputsParamNames().size());

    compare &= ( po1->getOutputs().size() == po2->getOutputs().size() );
    OSLM_ERROR_IF( "Number of ouputs are different.",
                   po1->getOutputs().size() != po2->getOutputs().size());

    compare &= ( po1->getOutputsParamNames().size()== po2->getOutputsParamNames().size());
    OSLM_ERROR_IF( "Number of OutputParamNames are different.",
                   po1->getOutputsParamNames().size()!= po2->getOutputsParamNames().size());

    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::comparePoint(::fwData::Point::sptr point1, ::fwData::Point::sptr point2)
{
    SLM_ASSERT( "Null point1 pointers", point1);
    SLM_ASSERT( "Null point2 pointers", point2);

    bool compare = true;
    fwVec3d coordPt1 = point1->getCoord();
    fwVec3d coordPt2 = point2->getCoord();

    compare &= ( coordPt1[0] == coordPt2[0] );
    OSLM_ERROR_IF("X coordinate of point are different.", coordPt1[0] != coordPt2[0]);

    compare &= ( coordPt1[1] == coordPt2[1] );
    OSLM_ERROR_IF("Y coordinate of point are different.", coordPt1[1] != coordPt2[1] );

    compare &= ( coordPt1[2] == coordPt2[2] );
    OSLM_ERROR_IF("Z coordinate of point are different.", coordPt1[2] != coordPt2[2]);

    return compare;
}

//------------------------------------------------------------------------------
bool ObjectComparator::comparePlane(::fwData::Plane::sptr plane1, ::fwData::Plane::sptr plane2)
{
    SLM_ASSERT( "Null plane1 pointers", plane1);
    SLM_ASSERT( "Null plane2 pointers", plane2);

    bool compare = true;

    compare &= ( plane1->getIsIntersection() == plane2->getIsIntersection() );
    OSLM_ERROR_IF("Attribut IsIntersection are different.", plane1->getIsIntersection() != plane2->getIsIntersection());

    compare &= (plane1->getPoints().size() == plane2->getPoints().size());
    OSLM_ERROR_IF("Number of points are different.", plane1->getPoints().size() != plane2->getPoints().size());

    compare &= (plane1->getPlane() == plane2->getPlane());
    OSLM_ERROR_IF("Plane is different.", !(plane1->getPlane() == plane2->getPlane()));

    ::fwData::Plane::PointContainer points = plane1->getPoints();
    for(int i= 0; i < points.size(); ++i)
    {
        compare &= comparePoint(points[i], plane2->getPoints()[i]);
    }
    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::comparePlaneList(::fwData::PlaneList::sptr planeList1, ::fwData::PlaneList::sptr planeList2)
{
    SLM_ASSERT( "Null planeList pointers", planeList1);
    SLM_ASSERT( "Null planeList pointers", planeList2);

    bool compare = true;
    compare &= ( planeList1->getPlanes().size() == planeList2->getPlanes().size() );
    OSLM_ERROR_IF("Number of planes are different.", planeList1->getPlanes().size() != planeList2->getPlanes().size());

    ::fwData::PlaneList::PlaneListContainer planes = planeList1->getPlanes();
    for(int i = 0; i < planes.size(); ++i)
    {
        compare &= comparePlane(planes[i], planeList2->getPlanes()[i]);
    }
    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareResection(::fwData::Resection::sptr resec1, ::fwData::Resection::sptr resec2)
{
    SLM_ASSERT( "Null resec1 pointers", resec1);
    SLM_ASSERT( "Null resec2 pointers", resec2);

    bool compare = true;

    SLM_ASSERT("Null plane list pointers on resec1.", resec1->getPlaneList());
    SLM_ASSERT("Null plane list pointers on resec2.", resec2->getPlaneList())

    compare &= (resec1->getPlaneList()->getPlanes().size() == resec2->getPlaneList()->getPlanes().size());
    OSLM_ERROR_IF( "Number of planet are different.",
                   resec1->getPlaneList()->getPlanes().size() != resec2->getPlaneList()->getPlanes().size());

    compare &= (resec1->getInputs().size() == resec2->getInputs().size());
    OSLM_ERROR_IF( "Number of inputs are different.", resec1->getInputs().size() != resec2->getInputs().size());

    compare &= (resec1->getOutputs().size() == resec2->getOutputs().size());
    OSLM_ERROR_IF( "Number of outputs are different.", resec1->getOutputs().size() != resec2->getOutputs().size());

    compare &= (resec1->getIsSafePart() == resec2->getIsSafePart());
    OSLM_ERROR_IF( "Attribute IsSafePart are different.", resec1->getIsSafePart() != resec2->getIsSafePart());

    compare &= (resec1->getName() == resec2->getName());
    OSLM_ERROR_IF( "Attribute resection name are different.", resec1->getName() != resec2->getName());

    compare &= (resec1->getIsVisible() == resec2->getIsVisible());
    OSLM_ERROR_IF( "Attribute isVisible are different.", resec1->getIsVisible() != resec2->getIsVisible());

    compare &= (resec1->getIsValid() == resec2->getIsValid());
    OSLM_ERROR_IF( "Attribute IsValid are different.", resec1->getIsValid() != resec2->getIsValid());

    return compare;
}

//------------------------------------------------------------------------------

bool ObjectComparator::compareResectionDB(::fwData::ResectionDB::sptr resecDB1, ::fwData::ResectionDB::sptr resecDB2)
{
    SLM_ASSERT("Null resecDB1 pointers", resecDB1);
    SLM_ASSERT("Null resecDB2 pointers", resecDB2);

    bool compare = true;


    compare &= resecDB1->getNumberOfResections() == resecDB2->getNumberOfResections();
    OSLM_ERROR_IF( "Number of resection are different.", resecDB1->getNumberOfResections()  != resecDB2->getNumberOfResections());

    for(int i=0; i < resecDB1->getNumberOfResections(); ++i)
    {
        compare &= compareResection(resecDB1->getResections()[i], resecDB2->getResections()[i] );
    }

    if(resecDB1->getSafeResection())
    {
        compare &= compareResection (resecDB1->getSafeResection(), resecDB2->getSafeResection());
    }

    return compare;
}

} // namespace fwDataTools
