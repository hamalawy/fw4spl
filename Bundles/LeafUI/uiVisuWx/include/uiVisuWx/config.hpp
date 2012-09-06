/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIVISUWX_CONFIG_HPP_
#define _UIVISUWX_CONFIG_HPP_


#ifdef _WIN32

    #ifdef UIVISUWX_EXPORTS
    #define UIVISUWX_API __declspec(dllexport)
    #else
    #define UIVISUWX_API __declspec(dllimport)
    #endif

    #define UIVISUWX_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef UIVISUWX_EXPORTS
    #define UIVISUWX_API __attribute__ ((visibility("default")))
    #define UIVISUWX_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define UIVISUWX_API __attribute__ ((visibility("hidden")))
    #define UIVISUWX_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define UIVISUWX_API
    #define UIVISUWX_CLASS_API

#endif

#endif //_UIVISUWX_CONFIG_HPP_
