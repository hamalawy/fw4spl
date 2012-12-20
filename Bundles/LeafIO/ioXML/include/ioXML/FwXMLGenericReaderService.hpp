/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#ifndef _IOXML_FWXMLGENERICREADERSERVICE_HPP_
#define _IOXML_FWXMLGENERICREADERSERVICE_HPP_

#include <string>
#include <boost/filesystem/path.hpp>

#include <fwComEd/helper/Composite.hpp>
#include <fwXML/reader/FwXMLObjectReader.hpp>
#include <io/IReader.hpp>

#include "ioXML/config.hpp"


namespace ioXML
{

/**
 * @class FwXMLGenericReaderService
 * @brief A generic reader which can load any kind of object in a fwXML archive
 * after loading data are shallow copied then some notifications are send. configuration
 * of service allow to define what event are send
 * @example :
 * <notify message="IMAGE_NEW" attachData="yes">
 * if attachData attribute is set to true then the loaded object is inserted in the dataInfo holder of the message
 * @TODO : create a factory on ObjMsg ( thanks to classFactoryRegistry)then change configuration to  <notify type="::fwComEd::ImageMsg" message="IMAGE_NEW" attachData="yes">
 */

class IOXML_CLASS_API FwXMLGenericReaderService : public ::io::IReader
{

public :
    fwCoreServiceClassDefinitionsMacro ( (FwXMLGenericReaderService)(::io::IReader) ) ;

    /**
     * @brief   constructor
     *
     */
    IOXML_API FwXMLGenericReaderService() throw();

    /**
     * @brief   destructor
     */
    IOXML_API virtual ~FwXMLGenericReaderService() throw();

protected:

    /** Search if archive extension is changed
     * @verbatim
     <service ... >
        <archiveExtension>.yaf</archiveExtension>
     </service>
     @endverbatim
     */
    IOXML_API virtual void configuring() throw(::fwTools::Failed);

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
    IOXML_API virtual std::vector< std::string > getSupportedExtensions() ;


    /// Override
    IOXML_API virtual void configureWithIHM();

    /// Change the extension of the compressed archive
    IOXML_API void setArchiveExtension( const std::string & _archiveExtenstion );

private :

    void notificationOfUpdate();

    ::fwData::Object::sptr loadData( const ::boost::filesystem::path inrFileDir );

    bool isAnFwxmlArchive( const ::boost::filesystem::path filePath );


    bool m_bServiceIsConfigured;

    ::boost::filesystem::path m_fsObjectPath;

    ::fwData::Object::sptr manageZipAndLoadData( const ::boost::filesystem::path path );
    ::boost::filesystem::path correctFileFormat( const ::boost::filesystem::path _filePath ) const;

    std::string m_archiveExtenstion;

    std::string m_inject;
};

} // namespace ioXML

#endif //_IOXML_FWXMLGENERICREADERSERVICE_HPP_
