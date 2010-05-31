/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Contributors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */

#ifndef _FWCOMED_IMAGEMSG_HPP_
#define _FWCOMED_IMAGEMSG_HPP_

#include <fwData/Object.hpp>
#include <fwData/Integer.hpp>

#include <fwServices/ObjectMsg.hpp>

#include "fwComEd/export.hpp"


namespace fwComEd
{

/**
 * @brief   Object event message specialized for image : store modification information
 * @class   ImageMsg
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 * @see     ::fwServices::ObjectMsg
 */
class FWCOMED_CLASS_API ImageMsg : public ::fwServices::ObjectMsg
{
public:
    fwCoreClassDefinitionsWithFactoryMacro((ImageMsg)(::fwServices::ObjectMsg), ( () ), new ImageMsg );

    /**
     * @name Event identifier
     */
    /// @{
    /// Event identifier used to inform for modification
    FWCOMED_API static std::string NEW_IMAGE;
    FWCOMED_API static std::string BUFFER;
    FWCOMED_API static std::string DIMENSION;
    FWCOMED_API static std::string SPACING;
    FWCOMED_API static std::string REGION;
    FWCOMED_API static std::string PIXELTYPE;
    FWCOMED_API static std::string TRANSFERTFUNCTION;
    FWCOMED_API static std::string WINDOWING;
    FWCOMED_API static std::string LANDMARK;
    FWCOMED_API static std::string NEW_DISTANCE;
    FWCOMED_API static std::string DISTANCE;
    FWCOMED_API static std::string DELETE_DISTANCE;
    FWCOMED_API static std::string SLICE_INDEX;
    FWCOMED_API static std::string ACTIVATE_SYNC_CROSS;
    FWCOMED_API static std::string INACTIVATE_SYNC_CROSS;
    FWCOMED_API static std::string CHANGE_SLICE_TYPE;
    FWCOMED_API static std::string OPACITY; ///< Notion of opacity. Used for negatoscope management at first time
    /// @}

    /**
     * @brief Constructor : does nothing.
     */
    FWCOMED_API ImageMsg() throw();

    /**
     * @brief   Destuctor : does nothing.
     */
    FWCOMED_API virtual ~ImageMsg() throw();

    FWCOMED_API void setSliceIndex(::fwData::Integer::sptr a, ::fwData::Integer::sptr f, ::fwData::Integer::sptr s, ::fwData::Object::sptr _pDataInfo = ::fwData::Object::sptr() );
    FWCOMED_API void setWindowMinMax(::fwData::Integer::sptr min, ::fwData::Integer::sptr max, ::fwData::Object::sptr _pDataInfo = ::fwData::Object::sptr() );

    FWCOMED_API void getSliceIndex(::fwData::Integer::sptr a, ::fwData::Integer::sptr f, ::fwData::Integer::sptr s ) const;
    FWCOMED_API void getWindowMinMax(::fwData::Integer::sptr min, ::fwData::Integer::sptr max ) const;

protected:
    ::fwData::Integer::sptr m_axialIndex;
    ::fwData::Integer::sptr m_frontalIndex;
    ::fwData::Integer::sptr m_sagittalIndex;

    ::fwData::Integer::sptr m_windowMin;
    ::fwData::Integer::sptr m_windowMax;


};

} // namespace fwComEd

#endif //_FWCOMED_IMAGEMSG_HPP_

