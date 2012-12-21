/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOGDCM_DICOMPATIENTWRITERSERVICE_HPP_
#define _IOGDCM_DICOMPATIENTWRITERSERVICE_HPP_

#include <string>
#include <boost/filesystem/path.hpp>

#include <io/IWriter.hpp>

#include "ioGdcm/config.hpp"

namespace fwData
{
    class Patient;
}


namespace ioGdcm
{

class IOGDCM_CLASS_API DicomPatientWriterService : public ::io::IWriter
{

public :
    fwCoreServiceClassDefinitionsMacro ( (DicomPatientWriterService)( ::io::IWriter) ) ;

    /**
     * @brief   constructor
     *
     */
    IOGDCM_API DicomPatientWriterService() throw();

    /**
     * @brief   destructor
     */
    IOGDCM_API virtual ~DicomPatientWriterService() throw();

protected:

    /// Override
    IOGDCM_API virtual void starting() throw(::fwTools::Failed);

    /// Override
    IOGDCM_API virtual void stopping() throw(::fwTools::Failed);

    /// Override
    IOGDCM_API void updating() throw(::fwTools::Failed);

    /// Override
    virtual void receiving( CSPTR(::fwServices::ObjectMsg) _msg ) throw(::fwTools::Failed){};

    /// Override
    IOGDCM_API void info(std::ostream &_sstream ) ;

    /// Override
    IOGDCM_API virtual std::vector< std::string > getSupportedExtensions() ;

    /// Override
    IOGDCM_API virtual std::string getSelectorDialogTitle();

    /// Override
    IOGDCM_API virtual void configureWithIHM();

    /// Return managed file type, here FOLDER
    IOGDCM_API ::io::IOPathType getIOPathType() const;

private :

    void savePatient(   const ::boost::filesystem::path patientDBPath,
            ::boost::shared_ptr< ::fwData::Patient > _pPatient );

};

} // namespace ioGdcm

#endif //_IOGDCM_DICOMPATIENTWRITERSERVICE_HPP_
