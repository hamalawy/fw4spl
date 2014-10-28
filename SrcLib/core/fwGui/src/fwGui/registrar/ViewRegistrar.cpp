/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <utility>

#include <boost/foreach.hpp>

#include <fwTools/fwID.hpp>
#include <fwServices/Base.hpp>

#include "fwGui/GuiRegistry.hpp"
#include "fwGui/registrar/ViewRegistrar.hpp"

namespace fwGui
{
namespace registrar
{

//-----------------------------------------------------------------------------

ViewRegistrar::ViewRegistrar(const std::string &sid)
    : m_parentWid(""),
      m_sid(sid)
{}

//-----------------------------------------------------------------------------

ViewRegistrar::~ViewRegistrar()
{}

//-----------------------------------------------------------------------------

::fwGui::container::fwContainer::sptr ViewRegistrar::getParent()
{
    ::fwGui::container::fwContainer::sptr parentContainer;
    if(!m_parentWid.empty())
    {
        parentContainer = ::fwGui::GuiRegistry::getWIDContainer(m_parentWid);
    }
    else
    {
        parentContainer = ::fwGui::GuiRegistry::getSIDContainer(m_sid);
    }
    return parentContainer;
}

//-----------------------------------------------------------------------------

void ViewRegistrar::setParent(std::string wid)
{
    OSLM_ASSERT("This method is available only if this container has a WID parent container", !m_parentWid.empty());
    m_parentWid = wid;
}

//-----------------------------------------------------------------------------

void ViewRegistrar::initialize( ::fwRuntime::ConfigurationElement::sptr configuration)
{
    OSLM_ASSERT("Bad configuration name "<<configuration->getName()<< ", must be viewRegistrar",
            configuration->getName() == "registry");

    // find parent container
    std::vector < ConfigurationType > vectParent = configuration->find("parent");
    if(!vectParent.empty())
    {
        ConfigurationType parent = vectParent.at(0);
        SLM_ASSERT("<parent> tag must have wid attribute", parent->hasAttribute("wid"));
        m_parentWid = parent->getAttributeValue("wid");
    }

    // index represents associated container with position in subViews vector
    unsigned int index = 0;
    // initialize m_sids and m_wids map with configuration
    std::vector < ConfigurationType > vectViews = configuration->find("view");
    BOOST_FOREACH( ConfigurationType view, vectViews)
    {
        SLM_ASSERT("<view> tag must have sid or wid attribute",
                view->hasAttribute("sid") || view->hasAttribute("wid"));
        if(view->hasAttribute("sid"))
        {
            bool start = false;
            if(view->hasAttribute("start"))
            {
                std::string startValue = view->getAttributeValue("start");
                SLM_ASSERT("Wrong value '"<< startValue <<"' for 'start' attribute (require yes or no)",
                        startValue == "yes" || startValue == "no");
                start = (startValue=="yes");
            }
            std::string sid = view->getAttributeValue("sid");
            m_sids[sid] = SIDContainerMapType::mapped_type(index, start);
        }
        else if(view->hasAttribute("wid"))
        {
            std::string wid = view->getAttributeValue("wid");
            m_wids[wid] = index;
        }
        index++;
    }

    // find menuBar
    std::vector < ConfigurationType > vectmenuBar = configuration->find("menuBar");
    if(!vectmenuBar.empty())
    {
        ConfigurationType menuBarCfg = vectmenuBar.at(0);
        if (menuBarCfg->hasAttribute("sid"))
        {
            bool start = false;
            if (menuBarCfg->hasAttribute("start"))
            {
                std::string startValue = menuBarCfg->getAttributeValue("start");
                SLM_ASSERT("Wrong value '"<< startValue <<"' for 'start' attribute (require yes or no)",
                           startValue == "yes" || startValue == "no");
                start = (startValue=="yes");
            }
            std::string sid = menuBarCfg->getAttributeValue("sid");
            m_menuBarSid =  std::make_pair( sid, start);
        }
    }

    // find toolBar
    std::vector < ConfigurationType > vectToolBar = configuration->find("toolBar");
    if(!vectToolBar.empty())
    {
        ConfigurationType toolBarCfg = vectToolBar.at(0);
        if (toolBarCfg->hasAttribute("sid"))
        {
            bool start = false;
            if (toolBarCfg->hasAttribute("start"))
            {
                std::string startValue = toolBarCfg->getAttributeValue("start");
                SLM_ASSERT("Wrong value '"<< startValue <<"' for 'start' attribute (require yes or no)",
                           startValue == "yes" || startValue == "no");
                start = (startValue=="yes");
            }
            std::string sid = toolBarCfg->getAttributeValue("sid");
            m_toolBarSid =  std::make_pair( sid, start);
        }
    }
}

//-----------------------------------------------------------------------------

void ViewRegistrar::manage(std::vector< ::fwGui::container::fwContainer::sptr > subViews )
{
    ::fwGui::container::fwContainer::sptr container;
    BOOST_FOREACH( SIDContainerMapType::value_type sid, m_sids)
    {
        OSLM_ASSERT("Container index "<< sid.second.first <<" is bigger than subViews size!", sid.second.first < subViews.size());
        container = subViews.at( sid.second.first );
        ::fwGui::GuiRegistry::registerSIDContainer(sid.first, container);
        if(sid.second.second) //service is auto started?
        {
            OSLM_ASSERT("Service "<<sid.first <<" not exists.", ::fwTools::fwID::exist(sid.first ) );
            ::fwServices::IService::sptr service = ::fwServices::get( sid.first ) ;
            OSLM_ASSERT("Service "<<sid.first <<" must be stopped.", service->isStopped() );
            SLM_TRACE("Starting " + sid.first);
            service->start();
        }
    }

    BOOST_FOREACH( WIDContainerMapType::value_type wid, m_wids)
    {
        OSLM_ASSERT("Container index "<< wid.second <<" is bigger than subViews size!", wid.second < subViews.size());
        container = subViews.at( wid.second );
        ::fwGui::GuiRegistry::registerWIDContainer(wid.first, container);
    }
}

//-----------------------------------------------------------------------------

void ViewRegistrar::manageMenuBar(::fwGui::container::fwMenuBar::sptr menuBar )
{
    ::fwGui::GuiRegistry::registerSIDMenuBar(m_menuBarSid.first, menuBar);
    if(m_menuBarSid.second) //service is auto started?
    {
        OSLM_ASSERT("Service "<<m_menuBarSid.first <<" not exists.", ::fwTools::fwID::exist(m_menuBarSid.first ) );
        ::fwServices::IService::sptr service = ::fwServices::get( m_menuBarSid.first ) ;
        service->start();
    }
}

//-----------------------------------------------------------------------------

void ViewRegistrar::manageToolBar(::fwGui::container::fwToolBar::sptr toolBar )
{
    ::fwGui::GuiRegistry::registerSIDToolBar(m_toolBarSid.first, toolBar);
    if(m_toolBarSid.second) //service is auto started?
    {
        OSLM_ASSERT("Service "<<m_toolBarSid.first <<" not exists.", ::fwTools::fwID::exist(m_toolBarSid.first ) );
        ::fwServices::IService::sptr service = ::fwServices::get( m_toolBarSid.first ) ;
        service->start();
    }
}

//-----------------------------------------------------------------------------

void ViewRegistrar::unmanage()
{
    BOOST_FOREACH( SIDContainerMapType::value_type sid, m_sids)
    {
        if(sid.second.second) //service is auto started?
        {
            OSLM_ASSERT("Service "<<sid.first <<" not exists.", ::fwTools::fwID::exist(sid.first ) );
            ::fwServices::IService::sptr service = ::fwServices::get( sid.first ) ;
            SLM_TRACE("Stopping " + sid.first);
            service->stop();
        }
        ::fwGui::GuiRegistry::unregisterSIDContainer(sid.first);
    }

    BOOST_FOREACH( WIDContainerMapType::value_type wid, m_wids)
    {
        ::fwGui::GuiRegistry::unregisterWIDContainer(wid.first);
    }
}

//-----------------------------------------------------------------------------

void ViewRegistrar::unmanageToolBar()
{
    if ( !m_toolBarSid.first.empty() )
    {
        if(m_toolBarSid.second) //service is auto started?
        {
            OSLM_ASSERT("Service "<<m_toolBarSid.first <<" not exists.", ::fwTools::fwID::exist(m_toolBarSid.first ) );
            ::fwServices::IService::sptr service = ::fwServices::get( m_toolBarSid.first ) ;
            service->stop();
        }
        ::fwGui::GuiRegistry::unregisterSIDToolBar(m_toolBarSid.first);
    }
}

//-----------------------------------------------------------------------------

void ViewRegistrar::unmanageMenuBar()
{
    if ( !m_menuBarSid.first.empty() )
    {
        if(m_menuBarSid.second) //service is auto started?
        {
            OSLM_ASSERT("Service "<<m_menuBarSid.first <<" not exists.", ::fwTools::fwID::exist(m_menuBarSid.first ) );
            ::fwServices::IService::sptr service = ::fwServices::get( m_menuBarSid.first ) ;
            service->stop();
        }
        ::fwGui::GuiRegistry::unregisterSIDMenuBar(m_menuBarSid.first);
    }
}

//-----------------------------------------------------------------------------

} // namespace registrar
} //namespace fwGui
