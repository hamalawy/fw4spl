/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/macros.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwComEd/fieldHelper/BackupHelper.hpp>
#include <io/IWriter.hpp>
#include <fwCore/base.hpp>
#include <fwData/Image.hpp>
#include <vtkIO/ImageWriter.hpp>

#include "ioQVTK/ImageWriterService.hpp"

#include <QFileDialog>
#include <QString>

namespace ioVTK
{

REGISTER_SERVICE( ::io::IWriter , ::ioVTK::ImageWriterService , ::fwData::Image ) ;

//------------------------------------------------------------------------------

ImageWriterService::ImageWriterService() throw() :
    m_bServiceIsConfigured(false),
    m_fsImgPath("")
{}

//------------------------------------------------------------------------------

ImageWriterService::~ImageWriterService() throw()
{}

//------------------------------------------------------------------------------

void ImageWriterService::configuring() throw(::fwTools::Failed)
{
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getExistingAttributeValue("id") ;
        m_fsImgPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = ::boost::filesystem::exists(m_fsImgPath);
        OSLM_TRACE("Filename found" << filename ) ;
    }
}

//------------------------------------------------------------------------------

void ImageWriterService::configureWithIHM()
{
  //  QString file = QFileDialog::getSaveFileName(0,QObject::tr("Choose an vtk file to save image"), QDir::currentPath(), QObject::tr("Images (*.vtk *.VTK )"));
    
    QString format = "vtk";
    QString initialPath = QDir::currentPath() + QObject::tr("/untitled.") + format;
    QString file = QFileDialog::getSaveFileName(0, QObject::tr("Choose an vtk file to save image"), initialPath,
                                QObject::tr("%1 Files (*.%2);;All Files (*)")
                                .arg(format.toUpper())
                                .arg(format));
    
    
	    
    if( file.isEmpty() == false)
    {
        m_fsImgPath = ::boost::filesystem::path(  file.toStdString(), ::boost::filesystem::native );
        m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void ImageWriterService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE("ImageWriterService::starting()");
}

//------------------------------------------------------------------------------

void ImageWriterService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE("ImageWriterService::stopping()");
}

//------------------------------------------------------------------------------

void ImageWriterService::info(std::ostream &_sstream )
{
    _sstream << "ImageWriterService::info";
}

//------------------------------------------------------------------------------

bool ImageWriterService::saveImage( const ::boost::filesystem::path vtkFile, ::boost::shared_ptr< ::fwData::Image > _pImg )
{
    SLM_TRACE("ImageWriterService::saveImage");
    ::vtkIO::ImageWriter myWriter;

    myWriter.setObject(_pImg);
    myWriter.setFile(vtkFile);

    bool bValue = true;
    myWriter.write();

 
    return bValue;
}

//------------------------------------------------------------------------------

void ImageWriterService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE("ImageWriterService::updating()");

      if( !m_bServiceIsConfigured )
    {
       configureWithIHM();
    }
    if( m_bServiceIsConfigured )
    {
      ::fwData::Image::sptr pImage = this->getObject< ::fwData::Image >() ;
        assert(pImage);
     saveImage(m_fsImgPath,pImage);
    }
    else
    {
      SLM_TRACE("NOT CONFIGURED ");
    }
}

} // namespace ioVtk
