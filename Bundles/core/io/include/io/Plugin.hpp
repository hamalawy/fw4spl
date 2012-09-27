/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IO_PLUGIN_HPP_
#define _IO_PLUGIN_HPP_

#include <fwRuntime/Plugin.hpp>

namespace io
{
/**
 * @brief   Implements a plugin for the io bundle.
 */
struct Plugin : public ::fwRuntime::Plugin
{
    /**
     * @brief   Destructor
     */
    ~Plugin() throw();

    /**
     * @brief Start method. (does nothing)
     *
     * This method is used by runtime in order to initialize the bundle.
     *
     * @exception ::fwRuntime::RuntimeException.
     */
    void start() throw(::fwRuntime::RuntimeException);

    /**
     * @brief Stop method. (does nothing)
     *
     * This method is used by runtime in order to close the bundle.
     */
    void stop() throw();

};

} // namespace io

#endif //_IO_PLUGIN_HPP_
