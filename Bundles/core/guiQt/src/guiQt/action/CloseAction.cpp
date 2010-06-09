/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwTools/ClassFactoryRegistry.hpp>
#include <fwServices/bundle/runtime.hpp>
#include <fwServices/macros.hpp>
#include <fwRuntime/helper.hpp>
#include <fwData/Object.hpp>
#include <fwRuntime/helper.hpp>
#include <fwRuntime/Extension.hpp>

#include "guiQt/action/CloseAction.hpp"

namespace guiQt
{
namespace action
{
REGISTER_SERVICE( ::guiQt::action::IAction , ::guiQt::action::CloseAction , ::fwTools::Object ) ;

CloseAction::CloseAction() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

CloseAction::~CloseAction() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void CloseAction::info(std::ostream &_sstream )
{
    SLM_TRACE_FUNC();
    _sstream << "Close Action" << std::endl;
}

//-----------------------------------------------------------------------------

void CloseAction::updating() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    ::fwServices::OSR::uninitializeRootObject();
}

}
}
