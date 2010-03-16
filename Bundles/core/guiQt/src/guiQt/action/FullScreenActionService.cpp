/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <sstream>

#include <fwCore/base.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>

#include "guiQt/action/FullScreenActionService.hpp"

namespace guiQt
{
namespace action
{

REGISTER_SERVICE( ::guiQt::action::IAction , ::guiQt::action::FullScreenActionService , ::fwTools::Object ) ;

FullScreenActionService::FullScreenActionService() throw(): m_isFullScreen(false)
{
}

//-----------------------------------------------------------------------------

FullScreenActionService::~FullScreenActionService() throw()
{
}

//-----------------------------------------------------------------------------

void FullScreenActionService::configuring() throw( ::fwTools::Failed )
{
    SLM_INFO("FullScreenActionService::configuring");
    this->::guiQt::action::IAction::configuring();
}

//-----------------------------------------------------------------------------

void FullScreenActionService::updating() throw(::fwTools::Failed)
{
    SLM_INFO("FullScreenActionService::updating");
   
    //this->::guiQt::action::IAction::updating();
    
     QWidget *mainWidget = qApp->activeWindow();

     m_isFullScreen = !m_isFullScreen;

     if(m_isFullScreen)
     {
	mainWidget->setWindowState(Qt::WindowFullScreen);
     }
     else
     {
        mainWidget->setWindowState(Qt::WindowNoState);
     }

   
}

//-----------------------------------------------------------------------------

void FullScreenActionService::info(std::ostream &_sstream )
{
    _sstream << "FullScreen Action" << std::endl;
}

}

}



