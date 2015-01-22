/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWCOM_HASSLOTS_HPP__
#define __FWCOM_HASSLOTS_HPP__

#include "fwCom/config.hpp"
#include "fwCom/Slots.hpp"

namespace fwCom
{

struct SlotBase;

/**
 * @class   HasSlots
 * @brief   This class proposes a mapping between a SlotKeyType and a SlotBase.
 * @date   2012.
 */
class HasSlots
{

public:

    typedef ::boost::shared_ptr< HasSlots > sptr;

    HasSlots(){}

    SPTR( SlotBase ) slot( const Slots::SlotKeyType & key ) const
    {
        return m_slots[key];
    }

    template< typename SlotType >
    SPTR( SlotType ) slot( const Slots::SlotKeyType & key ) const
    {
        SPTR( SlotType ) slot = ::boost::dynamic_pointer_cast< SlotType >( this->slot(key) );
        return slot;
    }


protected:

    /// Copy constructor forbidden
    HasSlots( const HasSlots& );

    /// Copy operator forbidden
    HasSlots& operator=( const HasSlots& );

    Slots m_slots;
};

} // namespace fwCom

#endif // __FWCOM_HASSLOTS_HPP__
