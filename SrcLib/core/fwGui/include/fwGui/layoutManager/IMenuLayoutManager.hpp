/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

/**
 * @file fwGui/IMenuLayoutManager.hpp
 * @brief This file defines the interface of the base class for managing a menu.
 *
 * @author IRCAD (Research and Development Team).
 * @date 2009-2010
 */

#ifndef _FWGUI_LAYOUTMANAGER_IMENULAYOUTMANAGER_HPP_
#define _FWGUI_LAYOUTMANAGER_IMENULAYOUTMANAGER_HPP_

#include <fwCore/base.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "fwGui/container/fwMenu.hpp"
#include "fwGui/container/fwMenuItem.hpp"
#include "fwGui/IMenuItemCallback.hpp"
#include "fwGui/config.hpp"

namespace fwGui
{
namespace layoutManager
{

/**
 * @brief   Defines the menu layout manager for IHM.
 * @class   IMenuLayoutManager.
 * @author  IRCAD (Research and Development Team).
 * @date    2009-2010.
 *
 */
class FWGUI_CLASS_API IMenuLayoutManager : public ::fwCore::BaseObject
{
public:
    fwCoreNonInstanciableClassDefinitionsMacro( (IMenuLayoutManager)(::fwCore::BaseObject) )

    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;
    typedef std::string RegistryKeyType;
    typedef std::vector< ::fwGui::IMenuItemCallback::sptr > CallbacksType;

    typedef enum {
        DEFAULT,
        QUIT,
        ABOUT,
        HELP,
        NEW,
        SEPARATOR
    }ActionType;

    class ActionInfo
    {
    public :

        ActionInfo() :
            m_name(""),
            m_shortcut(""),
            m_isCheckable (false),
            m_isRadio(false),
            m_type(DEFAULT),
            m_isSeparator(false),
            m_isMenu(false)
        {}

        std::string m_name;
        std::string m_shortcut;
        bool        m_isCheckable;
        bool        m_isRadio;
        ActionType  m_type;
        bool        m_isSeparator;
        bool        m_isMenu;
    };

    FWGUI_API const static RegistryKeyType REGISTRY_KEY;

    /// Constructor. Do nothing.
    FWGUI_API IMenuLayoutManager();

    /// Destructor. Do nothing.
    FWGUI_API virtual ~IMenuLayoutManager();

    /**
     * @brief Returns the vector of fwMenuItem managed by this layout.
     */
    FWGUI_API virtual std::vector< ::fwGui::container::fwMenuItem::sptr > getMenuItems();

    /**
     * @brief Returns the vector of fwMenu managed by this layout.
     */
    FWGUI_API virtual std::vector< ::fwGui::container::fwMenu::sptr > getMenus();

    /**
     * @brief Initialize layout managers.
     *
     * Example of configuration
     * @verbatim
       <service uid="myMenu" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no" >
           <gui>
               <layout>
                   <menuItem name="My item 1" shortcut="1" style="check" />
                   <separator />
                   <menuItem name="My item 2" shortcut="2" style="radio" />
                   <menuItem name="My item 3" shortcut="3" style="radio" />
                   <separator />
                   <menuItem name="Quit" shortcut="Ctrl+Q" specialAction="QUIT" />
               </layout>
           </gui>
           <registry>
               <menuItem sid="item1" start="no" />
               <menuItem sid="item2" start="no" />
               <menuItem sid="item3" start="no" />
               <menuItem sid="actionQuit" start="no" />
           </registry>
       </service>
       @endverbatim
     * This method analyzes the gui section of the configuration.
     *
     *  - <layout> (mandatory) : give the list of the menu item that will appear in the menu.
     *  - <menuItem name="My item 1" shortcut="1" style="check" specialAction="NEW" /> :
     *   - \b name (mandatory) : give the name of the menu item that will appear in the interface.
     *   - \b shortcut : give the shortcut for this menu item.
     *   - \b style {check|radio} : give the style of the menu item.
     *   - \b specialAction {DEFAULT | QUIT | NEW | ABOUT | HELP}: specify a pre define action. If it isn't define the value is DEFAULT.
     *  - <separator/> : allow to divide the menu by part (draw a line).
     */

    FWGUI_API virtual void initialize( ConfigurationType configuration);

    /**
     * @brief Instantiate actions with parent menu.
     */
    FWGUI_API virtual void createLayout( ::fwGui::container::fwMenu::sptr parent ) = 0;

    /**
     * @brief Destroy local actions.
     * @pre services using this actions must be stopped before.
     */
    FWGUI_API virtual void destroyLayout() = 0;


    /**
     * @brief Set the action visibility.
     */
    FWGUI_API virtual void menuItemSetVisible(::fwGui::container::fwMenuItem::sptr, bool isVisible) = 0;

    /**
     * @brief Set the action enable or not.
     */
    FWGUI_API virtual void menuItemSetEnabled(::fwGui::container::fwMenuItem::sptr, bool isEnabled) = 0;

    /**
     * @brief Set the action checked or not.
     */
    FWGUI_API virtual void menuItemSetChecked(::fwGui::container::fwMenuItem::sptr, bool isChecked) = 0;

    /**
     * @brief Sets callbacks associate with menu items.
     */
    FWGUI_API virtual void setCallbacks(CallbacksType callbacks) {m_callbacks = callbacks;};

protected:

    /**
     * @brief Helper to destroy local actions.
     */
    FWGUI_API virtual void destroyActions();

    /// All actions managed by this layout.
    std::vector< ::fwGui::container::fwMenuItem::sptr > m_menuItems;

    /// All actions managed by this layout.
    std::vector< ::fwGui::container::fwMenu::sptr > m_menus;

    /// Save action informations from configuration.
    std::vector< ActionInfo > m_actionInfo;

    /// Callbacks associate with menu items
    CallbacksType m_callbacks;
};

} // namespace layoutManager
} // namespace fwGui

#endif /*_FWGUI_LAYOUTMANAGER_IMENULAYOUTMANAGER_HPP_*/


