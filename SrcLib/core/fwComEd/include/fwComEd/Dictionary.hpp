/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWCOMED_DICTIONARY_HPP_
#define _FWCOMED_DICTIONARY_HPP_

#include <string>

#include "fwComEd/config.hpp"

namespace fwComEd
{
/**
 * @brief   Centralization of all identifiers used for communication and storage of dynamic attributes
 * @note    probably not dynamic enough but allows at least to have an overview of information that can be exchanged
 * 

 * @date    2007-2009.
 */
struct FWCOMED_CLASS_API Dictionary
{
    /// position oriented IDs
    FWCOMED_API const static std::string position;

    /// visualization oriented IDs
    /// @{
    FWCOMED_API static const std::string localGui;
    FWCOMED_API static const std::string m_axialSliceCountId;
    FWCOMED_API static const std::string m_frontalSliceCountId;
    FWCOMED_API static const std::string m_sagittalSliceCountId;
    FWCOMED_API static const std::string m_axialSliceIndexId;
    FWCOMED_API static const std::string m_frontalSliceIndexId;
    FWCOMED_API static const std::string m_sagittalSliceIndexId;
    FWCOMED_API static const std::string m_transferFunctionCompositeId;
    FWCOMED_API static const std::string m_globalMatrixId;
    FWCOMED_API static const std::string m_errorId;
    FWCOMED_API static const std::string m_imageLandmarksId;
    FWCOMED_API static const std::string m_imageDistancesId;
    FWCOMED_API static const std::string m_commentId;
    FWCOMED_API static const std::string m_colorId;
    FWCOMED_API static const std::string m_labelId;         ///< to assign a label
    FWCOMED_API static const std::string m_relatedServiceId; ///< to store uid (simple) of service which create the object
    FWCOMED_API static const std::string m_imageLabelId;
    FWCOMED_API static const std::string m_acquisitionCountId;
    /// @}

    /// processing oriented IDs
    /// @{
    FWCOMED_API static const std::string m_roi;
    /// @}


    /// processing oriented data
    /// @{
    FWCOMED_API static const std::string DATA_IS_LOCKED;
    /// @}
};

}

#endif /*_FWCOMED_DICTIONARY_HPP_*/
