/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/helper.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/bundle/runtime.hpp>

#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include <QApplication>
#include "guiQt/aspect/IQtMenu.hpp"

namespace guiQt
{

namespace aspect
{


//-----------------------------------------------------------------------------

IQtMenu::IQtMenu() throw()
{}

//-----------------------------------------------------------------------------

IQtMenu::~IQtMenu() throw()
{}

//-----------------------------------------------------------------------------

void IQtMenu::configuring() throw( ::fwTools::Failed )
{
//     SLM_TRACE_FUNC();
//     SLM_FATAL_IF( "Sorry, missing menus configuration" , !m_configuration->findConfigurationElement("menus") );
//     ::fwRuntime::ConfigurationElement::sptr menusCfgElt = m_configuration->findConfigurationElement("menus");
//     std::vector < ::fwRuntime::ConfigurationElement::sptr > vectConfig = menusCfgElt->find("menu");
//     SLM_ASSERT("Sorry, no menu configuration specified", !vectConfig.empty());
// 
//     ::fwRuntime::ConfigurationElementContainer::Iterator iter;
//     for (iter = vectConfig.begin() ; iter != vectConfig.end() ; ++iter)
//     {
//         SLM_ASSERT("Sorry, uid attribute missing", (*iter)->hasAttribute("uid"));
//         m_menusUID.push_back( (*iter)->getExistingAttributeValue("uid") ) ;
//     }
}

//-----------------------------------------------------------------------------

void IQtMenu::starting() throw( ::fwTools::Failed )
{
  /* Recuperation QApp*/
  
    // Creation de la fenetre/widget?
    

    // Recupeartion de la menuBar cree dans IQtAspect
    

    // Creation de menus et ajout a la menuBar
    

    // Recuperation de menus
    


    // Demarrage des menus et actions
   
  std::cout<< "---> menu.start()" << std::endl;

  QWidget *mainWidget = qApp->activeWindow();

	
  QMenuBar * menu = new QMenuBar(mainWidget);
  QMenu * file = new QMenu("File", mainWidget);
  menu->addMenu(file);
  menu->show();
  
  BOOST_FOREACH(std::string menuUID, m_menusUID)
  {
    //OSLM_FATAL_IF("Service "<<menuUID<<" doesn't exist.", ! ::fwTools::UUID::exist(menuUID, ::fwTools::UUID::SIMPLE ));
    ::fwServices::IService::sptr service = ::fwServices::get( menuUID ) ;
    ::guiQt::aspect::IQtMenu::sptr menu = ::guiQt::aspect::IQtMenu::dynamicCast(service);
    OSLM_FATAL_IF("Service "<<menuUID<<" is not a Menu.", !menu);
//    menu->start();

    }
}

//-----------------------------------------------------------------------------

void IQtMenu::stopping() throw( ::fwTools::Failed )
{
  /* Recuperation QApp*/
   
     // Recupeartion de la menuBar

    // Recuperation des menus
    
    // Arret des menus et actions
    
    // removes separators
   
}

//-----------------------------------------------------------------------------

void IQtMenu::updating() throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : an IMenu service does not be updated." );
}

//-----------------------------------------------------------------------------

void IQtMenu::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : an IMenu service does not received a message." );
}

//-----------------------------------------------------------------------------



}

}

