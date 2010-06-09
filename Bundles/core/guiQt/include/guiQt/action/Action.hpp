/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
#ifndef _GUIQT_ACTION_ACTION_HPP_
#define _GUIQT_ACTION_ACTION_HPP_

#include <fwServices/IService.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include <QApplication>
#include <QAction>
#include <QMenu>

// Ajouter convertisseur string Qt

//#include "guiQt/action/Shortcut.hpp"

namespace guiQt
{


namespace action
{


class Action : public QObject
{
    Q_OBJECT;

public :

//    fwCoreNonInstanciableClassDefinitionsMacro ( (Action)( ::fwServices::IService::Baseclass) ) ;
//    fwCoreAllowSharedFromThis();

    /**
    * @brief Constructor. Do nothing (Just initialize parameters).
    *
    * By default, the m_shortcutDef is not defined.
    */
    Action() throw() ;
    Action(std::string shortcutDef, bool isCheckable, bool isRadio, bool isCheck, bool enable) throw();

    /*
     * @brief Destructor. Do nothing.
     */
    virtual ~Action() throw() ;

    /**
     * @brief This method is used to change the menu name (Action::m_menuName).
     *
     * @param[in] _menuName Name of the new menu.
     */
     void setMenuName(std::string _menuName) ;

    /**
     * @brief This method is used to get the id of a MenuItem that represents this action.
     *
     * @return MenuItem id.
     */
     int getId() ;

    /**
     * @brief This method is used to get the name of a MenuItem.
     *
     * @return MenuItem name.
     */
     std::string getNameInMenu() ;

     QAction* getMenuItem();

    /**
     * @brief This method is used to get the name of a Menu.
     *
     * @return Menu name.
     */
     std::string getMenuName() ;

    /**
     * @brief This method is used to get the MenuItem.
     *
     * @return MenuItem corresponding to this action.
     */
     QAction* getMenuAction() ;

    /**
     * @brief   Retrieves the constant shortcut for the action.
     *
     * @return  Constant shared pointer to the shortcut.
     */
    // ::guiQt::action::Shortcut::csptr getShortcut() const;


    /**
     * @brief   Retrieves the shortcut for the action.
     *
     * @return  Shared pointer to the shortcut.
     */
    //  ::guiQt::action::Shortcut::sptr getShortcut();


    /**
     * @brief   Installs a new shortcut to the action.
     *
     * @param[in]   _shortcut   Shared pointer to the new shortcut.
     */
    // void setShortcut( ::gui::action::Shortcut::sptr _shortcut );

    /**
    * @brief    Check if the action is activated or not.
    *
    * @return   true if the action is activated.
    */
     bool isEnable();

    /**
     * @brief   Activate or not this action. unable action, unable the MenuItem also.
     *
     * @param[in] _enable true if the action is activated.
     */
     void setEnable(bool _enable);

    /**
     * @brief   Check or uncheck the MenuItem when it is possible.
     *
     * @param[in] _check true in order to check MenuItem.
     */
     // void setCheck(bool _check);

      void createActions();

      QMenu *currentMenu;

      virtual void configuring(::fwRuntime::ConfigurationElement::sptr configuration) throw( ::fwTools::Failed ) ;

     /*
      * @brief Add a new item in MenuBar, and register this action in ::gui::Manager.
      */
      virtual void starting() throw(::fwTools::Failed);

     /*
      * @brief This method remove the item situated in the menu and unregister this action in ::gui::Manager.
      */
       virtual void stopping() throw(::fwTools::Failed);
       /** @name Service methods ( override from ::fwServices::IService )
        * @{
        */
         /**
          * @brief This method is used to configure the class parameters.
          * @todo Remove assert on depreciated tag
          */

       /**
        * @brief This method is used to update services on notification. Do nothing.
        * @attention An Action service does not received a message.
        * @todo ACH: May this method be removed and imposed to children classes ??
        * @tode ACH : uncomment assertion
        */
         virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

       /**
        * @brief This method is used to update services (it manages the check / uncheck).
        *
        * @todo ACH: It is really used and called by its children classes.
        */
         virtual void updating() throw(::fwTools::Failed);

       /**
        * @brief This method gives information about the class. Do nothing.
        */
        virtual void info(std::ostream &_sstream ) ;
        ///@}
public slots :
      void run();

protected :

    static const std::map<std::string, int> SPECIAL_ACTION_TO_ID;

    /// To know if the action is checkable.
    bool m_isCheckable;

    /// To know if the action is a radio button.
    bool m_isRadio;

    /// To know if the action is check or uncheck when action is checkable.
    bool m_isCheck;

    /// To know if the action is disable or unable.
    bool m_enable;

    /// Contains the definition of the shortcut.  Example "F1".
    std::string m_shortcutDef;

    /// a shared pointer to the shortcut or null when none.
 //   ::gui::action::Shortcut::sptr   m_shortcut;

    /// Id of the MenuItem.
    int m_actionIdInMenu ;

    /// Name of the MenuItem.
    std::string m_actionNameInMenu ;

    /// Name of the Menu.
    std::string m_menuName ;

    // Action
    QAction *m_action;

    /**
     * @brief Configuration element used to configure service internal state using a generic XML like structure
     */
    ::fwRuntime::ConfigurationElement::sptr  m_configuration ;

};



} // namespace action
} // namespace gui

#endif

