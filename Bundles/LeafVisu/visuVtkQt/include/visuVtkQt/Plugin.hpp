/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUVTKQT_PLUGINN_HPP_
#define _VISUVTKQT_PLUGINN_HPP_

#include <fwRuntime/Plugin.hpp>

#include "visuVtkQt/config.hpp"

namespace visuVtkQt
{
/**
 * @brief   This class is started when the bundles is loaded.
 * @class   Plugin.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 */
struct  Plugin : public ::fwRuntime::Plugin
{
    /**
     * @brief   destructor
     */
    ~Plugin() throw();
    
    // Overrides
     void start() throw(::fwRuntime::RuntimeException);

    // Overrides    
     void stop() throw();
    
};

} // namespace visuVTK

#endif //_VISUVTK_PLUGIN_HPP_
