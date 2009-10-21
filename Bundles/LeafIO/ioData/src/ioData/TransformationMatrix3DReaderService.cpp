/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#include <fwDataIO/reader/TransformationMatrix3DReader.hpp>
#include <io/IReader.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/ObjectMsg.hpp>
#include <fwServices/bundle/runtime.hpp>
#include <fwData/TransformationMatrix3D.hpp>
#include <fwComEd/TransformationMatrix3DMsg.hpp>
#include <fwServices/macros.hpp>

#include <fstream>
#include <fwCore/base.hpp>
#include <iostream>

#include <boost/filesystem/operations.hpp>

#include "ioData/TransformationMatrix3DReaderService.hpp"

namespace ioData
{

//-----------------------------------------------------------------------------

REGISTER_SERVICE( ::io::IReader , ::ioData::TransformationMatrix3DReaderService , ::fwData::TransformationMatrix3D ) ;

//-----------------------------------------------------------------------------

TransformationMatrix3DReaderService::TransformationMatrix3DReaderService() :
	m_filename ("")
{}

//-----------------------------------------------------------------------------

void TransformationMatrix3DReaderService::info(std::ostream &_sstream )
{
	this->SuperClass::info( _sstream ) ;
	_sstream << std::endl << " TransformationMatrix3D object reader" ;
}

//-----------------------------------------------------------------------------

std::vector< std::string > TransformationMatrix3DReaderService::getSupportedExtensions()
{
	std::vector< std::string > extensions ;
	extensions.push_back(".trf");
	return extensions ;
}

//-----------------------------------------------------------------------------

void TransformationMatrix3DReaderService::starting( ) throw(::fwTools::Failed)
{
	SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

TransformationMatrix3DReaderService::~TransformationMatrix3DReaderService() throw()
{}

//-----------------------------------------------------------------------------

void TransformationMatrix3DReaderService::configuring( ) throw(::fwTools::Failed)
{
	OSLM_TRACE( "TransformationMatrix3DReaderService::configure : " << *m_configuration );
	if( m_configuration->findConfigurationElement("filename") )
	{
		std::string filename = m_configuration->findConfigurationElement("filename")->getValue() ;
		boost::filesystem::path location = boost::filesystem::path( filename ) ;
		m_filename = location;
	}
}

//-----------------------------------------------------------------------------

void TransformationMatrix3DReaderService::configureWithIHM()
{
	SLM_TRACE_FUNC();
	static wxString _sDefaultPath = _("");
	wxString title = _("Choose a file to load a transformation matrix");
	wxString file = wxFileSelector(
			title,
			_sDefaultPath,
			wxT(""),
			wxT(""),
			wxT("TRF files (*.trf)|*.trf"),
			wxFD_OPEN | wxFD_FILE_MUST_EXIST,
			wxTheApp->GetTopWindow() );

	if( file.IsEmpty() == false )
	{
		m_filename = ::boost::filesystem::path( wxConvertWX2MB(file), ::boost::filesystem::native );
		_sDefaultPath = wxConvertMB2WX( m_filename.branch_path().string().c_str() );
	}
}

//-----------------------------------------------------------------------------

void TransformationMatrix3DReaderService::stopping() throw(::fwTools::Failed)
{
	SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void TransformationMatrix3DReaderService::updating() throw(::fwTools::Failed)
{
	SLM_TRACE_FUNC();

	// Retrieve object
	::boost::shared_ptr< ::fwData::TransformationMatrix3D > matrix = this->getObject< ::fwData::TransformationMatrix3D >( );
	assert( matrix ) ;

	::fwDataIO::reader::TransformationMatrix3DReader reader;
	reader.setObject( matrix );
	reader.setFile(m_filename);
	reader.read();

	// Notify reading
//	::boost::shared_ptr< ::fwServices::IEditionService > editor = ::fwServices::get< fwServices::IEditionService >( this->getObject() ) ;
	//::boost::shared_ptr< ::fwComEd::TransformationMatrix3DMsg > msg( new ::fwComEd::TransformationMatrix3DMsg(this->getObject< ::fwData::TransformationMatrix3D >()) ) ;
	//msg->setValueModif(true);
	::fwComEd::TransformationMatrix3DMsg::NewSptr msg;
	msg->addEvent( ::fwComEd::TransformationMatrix3DMsg::MATRIX_IS_MODIFIED );
//	editor->notify( msg );
	::fwServices::IEditionService::notify(this->getSptr(), this->getObject(), msg);
}

//-----------------------------------------------------------------------------

}
