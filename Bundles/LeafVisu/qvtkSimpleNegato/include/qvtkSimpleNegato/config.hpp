/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef QVTKSIMPLENEGATO_CONFIG_HPP_
#define QVTKSIMPLENEGATO_CONFIG_HPP_


#ifdef _WIN32

    #ifdef QVTKSIMPLENEGATO_EXPORTS
    #define QVTKSIMPLENEGATO_API __declspec(dllexport)
    #else
    #define QVTKSIMPLENEGATO_API __declspec(dllimport)
    #endif

    #define QVTKSIMPLENEGATO_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef QVTKSIMPLENEGATO_EXPORTS
    #define QVTKSIMPLENEGATO_API __attribute__ ((visibility("default")))
    #define QVTKSIMPLENEGATO_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define QVTKSIMPLENEGATO_API __attribute__ ((visibility("hidden")))
    #define QVTKSIMPLENEGATO_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define QVTKSIMPLENEGATO_API
    #define QVTKSIMPLENEGATO_CLASS_API

#endif

#endif //VTKSIMPLENEGATO_CONFIG_HPP_
