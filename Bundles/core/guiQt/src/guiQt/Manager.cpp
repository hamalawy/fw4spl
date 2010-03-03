/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#include <cassert>
#include <vector>

#include <fwCore/base.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/bundle/runtime.hpp>

#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>

#include <iostream>

#include "guiQt/Manager.hpp"

#include <QApplication>
#include <QPushButton>
#include <QMainWindow>
#include <QFrame>
#include <QMenuBar>
#include <QDialog>
#include <QWidget>
	


namespace guiQt
{
/**
 * @brief Forward menu event received to the manager in order to update/execute the appropriate service
 * @author IRCAD (Research and Development Team).
 */

/**
 * @brief Application top window
 */


::guiQt::Manager::sptr Manager::m_instance;

//-----------------------------------------------------------------------------
Manager::Manager() throw()
{
	
}

//-----------------------------------------------------------------------------

Manager::~Manager() throw()
{
	
}


void Manager::initialize()
{
  int argc = 1;
  char* argv[1] = {"test"};
  
  QApplication app( argc,  argv);
  
  QWidget widget;
  widget.resize(950, 750);
  widget.show();
   
  app.setActiveWindow(&widget);

  
 //       ::fwServices::get< ::guiQt::aspect::IQtAspect >( getDefault()->m_unlessObject )->configure() ;
   //     ::fwServices::get< ::guiQt::aspect::IQtAspect >( getDefault()->m_unlessObject )->start();
   
    ::fwServices::OSR::initializeRootObject();
   
   app.exec();
}



const ::guiQt::Manager::sptr Manager::getDefault() throw()
{
    if(m_instance.get() == 0)
    {
        m_instance = ::guiQt::Manager::NewSptr();
    }
    return m_instance;
}

void Manager::registerAction( ::guiQt::action::IAction::sptr _action)
{
    SLM_ASSERT("Sorry, Action expired", _action);
    int id = _action->getId();

    OSLM_ASSERT("Sorry, Action with id " <<id<< " is already registered!",
            getDefault()->m_serviceCallBacks.find(id) == getDefault()->m_serviceCallBacks.end() );

    // Associated action and id
    std::cout<<" setAction : "<<*_action<<" \n";
    getDefault()->m_serviceCallBacks[id]= _action ;  
    
    getDefault()->m_serviceCallBacks[id].lock()->update() ;
    
    
    std::cout<<"Action : "<<_action->getNameInMenu() <<" ; id : "<<id<<"\n\n";
    Manager::processAction(id);
    
}

void Manager::processAction(int id )
{
    SLM_ASSERT("Sorry, Action not found: "<< id ,
            getDefault()->m_serviceCallBacks.find(id) != getDefault()->m_serviceCallBacks.end());

    SLM_ASSERT("Sorry, Action expired",
            !getDefault()->m_serviceCallBacks[id].expired() ) ;

    SLM_ASSERT("Sorry, Action is disabled",
            getDefault()->m_serviceCallBacks[id].lock()->isEnable());

    std::cout<<" processAction \n";
    std::cout<<" id : "<< id<<"\n";
	    
    //getDefault()->m_serviceCallBacks[id].lock()->update() ;
}
} // namespace gui
