/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <fwTools/UUID.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/bundle/runtime.hpp>

#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

//#include "guiQt/action/IAction.hpp"
#include "guiQt/aspect/IMenu.hpp"
#include "guiQt/aspect/IAspect.hpp"
#include "guiQt/aspect/DefaultMenuBar.hpp"

#include <QApplication>



REGISTER_SERVICE( ::guiQt::aspect::IMenuBar , ::guiQt::aspect::DefaultMenuBar , ::fwTools::Object );

namespace guiQt
{

namespace aspect
{


//-----------------------------------------------------------------------------

DefaultMenuBar::DefaultMenuBar() throw()
{}

//-----------------------------------------------------------------------------

DefaultMenuBar::~DefaultMenuBar() throw()
{}

//-----------------------------------------------------------------------------

void DefaultMenuBar::configuring() throw( ::fwTools::Failed )
{
  /*  SLM_TRACE_FUNC();
    SLM_FATAL_IF( "Sorry, missing menus configuration" , !m_configuration->findConfigurationElement("menus") );
    ::fwRuntime::ConfigurationElement::sptr menusCfgElt = m_configuration->findConfigurationElement("menus");
    std::vector < ::fwRuntime::ConfigurationElement::sptr > vectConfig = menusCfgElt->find("menu");
    SLM_ASSERT("Sorry, no menu configuration specified", !vectConfig.empty());

    ::fwRuntime::ConfigurationElementContainer::Iterator iter;
    for (iter = vectConfig.begin() ; iter != vectConfig.end() ; ++iter)
    {
        SLM_ASSERT("Sorry, uid attribute missing", (*iter)->hasAttribute("uid"));
        m_menusUID.push_back( (*iter)->getExistingAttributeValue("uid") ) ;
    }*/
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::starting() throw( ::fwTools::Failed )
{
  QWidget *mainWidget = qApp->activeWindow();

  QMenuBar * menu = new QMenuBar(mainWidget);
  menu->resize(950,30);
   
  menu->show();  //sinon ajout menu incorrete
  
   
  BOOST_FOREACH(std::string menuUID, m_menusUID)
  {
    //OSLM_FATAL_IF("Service "<<menuUID<<" doesn't exist.", ! ::fwTools::UUID::exist(menuUID, ::fwTools::UUID::SIMPLE ));
    ::fwServices::IService::sptr service = ::fwServices::get( menuUID ) ;
    ::guiQt::aspect::IMenu::sptr menu = ::guiQt::aspect::IMenu::dynamicCast(service);
    OSLM_FATAL_IF("Service "<<menuUID<<" is not a Menu.", !menu);
//    menu->start();

   }
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::stopping() throw( ::fwTools::Failed )
{
   
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::updating() throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : an DefaultMenuBar service does not be updated." );
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : an DefaultMenuBar service does not received a message." );
}

//-----------------------------------------------------------------------------

}

}
