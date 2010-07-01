/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef _CTRLFIELD_CONFIG_HPP_
#define _CTRLFIELD_CONFIG_HPP_

#ifdef _WIN32

	#ifdef CTRLFIELD_EXPORTS
	#define CTRLFIELD_API __declspec(dllexport)
	#else
	#define CTRLFIELD_API __declspec(dllimport)
	#endif

	#define CTRLFIELD_CLASS_API

	#pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

	#ifdef CTRLFIELD_EXPORTS
	#define CTRLFIELD_API __attribute__ ((visibility("default")))
	#define CTRLFIELD_CLASS_API __attribute__ ((visibility("default")))
	#else
	#define CTRLFIELD_API __attribute__ ((visibility("hidden")))
	#define CTRLFIELD_CLASS_API __attribute__ ((visibility("hidden")))
	#endif

#else

	#define CTRLFIELD_API
	#define CTRLFIELD_CLASS_API

#endif

#endif //_CTRLFIELD_CONFIG_HPP_
