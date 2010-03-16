/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef UIQT_CONFIG_HPP_
#define UIQT_CONFIG_HPP_


#ifdef _WIN32

    #ifdef UIQT_EXPORTS
    #define UIQT_API __declspec(dllexport)
    #else
    #define UIQT_API __declspec(dllimport)
    #endif

    #define UIQT_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef UIQT_EXPORTS
    #define UIQT_API __attribute__ ((visibility("default")))
    #define UIQT_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define UIQT_API __attribute__ ((visibility("hidden")))
    #define UIQT_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define UIQT_API
    #define UIQT_CLASS_API

#endif

#endif //VISUVTK_CONFIG_HPP_
