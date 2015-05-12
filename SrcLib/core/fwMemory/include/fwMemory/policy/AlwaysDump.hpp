/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWMEMORY_POLICY_ALWAYSDUMP_HPP__
#define __FWMEMORY_POLICY_ALWAYSDUMP_HPP__

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
 * @brief Always dump policy
 *
 * This policy will dump buffers as often as possible. As soon as a buffer is
 * released (ie the lock count become zero), it will be dumped.
 */
class FWMEMORY_CLASS_API AlwaysDump : public fwMemory::IPolicy
{
public:
    fwCoreClassDefinitionsWithFactoryMacro((AlwaysDump)(fwMemory::IPolicy),
                                           (()),
                                           ::fwMemory::policy::factory::New< AlwaysDump >);

    FWMEMORY_API virtual void allocationRequest( BufferInfo &info,
                                                 ::fwMemory::BufferManager::ConstBufferPtrType buffer,
                                                 BufferInfo::SizeType size );

    FWMEMORY_API virtual void setRequest( BufferInfo &info,
                                          ::fwMemory::BufferManager::ConstBufferPtrType buffer,
                                          BufferInfo::SizeType size );

    FWMEMORY_API virtual void reallocateRequest( BufferInfo &info,
                                                 ::fwMemory::BufferManager::ConstBufferPtrType buffer,
                                                 BufferInfo::SizeType newSize );

    FWMEMORY_API virtual void destroyRequest( BufferInfo &info, ::fwMemory::BufferManager::ConstBufferPtrType buffer );

    FWMEMORY_API virtual void lockRequest( BufferInfo &info, ::fwMemory::BufferManager::ConstBufferPtrType buffer );
    FWMEMORY_API virtual void unlockRequest( BufferInfo &info, ::fwMemory::BufferManager::ConstBufferPtrType buffer );

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

    const ::fwMemory::IPolicy::ParamNamesType &getParamNames() const
    {
        static ::fwMemory::IPolicy::ParamNamesType names;
        return names;
    }

protected:

    FWMEMORY_API size_t dump();
    FWMEMORY_API void apply();
};


} // namespace policy

} // namespace fwMemory

#endif // __FWMEMORY_POLICY_ALWAYSDUMP_HPP__

