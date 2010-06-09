/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOQVTK_PLUGIN_HPP_
#define _IOQVTK_PLUGIN_HPP_

#include <fwRuntime/Plugin.hpp>
#include "ioQVTK/config.hpp"
/**
 * @brief   Namespace ioVTK.
 * @namespace   ioVTK.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 *  The namespace ioVTK contains reader, writer and helper using the vtkIO lib for output and input actions between data and the frameWork
 */
namespace ioQVTK
{

class IOQVTK_CLASS_API Plugin : public ::fwRuntime::Plugin
{
public:
    //! @brief Destructor.
    IOQVTK_API ~Plugin() throw();

    /**
    * @brief Start method.
    *
    * @exception ::fwRuntime::RuntimeException.
    * This method is used by runtime in order to initialize the bundle.
    */
    IOQVTK_API void start() throw(::fwRuntime::RuntimeException);

    /**
    * @brief Stop method.
    *
    * This method is used by runtime in order to close the bundle.
    */
    IOQVTK_API void stop() throw();

};

} // namespace ioVTK

#endif //_IOQVTK_PLUGIN_HPP_
