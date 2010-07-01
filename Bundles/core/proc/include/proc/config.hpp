/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef PROC_CONFIG_HPP_
#define PROC_CONFIG_HPP_


#ifdef _WIN32

	#ifdef PROC_EXPORTS
	#define PROC_API __declspec(dllexport)
	#else
	#define PROC_API __declspec(dllimport)
	#endif
	
	#pragma warning(disable: 4290)
	#define PROC_CLASS_API
	
#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

	#ifdef PROC_EXPORTS
	#define PROC_API __attribute__ ((visibility("default")))
	#define PROC_CLASS_API __attribute__ ((visibility("default")))
	#else
	#define PROC_API __attribute__ ((visibility("hidden")))
	#define PROC_CLASS_API __attribute__ ((visibility("hidden")))
	#endif

#else

	#define PROC_API
	#define PROC_CLASS_API

#endif


#endif //PROC_CONFIG_HPP_
