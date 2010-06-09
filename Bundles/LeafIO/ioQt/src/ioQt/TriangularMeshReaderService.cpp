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


#include <fwDataIO/reader/TriangularMeshReader.hpp>

#include <fwComEd/TriangularMeshMsg.hpp>

#include "ioQt/TriangularMeshReaderService.hpp"

#include <QFileDialog>
#include <QString>

REGISTER_SERVICE( ::io::IReader , ::ioQt::TriangularMeshReaderService , ::fwData::TriangularMesh ) ;

namespace ioQt
{

TriangularMeshReaderService::TriangularMeshReaderService():
        m_fsMeshPath (""),
        m_bServiceIsConfigured(false)
{
}

//-----------------------------------------------------------------------------

void TriangularMeshReaderService::info(std::ostream &_sstream )
{
    this->SuperClass::info( _sstream ) ;
    _sstream << std::endl << " Triangular Mesh reader" ;
}

//-----------------------------------------------------------------------------

std::vector< std::string > TriangularMeshReaderService::getSupportedExtensions()
{
    std::vector< std::string > extensions ;
    extensions.push_back(".trian");
    return extensions ;
}

//-----------------------------------------------------------------------------

TriangularMeshReaderService::~TriangularMeshReaderService() throw()
{
}

//------------------------------------------------------------------------------

void TriangularMeshReaderService::configuring( ) throw(::fwTools::Failed)
{
    OSLM_INFO( "TriangularMeshReaderService::configure : " << *m_configuration );
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getValue() ;
        OSLM_INFO( "TriangularMeshReaderService::configure filename: " << filename );
        m_fsMeshPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void TriangularMeshReaderService::configureWithIHM()
{
    QString file = QFileDialog::getOpenFileName(0,QObject::tr("Open File"), QDir::currentPath(), QObject::tr("TrianMesh (*.trian)"));
    OSLM_TRACE("PATH_FILE : "<<file.toStdString()<<"\n");


    // If the user choose an vtk file, the image path is initialized and we tag the service as configured.
    if( file.isEmpty() == false )
    {
        m_fsMeshPath = ::boost::filesystem::path( file.toStdString(), ::boost::filesystem::native );
        m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void TriangularMeshReaderService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    configureWithIHM();

    if(m_bServiceIsConfigured)
    {
        // Retrieve object
        ::fwData::TriangularMesh::sptr mesh = this->getObject< ::fwData::TriangularMesh >( );
        assert( mesh ) ;

        ::fwDataIO::reader::TriangularMeshReader reader;
        reader.setObject( mesh );
        reader.setFile(m_fsMeshPath);
        reader.read();

        // Notify reading
        ::fwComEd::TriangularMeshMsg::NewSptr msg;
        msg->addEvent( ::fwComEd::TriangularMeshMsg::NEW_MESH );
        ::fwServices::IEditionService::notify(this->getSptr(), mesh, msg);
    }
}

}
