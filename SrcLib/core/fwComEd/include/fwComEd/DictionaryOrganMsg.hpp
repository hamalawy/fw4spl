/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWCOMED_DICTIONARYORGANMSG_HPP_
#define _FWCOMED_DICTIONARYORGANMSG_HPP_

#include <fwServices/ObjectMsg.hpp>

#include "fwComEd/config.hpp"


namespace fwComEd
{

/**
 * @brief   Object event message specialized for DictionaryOrgan : store modification information
 * @class   DictionaryOrganMsg
 * 
 * @date    2007-2009.
 * @see     ::fwServices::ObjectMsg
 */
class FWCOMED_CLASS_API DictionaryOrganMsg : public fwServices::ObjectMsg
{

public:

    fwCoreClassDefinitionsWithFactoryMacro((DictionaryOrganMsg)(::fwServices::ObjectMsg), ( ( ) ),
                                            ::fwServices::factory::message::New< DictionaryOrganMsg > );

    /// Event identifier used to inform that the value is modified.
    FWCOMED_API static std::string ROI4ORGAN_EXP_IS_MODIFIED;
    FWCOMED_API static std::string ROI4ORGAN_NODE_IS_MODIFIED;
    FWCOMED_API static std::string MASK_DATA_NODE_IS_MODIFIED;
    FWCOMED_API static std::string MESH_DATA_NODE_IS_MODIFIED;

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWCOMED_API DictionaryOrganMsg(::fwServices::ObjectMsg::Key key);

    /**
     * @brief   Destructor : does nothing.
     */
    FWCOMED_API virtual ~DictionaryOrganMsg() throw();
};


} // namespace fwComEd

#endif //_FWCOMED_DICTIONARYORGANMSG_HPP_

