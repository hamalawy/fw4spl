/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <wx/app.h>

#include <fwCore/base.hpp>

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include <fwRuntime/profile/Profile.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/RootManager.hpp>

#include <fwGuiWx/App.hpp>

#include "guiWx/Plugin.hpp"

namespace guiWx
{
//-----------------------------------------------------------------------------

// Create a new application object
IMPLEMENT_APP_NO_MAIN(::fwGuiWx::App);

//-----------------------------------------------------------------------------

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::guiWx::Plugin");

//-----------------------------------------------------------------------------

Plugin::~Plugin() throw()
{}

//-----------------------------------------------------------------------------

void Plugin::start() throw(::fwRuntime::RuntimeException)
{
    SLM_TRACE_FUNC();

    ::fwRuntime::profile::Profile::sptr profile = ::fwRuntime::profile::getCurrentProfile();
    SLM_ASSERT("Profile is not initialized", profile);
    ::fwRuntime::profile::Profile::ParamsContainer params = profile->getParams();
    int    argc = params.size();
    char** argv = profile->getRawParams();
    ::wxEntry( argc, argv ) ;
}

//-----------------------------------------------------------------------------

void Plugin::stop() throw()
{}

} // namespace guiWx
