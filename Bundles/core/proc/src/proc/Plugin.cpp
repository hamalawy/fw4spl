/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include <fwServices/macros.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Image.hpp>
#include <fwData/ProcessObject.hpp>

#include "proc/Threshold.hpp"
#include "proc/ThresholdEditor.hpp"
#include "proc/Plugin.hpp"

namespace proc
{
using namespace fwRuntime::utils;
static GenericExecutableFactoryRegistrar<Plugin> registrar("::proc::Plugin");

Plugin::~Plugin() throw()
{}

void Plugin::start() throw(::fwRuntime::RuntimeException)
{
	REGISTER_SERVICE(  ::proc::IProc , ::proc::Threshold , ::fwTools::Object) ;
	REGISTER_SERVICE(  ::proc::IProc , ::proc::Threshold , ::fwData::Composite) ;
	REGISTER_SERVICE(  ::proc::IProc , ::proc::Threshold , ::fwData::ProcessObject) ;
	REGISTER_SERVICE(  ::proc::IProc , ::proc::Threshold , ::fwData::Image) ;

	REGISTER_SERVICE( ::gui::editor::IEditor , ::proc::ThresholdEditor , ::proc::Threshold ) ;
}

void Plugin::stop() throw()
{}

} // namespace proc
