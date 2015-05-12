/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <vtkStringArray.h>

#include "vtkGdcmIO/helper/ImageDicomStream.hpp"

namespace vtkGdcmIO
{
namespace helper
{

//------------------------------------------------------------------------------

ImageDicomSource::ImageDicomSource( ImageDicomInfo::sptr dcmInfo ) :
    m_dcmInfo ( dcmInfo ),
    m_pos ( 0 ),
    m_success ( false )
{
    SLM_ASSERT( "ImageDicomSource needs dicom file to read a dciom image", dcmInfo->m_seriesFiles.size() > 0);
}

//------------------------------------------------------------------------------

bool filesStillExist( const ImageDicomInfo::SeriesFilesType & files )
{
    ::boost::filesystem::path filePath;
    bool allFilesExists = true;

    for(    ImageDicomInfo::SeriesFilesType::const_iterator itFile = files.begin();
            allFilesExists && ( itFile != files.end() );
            ++itFile )
    {
        filePath        = *itFile;
        allFilesExists &= ::boost::filesystem::exists(filePath);
    }
    return allFilesExists;
}

//------------------------------------------------------------------------------

bool ImageDicomSource::readImage()
{
    vtkSmartPointer< vtkStringArray > fileArray = vtkSmartPointer< vtkStringArray >::New();
    fileArray->Initialize();
    BOOST_FOREACH( std::string file, m_dcmInfo->m_seriesFiles )
    {
        OSLM_TRACE("Add " << file << " to vtkGdcmReader");
        fileArray->InsertNextValue( file.c_str() );
    }

    m_reader = vtkSmartPointer< vtkGDCMImageReader >::New();
    m_reader->FileLowerLeftOn();

    bool res = false;
    if ( filesStillExist( m_dcmInfo->m_seriesFiles ) )
    {
        m_reader->SetFileNames( fileArray );
        try
        {
            m_reader->Update();
            m_inputReader = static_cast<char*>( m_reader->GetOutput()->GetScalarPointer() );
            res           = true;
        }
        catch (std::exception &e)
        {
            m_reader      = 0;
            m_inputReader = 0;
            OSLM_ERROR ( "Error during conversion : " << e.what() );
        }
        catch (...)
        {
            m_reader      = 0;
            m_inputReader = 0;
            OSLM_ERROR ( "Unexpected error during conversion" );
        }
    }

    return res;
}


//------------------------------------------------------------------------------

std::streamsize ImageDicomSource::read(char* s, std::streamsize n)
{
    if ( m_pos == 0 )
    {
        m_success = this->readImage();
        OSLM_ERROR_IF( "Error while reading dicom files : " << m_dcmInfo->m_seriesFiles[0] << " ...", !m_success );
    }

    using namespace std;
    streamsize amt    = static_cast<streamsize>( m_dcmInfo->m_buffSizeInBytes - m_pos );
    streamsize result = std::min(n, amt);

    if (result != 0)
    {
        if( m_success )
        {
            std::copy( m_inputReader + m_pos, m_inputReader + m_pos + result, s );
        }
        m_pos += result;
        return result;
    }
    else
    {
        if ( m_reader )
        {
            m_reader = 0;
        }
        return -1; // EOF
    }
}


//------------------------------------------------------------------------------

ImageDicomStream::ImageDicomStream( ImageDicomInfo::sptr dcmInfo ) : m_dcmInfo ( dcmInfo )
{
}

//------------------------------------------------------------------------------

SPTR(std::istream) ImageDicomStream::get()
{
    SPTR(::boost::iostreams::stream<ImageDicomSource>) is
        = ::boost::make_shared< ::boost::iostreams::stream<ImageDicomSource> >( m_dcmInfo );

    return is;
}

//------------------------------------------------------------------------------

} // namespace helper
} // namespace vtkGdcmIO
