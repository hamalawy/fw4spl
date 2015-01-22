/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUI_ACTION_STARTERACTIONSERVICE_HPP_
#define _GUI_ACTION_STARTERACTIONSERVICE_HPP_

#include <vector>
#include <fwServices/IService.hpp>

#include <fwGui/IActionSrv.hpp>

#include "gui/config.hpp"

namespace gui
{
namespace action
{

/**
 * @brief   Apply an action (start, stop, ...) on a service specify by uid.
 * @class   StarterActionService
 *
 * This action works on a ::fwData::Object. It does the action specify by the specify config.
 * This action can be :
 *   - Start a service :
 * @verbatim
           <service uid="actionUid" type="::fwGui::IActionSrv" impl="::gui::action::StarterActionService" autoConnect="no">
              <start uid="Uid_of_the_service" />
           </service>
   @endverbatim
 * If the service is stopped, the service specified by "Uid_of_the_service" is started and updated. Otherwise it is just updated.
 *
 *   - Start a service if exists :
 * @verbatim
           <service uid="actionUid" type="::fwGui::IActionSrv" impl="::gui::action::StarterActionService" autoConnect="no">
              <start_if_exists uid="Uid_of_the_service" />
           </service>
   @endverbatim
 * Test if the service specified by "Uid_of_the_service" exists before starting it.
 *
 *   - Stop a service :
 * @verbatim
           <service uid="actionUid" type="::fwGui::IActionSrv" impl="::gui::action::StarterActionService" autoConnect="no">
              <stop uid="Uid_of_the_service" />
           </service>
   @endverbatim
 *  If the service specified by "Uid_of_the_service" has been started, it is stopped. Otherwise, nothing appends.
 *
 *   - Stop a service if exists :  Test if the service exist before stopping it
 * @verbatim
           <service uid="actionUid" type="::fwGui::IActionSrv" impl="::gui::action::StarterActionService" autoConnect="no">
              <stop_if_exists uid="Uid_of_the_service" />
           </service>
   @endverbatim
 *  Test if the service specified by "Uid_of_the_service" exists before stopping it.
 *
 *   - Start or stop the service:
 * @verbatim
           <service uid="actionUid" type="::fwGui::IActionSrv" impl="::gui::action::StarterActionService" autoConnect="no">
              <start_or_stop uid="Uid_of_the_service" />
           </service>
   @endverbatim
 * If the service is stopped, this works as start the service. If the service is started, this works as stop the service.
 *
 * To notice : when the starterActionService is stopped, it stops all the associated services which have been started by itself.
 */

class GUI_CLASS_API StarterActionService : public ::fwGui::IActionSrv
{

public :

    fwCoreServiceClassDefinitionsMacro ( (StarterActionService)(::fwGui::IActionSrv) ) ;
    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;

    /**
    * @brief Constructor. Do nothing.
    */
    GUI_API StarterActionService() throw();

    /**
    * @brief Destructor. Do nothing.
    */
    GUI_API virtual ~StarterActionService() throw();

protected:

    enum ActionType {
        START,
        STOP,
        START_OR_STOP,
        START_IF_EXISTS,
        STOP_IF_EXISTS,
        DO_NOTHING
    };

    /**
     * @brief This method gives information about the class. Do nothing.
     */
    GUI_API virtual void info(std::ostream &_sstream ) ;

    /**
     * @brief This method starts-updates or stops the specified services
     */
    GUI_API void updating()throw (fwTools::Failed);

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
    GUI_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /**
     * @brief This method is used to configure the service parameters: specifies which services must be started or stopped
     */
    GUI_API void configuring() throw( ::fwTools::Failed );

    GUI_API virtual void starting() throw(::fwTools::Failed);

    /**
     * @brief Uninitialized the service activity.
     * All services started by this action are stopped.
     */
    GUI_API virtual void stopping() throw(::fwTools::Failed);

private:
    typedef ::fwTools::fwID::IDType IDSrvType;
    typedef std::pair< IDSrvType, ActionType > PairIDActionType;
    typedef std::vector<PairIDActionType> VectPairIDActionType;
    typedef std::set<IDSrvType> SetIDSrvType;

    // vector representing uuid's services that must be started (true) or stopped (false)
    VectPairIDActionType m_uuidServices;
    SetIDSrvType m_idStartedSrvSet;
};


} // namespace action
} // namespace gui


#endif /*_GUI_ACTION_STARTERACTIONSERVICE_HPP_*/
