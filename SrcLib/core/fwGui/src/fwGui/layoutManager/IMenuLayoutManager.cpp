/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

/**
 * @file fwGui/layoutManager/IMenuLayoutManager.cpp
 * @brief This file defines the implementation of the base class for managing a menu.
 *
 * 
 * @date 2009-2010
 */

#include <boost/foreach.hpp>

#include "fwGui/layoutManager/IMenuLayoutManager.hpp"

namespace fwGui
{
namespace layoutManager
{

const IMenuLayoutManager::RegistryKeyType IMenuLayoutManager::REGISTRY_KEY = "::fwGui::MenuLayoutManager";

//-----------------------------------------------------------------------------

IMenuLayoutManager::IMenuLayoutManager()
{}

//-----------------------------------------------------------------------------

IMenuLayoutManager::~IMenuLayoutManager()
{}

//-----------------------------------------------------------------------------

void IMenuLayoutManager::initialize( ConfigurationType configuration)
{
    OSLM_ASSERT("Bad configuration name "<<configuration->getName()<< ", must be layout",
                configuration->getName() == "layout");


    ::fwRuntime::ConfigurationElementContainer::Iterator iter ;
    for( iter = configuration->begin() ; iter != configuration->end() ; ++iter )
    {
        if( (*iter)->getName() == "menuItem" )
        {
            ConfigurationType menuItem = *iter;
            ActionInfo info;

            SLM_ASSERT("Depreciated tag <state>", ! menuItem->hasAttribute("state"));
            SLM_ASSERT("Depreciated tag <enable>", ! menuItem->hasAttribute("enable"));

            SLM_ASSERT("missing <name> attribute", menuItem->hasAttribute("name"));
            if( menuItem->hasAttribute("name") )
            {
                info.m_name = menuItem->getExistingAttributeValue("name") ;
            }

            if( menuItem->hasAttribute("shortcut") )
            {
                info.m_shortcut = menuItem->getExistingAttributeValue("shortcut") ;
            }

            if( menuItem->hasAttribute("icon") )
            {
                info.m_icon = menuItem->getExistingAttributeValue("icon") ;
            }

            if( menuItem->hasAttribute("style") )
            {
                std::string style = menuItem->getExistingAttributeValue("style") ;
                info.m_isCheckable = (style == "check");
                info.m_isRadio = (style == "radio");
            }

            if( menuItem->hasAttribute("specialAction") )
            {
                std::string specialActionName = menuItem->getExistingAttributeValue("specialAction") ;
                if (specialActionName == "DEFAULT")
                {
                    info.m_type = DEFAULT;
                }
                else if (specialActionName == "QUIT")
                {
                    info.m_type = QUIT;
                }
                else if (specialActionName == "ABOUT")
                {
                    info.m_type = ABOUT;
                }
                else if (specialActionName == "HELP")
                {
                    info.m_type = HELP;
                }
                else if (specialActionName == "NEW")
                {
                    info.m_type = NEW;
                }
                else
                {
                    OSLM_FATAL("specialAction " << specialActionName << " is unknown." );
                }
            }

            m_actionInfo.push_back(info);
        }
        if( (*iter)->getName() == "separator" )
        {
            ActionInfo info;
            info.m_isSeparator = true;
            info.m_type = SEPARATOR;
            m_actionInfo.push_back( info ) ;
        }

        if( (*iter)->getName() == "menu" )
        {
            ActionInfo info;
            info.m_isMenu = true;
            if( (*iter)->hasAttribute("name") )
            {
                info.m_name = (*iter)->getExistingAttributeValue("name") ;
            }
            m_actionInfo.push_back( info ) ;
        }
    }
}

//-----------------------------------------------------------------------------

void IMenuLayoutManager::destroyActions()
{
    BOOST_FOREACH( ::fwGui::container::fwMenuItem::sptr menuItem, m_menuItems)
    {
        menuItem->destroyContainer();
    }
    m_menuItems.clear();
    BOOST_FOREACH( ::fwGui::container::fwMenu::sptr menu, m_menus)
    {
        menu->destroyContainer();
    }
    m_menus.clear();
}

//-----------------------------------------------------------------------------

std::vector< ::fwGui::container::fwMenuItem::sptr > IMenuLayoutManager::getMenuItems()
{
    return this->m_menuItems;
}

//-----------------------------------------------------------------------------

std::vector< ::fwGui::container::fwMenu::sptr > IMenuLayoutManager::getMenus()
{
    return this->m_menus;
}

//-----------------------------------------------------------------------------

} // namespace layoutManager
} // namespace fwGui



