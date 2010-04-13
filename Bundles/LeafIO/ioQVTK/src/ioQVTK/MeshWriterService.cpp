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

#include <fwData/TriangularMesh.hpp>

#include <vtkIO/MeshWriter.hpp>

#include "ioQVTK/MeshWriterService.hpp"
#include <QFileDialog>
#include <QString>


namespace ioVTK
{

REGISTER_SERVICE( ::io::IWriter , ::ioVTK::MeshWriterService , ::fwData::TriangularMesh ) ;

//------------------------------------------------------------------------------

MeshWriterService::MeshWriterService() throw() :
    m_bServiceIsConfigured(false),
    m_fsMeshPath("")
{}

//------------------------------------------------------------------------------

MeshWriterService::~MeshWriterService() throw()
{}

//------------------------------------------------------------------------------

void MeshWriterService::configuring() throw(::fwTools::Failed)
{
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getExistingAttributeValue("id") ;
        m_fsMeshPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = ::boost::filesystem::exists(m_fsMeshPath);
        OSLM_TRACE("Filename found" << filename ) ;
    }
}

//------------------------------------------------------------------------------

void MeshWriterService::configureWithIHM()
{  
    
    QString file = QFileDialog::getSaveFileName(0,QObject::tr("Choose an vtk file to save Mesh"), QDir::currentPath(), QObject::tr("MeshVTK (*.vtk *.VTK )"));
    std::cout<<"PATH_FILE : "<<file.toStdString()<<"\n";

    if( file.isEmpty() == false )
    {
      m_fsMeshPath = ::boost::filesystem::path( file.toStdString(), ::boost::filesystem::native );
      m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void MeshWriterService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE("MeshWriterService::starting()");
}

//------------------------------------------------------------------------------

void MeshWriterService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE("MeshWriterService::stopping()");
}

//------------------------------------------------------------------------------

void MeshWriterService::info(std::ostream &_sstream )
{
    _sstream << "MeshWriterService::info";
}

//------------------------------------------------------------------------------

void MeshWriterService::saveMesh( const ::boost::filesystem::path vtkFile, ::boost::shared_ptr< ::fwData::TriangularMesh > _pMesh )
{
    SLM_TRACE("MeshWriterService::saveMesh");
    ::vtkIO::MeshWriter myWriter;

    myWriter.setObject(_pMesh);
    myWriter.setFile(vtkFile);
    myWriter.write();

  
}

//------------------------------------------------------------------------------

void MeshWriterService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE("MeshWriterService::updating()");
/*
    if( m_bServiceIsConfigured )
    {
        // Retrieve dataStruct associated with this service
        ::fwData::TriangularMesh::sptr pTriangularMesh = this->getObject< ::fwData::TriangularMesh >() ;
        assert(pTriangularMesh);
     
        saveMesh(m_fsMeshPath,pTriangularMesh);

        m_bServiceIsConfigured = false;
    }*/
    
     if(m_bServiceIsConfigured)
    {
        // Retrieve object
        ::fwData::TriangularMesh::sptr pTriangularMesh = this->getObject< ::fwData::TriangularMesh >() ;
        assert(pTriangularMesh);

       saveMesh(m_fsMeshPath,pTriangularMesh);

    }
}

//------------------------------------------------------------------------------

} // namespace ioVtk
