/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef GUI_ASPECT_DEFAULTMENUSRV_HPP_
#define GUI_ASPECT_DEFAULTMENUSRV_HPP_

#include <fwGui/IMenuSrv.hpp>

#include "gui/export.hpp"

namespace gui
{
namespace aspect
{

/**
 * @brief   Defines the default menu for standard application
 * @class   DefaultMenuSrv
 */
class GUI_CLASS_API DefaultMenuSrv : public ::fwGui::IMenuSrv
{

public :

    fwCoreServiceClassDefinitionsMacro ( (DefaultMenuSrv)(::fwGui::IMenuSrv) ) ;

    /// Constructor. Do nothing.
    GUI_API DefaultMenuSrv() throw();

    /// Destructor. Do nothing.
    GUI_API virtual ~DefaultMenuSrv() throw();

protected :

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief Configuring method allows to configure a menu with several actions.

     * Example of configuration
     * @verbatim
      <service uid="menuBar" type="::fwGui::IMenuBarSrv" impl="::gui::aspect::DefaultMenuBarSrv" autoConnect="no" >
          <gui>
              <layout>
                  <menu name="My Menu"/>
                  <menu name="My Menu 2"/>
              </layout>
          </gui>
          <registry>
              <menu sid="myMenu" start="yes" />
              <menu sid="myMenu2" start="yes" />
          </registry>
      </service>
       @endverbatim
     *  - \<gui\> \</gui\> : (mandatory) describe the interface of the service.
     *    - <separator /> : allows to put a separator in the menu
     *  - \<registry\> \</registry\> : (mandatory) describe the service management.
     *
     * @warning
     * - The number of item in the gui section must be equal or greater than in the registry section.
     * - The order of the menu in each section (gui and registry) must be the same.\n
     *   For example: the menu named "My Menu" will be connected with the service which have the sid = "myMenu".
     * - A menu bar can't have the same service connected on two different menu.
     *
     *  @see ::fwGui::IMenuBarSrv::initialize(), ::fwGui::layoutManager::IMenuLayoutManager::initialize()
     */
    GUI_API virtual void configuring() throw( ::fwTools::Failed ) ;

    /**
     * @brief Create the menu items and start the managed services.
     * @see ::fwGui::IMenuSrv::create()
     */
    GUI_API virtual void starting() throw( ::fwTools::Failed ) ;

    /**
     * @brief Stop all actions and separator of this menu and remove menu to menuBar
     * @see ::fwGui::IMenuSrv::destroy()
     */
    GUI_API virtual void stopping() throw( ::fwTools::Failed ) ;

    /// Updating service, do nothing.
    GUI_API virtual void updating() throw(::fwTools::Failed);

    /// Updating service on notification, do nothing.
    GUI_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);
    ///@}

};

}

}

#endif // GUI_ASPECT_DEFAULTMENUSRV_HPP_
