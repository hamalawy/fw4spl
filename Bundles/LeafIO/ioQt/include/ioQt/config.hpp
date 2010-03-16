/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOQT_CONFIG_HPP_
#define _IOQT_CONFIG_HPP_


#ifdef _WIN32
    #pragma warning( disable : 4251 )


    #ifdef IOQT_EXPORTS
    #define IOQT_API __declspec(dllexport)
    #else
    #define IOQT_API __declspec(dllimport)
    #endif

    #define IOQT_CLASS_API
    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef IOQT_EXPORTS
    #define IOQT_API __attribute__ ((visibility("default")))
    #define IOQT_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define IOQT_API __attribute__ ((visibility("hidden")))
    #define IOQT_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define IOQT_API
    #define IOQT_CLASS_API

#endif

#endif // #ifndef _GUIQT_CONFIG_HPP_
