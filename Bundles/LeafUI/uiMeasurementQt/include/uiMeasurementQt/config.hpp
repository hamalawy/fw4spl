/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIMEASUREMENTQT_CONFIG_HPP_
#define _UIMEASUREMENTQT_CONFIG_HPP_


#ifdef _WIN32

    #ifdef UIMEASUREMENTQT_EXPORTS
    #define UIMEASUREMENTQT_API __declspec(dllexport)
    #else
    #define UIMEASUREMENTQT_API __declspec(dllimport)
    #endif

    #define UIMEASUREMENTQT_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef UIMEASUREMENTQT_EXPORTS
    #define UIMEASUREMENTQT_API __attribute__ ((visibility("default")))
    #define UIMEASUREMENTQT_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define UIMEASUREMENTQT_API __attribute__ ((visibility("hidden")))
    #define UIMEASUREMENTQT_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define UIMEASUREMENTQT_API
    #define UIMEASUREMENTQT_CLASS_API

#endif

#endif //_UIMEASUREMENTQT_CONFIG_HPP_
