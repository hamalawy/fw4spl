/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOXML_FWXMLPATIENT2PATIENTDBWRITERSERVICE_HPP_
#define _IOXML_FWXMLPATIENT2PATIENTDBWRITERSERVICE_HPP_

#include <string>
#include <boost/filesystem/path.hpp>

#include <io/IWriter.hpp>

#include "ioXML/config.hpp"


namespace fwData
{
    class Patient;
}


namespace ioXML
{

class IOXML_CLASS_API FwXMLPatient2PatientDBWriterService : public ::io::IWriter
{

public :
    fwCoreServiceClassDefinitionsMacro ( (FwXMLPatient2PatientDBWriterService)(::io::IWriter) ) ;

    /**
     * @brief   constructor
     *
     */
    IOXML_API FwXMLPatient2PatientDBWriterService() throw();

    /**
     * @brief   destructor
     */
    IOXML_API virtual ~FwXMLPatient2PatientDBWriterService() throw();

protected:

    /// Override
    IOXML_API virtual ::io::IOPathType getIOPathType() const;

    /// Override
    IOXML_API virtual void starting() throw(::fwTools::Failed);

    /// Override
    IOXML_API virtual void stopping() throw(::fwTools::Failed);

    /// Override
    IOXML_API void updating() throw(::fwTools::Failed);

    /// Override
    virtual void updating( ::boost::shared_ptr< const ::fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed) {} ;

    /// Override
    IOXML_API void info(std::ostream &_sstream ) ;

    /// Override
    IOXML_API virtual void configureWithIHM();

private :

    void savePatientDB( const ::boost::filesystem::path inrFileDir, ::boost::shared_ptr< ::fwData::PatientDB > _pPatientDB );

    bool isAnFwxmlArchive( const ::boost::filesystem::path filePath );
    void manageZipAndSavePatientDB( const ::boost::filesystem::path inrFileDir, ::fwData::PatientDB::sptr _pPatient );
    ::boost::filesystem::path correctFileFormat( const ::boost::filesystem::path _filePath ) const;

};

} // namespace ioXML

#endif //_IOXML_FWXMLPATIENT2PATIENTDBWRITERSERVICE_HPP_
