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

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>

#include <fwData/Image.hpp>
#include <fwData/Composite.hpp>

#include "ctrlField/Plugin.hpp"
#include "ctrlField/ImageSynchronizer.hpp"
#include "ctrlField/IFieldSynchronizer.hpp"

namespace ctrlField
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::ctrlField::Plugin");

Plugin::~Plugin() throw()
{}

void Plugin::start() throw(::fwRuntime::RuntimeException)
{
	REGISTER_SERVICE( ::ctrlField::IFieldSynchronizer , ::ctrlField::ImageSynchronizer , ::fwData::Composite ) ;
	REGISTER_SERVICE( ::ctrlField::IFieldSynchronizer , ::ctrlField::ImageSynchronizer , ::fwTools::Object ) ;
}

void Plugin::stop() throw()
{}

} // namespace ctrlField
