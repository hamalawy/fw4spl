/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */



#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwComEd/PatientDBMsg.hpp>

#include <io/IWriter.hpp>

#include <fwCore/base.hpp>

#include <fwTools/System.hpp>

#include <fwData/PatientDB.hpp>

#include <fwXML/writer/FwXMLObjectWriter.hpp>

#include <fwQt/ZipFolder.hpp>

#include "ioXML/FwXMLPatientDBWriterService.hpp"

#include <iostream>
#include <QFileDialog>
#include <QString>

namespace ioXML
{

REGISTER_SERVICE( ::io::IWriter , ::ioXML::FwXMLPatientDBWriterService , ::fwData::PatientDB ) ;
//REGISTER_EXECUTABLE( ::ioXML::FwXMLPatientDBWriterService, "ioXML::FwXMLPatientDBWriterService" );

//------------------------------------------------------------------------------

FwXMLPatientDBWriterService::FwXMLPatientDBWriterService() throw() :
    m_bServiceIsConfigured(false),
    m_fsPatientDBPath("")
{}

//------------------------------------------------------------------------------

FwXMLPatientDBWriterService::~FwXMLPatientDBWriterService() throw()
{}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::configuring() throw(::fwTools::Failed)
{}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::configureWithIHM()
{
   
    //QString folder = QFileDialog::getSaveFileName(0,QObject::tr("Choose an vtk file to save image"), QDir::currentPath(), QObject::tr("fwXML archive (*.fxz)|*.fxz|fwXML (*.xml)|*.xml"));

    QString format = "xml";
    QString initialPath = QDir::currentPath() + QObject::tr("/untitled.") + format;
    QString folder = QFileDialog::getSaveFileName(0, QObject::tr("Choose an fxz or xml file "), initialPath,
                                QObject::tr("%1 Files (*.%2);;All Files (*)")
                                .arg(format.toUpper())
                                .arg(format));
				
    if( folder.isEmpty() == false)
    {
        m_fsPatientDBPath = ::boost::filesystem::path(  folder.toStdString(), ::boost::filesystem::native );
        m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE("FwXMLPatientDBWriterService::starting()");
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE("FwXMLPatientDBWriterService::stopping()");
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::info(std::ostream &_sstream )
{
    _sstream << "FwXMLPatientDBWriterService::info" ;
}

//------------------------------------------------------------------------------

std::string FwXMLPatientDBWriterService::getCfgExtensionPoint()
{
    return "" ;
}

//------------------------------------------------------------------------------

std::string FwXMLPatientDBWriterService::getPersistanceId()
{
    return "ioITK::FwXMLPatientDBWriterService" ;
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::savePatientDB( const ::boost::filesystem::path inrFileDir, ::fwData::PatientDB::sptr _pPatient )
{
    SLM_TRACE("FwXMLPatientDBWriterService::createPatientDB");
    ::fwXML::writer::FwXMLObjectWriter myWriter;

    myWriter.setObject(_pPatient);
    myWriter.setFile(inrFileDir);
    myWriter.write();
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::updating() throw(::fwTools::Failed)
{
    SLM_TRACE("FwXMLPatientDBWriterService::updating()");

    if( !m_bServiceIsConfigured )
    {
       configureWithIHM();
    }
    if( m_bServiceIsConfigured )
    {
        // Retrieve dataStruct associated with this service
        ::fwData::PatientDB::sptr associatedPatientDB = this->getObject< ::fwData::PatientDB >();
        assert( associatedPatientDB ) ;

        m_fsPatientDBPath = correctFileFormat( m_fsPatientDBPath );

        if ( isAnFwxmlArchive( m_fsPatientDBPath ) )
        {
            manageZipAndSavePatientDB(m_fsPatientDBPath,associatedPatientDB);
        }
        else
        {
            savePatientDB(m_fsPatientDBPath,associatedPatientDB);
        }
    }
}

//------------------------------------------------------------------------------

bool FwXMLPatientDBWriterService::isAnFwxmlArchive( const ::boost::filesystem::path filePath ) const
{
    return ( ::boost::filesystem::extension(filePath) == ".fxz" );
}

//------------------------------------------------------------------------------

::boost::filesystem::path FwXMLPatientDBWriterService::correctFileFormat( const ::boost::filesystem::path _filePath ) const
{
    ::boost::filesystem::path newPath = _filePath;
    OSLM_TRACE("Data saved in : " << _filePath.string());
    if ( ::boost::filesystem::extension(_filePath) != ".fxz" && ::boost::filesystem::extension(_filePath) != ".xml" )
    {
        newPath = _filePath.string() + ".fxz";
        OSLM_TRACE("File renamed in : " << newPath.string());
    }

    return newPath;
}

//------------------------------------------------------------------------------

void FwXMLPatientDBWriterService::manageZipAndSavePatientDB( const ::boost::filesystem::path inrFileDir, ::fwData::PatientDB::sptr _pPatient )
{
    // Save in tmp folder
    ::boost::filesystem::path srcFolder = ::fwTools::System::getTemporaryFolder() / "fwxmlArchiveFolder";
    ::boost::filesystem::path xmlfile = srcFolder / "patient.xml";
    //::boost::filesystem::create_directories( srcFolder );
    savePatientDB(xmlfile,_pPatient);

    // Zip
    QString destZipFileName( inrFileDir.string().c_str() );
    QString srcFolderName( srcFolder.string().c_str() );
    
   // ::fwQt::ZipFolder::packFolder( srcFolderName, destZipFileName );

    // Remove temp folder
   // ::boost::filesystem::remove_all( srcFolder );
}

//------------------------------------------------------------------------------
} // namespace ioXML
