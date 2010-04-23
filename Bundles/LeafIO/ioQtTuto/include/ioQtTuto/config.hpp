/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef IOQTTUTO_CONFIG_HPP_
#define IOQTTUTO_CONFIG_HPP_


#ifdef _WIN32

    #ifdef IOQTTUTO_EXPORTS
    #define IOQTTUTO_API __declspec(dllexport)
    #else
    #define IOQTTUTO_API __declspec(dllimport)
    #endif

    #define IOQTTUTO_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef IOQTTUTO_EXPORTS
    #define IOQTTUTO_API __attribute__ ((visibility("default")))
    #define IOQTTUTO_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define IOQTTUTO_API __attribute__ ((visibility("hidden")))
    #define IOQTTUTO_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define IOQTTUTO_API
    #define IOQTTUTO_CLASS_API

#endif

#endif //IOQTTUTO_CONFIG_HPP_
