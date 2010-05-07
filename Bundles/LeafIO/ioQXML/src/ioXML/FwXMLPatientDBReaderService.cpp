/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>

#include <fwCore/base.hpp>

#include <fwData/Composite.hpp>
#include <fwData/PatientDB.hpp>

#include <fwServices/Factory.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwComEd/PatientDBMsg.hpp>

#include <io/IReader.hpp>

#include <fwTools/System.hpp>

#include <fwXML/reader/FwXMLObjectReader.hpp>

#include <fwQt/ZipFolder.hpp>

#include "ioXML/FwXMLPatientDBReaderService.hpp"

#include <iostream>
#include <QFileDialog>
#include <QString>

//------------------------------------------------------------------------------
//
namespace ioXML
{

REGISTER_SERVICE( ::io::IReader , ::ioXML::FwXMLPatientDBReaderService , ::fwData::PatientDB );

//------------------------------------------------------------------------------

FwXMLPatientDBReaderService::FwXMLPatientDBReaderService() throw() :
    m_bServiceIsConfigured(false),
    m_fsPatientDBPath("")
{}

//------------------------------------------------------------------------------

FwXMLPatientDBReaderService::~FwXMLPatientDBReaderService() throw()
{}

//------------------------------------------------------------------------------

void FwXMLPatientDBReaderService::configuring() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    // Test if in the service configuration the tag filename is defined. If it is defined, the image path is initialized and we tag the service as configured.
    if( m_configuration->findConfigurationElement("filename") )
    {
        std::string filename = m_configuration->findConfigurationElement("filename")->getExistingAttributeValue("id") ;
        m_fsPatientDBPath = ::boost::filesystem::path( filename ) ;
        m_bServiceIsConfigured = ::boost::filesystem::exists(m_fsPatientDBPath);
        if(m_bServiceIsConfigured)
        {
            OSLM_TRACE("Filename found in service configuration : patient path = " << filename ) ;
        }
        else
        {
            OSLM_WARN("filename not exist = " <<  filename ) ;
        }
    }
}

//------------------------------------------------------------------------------

void FwXMLPatientDBReaderService::configureWithIHM()
{ 
    QString file = QFileDialog::getOpenFileName(0,QObject::tr("Open File"), QDir::currentPath(), QObject::tr("fwXML (*.fxz *.xml)"));
    
    if( file.isEmpty() == false )
    {
     //   fixFilename(file);
      m_fsPatientDBPath =  ::boost::filesystem::path( file.toStdString(), ::boost::filesystem::native );
      m_bServiceIsConfigured = true;
    }
}

//------------------------------------------------------------------------------
//
void FwXMLPatientDBReaderService::fixFilename(QString _filename)
{
    m_fsPatientDBPath =  ::boost::filesystem::path( _filename.toStdString(), ::boost::filesystem::native );
    m_bServiceIsConfigured = true;

}

//------------------------------------------------------------------------------

void FwXMLPatientDBReaderService::starting() throw(::fwTools::Failed)
{
    SLM_TRACE("FwXMLPatientDBReaderService::starting()");
}

//------------------------------------------------------------------------------

void FwXMLPatientDBReaderService::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE("FwXMLPatientDBReaderService::stopping()");
}

//------------------------------------------------------------------------------

void FwXMLPatientDBReaderService::info(std::ostream &_sstream )
{
    _sstream << "FwXMLPatientDBReaderService::info" ;
}

//------------------------------------------------------------------------------

std::vector< std::string > FwXMLPatientDBReaderService::getSupportedExtensions()
{
    std::vector< std::string > extensions ;
    extensions.push_back(".xml");
    return extensions ;
}

//------------------------------------------------------------------------------

std::string FwXMLPatientDBReaderService::getSelectorDialogTitle()
{
    return "Choose a fxz or a xml file";
}

//------------------------------------------------------------------------------

::fwData::PatientDB::sptr FwXMLPatientDBReaderService::createPatientDB( const ::boost::filesystem::path inrFileDir )
{
    SLM_TRACE("FwXMLPatientDBReaderService::createPatientDB");
    ::fwXML::reader::FwXMLObjectReader myLoader;
    ::fwData::PatientDB::sptr pPatientDB;

    myLoader.setFile(inrFileDir);
    myLoader.read();
    
   // DialogBOx for WARNING here

    pPatientDB = ::fwData::PatientDB::dynamicCast( myLoader.getObject() );

    return pPatientDB;
}

//------------------------------------------------------------------------------

void FwXMLPatientDBReaderService::updating() throw(::fwTools::Failed)
{
    OSLM_TRACE("FwXMLPatientDBReaderService::updating()  m_fsPatientDBPath:"<<  m_fsPatientDBPath);

    if( !m_bServiceIsConfigured )
    {
       configureWithIHM();
    }
    if( m_bServiceIsConfigured )
    {

        ::fwData::PatientDB::sptr patientDB;
        if ( isAnFwxmlArchive( m_fsPatientDBPath ) )
        {
	  
	  std::cout<<" ARCHIVE file .fxz \n\n";
       //     patientDB = manageZipAndCreatePatientDB( m_fsPatientDBPath );
        }
        else
        {
	  std::cout<<"\n\n XML  : "<< m_fsPatientDBPath.string() <<" \n\n";
            patientDB = createPatientDB( m_fsPatientDBPath );
        }

        if ( patientDB != NULL )
        {
            if( patientDB->getPatientSize() > 0 )
            {
                // Retrieve dataStruct associated with this service
                ::fwData::PatientDB::sptr associatedPatientDB = this->getObject< ::fwData::PatientDB >();
                assert( associatedPatientDB ) ;
                associatedPatientDB->shallowCopy( patientDB );
                notificationOfDBUpdate();
            }
            else
            {
                   // DialogBOx for WARNING here

            }
        }
        else
        {
               // DialogBOx for WARNING here

        }
    }
}

//------------------------------------------------------------------------------

void FwXMLPatientDBReaderService::notificationOfDBUpdate()
{
    SLM_TRACE("FwXMLPatientDBReaderService::notificationOfDBUpdate");
    ::fwData::PatientDB::sptr pDPDB = this->getObject< ::fwData::PatientDB >();
    assert( pDPDB );

    ::fwComEd::PatientDBMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::PatientDBMsg::NEW_PATIENT, pDPDB );
    msg->addEvent( ::fwComEd::PatientDBMsg::NEW_LOADED_PATIENT );

    ::fwServices::IEditionService::notify( this->getSptr(),  pDPDB, msg );
}

//------------------------------------------------------------------------------

bool FwXMLPatientDBReaderService::isAnFwxmlArchive( const ::boost::filesystem::path filePath )
{
    return ( ::boost::filesystem::extension(filePath) == ".fxz" );
}

//------------------------------------------------------------------------------

::fwData::PatientDB::sptr FwXMLPatientDBReaderService::manageZipAndCreatePatientDB( const ::boost::filesystem::path _pArchivePath )
{

    ::fwData::PatientDB::sptr patientDB;

    // Unzip folder
    ::boost::filesystem::path destFolder = ::fwTools::System::getTemporaryFolder() / "fwxmlArchiveFolder";
    ::boost::filesystem::path xmlfile = destFolder / "patient.xml";

    OSLM_DEBUG("srcZipFileName = " << _pArchivePath );
    OSLM_DEBUG("destFolderName = " << destFolder );

    QString srcZipFileName( _pArchivePath.string().c_str() );
    QString destFolderName( destFolder.string().c_str() );
    
    
 //   ::fwQt::ZipFolder::unpackFolder( srcZipFileName, destFolderName );
 
    std::cout<<"\n\n XML  : "<< xmlfile.string() <<" \n";

    // Load
    patientDB = createPatientDB( xmlfile );

    // Remove temp folder
    ::boost::filesystem::remove_all( destFolder );

    return patientDB;
}

//------------------------------------------------------------------------------

} // namespace ioXML
