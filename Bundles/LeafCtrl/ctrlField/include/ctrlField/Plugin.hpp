/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef _CTRLFIELD_PLUGIN_HPP_
#define _CTRLFIELD_PLUGIN_HPP_

#include <fwRuntime/Plugin.hpp>

namespace ctrlField
{
/**
 * @brief Plugin
 */
class Plugin : public ::fwRuntime::Plugin
{
public:
	/**
	 * @brief	Destructor
	 */
	~Plugin() throw();
	
	/**
	* @brief Start method.
	*
	* @exception ::fwRuntime::RuntimeException.
	* This method is used by runtime to initialize the bundle.
	*/
	void start() throw(::fwRuntime::RuntimeException);

	/**
	* @brief Stop method.
	*
	* This method is used by runtime to stop the bundle.
	*/
	void stop() throw();
	
};

} // namespace vtkRender

#endif //_CTRLFIELD_PLUGIN_HPP_
