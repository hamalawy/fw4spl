/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <iostream>
#include <fstream>

#include <boost/filesystem/operations.hpp>

#include <io/IReader.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/ObjectMsg.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwData/TriangularMesh.hpp>
#include <fwCore/base.hpp>
#include <fwServices/macros.hpp>


#include <fwDataIO/writer/TriangularMeshWriter.hpp>

#include "ioQt/TriangularMeshWriterService.hpp"

#include <QFileDialog>
#include <QString>

REGISTER_SERVICE( ::io::IWriter , ::ioData::TriangularMeshWriterService , ::fwData::TriangularMesh ) ;

namespace ioData
{

TriangularMeshWriterService::TriangularMeshWriterService():
        m_bServiceIsConfigured(false)
{
}

//-----------------------------------------------------------------------------

void TriangularMeshWriterService::info(std::ostream &_sstream )
{
    this->SuperClass::info( _sstream ) ;
    _sstream << std::endl << " Triangular Mesh writer" ;
}

//-----------------------------------------------------------------------------

std::vector< std::string > TriangularMeshWriterService::getSupportedExtensions()
{
    std::vector< std::string > extensions ;
    extensions.push_back(".trian");
    return extensions ;
}

//-----------------------------------------------------------------------------

TriangularMeshWriterService::~TriangularMeshWriterService() throw()
{
}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::configuring( ) throw(::fwTools::Failed)
{
    OSLM_INFO( "TriangularMeshWriterService::configure : " << *m_configuration );
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getValue() ;
        OSLM_INFO( "TriangularMeshWriterService::configure filename: " << filename );
        m_fsMeshPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::configureWithIHM()
{
    
     QString file = QFileDialog::getSaveFileName(0,QObject::tr("Choose an TrianMesh file"), QDir::currentPath(), QObject::tr("TrianMesh (*.trian)"));
    std::cout<<"PATH_FILE : "<<file.toStdString()<<"\n";
    
    
    // If the user choose an vtk file, the image path is initialized and we tag the service as configured.
    if( file.isEmpty() == false )
    {
        m_fsMeshPath = ::boost::filesystem::path( file.toStdString(), ::boost::filesystem::native );
        m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    /*
    if(m_bServiceIsConfigured)
    {
        // Retrieve object
        ::fwData::TriangularMesh::sptr mesh = this->getObject< ::fwData::TriangularMesh >( );
        assert( mesh ) ;

        ::fwDataIO::writer::TriangularMeshWriter writer;
        writer.setObject( mesh );
        writer.setFile(m_fsMeshPath);
        writer.write();
    }
    */
    
    if( !m_bServiceIsConfigured )
    {
       configureWithIHM();
    }
    if( m_bServiceIsConfigured )
    {
       ::fwData::TriangularMesh::sptr mesh = this->getObject< ::fwData::TriangularMesh >( );
        assert( mesh ) ;

        ::fwDataIO::writer::TriangularMeshWriter writer;
        writer.setObject( mesh );
        writer.setFile(m_fsMeshPath);
        writer.write();
    }
    else
      	   std::cout<<"\n\n NOT CONFIGURED \n";
}
}
