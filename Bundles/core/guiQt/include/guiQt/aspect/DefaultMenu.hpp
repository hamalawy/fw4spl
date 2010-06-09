/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef GUIQT_ASPECT_DEFAULT_MENU_HPP_
#define GUIQT_ASPECT_DEFAULT_MENU_HPP_

#include "guiQt/config.hpp"


#include <set>

#include "guiQt/aspect/IMenu.hpp"
#include "guiQt/action/IAction.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

namespace guiQt
{

namespace aspect
{

/**
 * @brief   Defines the default menu for standard application
 * @class   DefaultMenu.
 * @author  IRCAD (Research and Development Team)

 * @date    2009.
 */
class GUIQT_CLASS_API DefaultMenu : public IMenu
{

public :

    /// Constructor. Do nothing.
    GUIQT_API DefaultMenu() throw();

    /// Destructor. Do nothing.
    GUIQT_API virtual ~DefaultMenu() throw();


   // static void registerAction( ::guiQt::action::IAction::sptr _action ) ;

protected :

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /// Retrieve DefaultMenu::m_menuName in configuration element
    GUIQT_API virtual void configuring() throw( ::fwTools::Failed ) ;

    GUIQT_API virtual void starting() throw( ::fwTools::Failed ) ;

    /**
     * @brief Stop all actions and separator of this menu and remove menu to wxMenuBar
     * @todo ACH : uncomment assert => all actions must be started
     * @todo Menu not empty on MAC with specials Actions like help, Quit...
     */
    GUIQT_API virtual void stopping() throw( ::fwTools::Failed ) ;

    /// Updating service, do nothing.
    GUIQT_API virtual void updating() throw(::fwTools::Failed);

    /// Updating service on notification, do nothing.
    GUIQT_API virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);
    ///@}

private:

    std::vector< std::string >      m_actionsUID ;
    std::string                     m_menuName ;
    const static std::string        SEPARATOR_UID;
};

}

}

#endif // GUI_ASPECT_DEFAULT_MENU_HPP_
