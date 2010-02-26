/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/assign/list_of.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>

#include "guiQt/action/IAction.hpp"
#include "guiQt/Manager.hpp"

#include <iostream>

#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QString>

namespace guiQt
{
namespace action
{
//const std::map<std::string, int> IAction::SPECIAL_ACTION_TO_ID = ::boost::assign::map_list_of("QUIT",sQ);

IAction::IAction() throw() : m_shortcutDef(""),
                    m_isCheckable(false),
                    m_isRadio(false),
                    m_isCheck(false),
                    m_enable(true)
{
}

//-----------------------------------------------------------------------------

IAction::~IAction() throw()
{
}

//-----------------------------------------------------------------------------

void IAction::configuring() throw( ::fwTools::Failed )
{
  
 std::cout<<"\n IAction::configuring() \n\n";

  
  
    SLM_TRACE("configuring action") ;
    SLM_ASSERT("id tag deprecated", !m_configuration->hasAttribute("id"));
    SLM_ASSERT("menu tag deprecated", !m_configuration->hasAttribute("menu"));
    SLM_ASSERT("name tag missing", m_configuration->hasAttribute("name"));

    if( m_configuration->hasAttribute("specialAction") )
    {
        std::string specialActionName = m_configuration->getExistingAttributeValue("specialAction") ;
	
	std::cout<<" Special A : "<<specialActionName<<"\n\n";
	
        OSLM_TRACE("Action identifier : " << specialActionName ) ;
        //m_actionIdInMenu = SPECIAL_ACTION_TO_WXID.find(specialActionName)->second;
    }
    else
    {
      
      // 
       // m_actionIdInMenu  = wxNewId();
    }

    if( m_configuration->hasAttribute("name") )
    {
        m_actionNameInMenu = m_configuration->getExistingAttributeValue("name") ;
        OSLM_TRACE("Action name : " << m_actionNameInMenu ) ;
    }
/*
    if( m_configuration->hasAttribute("shortcut") )
    {
        m_shortcutDef = m_configuration->getExistingAttributeValue("shortcut") ;
        OSLM_TRACE("shortcut : " << m_shortcutDef ) ;
    }*/
/*
    if( m_configuration->hasAttribute("style") )
    {
        std::string style = m_configuration->getExistingAttributeValue("style") ;
        OSLM_TRACE("style : " << style ) ;
        m_isCheckable = (style == "check");
        m_isRadio = (style == "radio");

        if ((m_isCheckable || m_isRadio) && m_configuration->hasAttribute("state") )
        {
            std::string state = m_configuration->getExistingAttributeValue("state");
            m_isCheck = (state == "checked");
        }
    }
    if( m_configuration->hasAttribute("enable") )
    {
        std::string enable = m_configuration->getExistingAttributeValue("enable") ;
        OSLM_TRACE("enable : " << enable ) ;
        m_enable = (enable =="true");
    }*/
}

//-----------------------------------------------------------------------------

void IAction::info(std::ostream &_sstream )
{
    _sstream << "IAction" << std::endl;
}

//-----------------------------------------------------------------------------

void IAction::starting() throw(::fwTools::Failed)
{
  
  std::cout<<" IAction::starting() \n\n";
  
    OSLM_TRACE("starting action " << this->getId() << " : info = " << *this) ;
    SLM_ASSERT("IAction must be associated with a menu", !m_menuName.empty());

    // Get the main widget
    QWidget *mainWidget = qApp->activeWindow();
   
   // QMenuBar *menuBar = mainWidget->findChild<QMenuBar *>();
   
   QList< QMenuBar*> allMenuBar =  mainWidget->findChildren<QMenuBar *>();
   QMenuBar *menuBar;
   
   if(!allMenuBar.isEmpty())
   {
     std::cout<<" \n List non vide \n"<< " Length : "<< allMenuBar.length();
     //menuBar = allMenus.at(0);
     menuBar = allMenuBar.first();
     
   }
    
     if(menuBar != 0)
  {
    std::cout<<" \n Pointeur menuBar non vide \n\n";
   
  }
  else
  {
    SLM_FATAL(" NO MENUBAR ");
  }

  
   std::cout<<" \n Nom menu : "<<m_menuName <<"\n\n";
   
   QString *s = new QString(m_menuName.c_str());

  // QMenu *menuFile = menuBar->findChild<QMenu *>(m_menuName.c_str());
   QList< QMenu*> allMenus =  mainWidget->findChildren<QMenu *>();
   QMenu *menuFile;

   if(!allMenus.isEmpty())
   {
     std::cout<<" \n List non vide \n"<< " Length : "<< allMenus.length();
     //menuBar = allMenus.at(0);
     menuFile = allMenus.first();
     
   }
   else
   {
     SLM_FATAL(" List Menus empty ");
   }

   

    // Adds menu item
    if(m_isCheckable || m_isRadio)
    {
  
    }
    else
    {    std::cout<<" \n ELSE \n\n";

      QAction *exit;
      exit = new QAction("Exit",menuFile);
      if(menuFile!=0)
       menuFile->addAction(exit);
      else
	SLM_FATAL(" NO MENUFILE ");
      QObject::connect(exit, SIGNAL(triggered()),qApp, SLOT(quit()));
    }

    //::guiQt::Manager::registerAction( this->getSptr() ) ;

   // setEnable(m_enable);
    //setCheck(m_isCheck);

}

//-----------------------------------------------------------------------------

void IAction::stopping() throw(::fwTools::Failed)
{
   
}

bool IAction::isEnable()
{
    return m_enable;
}

//-----------------------------------------------------------------------------
/*
void IAction::setCheck(bool _check)
{
    m_isCheck = _check;
    wxMenuItem* item = this->getMenuItem();
    if(item && item->IsCheckable() && (m_isCheckable || (_check && m_isRadio)))
    {
        item->Check(m_isCheck);
        wxFrame *frame = wxDynamicCast( wxTheApp->GetTopWindow() , wxFrame ) ;
        wxToolBar* toolBar =  frame->GetToolBar();
        if(toolBar != NULL)
        {
            if( toolBar->FindById( m_actionIdInMenu ) != NULL )
            {
                toolBar->ToggleTool(m_actionIdInMenu, m_isCheck);
            }
        }
    }
}
*/
//-----------------------------------------------------------------------------
/*
void IAction::setEnable(bool _enable)
{
    m_enable = _enable;
    wxMenuItem* item = getMenuItem();
    if(item)
    {
        item->Enable( m_enable);
        wxFrame *frame = wxDynamicCast( wxTheApp->GetTopWindow() , wxFrame ) ;
        wxToolBar* toolBar =  frame->GetToolBar();
        if(toolBar != NULL)
        {
            if( toolBar->FindById( m_actionIdInMenu ) != NULL )
            {
                toolBar->EnableTool(m_actionIdInMenu, m_enable);
            }
        }
    }
}
*/
//-----------------------------------------------------------------------------

void IAction::updating() throw(::fwTools::Failed)
{
    SLM_TRACE("IAction::updating");
    if (!m_isRadio)
    {
        setCheck(!m_isCheck);
    }
}

//-----------------------------------------------------------------------------

void IAction::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("an IAction service does not received a message." );
}

//-----------------------------------------------------------------------------

void IAction::setMenuName(std::string _menuName)
{
    m_menuName = _menuName ;
}

//-----------------------------------------------------------------------------

int IAction::getId()
{
    return m_actionIdInMenu ;
}

//-----------------------------------------------------------------------------

std::string IAction::getNameInMenu()
{
    return m_actionNameInMenu ;
}

//-----------------------------------------------------------------------------

std::string IAction::getMenuName()
{
    return m_menuName ;
}

//-----------------------------------------------------------------------------
/*
wxMenuItem* IAction::getMenuItem()
{
    // get Frame
    wxFrame *frame = wxDynamicCast( wxTheApp->GetTopWindow() , wxFrame ) ;
    SLM_ASSERT( "No wxFrame", frame ) ;

    // get MenuBar
    SLM_ASSERT( "No menu bar: MenuBar must be created by IAspect", frame->GetMenuBar() );
    wxMenuBar *menuBar =  frame->GetMenuBar();

    // get Menu
    SLM_ASSERT("Menu must exist", menuBar->FindMenu( ::fwWX::std2wx( m_menuName ) ) != wxNOT_FOUND );
    wxMenu *menuFile = menuBar->GetMenu( menuBar->FindMenu( ::fwWX::std2wx( m_menuName ) ) ) ;

    return menuFile->FindItem( m_actionIdInMenu );
}*/

//-----------------------------------------------------------------------------
/*
::gui::action::Shortcut::csptr IAction::getShortcut() const
{
    return m_shortcut;
}

//-----------------------------------------------------------------------------

::gui::action::Shortcut::sptr IAction::getShortcut()
{
    return m_shortcut;
}

//-----------------------------------------------------------------------------

void IAction::setShortcut( ::gui::action::Shortcut::sptr _shortcut )
{
    m_shortcut = _shortcut;
}
*/

}
}
