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
#include <QDesktopWidget>
#include <iostream>

namespace guiQt
{

// Dans plugin.xml : class = "::guiQt::Plugin"
static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::guiQt::Plugin");

Plugin::~Plugin() throw()
{}

void Plugin::windowClosed()
{    
  SLM_TRACE_FUNC();
  ::fwServices::OSR::uninitializeRootObject();
  
  SLM_TRACE(" OSR Unitialize DONE");

  ::fwRuntime::profile::Profile::sptr profile = ::fwRuntime::profile::getCurrentProfile();
  profile->stop();

  SLM_TRACE(" VRAIMENT LA Fin ");
// OSLM_TRACE(" BOOLEAN : "<<qApp->closingDown() );

}


void Plugin::exit()
{
  SLM_TRACE_FUNC();
  OSLM_TRACE(" BOOLEAN : "<<qApp->closingDown() );

  qApp->flush();
    SLM_TRACE(" <<<<<<<<<<<<<<<<  FLUSH DONE >>>>>>>>>>>>>< ");
  qApp->exit(0);
    SLM_TRACE(" <<<<<<<<<<<<<<<<  EXIT DONE >>>>>>>>>>>>>< ");
    OSLM_TRACE(" BOOLEAN : "<<qApp->closingDown() );

    // ::fwServices::OSR::uninitializeRootObject();
//     ::fwRuntime::profile::Profile::sptr profile = ::fwRuntime::profile::getCurrentProfile();
//     profile->stop();
}

void Plugin::printDestruction()
{
  SLM_TRACE_FUNC();
  OSLM_TRACE(" BOOLEAN : "<<qApp->closingDown() );
  SLM_TRACE(" <<<<<<<<<<<<<<<<  qApp DESTROYED >>>>>>>>>>>>>< ");
  OSLM_TRACE(" BOOLEAN : "<<qApp->closingDown() );

   
}

//-----------------------------------------------------------------------------

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

      
	int argc = 1;
	char** argv = NULL; 
	
	QApplication app( argc,  argv);
  	QObject::connect(&app, SIGNAL(lastWindowClosed()), this, SLOT(windowClosed()));
  	QObject::connect(&app, SIGNAL(aboutToQuit()), this, SLOT(exit()));
	QObject::connect(&app, SIGNAL(destroyed()), this, SLOT(printDestruction()));
	
	QWidget* mainWindow = new QMainWindow();
  
	QDesktopWidget *desk = QApplication::desktop();
	QRect screen = desk->screenGeometry(mainWindow);
	
	mainWindow->resize(screen.width(), screen.height());

	app.setActiveWindow(mainWindow);

	::fwServices::OSR::initializeRootObject();

	mainWindow->show();
	
	app.exec();
	
	//::guiQt::Manager::initialize();
    }
    else
    {
        SLM_FATAL("Starting Mode not available");
    }
}


//-----------------------------------------------------------------------------

void Plugin::stop() throw()
{
SLM_TRACE_FUNC();

}

} // namespace gui
