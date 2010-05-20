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
#include <QMenu>
#include <QAction>
#include <QList>
#include <QString>

namespace guiQt
{
namespace action
{

IAction::IAction() throw() : QObject(), ::fwServices::IService(),
		    m_shortcutDef(""),
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
    SLM_TRACE("configuring action") ;
    SLM_ASSERT("id tag deprecated", !m_configuration->hasAttribute("id"));
    SLM_ASSERT("menu tag deprecated", !m_configuration->hasAttribute("menu"));
    SLM_ASSERT("name tag missing", m_configuration->hasAttribute("name"));

    if( m_configuration->hasAttribute("specialAction") )
    {
        std::string specialActionName = m_configuration->getExistingAttributeValue("specialAction") ;
	
        OSLM_TRACE("Action identifier : " << specialActionName ) ;
    }
    else
    {
       // m_actionIdInMenu  = 3;
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
 // std::cout<<"    action " << this->getId() << " : info = " << *this;
//  std::cout<<"        m_menuName = "<<m_menuName<<" \n";
  
    OSLM_TRACE("starting action " << this->getId() << " : info = " << *this) ;
    SLM_ASSERT("IAction must be associated with a menu", !m_menuName.empty());

   // Get the main widget
   QWidget *mainWidget = qApp->activeWindow();
  
   QString *s = new QString(m_menuName.c_str());
   currentMenu = mainWidget->findChild<QMenu *>(m_menuName.c_str());
 
   createActions();
   
   // setEnable(m_enable);
    //setCheck(m_isCheck);
}

void IAction::run()
{ 
  this->update();
}


void IAction::createActions()
{
  m_action = new QAction(m_actionNameInMenu.c_str(), currentMenu);
  currentMenu->addAction(m_action);
  QObject::connect(m_action, SIGNAL(triggered()),this, SLOT(run()));

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
    
   
    QMenu *menuItem = this->getMenuItem();
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
      //  setCheck(!m_isCheck);
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
    //std::cout<<" IAction::setMenuName() \n";
    m_menuName = _menuName ;
}

//-----------------------------------------------------------------------------

std::string IAction::getMenuName()
{
    return m_menuName ;
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

QAction* IAction::getMenuItem()
{
  return m_action;
}

}
}
