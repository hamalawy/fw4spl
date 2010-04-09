/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/operations.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwComEd/fieldHelper/BackupHelper.hpp>
#include <fwComEd/TriangularMeshMsg.hpp>

#include <fwServices/ObjectMsg.hpp>

#include <io/IReader.hpp>

#include <fwCore/base.hpp>

#include <fwData/TriangularMesh.hpp>

#include <vtkIO/MeshReader.hpp>

#include "ioQVTK/MeshReaderService.hpp"
#include <QFileDialog>
#include <QString>

namespace ioQVTK
{

REGISTER_SERVICE( ::io::IReader , ::ioQVTK::MeshReaderService , ::fwData::TriangularMesh ) ;

//------------------------------------------------------------------------------

MeshReaderService::MeshReaderService() throw() :
    m_bServiceIsConfigured(false),
    m_fsMeshPath("")
{}

//------------------------------------------------------------------------------

MeshReaderService::~MeshReaderService() throw()
{}

//------------------------------------------------------------------------------

void MeshReaderService::configuring() throw(::fwTools::Failed)
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

void MeshReaderService::configureWithIHM()
{
    QString file = QFileDialog::getOpenFileName(0,QObject::tr("Open File"), QDir::currentPath(), QObject::tr("MeshVTK (*.vtk *.VTK )"));
    std::cout<<"PATH_FILE : "<<file.toStdString()<<"\n";

    if( file.isEmpty() == false )
    {
      m_fsMeshPath = ::boost::filesystem::path( file.toStdString(), ::boost::filesystem::native );
      m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void MeshReaderService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE("MeshReaderService::starting()");
}

//------------------------------------------------------------------------------

void MeshReaderService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE("MeshReaderService::stopping()");
}

//------------------------------------------------------------------------------

void MeshReaderService::info(std::ostream &_sstream )
{
    _sstream << "MeshReaderService::info";
}

//------------------------------------------------------------------------------

void MeshReaderService::loadMesh( const ::boost::filesystem::path vtkFile, ::fwData::TriangularMesh::sptr _pTriangularMesh )
{
    SLM_TRACE("MeshReaderService::loadMesh");
    ::vtkIO::MeshReader myReader;

    myReader.setObject(_pTriangularMesh);
    myReader.setFile(vtkFile);
    myReader.read();

 
}

//------------------------------------------------------------------------------

void MeshReaderService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE("MeshReaderService::updating()");
/*    if( !m_bServiceIsConfigured )
    {
       configureWithIHM();
    }
    if( m_bServiceIsConfigured )
    {
      ::fwData::TriangularMesh::sptr pTriangularMesh = this->getObject< ::fwData::TriangularMesh >() ;
        assert(pTriangularMesh);

	loadMesh(m_fsMeshPath, pTriangularMesh);
        notificationOfUpdate();
    }*/

   configureWithIHM();
    
    if(m_bServiceIsConfigured)
    {
        // Retrieve object
        ::fwData::TriangularMesh::sptr mesh = this->getObject< ::fwData::TriangularMesh >( );
        assert( mesh ) ;

        ::vtkIO::MeshReader reader;
        reader.setObject( mesh );
        reader.setFile(m_fsMeshPath);
        reader.read();

        // Notify reading
        ::fwComEd::TriangularMeshMsg::NewSptr msg;
        msg->addEvent( ::fwComEd::TriangularMeshMsg::NEW_MESH );
        ::fwServices::IEditionService::notify(this->getSptr(), mesh, msg);
    }
   
}

//------------------------------------------------------------------------------

void MeshReaderService::notificationOfUpdate()
{
    SLM_TRACE("MeshReaderService::notificationOfDBUpdate");
    ::fwData::TriangularMesh::sptr pTriangularMesh = this->getObject< ::fwData::TriangularMesh >();
    assert( pTriangularMesh );

//  ::fwServices::IEditionService::sptr editor = ::fwServices::get< ::fwServices::IEditionService >( pTriangularMesh ) ;
//  ::fwServices::ObjectMsg::sptr msg( new ::fwServices::ObjectMsg(pTriangularMesh) ) ;
//  msg->setAllModified( ) ;
    ::fwComEd::TriangularMeshMsg::NewSptr msg;;
    msg->addEvent( ::fwComEd::TriangularMeshMsg::NEW_MESH ) ;

//  editor->notify( msg );
    ::fwServices::IEditionService::notify(this->getSptr(), pTriangularMesh, msg);
}

//------------------------------------------------------------------------------


} // namespace ioVtk
