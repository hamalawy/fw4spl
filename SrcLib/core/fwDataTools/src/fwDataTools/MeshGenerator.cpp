/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include "fwDataTools/MeshGenerator.hpp"

namespace fwDataTools
{
//------------------------------------------------------------------------------

MeshGenerator::MeshGenerator()
{}

//------------------------------------------------------------------------------

MeshGenerator::~MeshGenerator()
{}

//------------------------------------------------------------------------------

void MeshGenerator::generateQuadMesh(::fwData::Mesh::sptr mesh)
{
    size_t nbPointsByEdge = 10;
    float edgeDim = 100.;
    ::fwData::Mesh::PointValueType pt1[3], pt2[3], pt3[3], pt4[3];
    ::fwData::Mesh::Id idx1, idx2, idx3, idx4;
    float step = edgeDim / nbPointsByEdge;

    //Face Y = edgeDim
    for(size_t x=0; x<nbPointsByEdge; x++)
    {
        for(size_t z=0; z<nbPointsByEdge; z++)
        {
            pt1[0] = x*step;
            pt1[1] = edgeDim;
            pt1[2] = z*step;

            pt2[0] = (x+1)*step;
            pt2[1] = edgeDim;
            pt2[2] = (z)*step;

            pt3[0] = x*step;
            pt3[1] = edgeDim;
            pt3[2] = (z+1)*step;

            pt4[0] = (x+1)*step;
            pt4[1] = edgeDim;
            pt4[2] = (z+1)*step;

            idx1 = this->addPoint(pt1, mesh);
            idx2 = this->addPoint(pt2, mesh);
            idx3 = this->addPoint(pt3, mesh);
            idx4 = this->addPoint(pt4, mesh);

            //create QuadCell (idx1, idx3, idx4, idx2)
            mesh->insertNextCell(idx1, idx3, idx4, idx2);
        }
    }

    //Face X = edgeDim
    for(size_t y=0; y<nbPointsByEdge; y++)
    {
        for(size_t z=0; z<nbPointsByEdge; z++)
        {
            pt1[0] = edgeDim;
            pt1[1] = y*step;
            pt1[2] = z*step;

            pt2[0] = edgeDim;
            pt2[1] = y*step;
            pt2[2] = (z+1)*step;

            pt3[0] = edgeDim;
            pt3[1] = (y+1)*step;
            pt3[2] = z*step;

            pt4[0] = edgeDim;
            pt4[1] = (y+1)*step;
            pt4[2] = (z+1)*step;

            idx1 = this->addPoint(pt1, mesh);
            idx2 = this->addPoint(pt2, mesh);
            idx3 = this->addPoint(pt3, mesh);
            idx4 = this->addPoint(pt4, mesh);

            //create QuadCell (idx1, idx3, idx4, idx2)
            mesh->insertNextCell(idx1, idx3, idx4, idx2);
        }
    }
}

//------------------------------------------------------------------------------

void MeshGenerator::generateTriangleMesh(::fwData::Mesh::sptr mesh)
{
    size_t nbPointsByEdge = 10;
    float edgeDim = 100.;

    ::fwData::Mesh::PointValueType pt1[3], pt2[3], pt3[3], pt4[3];
    ::fwData::Mesh::Id idx1, idx2, idx3, idx4;
    float step = edgeDim / nbPointsByEdge;

    //Face Z = 0
    for(size_t x=0; x<nbPointsByEdge; x++)
    {
        for(size_t y=0; y<nbPointsByEdge; y++)
        {
            pt1[0] = x*step;
            pt1[1] = y*step;
            pt1[2] = 0;

            pt2[0] = (x+1)*step;
            pt2[1] = y*step;
            pt2[2] = 0;

            pt3[0] = x*step;
            pt3[1] = (y+1)*step;
            pt3[2] = 0;

            pt4[0] = (x+1)*step;
            pt4[1] = (y+1)*step;
            pt4[2] = 0;

            idx1 = this->addPoint(pt1, mesh);
            idx2 = this->addPoint(pt2, mesh);
            idx3 = this->addPoint(pt3, mesh);
            idx4 = this->addPoint(pt4, mesh);

            //create TrianCell (idx1, idx4, idx2)
            mesh->insertNextCell(idx1, idx4, idx2);
            //create TrianCell (idx1, idx3, idx4)
            mesh->insertNextCell(idx1, idx3, idx4);
        }
    }

    //Face X = 0
    for(size_t y=0; y<nbPointsByEdge; y++)
    {
        for(size_t z=0; z<nbPointsByEdge; z++)
        {
            pt1[0] = 0;
            pt1[1] = y*step;
            pt1[2] = z*step;

            pt2[0] = 0;
            pt2[1] = y*step;
            pt2[2] = (z+1)*step;

            pt3[0] = 0;
            pt3[1] = (y+1)*step;
            pt3[2] = z*step;

            pt4[0] = 0;
            pt4[1] = (y+1)*step;
            pt4[2] = (z+1)*step;

            idx1 = this->addPoint(pt1, mesh);
            idx2 = this->addPoint(pt2, mesh);
            idx3 = this->addPoint(pt3, mesh);
            idx4 = this->addPoint(pt4, mesh);

            //create TrianCell (idx1, idx4, idx2)
            mesh->insertNextCell(idx1, idx4, idx2);
            //create TrianCell (idx1, idx3, idx4)
            mesh->insertNextCell(idx1, idx3, idx4);
        }
    }
}

//------------------------------------------------------------------------------

void MeshGenerator::toTriangularMesh(::fwData::Mesh::sptr mesh, ::fwData::TriangularMesh::sptr trian)
{
    FW_RAISE_IF("Can't convert this Mesh to TriangularMesh", !MeshGenerator::hasUniqueCellType(mesh, ::fwData::Mesh::TRIANGLE));

    // Clear the container cells and set its capacity to 0
    trian->clearCells();
    // Clear the container points and set its capacity to 0
    trian->clearPoints();

    size_t numberOfPoints = mesh->getNumberOfPoints();
    std::vector<float> vPoint(3, 0.0);
    trian->points().resize(numberOfPoints, vPoint);

    ::fwData::Mesh::PointMultiArrayType points = mesh->getPoints();
    typedef ::fwData::Mesh::PointMultiArrayType::index PointTypesIndex;
    for (PointTypesIndex i = 0; i != numberOfPoints; ++i)
    {
        ::fwData::TriangularMesh::PointContainer::value_type &point = trian->points()[i];
        for (PointTypesIndex j = 0; j != 3; ++j)
        {
            point[j] = points[i][j];
        }
    }

    size_t numberOfCells = mesh->getNumberOfCells();
    std::vector<int> vCell(3, 0);
    trian->cells().resize(numberOfCells, vCell);

    ::fwData::Mesh::CellDataMultiArrayType cells = mesh->getCellData();
    typedef ::fwData::Mesh::CellDataMultiArrayType::index CellTypesIndex;
    size_t cellsSize = numberOfCells*3;
    SLM_ASSERT("Wrong CellDataMultiArray size", cells.size() == cellsSize);
    for (CellTypesIndex i = 0; i < cellsSize; i+=3)
    {
        ::fwData::TriangularMesh::CellContainer::value_type &vCells = trian->cells()[i/3];
        vCells[0] = cells[i];
        vCells[1] = cells[i+1];
        vCells[2] = cells[i+2];
    }
}

//------------------------------------------------------------------------------

void MeshGenerator::fromTriangularMesh(::fwData::TriangularMesh::sptr trian, ::fwData::Mesh::sptr mesh)
{
    ::fwData::TriangularMesh::PointContainer& vPoints = trian->points();
    ::fwData::TriangularMesh::CellContainer& vCells = trian->cells();

    mesh->allocate(vPoints.size(), vCells.size(), vCells.size()*3);

    ::fwData::Array::sptr pointsArray = mesh->getPointsArray();
    ::fwData::Array::sptr cellsArray = mesh->getCellDataArray();
    ::fwData::Array::sptr cellDataOffsetArray = mesh->getCellDataOffsetsArray();
    ::fwData::Array::sptr cellTypeArray = mesh->getCellTypesArray();

    SLM_ASSERT("Wrong CellTypeArray dim", cellTypeArray->getSize().size()==1);
    SLM_ASSERT("Wrong CellTypeArray size", cellTypeArray->getSize()[0]==vCells.size());
    SLM_ASSERT("Wrong pointsArray dim", pointsArray->getSize().size()==1);
    SLM_ASSERT("Wrong pointsArray size", pointsArray->getSize()[0]==vPoints.size());
    SLM_ASSERT("Wrong cellsArray dim", cellsArray->getSize().size()==1);
    SLM_ASSERT("Wrong cellsArray size", cellsArray->getSize()[0]==vCells.size()*3);
    SLM_ASSERT("Wrong cellDataOffsetArray dim", cellDataOffsetArray->getSize().size()==1);
    SLM_ASSERT("Wrong cellDataOffsetArray size", cellDataOffsetArray->getSize()[0]==vCells.size());

    //Initialized pointsArray
    for(size_t i=0; i<vPoints.size(); i++)
    {
        mesh->insertNextPoint(vPoints[i][0], vPoints[i][1], vPoints[i][2]);
    }

    //Initialized cellsArray
    ::fwData::Mesh::CellValueType* cells = cellsArray->begin< ::fwData::Mesh::CellValueType >();
    for(size_t i=0; i<vCells.size(); i++)
    {
        mesh->insertNextCell(vCells[i][0], vCells[i][1], vCells[i][2]);
    }
}

//------------------------------------------------------------------------------

bool MeshGenerator::hasUniqueCellType(::fwData::Mesh::sptr mesh, ::fwData::Mesh::CellTypes cell)
{
    bool res = true;
    ::fwData::Mesh::CellTypesMultiArrayType cellTypes = mesh->getCellTypes();

    BOOST_FOREACH(::fwData::Mesh::CellTypes type, cellTypes)
    {
        if(type != cell)
        {
            return false;
        }
    }
    return res;
}

//------------------------------------------------------------------------------

::fwData::Mesh::Id MeshGenerator::addPoint(::fwData::Mesh::PointValueType* pt, ::fwData::Mesh::sptr mesh)
{
    Point myPoint(pt[0], pt[1], pt[2]);

    PointsMapType::iterator it = m_points.find(myPoint);
    if(it != m_points.end())
    {
        return it->second;
    }
    ::fwData::Mesh::Id idx = mesh->insertNextPoint(pt[0], pt[1], pt[2]);
    m_points[myPoint] = idx;
    return idx;
}

//------------------------------------------------------------------------------


} // namespace fwDataTools
