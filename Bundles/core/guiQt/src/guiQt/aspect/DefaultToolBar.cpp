/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */



#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/bundle/runtime.hpp>

#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "guiQt/action/IAction.hpp"
#include "guiQt/aspect/DefaultToolBar.hpp"

#include <QMainWindow>
#include <QToolBar>
 #include <QSpinBox>
 
 
REGISTER_SERVICE( ::guiQt::aspect::IToolBar , ::guiQt::aspect::DefaultToolBar , ::fwTools::Object ) ;

namespace guiQt
{

namespace aspect
{

DefaultToolBar::DefaultToolBar() 
{}

//-----------------------------------------------------------------------------

DefaultToolBar::~DefaultToolBar() throw()
{}

//-----------------------------------------------------------------------------

void DefaultToolBar::starting()  throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

     
    QWidget *mainWidget = qApp->activeWindow();
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(mainWidget);

    
   // QList< QToolBar*> allToolBar =  mainWindow->findChildren<QToolBar *>();
    QToolBar *toolBar = new QToolBar("ToolBar",mainWindow);
    // ajouter posibilte choix position
    mainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

//      if(!allToolBar.isEmpty())
//    {
//      toolBar = allToolBar.first();
//    }
//    else
//    {
//      
//    }

     // If no menu bar yet : create it
    
     // Reconfiguring and starting appropriate actions
     ::fwRuntime::ConfigurationElementContainer::Iterator iter ;
     std::string name;
     for( iter = m_configuration->begin() ; iter != m_configuration->end() ; ++iter )
     {
         if( (*iter)->getName() == "toolBitmapSize" )
         {
             int height = -1;
             int width = -1;
             if((*iter)->hasAttribute("height"))
             {
                 height = ::boost::lexical_cast< int > ((*iter)->getExistingAttributeValue("height"));
             }
             if((*iter)->hasAttribute("width"))
             {
                 width = ::boost::lexical_cast< int > ((*iter)->getExistingAttributeValue("width"));
             }
        //     m_toolBar->SetToolBitmapSize( wxSize(width, height) );
         }

         if( (*iter)->getName() == "action" )
         {
             SLM_ASSERT("id tag depreciated", !(*iter)->hasAttribute("id"));
             SLM_ASSERT("uid attribute missing", (*iter)->hasAttribute("uid"));

             std::string uid =  (*iter)->getExistingAttributeValue("uid") ;
             ::guiQt::action::IAction::sptr action = ::guiQt::action::IAction::dynamicCast( ::fwServices::get(uid) );
             SLM_ASSERT("action missing", action);
             int id = action->getId();

             name = "";
             if((*iter)->hasAttribute("name"))
             {
                 name = (*iter)->getExistingAttributeValue("name");
             }

         }
         if( (*iter)->getName() == "separator" )
         {
          //   m_toolBar->AddSeparator();
         }
     }
    // m_toolBar->Realize();
}

//-----------------------------------------------------------------------------

void DefaultToolBar::stopping()  throw ( ::fwTools::Failed )
{
    // 
}

//-----------------------------------------------------------------------------

void DefaultToolBar::configuring()  throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void DefaultToolBar::updating() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void DefaultToolBar::updating(::fwServices::ObjectMsg::csptr _msg) throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void DefaultToolBar::info( std::ostream &_sstream )
{
    SLM_TRACE_FUNC();
}

}

}
