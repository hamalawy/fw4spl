/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/bundle/runtime.hpp>

#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "guiQt/action/IAction.hpp"
#include "guiQt/aspect/IMenu.hpp"
#include "guiQt/aspect/IAspect.hpp"
#include "guiQt/aspect/DefaultMenu.hpp"

 #include <QList>
 
 #include <iostream>
 #include <QString>
 
REGISTER_SERVICE( ::guiQt::aspect::IMenu , ::guiQt::aspect::DefaultMenu , ::fwTools::Object );

namespace guiQt
{

namespace aspect
{
const std::string  DefaultMenu::SEPARATOR_UID = "SEPARATOR_UID";

//-----------------------------------------------------------------------------

DefaultMenu::DefaultMenu() throw()
{}

//-----------------------------------------------------------------------------

DefaultMenu::~DefaultMenu() throw()
{}

//-----------------------------------------------------------------------------

void DefaultMenu::configuring() throw( ::fwTools::Failed )
{
    SLM_ASSERT("name tag missing", m_configuration->hasAttribute("name")) ;
    m_menuName = m_configuration->getExistingAttributeValue("name");

    ::fwRuntime::ConfigurationElementContainer::Iterator iter ;
    for( iter = m_configuration->begin() ; iter != m_configuration->end() ; ++iter )
    {
        if( (*iter)->getName() == "action" )
        {
            SLM_ASSERT("uid attribute missing", (*iter)->hasAttribute("uid"));
            SLM_ASSERT("uid attribute == SEPARATOR_UID", (*iter)->getExistingAttributeValue("uid")!=::guiQt::aspect::DefaultMenu::SEPARATOR_UID);
            m_actionsUID.push_back( (*iter)->getExistingAttributeValue("uid") ) ;
        }
        if( (*iter)->getName() == "separator" )
        {
            m_actionsUID.push_back( ::guiQt::aspect::DefaultMenu::SEPARATOR_UID ) ;
        }
    }

}

//-----------------------------------------------------------------------------

void DefaultMenu::starting() throw( ::fwTools::Failed )
{
  // Get the main widget
  QWidget *mainWidget = qApp->activeWindow();
  
 // QMenuBar *menuBar = mainWidget->findChild<QMenuBar *>();
   QList< QMenuBar*> allMenuBar =  mainWidget->findChildren<QMenuBar *>();
   QMenuBar *menuBar;
   
   if(!allMenuBar.isEmpty())
   {
    // std::cout<<" \n List non vide \n"<< " Length : "<< allMenuBar.length();
     //menuBar = allMenus.at(0);
     menuBar = allMenuBar.first();
     
   }
   else
   {
     SLM_FATAL(" List MenuBar empty ");
   }
   
  
  if(menuBar != 0)
  {
    //std::cout<<" \n Pointeur menuBar non vide \n\n";
    std::cout<<" Name :"<< m_menuName<<"\n\n";
    // recuperer nom du menu
    QMenu *menu = new QMenu(m_menuName.c_str(), mainWidget);
    menuBar->addMenu(menu);
   // menuBar->show(); // faire en dernier
  }
  else
  {
    SLM_FATAL(" NO MENUBAR ");
  }

  // start() actions
 

 std::vector< ::guiQt::action::IAction::sptr > allActions = ::fwServices::OSR::getServices< ::guiQt::action::IAction >() ;
    for(std::vector< std::string >::iterator iterUUID = m_actionsUID.begin() ; iterUUID != m_actionsUID.end() ; ++iterUUID )
    {
        
            bool actionIsFound = false;
            for(    std::vector< ::guiQt::action::IAction::sptr >::iterator iterAction = allActions.begin();
                    iterAction != allActions.end() && ! actionIsFound ;
                    ++iterAction )
            {
                if( (*iterAction)->getUUID() == *iterUUID )
                {
                    (*iterAction)->setMenuName( m_menuName ) ;
		    std::cout<<"---> Start Actions \n";
                    (*iterAction)->start() ;
                    actionIsFound = true;
                }
		
            }
            OSLM_ASSERT("Action "<<  *iterUUID << " Not Found", actionIsFound);    

    }  
       
     
    
    //
}

/*
    void DefaultMenu::registerAction( ::guiQt::action::IAction::sptr _action )
    {
      
    }*/



//-----------------------------------------------------------------------------

void DefaultMenu::stopping() throw( ::fwTools::Failed )
{
    
}

//-----------------------------------------------------------------------------

void DefaultMenu::updating() throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : an DefaultMenu service does not be updated." );
}

//-----------------------------------------------------------------------------

void DefaultMenu::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : an DefaultMenu service does not received a message." );
}

//-----------------------------------------------------------------------------

}

}
