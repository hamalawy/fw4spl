/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwTools/UUID.hpp>

#include <fwServices/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "guiQt/view/DefaultView.hpp"
#include "guiQt/editor/IEditor.hpp"

#include <QApplication>
#include <QLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>


namespace guiQt
{
namespace view
{

//-----------------------------------------------------------------------------

REGISTER_SERVICE( ::guiQt::view::IView , ::guiQt::view::DefaultView , ::fwTools::Object ) ;

//-----------------------------------------------------------------------------

DefaultView::DefaultView() throw()
{
}

//-----------------------------------------------------------------------------

DefaultView::~DefaultView() throw()
{}

//-----------------------------------------------------------------------------

void DefaultView::configuring() throw( ::fwTools::Failed )
{
    assert( m_configuration->getName() == "service" );
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
	
	 if( (*iter)->hasAttribute("title") )
        {
            vi.m_title = (*iter)->getExistingAttributeValue("title");
        }
	if( (*iter)->hasAttribute("movable") )
        {
            std::string movable = (*iter)->getExistingAttributeValue("movable") ;
            vi.m_movable = ::boost::lexical_cast< int >(movable);
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
        if((*iter)->hasAttribute("autoStart"))
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

void DefaultView::reconfiguring() throw( ::fwTools::Failed )
{
    SLM_FATAL("ACH : This method is not implemented, does nothing, why is called ?");
}

//-----------------------------------------------------------------------------

void DefaultView::info(std::ostream &_sstream )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------
// void DefaultView::createDockWidget(QString title, QWidget *parent)
// {
//       if(pi->second.m_title=="")        
//       {  
// 	widget = new QDockWidget((pi->first).c_str(), m_manager);
//       }
//       else 
//       { 
// 	widget = new QDockWidget((pi->second.m_title).c_str(), m_manager);
//       }
// 	
//         widget->setFeatures(QDockWidget::AllDockWidgetFeatures);
// }

//-----------------------------------------------------------------------------

void DefaultView::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->initGuiParentContainer();
    //::guiQt::editor::IEditor::starting();

    m_manager =  qobject_cast<QMainWindow *>(m_container);
    QMainWindow* centerView = new QMainWindow();
    // Pour MacOs
    m_manager->setUnifiedTitleAndToolBarOnMac(true);

    PanelContainer::iterator pi = m_panels.begin();
    for ( pi; pi!= m_panels.end() ; ++pi )
    {	
	QDockWidget *widget;

        pi->second.m_panel = new QWidget();
        pi->second.m_panel->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
	pi->second.m_panel->setMinimumSize(pi->second.m_minSize.first, pi->second.m_minSize.second);

        
        if(pi == m_panels.begin())
        {
	   centerView->setCentralWidget(pi->second.m_panel);	
	  // widget->deleteLater();
        }
        else
        {
	   if(pi->second.m_title=="")        
	  {  
	    widget = new QDockWidget((pi->first).c_str(), m_manager);
	  }
	  else 
	  { 
	    widget = new QDockWidget((pi->second.m_title).c_str(), m_manager);
	  }
	    widget->setFeatures(QDockWidget::AllDockWidgetFeatures);
	    widget->setWidget(pi->second.m_panel);  // IMPORTANT : must use the widget provided by dockWidget though setWidget()

	    centerView->addDockWidget(Qt::RightDockWidgetArea,  widget);	 
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

void DefaultView::updating() throw(::fwTools::Failed)
{
    // ACH Comment because udapte is made on service on layout switching in ConfigVisuActionService
    // SLM_FATAL("ACH : This method is not implemented because it does nothing, why is called ?");
}

//-----------------------------------------------------------------------------

void DefaultView::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : This method is not implemented because it does nothing, why is called ?");
}
//-----------------------------------------------------------------------------

void DefaultView::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->unregisterAllQtContainer();

    // Destroy wxAuiManager
    if( m_manager )
    {
        //delete it
    }
    this->resetGuiParentContainer();
}

//-----------------------------------------------------------------------------

void DefaultView::swappping() throw( ::fwTools::Failed )
{
    SLM_FATAL("ACH : This method is not implemented, why is called ?");
}

//-----------------------------------------------------------------------------

}

}
