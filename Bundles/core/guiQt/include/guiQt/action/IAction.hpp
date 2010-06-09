/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
#ifndef _GUIQT_ACTION_IACTION_HPP_
#define _GUIQT_ACTION_IACTION_HPP_

#include <fwServices/IService.hpp>
#include "guiQt/config.hpp"

#include <QApplication>
#include <QAction>
#include <QMenu>

// Ajouter convertisseur string Qt

//#include "guiQt/action/Shortcut.hpp"

namespace guiQt
{


namespace action
{

class Action;

class GUIQT_CLASS_API IAction : public QObject, public ::fwServices::IService
{

public :

    fwCoreNonInstanciableClassDefinitionsMacro ( (IAction)( ::fwServices::IService::Baseclass) ) ;
    fwCoreAllowSharedFromThis();

    /**
    * @brief Constructor. Do nothing (Just initialize parameters).
    *
    * By default, the m_shortcutDef is not defined.
    */
    GUIQT_API IAction() throw() ;

    /*
     * @brief Destructor. Do nothing.
     */
    GUIQT_API virtual ~IAction() throw() ;

    /**
     * @brief This method is used to change the menu name (IAction::m_menuName).
     *
     * @param[in] _menuName Name of the new menu.
     */
     GUIQT_API void setMenuName(std::string _menuName) ;

    /**
     * @brief This method is used to get the id of a MenuItem that represents this action.
     *
     * @return MenuItem id.
     */
     GUIQT_API int getId() ;

    /**
     * @brief This method is used to get the name of a MenuItem.
     *
     * @return MenuItem name.
     */
     GUIQT_API std::string getNameInMenu() ;

     GUIQT_API QAction* getMenuItem();

    /**
     * @brief This method is used to get the name of a Menu.
     *
     * @return Menu name.
     */
     GUIQT_API std::string getMenuName() ;

    /**
     * @brief This method is used to get the MenuItem.
     *
     * @return MenuItem corresponding to this action.
     */
     GUIQT_API QAction* getMenuAction() ;

    /**
    * @brief    Check if the action is activated or not.
    *
    * @return   true if the action is activated.
    */
     GUIQT_API bool isEnable();

    /**
     * @brief   Activate or not this action. unable action, unable the MenuItem also.
     *
     * @param[in] _enable true if the action is activated.
     */
     GUIQT_API void setEnable(bool _enable);

    /**
     * @brief   Check or uncheck the MenuItem when it is possible.
     *
     * @param[in] _check true in order to check MenuItem.
     */
     // void setCheck(bool _check);

      GUIQT_API void createActions();

//      QMenu *currentMenu;s

protected :

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief This method is used to configure the class parameters.
     * @todo Remove assert on depreciated tag
     */
     GUIQT_API virtual void configuring() throw( ::fwTools::Failed ) ;

    /*
     * @brief Add a new item in MenuBar, and register this action in ::gui::Manager.
     */
     GUIQT_API virtual void starting() throw(::fwTools::Failed);

    /*
     * @brief This method remove the item situated in the menu and unregister this action in ::gui::Manager.
     */
      GUIQT_API virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief This method is used to update services on notification. Do nothing.
     * @attention An IAction service does not received a message.
     * @todo ACH: May this method be removed and imposed to children classes ??
     * @tode ACH : uncomment assertion
     */
      GUIQT_API virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /**
     * @brief This method is used to update services (it manages the check / uncheck).
     *
     * @todo ACH: It is really used and called by its children classes.
     */
      GUIQT_API virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief This method gives information about the class. Do nothing.
     */
     virtual void info(std::ostream &_sstream ) ;

    ///@}
private:
     Action *m_action;
};



} // namespace action
} // namespace gui

#endif

