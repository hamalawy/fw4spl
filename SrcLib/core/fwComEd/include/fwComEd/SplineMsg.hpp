/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWCOMED_SPLINEMSG_HPP_
#define _FWCOMED_SPLINEMSG_HPP_

#include <fwServices/ObjectMsg.hpp>

#include "fwComEd/export.hpp"


namespace fwComEd
{

/**
 * @brief   Object message specialized for Spline : store modification information
 * @class   SplineMsg
 * 
 * @date    2007-2009.
 * @see     ::fwServices::ObjectMsg
 */
class FWCOMED_CLASS_API SplineMsg : public fwServices::ObjectMsg
{

public:

    fwCoreClassDefinitionsWithFactoryMacro((SplineMsg)(::fwServices::ObjectMsg), ( ( ) ),
                                           ::fwServices::factory::message::New< SplineMsg > );

    /// Event identifier used to inform for modification
    FWCOMED_API static std::string NEW_SPLINE;

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWCOMED_API SplineMsg(::fwServices::ObjectMsg::Key key);

    /**
     * @brief   Destuctor : do nothing.
     */
    FWCOMED_API virtual ~SplineMsg() throw();

};

} // namespace op

#endif //_FWCOMED_SPLINEMSG_HPP_

