/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIQTIMAGE_CONFIG_HPP_
#define _UIQTIMAGE_CONFIG_HPP_


#ifdef _WIN32

    #ifdef UIQTIMAGE_EXPORTS
    #define UIQTIMAGE_API __declspec(dllexport)
    #else
    #define UIQTIMAGE_API __declspec(dllimport)
    #endif

    #define UIQTIMAGE_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef UIQTIMAGE_EXPORTS
    #define UIQTIMAGE_API __attribute__ ((visibility("default")))
    #define UIQTIMAGE_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define UIQTIMAGE_API __attribute__ ((visibility("hidden")))
    #define UIQTIMAGE_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define UIQTIMAGE_API
    #define UIQTIMAGE_CLASS_API

#endif

#endif //_UIIMAGE_CONFIG_HPP_
