/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem.hpp>

#include <fwCore/base.hpp>

#include <itkImageSeriesWriter.h>
#include <itkNumericSeriesFileNames.h>
#include <itkIntensityWindowingImageFilter.h>
#include <itkJPEGImageIOFactory.h>

#include <fwTools/IntrinsicTypes.hpp>
#include <fwTools/DynamicTypeKeyTypeMapping.hpp>
#include <fwTools/Dispatcher.hpp>
#include <fwTools/IntrinsicTypes.hpp>
#include <fwTools/DynamicTypeKeyTypeMapping.hpp>

#include <fwComEd/Dictionary.hpp>
#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>

#include <fwData/Integer.hpp>
#include <fwData/Composite.hpp>
#include <fwData/Image.hpp>
#include <fwData/TransferFunction.hpp>

#include <fwDataIO/writer/registry/macros.hpp>

#include "fwItkIO/itk.hpp"
#include "fwItkIO/helper/ProgressItkToFw.hpp"
#include "fwItkIO/JpgImageWriter.hpp"


fwDataIOWriterRegisterMacro( ::fwItkIO::JpgImageWriter );


namespace fwItkIO
{

//------------------------------------------------------------------------------

JpgImageWriter::JpgImageWriter(::fwDataIO::writer::IObjectWriter::Key key) :
        ::fwData::location::enableFolder< ::fwDataIO::writer::IObjectWriter >(this)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

JpgImageWriter::~JpgImageWriter()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

struct JpgITKSaverFunctor
{
    JpgITKSaverFunctor()
    {
        // force register/link_with JPEGImageIOFactory
        ::itk::JPEGImageIOFactory::RegisterOneFactory();
    }
    struct Parameter
    {
        std::string                   m_filename;
        ::fwData::Image::sptr         m_dataImage;
        ::fwItkIO::JpgImageWriter::sptr m_fwWriter;
    };

    template<class PIXELTYPE>
    void operator()( const  Parameter &param )
    {
        OSLM_DEBUG( "itk::ImageSeriesWriter with PIXELTYPE "<<  fwTools::DynamicType::string<PIXELTYPE>() );

        ::fwData::Image::sptr image = param.m_dataImage;

        // VAG attention : ImageFileReader ne notifie AUCUNE progressEvent mais son ImageIO oui!!!! mais ImageFileReader ne permet pas de l'atteindre
        // car soit mis a la mano ou alors construit lors de l'Update donc trop tard
        // Il faut dont creer une ImageIO a la mano (*1*): affecter l'observation  sur IO (*2*) et mettre le IO dans le reader (voir *3*)

        // Reader IO (*1*)
        typename itk::ImageIOBase::Pointer imageIOWrite = itk::ImageIOFactory::CreateImageIO( "image.jpg", itk::ImageIOFactory::WriteMode);
        assert( imageIOWrite.IsNotNull() );

        // create writer
        typedef itk::Image< PIXELTYPE, 3> itkImageType;
        typedef itk::Image< unsigned char, 2 >      Image2DType;
        typedef typename  itk::ImageSeriesWriter< itkImageType, Image2DType > WriterType;
        typename WriterType::Pointer writer = WriterType::New();

        // set observation (*2*)
        itk::LightProcessObject::Pointer castHelper= (itk::LightProcessObject *)(imageIOWrite.GetPointer());
        assert( castHelper.IsNotNull() );
        Progressor progress(castHelper, param.m_fwWriter, param.m_filename);

        // create itk Image
        typename itkImageType::Pointer itkImage = ::fwItkIO::itkImageFactory<itkImageType>( image );

        typedef ::itk::IntensityWindowingImageFilter< itkImageType, itkImageType > RescaleFilterType;
        typename RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();

        double min, max;
        ::fwData::Composite::sptr poolTF;
        poolTF = image->getField< ::fwData::Composite>( ::fwComEd::Dictionary::m_transferFunctionCompositeId );
        if(poolTF)
        {
            ::fwData::Composite::iterator iter = poolTF->find(::fwData::TransferFunction::s_DEFAULT_TF_NAME);
            if(iter != poolTF->end())
            {
                ::fwData::TransferFunction::sptr tf;
                tf = ::fwData::TransferFunction::dynamicCast(iter->second);
                min = tf->getWLMinMax().first;
                max = tf->getWLMinMax().second;
            }
        }
        else
        {
            ::fwComEd::fieldHelper::MedicalImageHelpers::getMinMax(image, min, max);
        }

        rescaleFilter->SetWindowMinimum( min );
        rescaleFilter->SetWindowMaximum( max );
        rescaleFilter->SetOutputMinimum( 0 );
        rescaleFilter->SetOutputMaximum( 255 );
        rescaleFilter->InPlaceOff();
        rescaleFilter->SetInput( itkImage );
        rescaleFilter->Update();

        writer->SetInput( rescaleFilter->GetOutput() );

        typedef itk::NumericSeriesFileNames    NameGeneratorType;

        NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();

        std::string format = param.m_filename;
        format += "/%04d.jpg";
        nameGenerator->SetSeriesFormat( format.c_str() );
        nameGenerator->SetStartIndex( 1 );
        nameGenerator->SetEndIndex( image->getSize()[2] );
        nameGenerator->SetIncrementIndex( 1 );

        writer->SetFileNames( nameGenerator->GetFileNames() );

        writer->SetImageIO( imageIOWrite  );

        // save image;
        writer->Update();
    }
};

//------------------------------------------------------------------------------

void JpgImageWriter::write()
{
    assert( !m_object.expired() );
    assert( m_object.lock() );

    JpgITKSaverFunctor::Parameter saverParam;
    saverParam.m_filename =  this->getFolder().string();
    saverParam.m_dataImage = this->getConcreteObject();
    saverParam.m_fwWriter =  this->getSptr();
    assert( saverParam.m_dataImage );

    ::fwTools::Dispatcher< ::fwTools::IntrinsicTypes , JpgITKSaverFunctor >::invoke( saverParam.m_dataImage->getPixelType(), saverParam );
}

//------------------------------------------------------------------------------

std::string  JpgImageWriter::extension()
{
    return ".jpg";
}

} // namespace fwItkIO
