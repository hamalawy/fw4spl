/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWCOMED_FLOATMSG_HPP_
#define _FWCOMED_FLOATMSG_HPP_

#include <fwServices/ObjectMsg.hpp>

#include "fwComEd/config.hpp"


namespace fwComEd
{

/**
 * @brief   Object event message specialized for float field : store modification information
 * @class   FloatMsg
 * 
 * @date    2007-2009.
 * @see     ::fwServices::ObjectMsg
 */
class FWCOMED_CLASS_API FloatMsg : public fwServices::ObjectMsg
{

public:

    fwCoreClassDefinitionsWithFactoryMacro((FloatMsg)(::fwServices::ObjectMsg), ( ( ) ),
                                           ::fwServices::factory::message::New< FloatMsg >);

    /// Event identifier used to inform that the value is modified.
    FWCOMED_API static std::string VALUE_IS_MODIFIED;

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWCOMED_API FloatMsg(::fwServices::ObjectMsg::Key key);

    /**
     * @brief   Destructor : does nothing.
     */
    FWCOMED_API virtual ~FloatMsg() throw();
};


} // namespace fwComEd

#endif //_FWCOMED_FloatMSG_HPP_

