/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_CONFIG_HPP__
#define __GDCMIO_CONFIG_HPP__


#ifdef _WIN32

    #ifdef GDCMIO_EXPORTS
    #define GDCMIO_API __declspec(dllexport)
    #define GDCMIO_TEMPLATE_API
    #else
    #define GDCMIO_API __declspec(dllimport)
    #define GDCMIO_TEMPLATE_API extern
    #endif

    #define GDCMIO_CLASS_API

    #pragma warning(disable: 4290)
    #pragma warning(disable: 4800) // avoid waring on return hard coded value

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

        #ifdef GDCMIO_EXPORTS
        #define GDCMIO_API __attribute__ ((visibility("default")))
        #define GDCMIO_CLASS_API __attribute__ ((visibility("default")))
        #define GDCMIO_TEMPLATE_API
        #else
        //#define GDCMIO_API __attribute__ ((visibility("hidden")))
        //#define GDCMIO_CLASS_API __attribute__ ((visibility("hidden")))
        #define GDCMIO_API
        #define GDCMIO_CLASS_API
        #define GDCMIO_TEMPLATE_API extern
        #endif

#else

        #define GDCMIO_API
        #define GDCMIO_CLASS_API
        #define GDCMIO_TEMPLATE_API

#endif


#endif //_GDCMIO_CONFIG_HPP__

