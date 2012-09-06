/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VTKIO_VTK_HPP_
#define _VTKIO_VTK_HPP_

#include <boost/shared_ptr.hpp>

#include <fwData/Image.hpp>
#include <fwData/TriangularMesh.hpp>
#include <fwData/TransformationMatrix3D.hpp>

#include "vtkIO/config.hpp"


// forward declaration
class vtkPolyData;
class vtkImageData;
class vtkImageImport;
class vtkMatrix4x4;
class vtkLookupTable;

/**
 * @brief   Namespace vtkIO.
 * @namespace   vtkIO.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 *  The namespace vtkIO contains reader, writer and helper using the VTK lib for output and input actions between data and the frameWork
 */
namespace vtkIO
{

/*!
 * @brief Convert ::fwData::Image PixelType to the VTK data type of pixels.
 *
 * @param[in] image ::fwData::Image::sptr.
 * @return int : VTK data scalar type
 *
 */
VTKIO_API int getVtkScalarType(::fwData::Image::sptr image);

/**
 * @brief Allocate a new vtkImageData* from a ::fwData::Image::sptr.
 *
 * @param[in] _data ::fwData::Image::sptr.
 * @param[out] dst, the vtk image to fill if provided
 *
 * if dst is provided the new image will be imported into the dst image else
 * allocate a new vtkImage from a ::fwData::Image, vtkImage doesn't manage its buffer (the buffer is not copied)
 *
 */
VTKIO_API void toVTKImage( ::fwData::Image::sptr _data ,  vtkImageData *dst);

/*!
 * @brief Convert a vtkImageData* to a ::fwData::Image::sptr.
 *
 * @param[in] _source vtkImageData*.
 * @param[out] _destination ::fwData::Image::sptr.
 *
 * Throw an exception if the conversion fails
 */
VTKIO_API void fromVTKImage( vtkImageData* _source, ::fwData::Image::sptr _destination );

/*!
 * @brief Configure a vtkImageImport* from a ::fwData::Image::sptr.
 *
 * @param[in] _pDataImage ::fwData::Image::sptr.
 * @param[out] _pImageImport ::vtkImageImport*.
 */
VTKIO_API void configureVTKImageImport( ::vtkImageImport * _pImageImport, ::fwData::Image::sptr _pDataImage );

/*!
 * @brief Convert a ::fwData::TriangularMesh::sptr to a vtkPolyData*.
 *
 * @param[in] _mesh ::fwData::TriangularMesh::sptr.
 * @return vtkPolyData*.
 */
VTKIO_API vtkPolyData*  toVTKMesh( ::fwData::TriangularMesh::sptr _mesh );

/*!
 * @brief Update a vtkPolyData* with ::fwData::TriangularMesh::sptr points
 *
 * @param[out] polyData vtkPolyData*
 * @param[in]  mesh ::fwData::TriangularMesh::sptr
 * @return vtkPolyData*
 *
 * Warning : be carefull with updatePoints : this may change the number of
 * vertex of the polydata, cells will not be updated.
 * Returns the updated vtkPolyPata
 */
VTKIO_API vtkPolyData*  updatePolyDataPoints(vtkPolyData* polyDataDst, ::fwData::TriangularMesh::sptr meshSrc );

/*!
 * @brief Convert a vtkPolyData* to a ::fwData::TriangularMesh::sptr.
 *
 * @param[in] _polyData vtkPolyData*.
 * @param[out] _triangularMesh ::fwData::TriangularMesh::sptr.
 * @return bool.
 *
 * Returns \b true if the conversion is a success and \b false if it fails
 */
VTKIO_API  bool fromVTKMesh( vtkPolyData *_polyData, ::fwData::TriangularMesh::sptr _triangularMesh );


/*
 * @brief compute the volume of the mesh using MassProperties vtk class
 */
VTKIO_API  double computeVolume(  ::fwData::TriangularMesh::sptr _triangularMesh );

/*
* @brief compute the volume of the mesh using ImageStencil
*/
VTKIO_API  double computeVolumeWithStencil(  ::fwData::TriangularMesh::sptr _triangularMesh );

/*!
 * @brief Convert a ::fwData::TransformationMatrix3D::sptr to a vtkMatrix4x4*.
 *
 * @param[in] _transfoMatrix ::fwData::TransformationMatrix3D::sptr.
 * @return vtkPolyData*.
 */
VTKIO_API vtkMatrix4x4 *  toVTKMatrix(  ::fwData::TransformationMatrix3D::sptr _transfoMatrix );

/*!
 * @brief Convert a vtkMatrix4x4* to a ::fwData::TransformationMatrix3D::sptr.
 *
 * @param[in] _matrix vtkMatrix4x4*.
 * @param[out] _transfoMatrix ::fwData::TransformationMatrix3D::sptr.
 * @return bool.
 *
 * Returns \b true if the conversion is a success and \b false if it fails
 */
VTKIO_API  bool fromVTKMatrix( vtkMatrix4x4* _matrix,  ::fwData::TransformationMatrix3D::sptr _transfoMatrix);

}

#endif // _VTKIO_VTK_HPP_
