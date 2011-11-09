/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <vtkSmartPointer.h>
#include <vtkXMLImageDataReader.h>
#include <vtkImageData.h>

#include <fwTools/ClassRegistrar.hpp>

#include <fwCore/base.hpp>

#include "vtkIO/vtk.hpp"
#include "vtkIO/VtiImageReader.hpp"
#include "vtkIO/helper/ProgressVtkToFw.hpp"

REGISTER_BINDING_BYCLASSNAME( ::fwDataIO::reader::IObjectReader , ::vtkIO::VtiImageReader, ::vtkIO::VtiImageReader );


namespace vtkIO
{
//------------------------------------------------------------------------------

VtiImageReader::VtiImageReader()
: ::fwData::location::enableSingleFile< ::fwDataIO::reader::IObjectReader >(this)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

VtiImageReader::~VtiImageReader()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void VtiImageReader::read()
{
    assert( !m_object.expired() );
    assert( m_object.lock() );

    ::fwData::Image::sptr pImage = getConcreteObject();

    vtkSmartPointer< vtkXMLImageDataReader > reader = vtkSmartPointer< vtkXMLImageDataReader >::New();
    reader->SetFileName(this->getFile().string().c_str());

    Progressor progress(reader, this->getSptr(), this->getFile().string());

    reader->Update();

    vtkDataObject *obj = reader->GetOutput();
    vtkImageData* img = vtkImageData::SafeDownCast(obj);
    FW_RAISE_IF("VtiImageReader cannot read Vti image file :"<<this->getFile().string(), !img);
    try
    {
        ::vtkIO::fromVTKImage( img, pImage);
    }
    catch( std::exception &e)
    {
        FW_RAISE("VTIImage to fwData::Image failed "<<e.what());
    }
}

//------------------------------------------------------------------------------

std::string  VtiImageReader::extension()
{
   return ".vti";
}

} // namespace vtkIO
