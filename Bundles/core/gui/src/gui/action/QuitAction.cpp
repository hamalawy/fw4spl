/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwServices/macros.hpp>

#include <fwGui/Application.hpp>
#include <fwGui/Cursor.hpp>

#include "gui/action/QuitAction.hpp"

namespace gui
{
namespace action
{

fwServicesRegisterMacro( ::fwGui::IActionSrv , ::gui::action::QuitAction , ::fwData::Object ) ;

//-----------------------------------------------------------------------------

QuitAction::QuitAction() throw()
{}

//-----------------------------------------------------------------------------

QuitAction::~QuitAction() throw()
{}

//-----------------------------------------------------------------------------

void QuitAction::configuring() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC() ;
    this->initialize();
}

//-----------------------------------------------------------------------------

void QuitAction::receiving( ::fwServices::ObjectMsg::csptr _msg ) throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void QuitAction::starting() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    this->actionServiceStarting();
}

//-----------------------------------------------------------------------------

void QuitAction::stopping() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    this->actionServiceStopping();
}

//-----------------------------------------------------------------------------

void QuitAction::info(std::ostream &_sstream )
{
    _sstream << "Quit Action" << std::endl;
}

//-----------------------------------------------------------------------------

void QuitAction::updating() throw( ::fwTools::Failed )
{
    ::fwGui::Cursor cursor;
    cursor.setCursor(::fwGui::ICursor::BUSY);
    ::fwGui::Application::New()->exit(0);
    cursor.setDefaultCursor();
}

//-----------------------------------------------------------------------------

}
}
