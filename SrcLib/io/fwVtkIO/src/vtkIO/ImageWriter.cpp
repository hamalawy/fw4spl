/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <vtkImageData.h>
#include <vtkGenericDataObjectWriter.h>
#include <vtkSmartPointer.h>

#include <fwCore/base.hpp>

#include <fwDataIO/writer/registry/macros.hpp>

#include "vtkIO/vtk.hpp"
#include "vtkIO/ImageWriter.hpp"
#include "vtkIO/helper/ProgressVtkToFw.hpp"

fwDataIOWriterRegisterMacro( ::vtkIO::ImageWriter );


namespace vtkIO
{
//------------------------------------------------------------------------------

ImageWriter::ImageWriter(::fwDataIO::writer::IObjectWriter::Key key) :
        ::fwData::location::enableSingleFile< ::fwDataIO::writer::IObjectWriter >(this)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

ImageWriter::~ImageWriter()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void ImageWriter::write()
{
    assert( !m_object.expired() );
    assert( m_object.lock() );

    ::fwData::Image::sptr pImage = getConcreteObject();

    vtkSmartPointer< vtkGenericDataObjectWriter > writer = vtkSmartPointer< vtkGenericDataObjectWriter >::New();
    vtkSmartPointer< vtkImageData > vtkImage = vtkSmartPointer< vtkImageData >::New();
    ::vtkIO::toVTKImage( pImage, vtkImage );
    writer->SetInput( vtkImage );
    writer->SetFileName(this->getFile().string().c_str());
    writer->SetFileTypeToBinary();

    //add progress observation
    Progressor progress(writer, this->getSptr(), this->getFile().string());

    writer->Write();
}

//------------------------------------------------------------------------------

std::string  ImageWriter::extension()
{
   return ".vtk";
}

} // namespace vtkIO
