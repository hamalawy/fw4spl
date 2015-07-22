/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSERVICES_SCONFIGCONTROLLER_HPP__
#define __FWSERVICES_SCONFIGCONTROLLER_HPP__

#include <fwTools/Failed.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/EConfigurationElement.hpp>

#include "fwServices/helper/ConfigLauncher.hpp"
#include "fwServices/config.hpp"
#include "fwServices/IController.hpp"

namespace fwServices
{

/**
 * @class   SConfigController
 * @brief   To manage configuration file defines in xml extension.
 * @date    2010-2013.
 *
 * This action works on a ::fwData::Composite. It action starts/stops a template configuration given by its identifier in this action configuration.
 *  - You can specified pattern to replace in the template configuration by the tag 'replace'.
 *  - You can specified pattern to replace by the uid of the object contained by the composite with the given key
 *  - The pattern GENERIC_UID is replaced by a generated unique identifier when the configuration is launch.
 *  This assure that the created object and services in the configuration have a unique uid even if this
 *  configuration is launch several times.
 *
 * Example of this service configuration
 * @verbatim
   <service impl="::fwServices::SConfigController" type="::fwServices::IController">
       <config>
           <appConfig id="IdOfConfig" >
               <parameters>
                   <parameter replace="SERIESDB" by="medicalData"  />
                   <parameter replace="IMAGE" by="@values.image"  />
               </parameters>
           </appConfig>
       </config>
   </service>
   @endverbatim
 */
class FWSERVICES_CLASS_API SConfigController : public ::fwServices::IController
{

public:

    fwCoreServiceClassDefinitionsMacro ( (SConfigController)(::fwServices::IController) );

    /// Constructor. Does nothing.
    FWSERVICES_API SConfigController() throw();

    /// Destructor. Does nothing.
    FWSERVICES_API virtual ~SConfigController() throw();

protected:

    /// Starts the config
    virtual void starting() throw(::fwTools::Failed);

    /// Stops the config
    virtual void stopping() throw(::fwTools::Failed);

    /// Does nothing
    virtual void receiving( CSPTR(::fwServices::ObjectMsg) _msg ) throw(::fwTools::Failed);

    /// Does nothing
    virtual void updating() throw(::fwTools::Failed);


    /**
     * @brief Declare the configuration to associate with an operator type and a view config
     *
     * Call the IAction::configuring()
     *
     * Example of this service configuration
     * @verbatim
       <service impl="::fwServices::SConfigController" type="::fwServices::IController">
            <config>
                <appConfig id="IdOfConfig" >
                    <parameters>
                        <parameter replace="SERIESDB" by="medicalData"  />
                        <parameter replace="IMAGE" by="@values.image"  />
                    </parameters>
                </appConfig>
            </config>
       </service>
        @endverbatim
     * It MUST have at least one key node and at least one replace node.
     */
    virtual void configuring() throw(fwTools::Failed);

    /// Swaps the config
    virtual void swapping() throw ( ::fwTools::Failed );

    /// Overrides
    virtual void info( std::ostream &_sstream );

private:

    /// AppConfig manager
    ::fwServices::helper::ConfigLauncher::sptr m_configLauncher;

};

} // fwServices


#endif // __FWSERVICES_SCONFIGCONTROLLER_HPP__


