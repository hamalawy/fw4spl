/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWCOMED_POINTMSG_HPP_
#define _FWCOMED_POINTMSG_HPP_

#include <fwServices/ObjectMsg.hpp>

#include "fwComEd/config.hpp"


namespace fwComEd
{

/**
 * @brief   Object message specialized for Point : store modification information
 * @class   PointMsg
 * 
 * @date    2009
 * @see     ::fwServices::ObjectMsg
 */
class FWCOMED_CLASS_API PointMsg : public ::fwServices::ObjectMsg
{

public:

    fwCoreClassDefinitionsWithFactoryMacro((PointMsg)(::fwServices::ObjectMsg), ( () ),
                                            ::fwServices::factory::message::New< PointMsg > );

    /**
     * @name Event identifier
     * @{
     */
    /// @brief Event identifier used to inform for modification
    FWCOMED_API static std::string VALUE_IS_MODIFIED;
    FWCOMED_API static std::string POINT_IS_MODIFIED;
    FWCOMED_API static std::string START_POINT_INTERACTION;
    /// @}

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWCOMED_API PointMsg(::fwServices::ObjectMsg::Key key);

    /**
      * @brief  Destuctor : does nothing.
      */
    FWCOMED_API virtual ~PointMsg() throw();

};

} // namespace fwComEd

#endif //_FWCOMED_POINTMSG_HPP_

