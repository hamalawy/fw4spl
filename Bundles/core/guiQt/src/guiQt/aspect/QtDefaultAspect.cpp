/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

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
#include "guiQt/aspect/QtDefaultAspect.hpp"


namespace guiQt
{
namespace aspect
{

REGISTER_SERVICE( ::guiQt::aspect::IQtAspect , ::guiQt::aspect::QtDefaultAspect , ::fwTools::Object ) ;


QtDefaultAspect::QtDefaultAspect() throw() : m_uid(""), m_autostart(false)
{
}

//-----------------------------------------------------------------------------

QtDefaultAspect::~QtDefaultAspect() throw()
{
}

//-----------------------------------------------------------------------------

void QtDefaultAspect::configuring() throw( ::fwTools::Failed )
{
  std::cout <<" Default Aspect  \n";
    this->::guiQt::aspect::IQtAspect::configuring();
   // il faut un setConfig
    if(m_configuration->findConfigurationElement("view"))
    {
        std::cout <<" Default Aspect ------------> IF  \n";

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
}

//-----------------------------------------------------------------------------

void QtDefaultAspect::starting() throw(::fwTools::Failed)
{
  std::cout<<" DefaultAspect::starting() \n";
  std::cout<<" m_uid : "<<m_uid<<" \n";

   // this->::guiQt::aspect::IQtAspect::starting();
    
            ::fwServices::IService::sptr service = ::fwServices::get( m_uid ) ;
            service->start();
	    
	    
    // Starting views
 //   this->registerAspect();
//     if(!m_uid.empty())
//     {
//         ::fwWX::IGuiContainer::registerGlobalWxContainer(m_uid, wxTheApp->GetTopWindow());
//         if(m_autostart)
//         {
//             wxTheApp->GetTopWindow()->Update();
//             OSLM_ASSERT("Service "<<m_uid<<" doesn't exist.", ::fwTools::UUID::exist(m_uid, ::fwTools::UUID::SIMPLE ));
//             ::fwServices::IService::sptr service = ::fwServices::get( m_uid ) ;
//             service->start();
//         }
//     }
}

//-----------------------------------------------------------------------------

void QtDefaultAspect::stopping() throw(::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void QtDefaultAspect::info(std::ostream &_sstream )
{
    _sstream << "Manage aspect in main GUI application" ;
}

//-----------------------------------------------------------------------------

void QtDefaultAspect::updating( ::fwServices::ObjectMsg::csptr _msg ) throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void QtDefaultAspect::updating() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

}
}

