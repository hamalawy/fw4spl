/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */



#ifndef _FWQT_CONFIG_HPP_
#define _FWQT_CONFIG_HPP_

#ifdef _WIN32

    #ifdef FWQT_EXPORTS
    #define FWQT_API __declspec(dllexport)
    #else
    #define FWQT_API __declspec(dllimport)
    #endif

    #define FWQT_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef FWQT_EXPORTS
    #define FWQT_API __attribute__ ((visibility("default")))
    #define FWQT_CLASS_API __attribute__ ((visibility("default")))
    #else
    //#define FWQT_API __attribute__ ((visibility("hidden")))
    //#define FWQT_CLASS_API __attribute__ ((visibility("hidden")))
    #define FWQT_API
    #define FWQT_CLASS_API
    #endif

#else

    #define FWQT_API
    #define FWQT_CLASS_API

#endif

#endif //_FWQT_CONFIG_HPP_
