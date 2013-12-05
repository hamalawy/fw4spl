/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FILTERVRRENDER_CONFIG_HPP_
#define _FILTERVRRENDER_CONFIG_HPP_


#ifdef _WIN32

    #ifdef FILTERVRRENDER_EXPORTS
    #define FILTERVRRENDER_API __declspec(dllexport)
    #else
    #define FILTERVRRENDER_API __declspec(dllimport)
    #endif

    #define FILTERVRRENDER_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef FILTERVRRENDER_EXPORTS
    #define FILTERVRRENDER_API __attribute__ ((visibility("default")))
    #define FILTERVRRENDER_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define FILTERVRRENDER_API __attribute__ ((visibility("hidden")))
    #define FILTERVRRENDER_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define FILTERVRRENDER_API
    #define FILTERVRRENDER_CLASS_API

#endif

#endif //_FILTERVRRENDER_CONFIG_HPP_

