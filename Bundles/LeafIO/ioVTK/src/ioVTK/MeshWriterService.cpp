/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#include <wx/wx.h>
#include <wx/version.h>

#include <fwServices/macros.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwComEd/fieldHelper/BackupHelper.hpp>

#include <io/IWriter.hpp>

#include <fwCore/base.hpp>

#include <fwData/TriangularMesh.hpp>

#include <fwWX/ProgressTowx.hpp>
#include <vtkIO/MeshWriter.hpp>

#include "ioVTK/MeshWriterService.hpp"


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
	static wxString _sDefaultPath = _("");
	wxString title = _("Choose an vtk file to save Mesh");
	wxString file = wxFileSelector(
			title,
			_sDefaultPath,
			wxT(""),
			wxT(""),
			wxT("Vtk (*.vtk)|*.vtk"),
			wxFD_SAVE,

			wxTheApp->GetTopWindow() );

	if( file.IsEmpty() == false)
	{
		m_fsMeshPath = ::boost::filesystem::path( wxConvertWX2MB(file), ::boost::filesystem::native );
		m_bServiceIsConfigured = true;
		_sDefaultPath = wxConvertMB2WX( m_fsMeshPath.branch_path().string().c_str() );
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

	try
	{
		::fwWX::ProgressTowx progressMeterGUI("Saving Meshs ");
		myWriter.addHandler( progressMeterGUI );
		myWriter.write();

	}
	catch (const std::exception & e)
	{
		std::stringstream ss;
		ss << "Warning during loading : " << e.what();
		wxString wxStmp( ss.str().c_str(), wxConvLocal );
		wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );
	}
	catch( ... )
	{
		std::stringstream ss;
		ss << "Warning during loading : ";
		wxString wxStmp( ss.str().c_str(), wxConvLocal );
		wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );
	}
}

//------------------------------------------------------------------------------

void MeshWriterService::updating() throw(::fwTools::Failed)
{
	SLM_TRACE("MeshWriterService::updating()");

	if( m_bServiceIsConfigured )
	{
		// Retrieve dataStruct associated with this service
		::fwData::TriangularMesh::sptr pTriangularMesh = this->getObject< ::fwData::TriangularMesh >() ;
		assert(pTriangularMesh);
		wxBeginBusyCursor();
		saveMesh(m_fsMeshPath,pTriangularMesh);
		wxEndBusyCursor();

		m_bServiceIsConfigured = false;
	}
}

//------------------------------------------------------------------------------

} // namespace ioVtk
