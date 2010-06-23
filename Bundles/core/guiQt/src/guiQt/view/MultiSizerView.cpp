/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwTools/UUID.hpp>

#include <fwServices/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "guiQt/view/MultiSizerView.hpp"

#include <QApplication>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QDockWidget>



#include <QPushButton>
#include <QString>


namespace gui
{
namespace view
{

//-----------------------------------------------------------------------------

REGISTER_SERVICE( ::guiQt::view::IView , ::gui::view::MultiSizerView , ::fwTools::Object ) ;

//-----------------------------------------------------------------------------

MultiSizerView::MultiSizerView() throw() : m_bOrient(true)
{}

//-----------------------------------------------------------------------------

MultiSizerView::~MultiSizerView() throw()
{}

//-----------------------------------------------------------------------------

void MultiSizerView::configuring() throw( ::fwTools::Failed )
{
    assert( m_configuration->getName() == "service" );
    SLM_FATAL_IF( "Depreciated tag \"win\" in configuration", m_configuration->findConfigurationElement("win") );

    SLM_FATAL_IF( "missing views configuration" , !m_configuration->findConfigurationElement("views") );
    ::fwRuntime::ConfigurationElement::sptr viewsCfgElt = m_configuration->findConfigurationElement("views");
    if(viewsCfgElt->hasAttribute("orientation") )
    {
        std::string orient = viewsCfgElt->getExistingAttributeValue("orientation") ;
        if(orient == "vertical")
        {
            m_bOrient = true  ;
        }
        else if(orient == "horizontal")
        {
            m_bOrient = false   ;
        }
    }

    std::vector < ::fwRuntime::ConfigurationElement::sptr > vectConfig = viewsCfgElt->find("view");
    assert(!vectConfig.empty());
    m_views.clear();
    ::fwRuntime::ConfigurationElementContainer::Iterator iter;
    for (iter = vectConfig.begin() ; iter != vectConfig.end() ; ++iter)
    {
        SLM_FATAL_IF("<views> node can only contain <view> node", (*iter)->getName()!="view" );
        ViewInfo vi;

        SLM_FATAL_IF("<view> node must contain uid attribute", !(*iter)->hasAttribute("uid") );
        vi.m_uid = (*iter)->getExistingAttributeValue("uid");
        if( (*iter)->hasAttribute("proportion") )
        {
            std::string proportion = (*iter)->getExistingAttributeValue("proportion") ;
            vi.m_proportion = ::boost::lexical_cast< int >(proportion) ;
        }

        if( (*iter)->hasAttribute("border") )
        {
            std::string border = (*iter)->getExistingAttributeValue("border") ;
            vi.m_border = ::boost::lexical_cast< int >(border) ;
        }

        if( (*iter)->hasAttribute("caption") )
        {
            vi.m_caption.first = true;
            vi.m_caption.second = (*iter)->getExistingAttributeValue("caption") ;
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
        if( (*iter)->hasAttribute("autoStart") )
        {
            std::string autostart = (*iter)->getExistingAttributeValue("autoStart");
            OSLM_ASSERT("Sorry, value "<<autostart<<" is not correct for attribute autoStart.",
                    autostart == "yes" || autostart == "no");
            vi.m_autostart = (autostart == "yes");
        }
        m_views.push_back(vi);
    }
}

//-----------------------------------------------------------------------------

void MultiSizerView::reconfiguring() throw( ::fwTools::Failed )
{
    SLM_FATAL("ACH : This method is not implemented, does nothing, why is called ?");
}

//-----------------------------------------------------------------------------

void MultiSizerView::info(std::ostream &_sstream )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void MultiSizerView::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->initGuiParentContainer(); // a chanegr en gui:: ...

    //QWidget *mainWidget =  m_globalUIDToQtContainer[this->getUUID()];  //this->getQtContainer();  mainWidget = m_container

    m_manager =  qobject_cast<QMainWindow *>(m_container);

    QWidget* centerView = new QWidget();

    QLayout *layout;

    if(m_bOrient)
    {
        layout = new  QVBoxLayout();
    }
    else
    {
        layout = new  QHBoxLayout();
    }
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);


    std::list<ViewInfo>::iterator pi = m_views.begin();
    for ( pi; pi!= m_views.end() ; ++pi )
    {

        pi->m_panel = new QWidget(centerView);

        if( pi->m_proportion==0)
        {
            pi->m_panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        }
        else
            pi->m_panel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	

        layout->addWidget( pi->m_panel);

        this->registerQtContainer(pi->m_uid, pi->m_panel);

        if(pi->m_autostart)
        {
            OSLM_ASSERT("Service "<<pi->m_uid<<" doesn't exist.", ::fwTools::UUID::exist(pi->m_uid, ::fwTools::UUID::SIMPLE ));
            ::fwServices::IService::sptr service = ::fwServices::get( pi->m_uid ) ;
            service->start();
        }

    }
    centerView->setLayout(layout);

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

        //        if( pi->m_proportion==0)
        //       {
        //         centerView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        //       }
        //       else
        //         centerView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        centerView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding); // TRES IMPORTANT

        subLayout->addWidget( centerView);
        m_container->setLayout(subLayout);

    }

}
//-----------------------------------------------------------------------------

void MultiSizerView::updating() throw(::fwTools::Failed)
{
    // ACH Comment because udapte is made on service on layout switching in ConfigVisuActionService
    // SLM_FATAL("ACH : This method is not implemented because it does nothing, why is called ?");
}

//-----------------------------------------------------------------------------

void MultiSizerView::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : This method is not implemented because it does nothing, why is called ?");
}
//-----------------------------------------------------------------------------

void MultiSizerView::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
   // this->unregisterAllWxContainer();
    this->resetGuiParentContainer();
}

//-----------------------------------------------------------------------------

void MultiSizerView::swappping() throw( ::fwTools::Failed )
{
    SLM_FATAL("ACH : This method is not implemented, why is called ?");
}

//-----------------------------------------------------------------------------

}

}

