/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifdef _WIN32


#ifndef __FWRUNTIME_DL_WIN32_HPP__
#define __FWRUNTIME_DL_WIN32_HPP__


#include <windows.h>

#include "fwRuntime/config.hpp"
#include "fwRuntime/dl/Native.hpp"


namespace fwRuntime
{

namespace dl
{

/**
 * @brief   Implements a win32 native module.
 * @struct  Win32
 * @date    2004-2009
 *
 */
struct Win32 : public Native
{
    /**
     * @brief       Constructor.
     *
     * @param[in]   modulePath      a path to the module to manage
     */
    Win32( const boost::filesystem::path & modulePath ) throw();

    /**
     * @brief   Tells if the module is loaded.
     *
     * @return  true or false
     */
    bool isLoaded() const throw();

    /**
     * @brief       Retrieves the address of a symbol specified by its name.
     *
     * @param[in]   name    a string containing the symbol name.
     *
     * @return      a pointer to the found symbol or null if none has been found
     */
    void * getSymbol(const std::string& name) const throw(RuntimeException);

    /**
     * @brief   Loads the module.
     */
    void load() throw(RuntimeException);

    /**
     * @brief   Undloads the module.
     */
    void unload() throw(RuntimeException);


    private:

        /**
         * @brief   A handle of the loaded module.
         */
        HMODULE m_handle;

};


} // namespace dl

} // namespace fwRuntime


#endif // __FWRUNTIME_DL_WIN32_HPP__


#endif // #ifdef _WIN32
