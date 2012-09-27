/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIMEASUREMENT_CONFIG_HPP_
#define _UIMEASUREMENT_CONFIG_HPP_


#ifdef _WIN32

    #ifdef UIMEASUREMENT_EXPORTS
    #define UIMEASUREMENT_API __declspec(dllexport)
    #else
    #define UIMEASUREMENT_API __declspec(dllimport)
    #endif

    #define UIMEASUREMENT_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef UIMEASUREMENT_EXPORTS
    #define UIMEASUREMENT_API __attribute__ ((visibility("default")))
    #define UIMEASUREMENT_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define UIMEASUREMENT_API __attribute__ ((visibility("hidden")))
    #define UIMEASUREMENT_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define UIMEASUREMENT_API
    #define UIMEASUREMENT_CLASS_API

#endif

#endif //_UIMEASUREMENT_CONFIG_HPP_
