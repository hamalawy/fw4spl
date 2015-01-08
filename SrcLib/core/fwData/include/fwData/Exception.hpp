/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_EXCEPTION_HPP__
#define __FWDATA_EXCEPTION_HPP__

#include <exception>
#include <string>

#include "fwCore/Exception.hpp"
#include "fwData/config.hpp"

namespace fwData
{

/**
 * @brief   Implements data exception class.
 */
struct FWDATA_CLASS_API Exception : public ::fwCore::Exception
{

    /**
     * @brief       Constructor
     *
     * @param[in]   message a string containing the failure message
     */
    FWDATA_API Exception(const std::string &message) throw();

    /**
     * @brief   Destructor : do nothing.
     */
    FWDATA_API ~Exception() throw();


};

} // namespace fwData

#endif // __FWDATA_EXCEPTION_HPP__

