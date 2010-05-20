/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <fwCore/base.hpp>

#include <fwTools/ClassFactoryRegistry.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>

#include "guiQt/action/QuitAction.hpp"
#include "guiQt/action/IAction.hpp"
#include <fwRuntime/profile/Profile.hpp>


namespace guiQt
{
namespace action
{
REGISTER_SERVICE( ::guiQt::action::IAction , ::guiQt::action::QuitAction , ::fwTools::Object ) ;

QuitAction::QuitAction() throw()
{
}

//-----------------------------------------------------------------------------

QuitAction::~QuitAction() throw() 
{
}

//-----------------------------------------------------------------------------

void QuitAction::info(std::ostream &_sstream )
{
    _sstream << "Quit Action" << std::endl;
}

//-----------------------------------------------------------------------------

void QuitAction::updating() throw( ::fwTools::Failed )
{
  // Lancement de l action   ---> correspond au  getDefault()->m_serviceCallBacks[id].lock()->update() ;
    SLM_INFO("QuitAction::updating");
    
    ::fwServices::OSR::uninitializeRootObject();
    qApp->activeWindow()->close();
}
}
}
