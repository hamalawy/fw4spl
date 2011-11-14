/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwComEd/fieldHelper/BackupHelper.hpp>

#include <io/IWriter.hpp>

#include <fwCore/base.hpp>

#include <fwData/TriangularMesh.hpp>
#include <fwData/location/Folder.hpp>
#include <fwData/location/SingleFile.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/Cursor.hpp>

#include <fwGui/dialog/ProgressDialog.hpp>
#include <vtkIO/TriangularMeshWriter.hpp>

#include "ioVTK/TriangularMeshWriterService.hpp"


namespace ioVTK
{

REGISTER_SERVICE( ::io::IWriter , ::ioVTK::TriangularMeshWriterService , ::fwData::TriangularMesh ) ;

//------------------------------------------------------------------------------

TriangularMeshWriterService::TriangularMeshWriterService() throw() :
    m_bServiceIsConfigured(false),
    m_fsMeshPath("")
{}

//------------------------------------------------------------------------------

TriangularMeshWriterService::~TriangularMeshWriterService() throw()
{}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::configuring() throw(::fwTools::Failed)
{
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getExistingAttributeValue("id") ;
        m_fsMeshPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = true;
        OSLM_TRACE("Filename found" << filename ) ;
    }
}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::configureWithIHM()
{
    SLM_TRACE_FUNC();
    static ::boost::filesystem::path _sDefaultPath("");

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle("Choose a vtk file to save triangle mesh");
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.addFilter("Vtk","*.vtk");
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);

    ::fwData::location::SingleFile::sptr  result;
    result= ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );
    if (result)
    {
        m_fsMeshPath = result->getPath();
        m_bServiceIsConfigured = true;
        _sDefaultPath = m_fsMeshPath.parent_path();
    }
}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::info(std::ostream &_sstream )
{
    _sstream << "TriangularMeshWriterService::info";
}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::saveMesh( const ::boost::filesystem::path vtkFile, ::fwData::TriangularMesh::sptr _pMesh )
{
    SLM_TRACE_FUNC();
    ::vtkIO::TriangularMeshWriter::NewSptr myWriter;

    myWriter->setObject(_pMesh);
    myWriter->setFile(vtkFile);

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Saving Mesh");
        myWriter->addHandler( progressMeterGUI );
        myWriter->write();

    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during saving : " << e.what();

        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning",
                ss.str(),
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning",
                "Warning during saving",
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
}

//------------------------------------------------------------------------------

void TriangularMeshWriterService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    if( m_bServiceIsConfigured )
    {
        // Retrieve dataStruct associated with this service
        ::fwData::TriangularMesh::sptr pTriangularMesh = this->getObject< ::fwData::TriangularMesh >() ;
        SLM_ASSERT("pTriangularMesh not instanced", pTriangularMesh);

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);

        saveMesh(m_fsMeshPath,pTriangularMesh);

        cursor.setDefaultCursor();

        m_bServiceIsConfigured = false;
    }
}

//------------------------------------------------------------------------------

} // namespace ioVtk
