/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwTools/UUID.hpp>

#include <fwServices/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "guiQt/view/MultiView.hpp"

#include <QApplication>
#include <QDesktopWidget>

namespace guiQt
{
namespace view
{

//-----------------------------------------------------------------------------

REGISTER_SERVICE( ::guiQt::view::IView , ::guiQt::view::MultiView , ::fwTools::Object ) ;

//-----------------------------------------------------------------------------

MultiView::MultiView() throw()
{}

//-----------------------------------------------------------------------------

MultiView::~MultiView() throw()
{}

//-----------------------------------------------------------------------------

void MultiView::configuring() throw( ::fwTools::Failed )
{
  
  std::cout<<"   \n MultiView::configuring()  \n ";
  
    assert( m_configuration->getName() == "service" );
    SLM_FATAL_IF( "Depreciated tag \"win\" in configuration", m_configuration->findConfigurationElement("win") );

    SLM_FATAL_IF( "missing views configuration" , !m_configuration->findConfigurationElement("views") );

    ::fwRuntime::ConfigurationElement::sptr viewsCfgElt = m_configuration->findConfigurationElement("views");

    std::vector < ::fwRuntime::ConfigurationElement::sptr > vectConfig = viewsCfgElt->find("view");
    assert(!vectConfig.empty());

    m_panels.clear();
    ::fwRuntime::ConfigurationElementContainer::Iterator iter;
    for (iter = vectConfig.begin() ; iter != vectConfig.end() ; ++iter)
    {
        SLM_FATAL_IF("<views> node can only contain <view> node", (*iter)->getName()!="view" );
        ViewInfo vi;
        std::string uid;

        SLM_FATAL_IF("<view> node must contain uid attribute", !(*iter)->hasAttribute("uid") );
        uid = (*iter)->getExistingAttributeValue("uid");
        if( (*iter)->hasAttribute("align") )
        {
            vi.m_align = (*iter)->getExistingAttributeValue("align");
        }

        if( (*iter)->hasAttribute("minWidth") )
        {
            std::string width = (*iter)->getExistingAttributeValue("minWidth") ;
            vi.m_minSize.first = ::boost::lexical_cast< int >(width) ;
        }

        if( (*iter)->hasAttribute("minHeight") )
        {
            std::string height = (*iter)->getExistingAttributeValue("minHeight") ;
            vi.m_minSize.second = ::boost::lexical_cast< int >(height) ;
        }

        if( (*iter)->hasAttribute("resizable") )
        {
            std::string resizable = (*iter)->getExistingAttributeValue("resizable") ;
            OSLM_ASSERT("Incorrect value for \"resizable\" attribute "<<resizable,
                               (resizable == "yes") || (resizable == "no"));
            vi.m_isResizable = (resizable=="yes") ;
        }

        if( (*iter)->hasAttribute("position") )
        {
            std::string position = (*iter)->getExistingAttributeValue("position") ;
            vi.m_position = ::boost::lexical_cast< int >(position);
        }

        if( (*iter)->hasAttribute("layer") )
        {
            std::string layer = (*iter)->getExistingAttributeValue("layer") ;
            vi.m_layer = ::boost::lexical_cast< int >(layer);
        }

        if( (*iter)->hasAttribute("row") )
        {
            std::string row = (*iter)->getExistingAttributeValue("row") ;
            vi.m_row = ::boost::lexical_cast< int >(row);
        }

        if( (*iter)->hasAttribute("visible") )
        {
            std::string visible = (*iter)->getExistingAttributeValue("visible") ;
            OSLM_ASSERT("Incorrect value for \"visible\" attribute "<<visible,
                    (visible == "true") || (visible == "false") ||
                    (visible == "yes") || (visible == "no"));
            vi.m_visible = ((visible == "true") || (visible == "yes"));
        }

        if( (*iter)->hasAttribute("caption") )
        {
            vi.m_caption.first = true;
            vi.m_caption.second = (*iter)->getExistingAttributeValue("caption") ;
        }

        if( (*iter)->hasAttribute("autoStart") )
        {
            std::string autostart = (*iter)->getExistingAttributeValue("autoStart");
            OSLM_ASSERT("Sorry, value "<<autostart<<" is not correct for attribute autoStart.",
                    autostart == "yes" || autostart == "no");
            vi.m_autostart = (autostart == "yes");
        }
        m_panels[uid] = vi;
    }
}

//-----------------------------------------------------------------------------

void MultiView::reconfiguring() throw( ::fwTools::Failed )
{
    SLM_FATAL("ACH : This method is not implemented, does nothing, why is called ?");
}

//-----------------------------------------------------------------------------

void MultiView::info(std::ostream &_sstream )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void MultiView::starting() throw(::fwTools::Failed)
{
  
  std::cout<<"  \n  MultiView::starting()  \n";
    SLM_TRACE_FUNC();
    this->initGuiParentContainer();

    QWidget *mainWidget = this->getQtContainer();
    m_manager =  qobject_cast<QMainWindow *>(mainWidget);
     
    QDesktopWidget *desk = QApplication::desktop();
    QRect screen = desk->screenGeometry(m_manager);
    
    
    PanelContainer::iterator pi = m_panels.begin();
    for ( pi; pi!= m_panels.end() ; ++pi )
    {
	pi->second.m_panel = new QDockWidget((this->getUUID()).c_str(),m_manager);
	pi->second.m_panel->setMinimumSize(screen.width()/3, screen.height()/3);
	pi->second.m_panel->resize(screen.width()/3, screen.height()/3);


        if(pi->second.m_align=="center")        
	{ 
	 m_manager->setCentralWidget( pi->second.m_panel);
	}
        else if(pi->second.m_align=="right")
	{ 
	  pi->second.m_panel->setFeatures(QDockWidget::AllDockWidgetFeatures);
	  m_manager->addDockWidget(Qt::RightDockWidgetArea,  pi->second.m_panel);
	}
        else if(pi->second.m_align=="left") 
	{ 
	  pi->second.m_panel->setFeatures(QDockWidget::AllDockWidgetFeatures);
	  m_manager->addDockWidget(Qt::LeftDockWidgetArea,  pi->second.m_panel);
	}
        else if(pi->second.m_align=="bottom")
	{ 
	  pi->second.m_panel->setFeatures(QDockWidget::AllDockWidgetFeatures);
	  m_manager->addDockWidget(Qt::BottomDockWidgetArea,  pi->second.m_panel);
	}
        else if(pi->second.m_align=="top")
	{ 
	  pi->second.m_panel->setFeatures(QDockWidget::AllDockWidgetFeatures);
	  m_manager->addDockWidget(Qt::TopDockWidgetArea,  pi->second.m_panel);
	}
	
	
        this->registerQtContainer(pi->first,  pi->second.m_panel);

        if(pi->second.m_autostart)
        {
            OSLM_ASSERT("Service "<<pi->first<<" doesn't exist.", ::fwTools::UUID::exist(pi->first, ::fwTools::UUID::SIMPLE ));
            ::fwServices::IService::sptr service = ::fwServices::get( pi->first ) ;
            service->start();
        }
    }
    m_manager->show();
}
//-----------------------------------------------------------------------------

void MultiView::updating() throw(::fwTools::Failed)
{
    // ACH Comment because udapte is made on service on layout switching in ConfigVisuActionService
    // SLM_FATAL("ACH : This method is not implemented because it does nothing, why is called ?");
}

//-----------------------------------------------------------------------------

void MultiView::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : This method is not implemented because it does nothing, why is called ?");
}
//-----------------------------------------------------------------------------

void MultiView::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->unregisterAllQtContainer();

    // Destroy wxAuiManager
    if( m_manager )
    {
      
    }
    this->resetGuiParentContainer();
}

//-----------------------------------------------------------------------------

void MultiView::swappping() throw( ::fwTools::Failed )
{
    SLM_FATAL("ACH : This method is not implemented, why is called ?");
}

//-----------------------------------------------------------------------------

}

}

