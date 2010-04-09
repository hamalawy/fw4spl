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
  m_nbView=0;
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
	
	std::cout<<" Incrementation nombre de view \n";
	m_nbView++;
	
	
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
	
	std::cout<<" UId panels : "<<uid<<"\n";
        m_panels[uid] = vi;
    }
    
    std::cout<<" Nombre de view : "<<m_nbView<<" \n";
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

void DefaultView::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->initGuiParentContainer();
    
    QWidget *mainWidget = this->getQtContainer();
    m_manager =  qobject_cast<QMainWindow *>(mainWidget);
    // Pour MacOs
    m_manager->setUnifiedTitleAndToolBarOnMac(true);
    
    if(m_manager->layout()!=0)
    {
      std::cout<<" ALREADY LAYOUT EXIST \n";
    }
    else
      std::cout<<" NO LAYOUT \n";
      

    QDesktopWidget *desk = QApplication::desktop();
    QRect screen = desk->screenGeometry(m_manager);

    PanelContainer::iterator pi = m_panels.begin();
    for ( pi; pi!= m_panels.end() ; ++pi )
    {    
	pi->second.m_panel = new QDockWidget((this->getUUID()).c_str(), m_manager);
	//pi->second.m_panel->setMinimumSize(300, 300);

	if(pi == m_panels.begin())
        {
//	    pi->second.m_panel->setMinimumSize(pi->second.m_minSize.second, pi->second.m_minSize.first);
	   // pi->second.m_panel->resize(screen.width()/2, screen.height());
	    if(m_nbView==1)
	    {
	      pi->second.m_panel->setMaximumSize((screen.width()), screen.height());
	      pi->second.m_panel->setGeometry(0, 0, screen.width(), screen.height());
	    }
	    else
	    {
	        pi->second.m_panel->setMaximumSize((screen.width())/2, screen.height());
		pi->second.m_panel->setGeometry(0, 0, screen.width()/2, screen.height());
	    }
	 

	   // m_manager->setCentralWidget( pi->second.m_panel);
	  m_manager->addDockWidget(Qt::LeftDockWidgetArea,  pi->second.m_panel);

        }
        else
        {

	  //pi->second.m_panel->setMinimumSize(pi->second.m_minSize.second, pi->second.m_minSize.first);

	    pi->second.m_panel->setMaximumSize(screen.width()/2, screen.height());
	   //i->second.m_panel->resize(screen.width()/2, screen.height()/2);
	    pi->second.m_panel->setGeometry(screen.width()/2, 0, screen.width()/2, screen.height()/(m_nbView-1));
	    
//	    pi->second.m_panel->setFeatures(QDockWidget::AllDockWidgetFeatures);

	    m_manager->addDockWidget(Qt::RightDockWidgetArea,  pi->second.m_panel);
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

