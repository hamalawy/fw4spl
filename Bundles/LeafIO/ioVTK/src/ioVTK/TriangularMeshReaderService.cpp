/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/operations.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwComEd/TriangularMeshMsg.hpp>

#include <fwServices/ObjectMsg.hpp>

#include <io/IReader.hpp>

#include <fwCore/base.hpp>

#include <fwData/TriangularMesh.hpp>
#include <fwData/location/Folder.hpp>
#include <fwData/location/SingleFile.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/Cursor.hpp>

#include <fwGui/dialog/ProgressDialog.hpp>
#include <vtkIO/TriangularMeshReader.hpp>

#include "ioVTK/TriangularMeshReaderService.hpp"


namespace ioVTK
{

fwServicesRegisterMacro( ::io::IReader , ::ioVTK::TriangularMeshReaderService , ::fwData::TriangularMesh ) ;

//------------------------------------------------------------------------------


::io::IOPathType TriangularMeshReaderService::getIOPathType() const
{
    return ::io::FILE;
}

//------------------------------------------------------------------------------

void TriangularMeshReaderService::configureWithIHM()
{
    SLM_TRACE_FUNC();

    static ::boost::filesystem::path _sDefaultPath("");

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle("Choose a vtk file to load triangle mesh");
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.addFilter("Vtk","*.vtk");
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

void TriangularMeshReaderService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void TriangularMeshReaderService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void TriangularMeshReaderService::info(std::ostream &_sstream )
{
    _sstream << "TriangularMeshReaderService::info";
}

//------------------------------------------------------------------------------

void TriangularMeshReaderService::loadMesh( const ::boost::filesystem::path vtkFile, ::fwData::TriangularMesh::sptr _pTriangularMesh )
{
    SLM_TRACE_FUNC();
    ::vtkIO::TriangularMeshReader::NewSptr myReader;

    myReader->setObject(_pTriangularMesh);
    myReader->setFile(vtkFile);

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Loading Mesh");
        myReader->addHandler( progressMeterGUI );
        myReader->read();
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during loading : " << e.what();

        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning",
                ss.str(),
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        std::stringstream ss;
        ss << "Warning during loading. ";
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning",
                "Warning during loading.",
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
}

//------------------------------------------------------------------------------

void TriangularMeshReaderService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    if( this->hasLocationDefined() )
    {
        // Retrieve dataStruct associated with this service
        ::fwData::TriangularMesh::sptr pTriangularMesh = this->getObject< ::fwData::TriangularMesh >() ;
        SLM_ASSERT("pTriangularMesh not instanced", pTriangularMesh);

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);

        loadMesh( this->getFile(), pTriangularMesh);
        notificationOfUpdate();

        cursor.setDefaultCursor();
    }
}

//------------------------------------------------------------------------------

void TriangularMeshReaderService::notificationOfUpdate()
{
    SLM_TRACE_FUNC();
    ::fwData::TriangularMesh::sptr pTriangularMesh = this->getObject< ::fwData::TriangularMesh >();
    SLM_ASSERT("pTriangularMesh not instanced", pTriangularMesh);

    ::fwComEd::TriangularMeshMsg::NewSptr msg;;
    msg->addEvent( ::fwComEd::TriangularMeshMsg::NEW_MESH ) ;
    ::fwServices::IEditionService::notify(this->getSptr(), pTriangularMesh, msg);
}

//------------------------------------------------------------------------------


} // namespace ioVtk
