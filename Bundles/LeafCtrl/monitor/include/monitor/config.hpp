/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _MONITOR_CONFIG_HPP_
#define _MONITOR_CONFIG_HPP_

#ifdef _WIN32

    #ifdef MONITOR_EXPORTS
    #define MONITOR_API __declspec(dllexport)
    #else
    #define MONITOR_API __declspec(dllimport)
    #endif

    #define MONITOR_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef MONITOR_EXPORTS
    #define MONITOR_API __attribute__ ((visibility("default")))
    #define MONITOR_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define MONITOR_API __attribute__ ((visibility("hidden")))
    #define MONITOR_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define MONITOR_API
    #define MONITOR_CLASS_API

#endif

#endif // _MONITOR_CONFIG_HPP_
