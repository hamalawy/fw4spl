/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include <fwServices/macros.hpp>

#include <fwXML/ImageFileFormatService.hpp>

#include "ioXML/Plugin.hpp"

namespace ioXML
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("ioXML::Plugin");

Plugin::~Plugin() throw()
{}

void Plugin::start() throw(::fwRuntime::RuntimeException)
{
    if ( this->getBundle()->hasParameter("defaultImageWriter") )
    {
        std::string libwriter(  this->getBundle()->getParameterValue("defaultImageWriter") ) ;
        ::fwXML::ImageFileFormatService::setPreferedWriter( libwriter );
    }
}

void Plugin::stop() throw()
{}

} // namespace operators
