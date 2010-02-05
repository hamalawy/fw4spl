/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_CONFIG_HPP_
#define _FWDATA_CONFIG_HPP_

#ifdef _WIN32

        #ifdef FWDATA_EXPORTS
        #define FWDATA_API __declspec(dllexport)
        #else
        #define FWDATA_API __declspec(dllimport)
        #endif

        #define FWDATA_CLASS_API

        #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

        #ifdef FWDATA_EXPORTS
        #define FWDATA_API __attribute__ ((visibility("default")))
        #define FWDATA_CLASS_API __attribute__ ((visibility("default")))
        #else
        #define FWDATA_API __attribute__ ((visibility("hidden")))
        #define FWDATA_CLASS_API __attribute__ ((visibility("hidden")))
        #endif

#else

        #define FWDATA_API
        #define FWDATA_CLASS_API

#endif

#endif //FWDATA_CONFIG_API
