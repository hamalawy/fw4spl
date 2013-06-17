/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef FWMEMORY_NEVERDUMP_HPP_
#define FWMEMORY_NEVERDUMP_HPP_

#include <boost/shared_ptr.hpp>

#include <fwCore/base.hpp>

#include "fwMemory/BufferInfo.hpp"
#include "fwMemory/IPolicy.hpp"
#include "fwMemory/policy/factory/new.hpp"
#include "fwMemory/config.hpp"

namespace fwMemory
{

namespace policy
{

/**
 * @brief Never dump policy
 *
 * This policy will never take the initiative to free memory. This is the policy
 * used when no automatic memory management is wanted. Memory will be dumped on
 * demand.
 */
class FWMEMORY_CLASS_API NeverDump : public fwMemory::IPolicy
{
public :

    fwCoreClassDefinitionsWithFactoryMacro((NeverDump)(fwMemory::IPolicy),
                                           (()),
                                           ::fwMemory::policy::factory::New< NeverDump >) ;

    FWMEMORY_API virtual void allocationRequest( BufferInfo &info,
            ::fwMemory::BufferManager::ConstBufferPtrType buffer, BufferInfo::SizeType size ) ;

    FWMEMORY_API virtual void setRequest( BufferInfo &info,
            ::fwMemory::BufferManager::ConstBufferPtrType buffer, BufferInfo::SizeType size ) ;

    FWMEMORY_API virtual void reallocateRequest( BufferInfo &info,
            ::fwMemory::BufferManager::ConstBufferPtrType buffer, BufferInfo::SizeType newSize ) ;

    FWMEMORY_API virtual void destroyRequest( BufferInfo &info, ::fwMemory::BufferManager::ConstBufferPtrType buffer );

    FWMEMORY_API virtual void lockRequest( BufferInfo &info, ::fwMemory::BufferManager::ConstBufferPtrType buffer );
    FWMEMORY_API virtual void unlockRequest( BufferInfo &info, ::fwMemory::BufferManager::ConstBufferPtrType buffer ) ;

    FWMEMORY_API virtual void dumpSuccess( BufferInfo &info, ::fwMemory::BufferManager::ConstBufferPtrType buffer );
    FWMEMORY_API virtual void restoreSuccess( BufferInfo &info, ::fwMemory::BufferManager::ConstBufferPtrType buffer );

    FWMEMORY_API virtual void refresh();

    bool setParam(const std::string &name, const std::string &value)
    {
        FwCoreNotUsedMacro(name);
        FwCoreNotUsedMacro(value);
        return false;
    }

    FWMEMORY_API virtual std::string getParam(const std::string &name, bool *ok = NULL ) const;

    const fwMemory::IPolicy::ParamNamesType &getParamNames() const
    {
        static fwMemory::IPolicy::ParamNamesType names;
        return names;
    }
};


} // namespace policy

} // namespace fwMemory

#endif // FWMEMORY_NEVERDUMP_HPP_

