/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIIO_PLUGIN_HPP_
#define _UIIO_PLUGIN_HPP_

#include <fwRuntime/Plugin.hpp>

#include "uiIOQt/config.hpp"
/**
 * @brief   Namespace uiIO.
 * @namespace   uiIO.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 *  The namespace uiIO contains a simple service to manipulate IO with IHM.
 */
namespace uiIOQt
{

class UIIOQT_CLASS_API Plugin : public ::fwRuntime::Plugin
{

public :

    /// PLugin destructor
	UIIOQT_API ~Plugin() throw();
    
    /// This method is used by runtime to initialize the bundle.
	UIIOQT_API void start() throw( ::fwRuntime::RuntimeException );

    /// This method is used by runtime to stop the bundle.
	UIIOQT_API void stop() throw();
    
};

} // namespace uiIO

#endif //_UIIO_PLUGIN_HPP_
