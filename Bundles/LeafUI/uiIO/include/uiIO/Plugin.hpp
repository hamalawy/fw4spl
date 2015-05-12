/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIIO_PLUGIN_HPP__
#define __UIIO_PLUGIN_HPP__

#include <fwRuntime/Plugin.hpp>


namespace uiIO
{

class Plugin : public ::fwRuntime::Plugin
{

public:

    /// PLugin destructor
    ~Plugin() throw();

    /// This method is used by runtime to initialize the bundle.
    void start() throw( ::fwRuntime::RuntimeException );

    /// This method is used by runtime to stop the bundle.
    void stop() throw();

};

} // namespace uiIO

#endif //__UIIO_PLUGIN_HPP__
