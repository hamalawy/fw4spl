/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwTools/ClassFactoryRegistry.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>


#include "guiQt/action/NothingAction.hpp"

namespace guiQt
{
namespace action
{
REGISTER_SERVICE( ::guiQt::action::IAction , ::guiQt::action::NothingAction , ::fwTools::Object ) ;


NothingAction::NothingAction() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

NothingAction::~NothingAction() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void NothingAction::info(std::ostream &_sstream )
{
    _sstream << "Nothing Action" << std::endl;
}

//-----------------------------------------------------------------------------

void NothingAction::updating() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    SLM_TRACE("NothingAction::update") ;

}
}
}

