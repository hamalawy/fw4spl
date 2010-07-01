/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef _PROC_PLUGIN_HPP_
#define _PROC_PLUGIN_HPP_

#include <fwRuntime/Plugin.hpp>

namespace proc
{
/**
 * @brief Plugin
 */
struct Plugin : public ::fwRuntime::Plugin
{
	/**
	 * @brief	destructor
	 */
	~Plugin() throw();
	
	// Overrides
	void start() throw(::fwRuntime::RuntimeException);

	// Overrides	
	void stop() throw();
	
};

} // namespace proc

#endif //_PROC_PLUGIN_HPP_
