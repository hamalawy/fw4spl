/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUI_ACTION_CONFIGACTIONSRVWITHKEY_HPP_
#define _GUI_ACTION_CONFIGACTIONSRVWITHKEY_HPP_

#include <fwTools/Failed.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/EConfigurationElement.hpp>

#include <gui/action/ConfigActionSrv.hpp>

#include "gui/config.hpp"

namespace gui
{
namespace action
{

/**
 * @class   ConfigActionSrvWithKey
 * @brief   To manage configuration file defines in xml extension.
 * 
 * @date    2010.
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
   <service impl="::gui::action::ConfigActionSrvWithKey" type="::fwGui::IActionSrv">
       <config id="IdOfTemplateConfig" />
       <replace val="VALUE" pattern="PATTERN_TO_REPLACE_BY_VALUE" />
       <key id="KEY" pattern="PATTERN_TO_REPLACE_BY_UID_OF_KEY" />
   </service>
   @endverbatim
 * @see ::gui::action::ConfigActionSrv
 */
class GUI_CLASS_API ConfigActionSrvWithKey : public ::gui::action::ConfigActionSrv
{

public :

    fwCoreServiceClassDefinitionsMacro ( (ConfigActionSrvWithKey)(::gui::action::ConfigActionSrv) ) ;

    /// Constructor. Do nothing.
    GUI_API ConfigActionSrvWithKey() throw() ;

    /// Destructor. Do nothing.
    GUI_API virtual ~ConfigActionSrvWithKey() throw() ;

protected:

    ///This method launches the IAction::starting method.
    virtual void starting() throw(::fwTools::Failed);

    ///This method launches the IAction::stopping method.
    virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief Management of observations ( overrides ).
     *
     * Stop configuration when it receives "WINDOW_CLOSED" event (ie. close the param view).
     */
    virtual void receiving( CSPTR(::fwServices::ObjectMsg) _msg ) throw(::fwTools::Failed);

    /**
     * @brief Starts the view and initialize the operator.
     */
    virtual void updating() throw(::fwTools::Failed);


    /**
     * @brief Declare the configuration to associate with an operator type and a view config
     *
     * Call the IAction::configuring()
     *
     * Example of this service configuration
     * @verbatim
       <service impl="::gui::action::ConfigActionSrvWithKey" type="::fwGui::IActionSrv">
           <config id="IdOfTemplateConfig" />
           <replace val="VALUE" pattern="PATTERN_TO_REPLACE_BY_VALUE" />
           <key id="KEY" pattern="PATTERN_TO_REPLACE_BY_UID_OF_KEY" />
       </service>
        @endverbatim
     * It MUST have at least one key node and at least one replace node.
     */
    virtual void configuring() throw(fwTools::Failed);

    /// Overrides
    virtual void info( std::ostream &_sstream ) ;

private:

    /**
     * @brief Read the configuration and show the parameters view.
     */
    void startConfig();

    /**
     * @brief Close the parameters view.
     */
    void stopConfig();

    typedef std::map< std::string, std::string > KeyAdaptorType;

    /**
     * @brief keep the association between the PATTERN and the associated key as m_keyAdaptors[PATTERN] = AssociatedKey.
     */
    KeyAdaptorType m_keyAdaptors;

};

} //action
} // GUI


#endif // _GUI_ACTION_CONFIGACTIONSRVWITHKEY_HPP_


