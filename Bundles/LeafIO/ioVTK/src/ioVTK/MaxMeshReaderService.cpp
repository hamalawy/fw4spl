/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <iostream>
#include <fstream>

#include <boost/filesystem/operations.hpp>

#include <vtk3DSImporter.h>

#include <fwServices/Base.hpp>
#include <fwServices/ObjectMsg.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/macros.hpp>

#include <fwData/TriangularMesh.hpp>
#include <fwData/Model.hpp>
#include <fwData/location/Folder.hpp>
#include <fwData/location/SingleFile.hpp>

#include <fwGui/dialog/LocationDialog.hpp>


#include <fwCore/base.hpp>

#include <fwComEd/ModelMsg.hpp>

#include "ioVTK/MaxMeshReaderService.hpp"

fwServicesRegisterMacro( ::io::IReader , ::ioVTK::MaxMeshReaderService , ::fwData::Model );

//------------------------------------------------------------------------------

/* Find the node with the specified name */
static void *list_find (vtk3DSList **root, const char *name)
{
    OSLM_DEBUG("list_find: " << name);
    vtk3DSList *p;
    int i = 0;
    for (p = *root; p != (vtk3DSList *) NULL; p = (vtk3DSList *) p->next) {
        OSLM_DEBUG("List_find - " << i << " name: " << p->name);
        ++i;
        if (strcmp(p->name, name) == 0) {
            break;
        }
    }
    return (void *) p;
}

namespace ioVTK
{

//------------------------------------------------------------------------------

void MaxMeshReaderService::info(std::ostream &_sstream )
{
    this->BaseClass::info( _sstream ) ;
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

void MaxMeshReaderService::configureWithIHM()
{
    SLM_TRACE_FUNC();

    static ::boost::filesystem::path _sDefaultPath("");

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle("Choose an 3ds file");
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.addFilter("3DS","*.3ds");
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::READ);
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::FILE_MUST_EXIST);

    ::fwData::location::SingleFile::sptr  result;
    result= ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath = result->getPath().parent_path();
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
        this->setFile(result->getPath());
    }
    else
    {
        this->clearLocations();
    }

}

//------------------------------------------------------------------------------


::io::IOPathType MaxMeshReaderService::getIOPathType() const
{
    return ::io::FILE;
}

//------------------------------------------------------------------------------

void MaxMeshReaderService::updating() throw(::fwTools::Failed)
{
    if( this->hasLocationDefined() )
    {
        /// Retrieve object
        ::fwData::Model::sptr model = this->getObject< ::fwData::Model >( );
        SLM_ASSERT("model not instanced", model);
        ::fwData::Model::NewSptr backupModel;
        backupModel->shallowCopy(model);
        model->getRefMap().clear();

        vtk3DSImporter *importer1 = vtk3DSImporter::New();
        importer1->SetFileName(this->getFile().string().c_str());
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
        ::fwComEd::ModelMsg::NewSptr msg;;
        msg->addEvent( ::fwComEd::ModelMsg::NEW_MODEL, backupModel ) ;
        ::fwServices::IEditionService::notify(this->getSptr(), model, msg);
        importer1->Delete();
    }
}

}
