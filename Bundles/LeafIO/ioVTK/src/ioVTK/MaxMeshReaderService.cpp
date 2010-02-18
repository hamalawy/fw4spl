/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <iostream>
#include <fstream>

#include <boost/filesystem/operations.hpp>

#include <vtk3DSImporter.h>

#include <fwServices/helper.hpp>
#include <fwServices/ObjectMsg.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/macros.hpp>

#include <fwData/TriangularMesh.hpp>
#include <fwData/Model.hpp>
#include <fwCore/base.hpp>

#include <fwComEd/ModelMsg.hpp>


#include "ioVTK/MaxMeshReaderService.hpp"

REGISTER_SERVICE( ::io::IReader , ::ioVTK::MaxMeshReaderService , ::fwData::Model );

//------------------------------------------------------------------------------

/* Find the node with the specified name */
static void *list_find (vtk3DSList **root, const char *name)
{
    OSLM_DEBUG("list_find: " << name);
    vtk3DSList *p;
    int i = 0;
    for (p = *root; p != (vtk3DSList *) NULL; p = (vtk3DSList *) p->next) {
        OSLM_DEBUG("List_find - " << i++ << " name: " << p->name);
        if (strcmp(p->name, name) == 0) {
            break;
        }
    }
    return (void *) p;
}

namespace ioVTK
{

MaxMeshReaderService::MaxMeshReaderService() throw():
    m_bServiceIsConfigured(false)
{
    SLM_INFO( "[MaxMeshReaderService::MaxMeshReaderService]");
}

//------------------------------------------------------------------------------

void MaxMeshReaderService::info(std::ostream &_sstream )
{
    this->SuperClass::info( _sstream ) ;
    _sstream << std::endl << " 3ds file reader" ;
}

//------------------------------------------------------------------------------

std::vector< std::string > MaxMeshReaderService::getSupportedExtensions()
{
    std::vector< std::string > extensions ;
    extensions.push_back(".3ds");
    return extensions ;
}

//------------------------------------------------------------------------------

MaxMeshReaderService::~MaxMeshReaderService() throw()
{
    SLM_INFO( "[MaxMeshReaderService::~MaxMeshReaderService]");
}

//------------------------------------------------------------------------------

void MaxMeshReaderService::configureWithIHM()
{
    static wxString _sDefaultPath = _("");
    wxString title = _("Choose a 3ds file");
    wxString folder = wxFileSelector(
            title,
            _sDefaultPath,
            wxT(""),
            wxT(""),
            wxT("3DS|*.3ds"),
#if wxCHECK_VERSION(2, 8, 0)
            wxFD_FILE_MUST_EXIST,
#else
            wxFILE_MUST_EXIST,
#endif
            wxTheApp->GetTopWindow() );

    if( folder.IsEmpty() == false)
    {
        m_fsMeshPath = ::boost::filesystem::path( wxConvertWX2MB(folder), ::boost::filesystem::native );
        m_bServiceIsConfigured = true;
        _sDefaultPath = wxConvertMB2WX( m_fsMeshPath.branch_path().string().c_str() );
    }
}

//------------------------------------------------------------------------------

void MaxMeshReaderService::configuring( ) throw(::fwTools::Failed)
{
    SLM_INFO( "[MaxMeshReaderService::configure]");
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getValue() ;
        OSLM_ASSERT( "3ds file doesn't exist: " << filename, ::boost::filesystem::exists(filename) );
        m_fsMeshPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = ::boost::filesystem::exists(filename);
    }
}

//------------------------------------------------------------------------------

void MaxMeshReaderService::updating() throw(::fwTools::Failed)
{
    if( m_bServiceIsConfigured )
    {
        /// Retrieve object
        ::fwData::Model::sptr model = this->getObject< ::fwData::Model >( );
        assert( model ) ;
        model->getRefMap().clear();
        /// Retrieve Reader for location
        ::io::IReader *readerInfo = dynamic_cast< ::io::IReader *>( this ) ;
        assert( readerInfo ) ;

        vtk3DSImporter *importer1 = vtk3DSImporter::New();
        importer1->SetFileName(m_fsMeshPath.native_file_string().c_str());
        importer1->ComputeNormalsOn();
        importer1->Read();

        vtk3DSMesh * MeshList = importer1->MeshList;
        vtk3DSMesh *mesh3ds;
        // walk the list of meshes, creating actors
        for (mesh3ds = MeshList; mesh3ds != (vtk3DSMesh *) NULL; mesh3ds = (vtk3DSMesh *) mesh3ds->next)
        {
            OSLM_DEBUG("read : " << mesh3ds->name);
            ::fwData::TriangularMesh::NewSptr mesh;
            vtk3DSFace *face;
            face = mesh3ds->face;
            OSLM_DEBUG("mesh->faces : " << mesh3ds->faces);
            for (int i = 0; i < mesh3ds->faces; i++, face++)
            {
                std::vector< int > cell(3) ;
                cell[0] = face->a;
                cell[1] = face->b;
                cell[2] = face->c;
                mesh->cells().push_back( cell ) ;
            }

            OSLM_DEBUG("mesh->vertices : " << mesh3ds->vertices);
            for (int i = 0; i < mesh3ds->vertices; i++)
            {
                float *p = mesh3ds->vertex[i];
                std::vector< float > point(3) ;
                point[0] = p[0];
                point[1] = p[1];
                point[2] = p[2];
                mesh->points().push_back( point) ;
            }
            vtk3DSMatProp *vtkMmat;
            vtkMmat = (vtk3DSMatProp *)list_find((vtk3DSList **)&importer1->MatPropList, mesh3ds->mtl[0]->name);
            ::fwData::Color::ColorArray  m_vRGBA;
            m_vRGBA[0] = vtkMmat->ambient.red;
            m_vRGBA[1] = vtkMmat->ambient.green;
            m_vRGBA[2] = vtkMmat->ambient.blue;
            m_vRGBA[3] = 1.0;
            ::fwData::Material::NewSptr dataMat;
            dataMat->ambient()->setCRefRGBA(m_vRGBA);
            model->getRefMap().insert (
                    std::pair< ::fwData::TriangularMesh::sptr , ::fwData::Material::sptr >(mesh, dataMat));

        }
        /// Notify reading
//      ::boost::shared_ptr< ::fwServices::IEditionService > editor = ::fwServices::get< ::fwServices::IEditionService >( model ) ;
        //::boost::shared_ptr< ::fwServices::ObjectMsg > msg( new ::fwServices::ObjectMsg(model) ) ;
        //msg->setAllModified( ) ;
        ::fwComEd::ModelMsg::NewSptr msg;;
        msg->addEvent( ::fwComEd::ModelMsg::NEW_MODEL ) ;
//      editor->notify( msg );
        ::fwServices::IEditionService::notify(this->getSptr(), model, msg);
        importer1->Delete();
    }
}

}
