/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWATOMSFILTER_CONFIG_HPP_
#define _FWATOMSFILTER_CONFIG_HPP_

    #ifdef _WIN32

        #ifdef FWATOMSFILTER_EXPORTS
            #define FWATOMSFILTER_API __declspec(dllexport)
        #else
            #define FWATOMSFILTER_API __declspec(dllimport)
        #endif

        #define FWATOMSFILTER_CLASS_API

        #pragma warning(disable: 4290)

    #elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

        #ifdef FWATOMSFILTER_EXPORTS
            #define FWATOMSFILTER_API __attribute__ ((visibility("default")))
            #define FWATOMSFILTER_CLASS_API __attribute__ ((visibility("default")))
        #else
            #define FWATOMSFILTER_API __attribute__ ((visibility("hidden")))
            #define FWATOMSFILTER_CLASS_API __attribute__ ((visibility("hidden")))
        #endif

    #else

        #define FWATOMSFILTER_API
        #define FWATOMSFILTER_CLASS_API

    #endif

#endif //FWATOMSFILTER_API

