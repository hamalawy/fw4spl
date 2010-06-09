/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/bundle/runtime.hpp>
#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>
#include <fwData/Object.hpp>

#include "guiQt/aspect/IAspect.hpp"

#include <QApplication>
#include <QMenuBar>

#include <iostream>

namespace guiQt
{

namespace aspect
{

IAspect::IAspect() throw():
    m_name("NO NAME"),
    m_minSizeHeight(-1),
    m_minSizeWidth(-1)
{
    SLM_TRACE_FUNC();
}

//---------------------------------------------------------------------------

IAspect::~IAspect() throw()
{
    SLM_TRACE_FUNC();
}

//---------------------------------------------------------------------------

std::string IAspect::getName()
{
    SLM_TRACE_FUNC();
    return m_name ;
}


//---------------------------------------------------------------------------

void IAspect::configuring() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    SLM_TRACE("IAspect::configuring");

    /* Parsing  de <service ... <name> <icon> <minSize> <menus> <toolBar>.... </service> */

    SLM_TRACE("IQtAspect::configuring");
    SLM_FATAL_IF( "Depreciated tag \"views\" in configuration", m_configuration->findConfigurationElement("views") );
    SLM_FATAL_IF( "Depreciated tag \"menus\" in configuration", m_configuration->findConfigurationElement("menus") );
    SLM_FATAL_IF( "Depreciated tag \"toolbar\" in configuration", m_configuration->findConfigurationElement("toolbar") );

    assert( m_configuration->getName() == "aspect" || m_configuration->getName() == "service" );

    ::fwRuntime::ConfigurationElementContainer::Iterator iter ;
    for( iter = m_configuration->begin() ; iter != m_configuration->end() ; ++iter )
    {
        SLM_ASSERT( "actions tag is depreciated", (*iter)->getName() != "actions" );

        if( (*iter)->getName() == "name" )
        {
            m_name = (*iter)->getValue();
        }

        if( (*iter)->getName() == "icon" )
        {
            OSLM_TRACE("icon: "<< (*iter)->getValue());
            //     m_icon = ::boost::shared_ptr< wxIcon >(createIcon((*iter)->getValue()));
        }

        if( (*iter)->getName() == "minSize" )
        {
            //TODO: it should be useful to verify the config is well formed...
            //assert((*iter)->hasAttribute("height") && (*iter)->hasAttribute("width") );
            //m_minSizeHeight = ::boost::lexical_cast<int >((*iter)->getExistingAttributeValue("height").c_str());
            //m_minSizeWidth = ::boost::lexical_cast<int >((*iter)->getExistingAttributeValue("width").c_str());
            if( (*iter)->hasAttribute("height") )
            {
                m_minSizeHeight = ::boost::lexical_cast<int >((*iter)->getExistingAttributeValue("height").c_str());
            }
            if( (*iter)->hasAttribute("width") )
            {
                m_minSizeWidth = ::boost::lexical_cast<int >((*iter)->getExistingAttributeValue("width").c_str());
            }
        }
    }
}

//---------------------------------------------------------------------------

void IAspect::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}
//---------------------------------------------------------------------------

void IAspect::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//---------------------------------------------------------------------------

void IAspect::info(std::ostream &_sstream )
{
    SLM_TRACE_FUNC();
    _sstream << "Manage aspect in main GUI application" ;
}
}
}

