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
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>

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
	 if( (*iter)->hasAttribute("title") )
        {
            vi.m_title = (*iter)->getExistingAttributeValue("title");
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
	 if( (*iter)->hasAttribute("movable") )
        {
            std::string movable = (*iter)->getExistingAttributeValue("movable") ;
            vi.m_movable = ::boost::lexical_cast< int >(movable);
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
  
    SLM_TRACE_FUNC();
    this->initGuiParentContainer();

    //QWidget *mainWidget = this->getQtContainer();
    m_manager =  qobject_cast<QMainWindow *>(m_container);
    QMainWindow* centerView = new QMainWindow();
   
    PanelContainer::iterator pi = m_panels.begin();
    for ( pi; pi!= m_panels.end() ; ++pi )
    {
      QDockWidget *widget;
      if(pi->second.m_title=="")        
      {  
	widget = new QDockWidget((pi->first).c_str(), m_manager);
      }
      else 
      { 
	widget = new QDockWidget((pi->second.m_title).c_str(), m_manager);

      }
 	widget->setFeatures(QDockWidget::AllDockWidgetFeatures);

	pi->second.m_panel = new QWidget();
	pi->second.m_panel->setWindowTitle((this->getUUID()).c_str());
	pi->second.m_panel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	pi->second.m_panel->setMinimumSize(pi->second.m_minSize.first, pi->second.m_minSize.second);
        widget->setWidget(pi->second.m_panel);


        if(pi->second.m_align=="center")        
	{  
	 centerView->setCentralWidget(pi->second.m_panel);	
	}
        else if(pi->second.m_align=="right")
	{ 
	  centerView->addDockWidget(Qt::RightDockWidgetArea,  widget);	 

	}
        else if(pi->second.m_align=="left") 
	{ 
	  centerView->addDockWidget(Qt::LeftDockWidgetArea,  widget);	 
	}
        else if(pi->second.m_align=="top")
	{ 
	  centerView->addDockWidget(Qt::TopDockWidgetArea, widget);	
	}
        else if(pi->second.m_align=="bottom")
	{ 
	  centerView->addDockWidget(Qt::BottomDockWidgetArea, widget);	 
	}
	else
	{
	  // on verra
	  SLM_TRACE(" NO DOCKWIGET POSITION SPECIFIED ");
	}
	
	if(pi->second.m_movable==0)        
	{  
	  widget->setFeatures(QDockWidget::NoDockWidgetFeatures);
	}
	

        this->registerQtContainer(pi->first,  pi->second.m_panel);

        if(pi->second.m_autostart)
        {
            OSLM_ASSERT("Service "<<pi->first<<" doesn't exist.", ::fwTools::UUID::exist(pi->first, ::fwTools::UUID::SIMPLE ));
            ::fwServices::IService::sptr service = ::fwServices::get( pi->first ) ;
            service->start();

        }
    }
    //m_manager->show();
    
     if(m_manager) // Si le cast a reussi donc si c'est une vu imbriquée dans une autre
    {
      centerView->setParent(m_manager);
      m_manager->setCentralWidget(centerView);
     }
     else
      {
	  // A voir : test V ou H
	  QHBoxLayout *subLayout = new  QHBoxLayout();
	  centerView->setParent(m_container);

	  centerView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding); // TRES IMPORTANT
	    
	  subLayout->addWidget( centerView);
	  m_container->setLayout(subLayout);
	  
	}
    
    
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
