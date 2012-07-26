/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWSERVICES_REGISTRY_SERVICECONFIG_HPP_
#define _FWSERVICES_REGISTRY_SERVICECONFIG_HPP_

#include <map>

#include <fwTools/Object.hpp>
#include <fwTools/macros.hpp>

#include "fwServices/config.hpp"

namespace fwServices
{

namespace registry
{


/**
 * @class ServiceConfigInfo
 * @author  IRCAD (Research and Development Team).
 */
class FWSERVICES_CLASS_API ServiceConfigInfo : public ::fwCore::BaseObject
{
public :

    fwCoreClassDefinitionsWithFactoryMacro( (ServiceConfigInfo)(::fwCore::BaseObject), (()), new ServiceConfigInfo );

    /// Constructor, do nothing.
    ServiceConfigInfo(){}

    /// Destructor, do nothing.
    virtual ~ServiceConfigInfo(){}

    std::string service;
    std::string desc;
    ::fwRuntime::ConfigurationElement::csptr config;
};


/**
 * @brief This class allows to register all the service configuration which has the point extension
 *        "::fwServices::registry::ServiceConfig".
 * @class ServiceConfig
 * @author  IRCAD (Research and Development Team).
 *
 *
 * Registry config like :
 * @verbatim
<extension implements="::fwServices::registry::ServiceConfig">
    <id>IOSelectorVRMedPatientDBWriter</id>
    <service>::uiIO::editor::IOSelectorService</service>
    <desc>IOSelector config for VRMed patientDB writer</desc>
    <config>
        <type mode="writer" />
        <selection mode="exclude" />
        <addSelection service="::ioXML::FwXMLGenericWriterService" />
        <addSelection service="::uiDicomViewer::DicomPatientDBWriterService" />
    </config>
</extension>
   @endverbatim
 */
class FWSERVICES_CLASS_API ServiceConfig : public ::fwCore::BaseObject
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (ServiceConfig)(::fwCore::BaseObject), (()), new ServiceConfig) ;

    /// Return the default global instance of ServiceConfig
    FWSERVICES_API static ServiceConfig::sptr getDefault();

    /// Destructor
    FWSERVICES_API virtual ~ServiceConfig();

    /**
     * @brief Parses bundle information to retrieve service declaration.
     * @warning This method must be launch only once. The same extension will not be parsed twice.
     * @note This method is thread safe.
     */
    FWSERVICES_API void parseBundleInformation();

    /**
     * @brief Register a new service configuration
     * @param configId the identifier of the registered configuration.
     * @param service  the implementation of the service
     * @param desc     the description of the configuration
     * @param config   the registered config
     * @note This method is thread safe
     */
    FWSERVICES_API void addServiceConfigInfo( const std::string & configId,
                                              const std::string & service,
                                              const std::string & desc,
                                              ::fwRuntime::ConfigurationElement::csptr config );

    /**
     * @brief Returns the configuration with the given id for the service with the given implementation
     * @note This method is thread safe
     */
    FWSERVICES_API ::fwRuntime::ConfigurationElement::csptr getServiceConfig( const std::string & configId,
                                                                              const std::string &serviceImpl="" ) const;

    /**
     * @brief Returns a vector containing the names of the available config for the service with the given
     * implementation
     * @note This method is thread safe
     */
    FWSERVICES_API std::vector< std::string > getAllConfigForService( std::string serviceImpl ) const;

    /**
     * @brief Clear the registry.
     * @note This method is thread safe.
     */
    FWSERVICES_API void clearRegistry();

protected :

    typedef std::map< std::string, ServiceConfigInfo::sptr > Registry;

    /// Container of service information <configId, service config information>
    Registry m_reg;

    /// Constructor
    FWSERVICES_API ServiceConfig();

    const static std::string CONFIG_EXT_POINT;

    /// Used to protect the registry access.
    mutable ::fwCore::mt::ReadWriteMutex m_registryMutex;

    /// The global instance of the service config.
    static ServiceConfig::sptr s_currentServiceConfig;
};

} // namespace registry

} // namespace fwServices

#endif // _FWSERVICES_REGISTRY_SERVICECONFIG_HPP_


