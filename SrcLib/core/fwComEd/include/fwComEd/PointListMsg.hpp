/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWCOMED_POINTLISTMSG_HPP_
#define _FWCOMED_POINTLISTMSG_HPP_

#include <fwServices/ObjectMsg.hpp>

#include "fwComEd/export.hpp"


namespace fwComEd
{

/**
 * @brief   Object event message specialized for PointList : store modification information
 * @class   PointListMsg
 * 
 * @date    2007-2009.
 * @see     ::fwServices::ObjectMsg
 */
class FWCOMED_CLASS_API PointListMsg : public fwServices::ObjectMsg
{

public:

    fwCoreClassDefinitionsWithFactoryMacro((PointListMsg)(::fwServices::ObjectMsg), ( ( ) ),
                                           ::fwServices::factory::message::New< PointListMsg >);

    /**
     * @name Event identifier
     * @{
     */
    /// Event identifier used to inform for modification
    FWCOMED_API static std::string ELEMENT_MODIFIED;
    FWCOMED_API static std::string ELEMENT_ADDED;
    FWCOMED_API static std::string ELEMENT_REMOVED;
    /// @}

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWCOMED_API PointListMsg(::fwServices::ObjectMsg::Key key);

    /**
     * @brief   Destructor : does nothing.
     */
    FWCOMED_API virtual ~PointListMsg() throw();
};


} // namespace fwComEd

#endif //_FWCOMED_POINTLISTMSG_HPP_

