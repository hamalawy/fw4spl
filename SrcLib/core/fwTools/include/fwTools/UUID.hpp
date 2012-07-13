/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWTOOLS_UUID_HPP_
#define _FWTOOLS_UUID_HPP_

#include <string>
#include <map>

#include <fwCore/base.hpp>
#include <fwCore/mt/types.hpp>

#include "fwTools/Object.hpp"
#include "fwTools/config.hpp"

namespace fwTools
{
/**
 * @brief   Management of UUID on objects.
 * @class   UUID.
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 *
 */
class FWTOOLS_CLASS_API UUID
{
public:

    fwCoreClassDefinitionsWithFactoryMacro((UUID), (( )), new UUID );

    typedef std::string UUIDType;

    /**
     * @brief   Return true iff the given uuid is used
     * @note This method is thread-safe.
     */
    FWTOOLS_API static bool exist( const UUIDType & uuid);

    /**
     * @brief   Return an uuid to the given object : if no one previously set then generate a new one
     * @note This method is thread-safe.
     */
    FWTOOLS_API static const UUIDType& get(::fwTools::Object::sptr object);

    /**
     * @brief   Return a smart ptr on the object related to a given UUID : return null shared if not supervised
     * @note This method thread-safe.
     */
    FWTOOLS_API static ::fwTools::Object::sptr get( const UUIDType & uuid );

    /**
     * @brief   Return a new extended UUID;
     * @note This method is thread-safe.
     */
    FWTOOLS_API static UUIDType generateUUID();

    /**
     * @brief Attempt to set an UUID. If uuid already exists, do nothing.
     * @return true if the uuid is set, else false.
     * @note This method is thread-safe.
     */
    FWTOOLS_API static bool set(::fwTools::Object::sptr object,  const UUID::UUIDType & uuid);

protected :

    /// Store association ::boost::weak_ptr <--> uuid as a string
    typedef std::map< UUIDType, ::fwTools::Object::wptr > UUIDContainer;

    FWTOOLS_API static UUIDContainer s_uuidMap;

    /**
     * @brief   Default constructor : does nothing.
     */
    FWTOOLS_API UUID();

    /**
     * @brief   Destructor : does nothing.
     */
    FWTOOLS_API virtual ~UUID();

private:

    /// Local UUID, empty by default if not generated.
    UUIDType m_uuid;

    /// Read/Write mutex used to thread-safe UUID get/set/exist methods.
    static ::fwCore::mt::ReadWriteMutex s_rwMutex;

    /// Mutex used by generateUUID().
    static ::fwCore::mt::Mutex s_mutex;

};

}

#endif /* _FWTOOLS_UUID_HPP_ */
