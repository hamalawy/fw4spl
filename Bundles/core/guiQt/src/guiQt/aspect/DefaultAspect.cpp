/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
#include <QApplication>
#include <QWidget>

#include <boost/foreach.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/bundle/runtime.hpp>
#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>
#include <fwData/Object.hpp>
#include <fwTools/UUID.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwData/Object.hpp>

#include <fwQt/IGuiContainer.hpp>


#include "guiQt/aspect/DefaultAspect.hpp"


namespace guiQt
{
namespace aspect
{

REGISTER_SERVICE( ::guiQt::aspect::IAspect , ::guiQt::aspect::DefaultAspect , ::fwTools::Object ) ;


DefaultAspect::DefaultAspect() throw() : m_uid(""), m_autostart(false)
{
}

//-----------------------------------------------------------------------------

DefaultAspect::~DefaultAspect() throw()
{
}

//-----------------------------------------------------------------------------

void DefaultAspect::configuring() throw( ::fwTools::Failed )
{
    this->::guiQt::aspect::IAspect::configuring();
    if(m_configuration->findConfigurationElement("view"))
    {
        ::fwRuntime::ConfigurationElement::sptr viewCfgElt = m_configuration->findConfigurationElement("view");
        SLM_ASSERT("Sorry, \"uid\" attribute is missing.", viewCfgElt->hasAttribute("uid") );
        m_uid = viewCfgElt->getExistingAttributeValue("uid") ;
        if(viewCfgElt->hasAttribute("autoStart"))
        {
            std::string autostart = viewCfgElt->getExistingAttributeValue("autoStart");
            OSLM_ASSERT("Sorry, value "<<autostart<<" is not correct for attribute autoStart.",
                    autostart == "yes" || autostart == "no");
            m_autostart = (autostart == "yes");
        }
    }
    else
    {
        SLM_WARN("No main view specified.");
    }
    
    m_managedServices.clear();
    if (m_configuration->findConfigurationElement("services"))
    {
        ::fwRuntime::ConfigurationElement::sptr servicesCfgElt = m_configuration->findConfigurationElement("services");

        std::vector < ::fwRuntime::ConfigurationElement::sptr > vectConfig = servicesCfgElt->find("service");
        assert(!vectConfig.empty());

        ::fwRuntime::ConfigurationElementContainer::Iterator iter;
        for (iter = vectConfig.begin() ; iter != vectConfig.end() ; ++iter)
        {
            SLM_FATAL_IF("<services> node can only contain <service> node", (*iter)->getName()!="service" );
            SLM_FATAL_IF("<service> node must contain uid attribute", !(*iter)->hasAttribute("uid") );
            m_managedServices.push_back((*iter)->getExistingAttributeValue("uid"));
        }
    } 
    
}

//-----------------------------------------------------------------------------

void DefaultAspect::starting() throw(::fwTools::Failed)
{
  
  QWidget *container = qApp->activeWindow();
  ::fwQt::IGuiContainer::registerGlobalQtContainer(m_uid, container);

  if(m_uid!="")
  {
    ::fwServices::IService::sptr service = ::fwServices::get( m_uid ) ;
    service->start();
  }
  
   BOOST_FOREACH(std::string serviceUID, m_managedServices)
    {
        OSLM_FATAL_IF("Service " << serviceUID << " doesn't exist.", ! ::fwTools::UUID::exist(serviceUID, ::fwTools::UUID::SIMPLE ));
        ::fwServices::IService::sptr service = ::fwServices::get( serviceUID ) ;
        service->start();
    }
}

//-----------------------------------------------------------------------------

void DefaultAspect::stopping() throw(::fwTools::Failed)
{
  SLM_TRACE_FUNC();
  
   BOOST_REVERSE_FOREACH(std::string serviceUID, m_managedServices)
    {
        OSLM_FATAL_IF("Service " << serviceUID << " doesn't exist.", ! ::fwTools::UUID::exist(serviceUID, ::fwTools::UUID::SIMPLE ));
        ::fwServices::IService::sptr service = ::fwServices::get( serviceUID ) ;
        service->stop();
    }
  
  if(!m_uid.empty())
    {
        ::fwQt::IGuiContainer::unregisterGlobalQtContainer(m_uid);
        if (::fwTools::UUID::exist(m_uid, ::fwTools::UUID::SIMPLE ))
        {
            ::fwServices::IService::sptr service = ::fwServices::get( m_uid ) ;
            service->stop();
        }
        m_uid = "";
    }
}

//-----------------------------------------------------------------------------

void DefaultAspect::info(std::ostream &_sstream )
{
    _sstream << "Manage aspect in main GUI application" ;
}

//-----------------------------------------------------------------------------

void DefaultAspect::updating( ::fwServices::ObjectMsg::csptr _msg ) throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void DefaultAspect::updating() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

}
}

