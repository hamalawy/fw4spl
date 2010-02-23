/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include <fwCore/base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwRuntime/profile/Profile.hpp>

#include "guiQt/Manager.hpp"
#include "guiQt/Plugin.hpp"
#include <QApplication>
#include <QWidget>
#include <iostream>

namespace guiQt
{

// Dans plugin.xml : class = "::guiQt::Plugin"
static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::guiQt::Plugin");

Plugin::~Plugin() throw()
{}

//-----------------------------------------------------------------------------
/*
void Plugin::start() throw(::fwRuntime::RuntimeException)
{
	SLM_TRACE("starting guiQt bundle");
	
	std::cout<< "---> Start()" << std::endl;

	if ( this->getBundle()->hasParameter("rootObject")
	        && this->getBundle()->hasParameter("config")
	        && this->getBundle()->hasParameter("configFile"))
	{
		std::string objectClassName(  this->getBundle()->getParameterValue("rootObject") ) ;
		std::string objectConfigurationName( this->getBundle()->getParameterValue("config") ) ;
		std::string objectConfigurationFile( this->getBundle()->getParameterValue("configFile") ) ;

		::fwServices::OSR::setRootObjectClassName( objectClassName ) ;
		::fwServices::OSR::setRootObjectConfigurationName(objectConfigurationName) ;
		::fwServices::OSR::setRootObjectConfigurationFile(objectConfigurationFile) ;
	}
	else
	{
	    SLM_FATAL(" Bundle gui, missing param : rootObject, config, configFile in profile");
	}

	

	if( this->getBundle()->hasParameter("startingMode") )
	{

			std::cout<< "---> Manager" << std::endl;

		::guiQt::Manager::initialize() ;
	}
	else
	{
		
	}
}*/

void Plugin::start() throw(::fwRuntime::RuntimeException)
{
    SLM_TRACE("starting gui bundle");

    if ( this->getBundle()->hasParameter("rootObject")
            && this->getBundle()->hasParameter("config")
            && this->getBundle()->hasParameter("configFile"))
    {
        std::string objectClassName(  this->getBundle()->getParameterValue("rootObject") ) ;
        std::string objectConfigurationName( this->getBundle()->getParameterValue("config") ) ;
        std::string objectConfigurationFile( this->getBundle()->getParameterValue("configFile") ) ;

        ::fwServices::OSR::setRootObjectClassName( objectClassName ) ;
        ::fwServices::OSR::setRootObjectConfigurationName(objectConfigurationName) ;
        ::fwServices::OSR::setRootObjectConfigurationFile(objectConfigurationFile) ;
    }
    else
    {
        SLM_FATAL(" Bundle gui, missing param : rootObject, config, configFile in profile");
    }

    SLM_FATAL_IF("Depreciated parameter Aspect", this->getBundle()->hasParameter("Aspect"));

    if( this->getBundle()->hasParameter("startingMode")
            &&  this->getBundle()->getParameterValue("startingMode") == "windows")
    {
        ::fwRuntime::profile::Profile::sptr profile = ::fwRuntime::profile::getCurrentProfile();
        SLM_ASSERT("Profile is not initialized", profile);
        ::fwRuntime::profile::Profile::ParamsContainer params = profile->getParams();
	
	// 
	int    argc = params.size();
        char** argv = profile->getRawParams();

	::guiQt::Manager::initialize();
    }
    else
    {
        SLM_FATAL("Starting Mode not available");
    }
}


//-----------------------------------------------------------------------------

void Plugin::stop() throw()
{}

} // namespace gui
