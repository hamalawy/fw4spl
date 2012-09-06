/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef VISUVTKWX_CONFIG_HPP_
#define VISUVTKWX_CONFIG_HPP_


#ifdef _WIN32

    #ifdef VISUVTKWX_EXPORTS
    #define VISUVTKWX_API __declspec(dllexport)
    #else
    #define VISUVTKWX_API __declspec(dllimport)
    #endif

    #define VISUVTKWX_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef VISUVTKWX_EXPORTS
    #define VISUVTKWX_API __attribute__ ((visibility("default")))
    #define VISUVTKWX_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define VISUVTKWX_API __attribute__ ((visibility("hidden")))
    #define VISUVTKWX_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define VISUVTKWX_API
    #define VISUVTKWX_CLASS_API

#endif

#endif //VISUVTKWX_CONFIG_HPP_
