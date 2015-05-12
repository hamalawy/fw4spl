/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWMEMORY_POLICY_VALVEDUMP_HPP__
#define __FWMEMORY_POLICY_VALVEDUMP_HPP__

#include <boost/shared_ptr.hpp>

#include <fwCore/base.hpp>

#include "fwMemory/BufferInfo.hpp"
#include "fwMemory/BufferManager.hpp"
#include "fwMemory/IPolicy.hpp"
#include "fwMemory/policy/factory/new.hpp"
#include "fwMemory/config.hpp"

namespace fwMemory
{

namespace policy
{

/**
 * @brief Valve dump policy
 *
 * This policy is monitoring system memory usage and will trigger memory dump
 * when the amount of system free memory is lower than the minFreeMem parameter.
 *
 * An hysteresis parameter exists to free more memory when the process is
 * triggered. If the free system memory amount is lower than the minFreeMem,
 * this policy will try to reach minFreeMem + hysteresisOffset bytes of free
 * memory.
 */
class FWMEMORY_CLASS_API ValveDump : public fwMemory::IPolicy
{
public:
    fwCoreClassDefinitionsWithFactoryMacro((ValveDump)(fwMemory::IPolicy),
                                           (()),
                                           ::fwMemory::policy::factory::New< ValveDump >);

    FWMEMORY_API ValveDump();

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

    FWMEMORY_API bool setParam(const std::string &name, const std::string &value);
    FWMEMORY_API virtual std::string getParam(const std::string &name, bool *ok = NULL ) const;
    FWMEMORY_API const ::fwMemory::IPolicy::ParamNamesType &getParamNames() const;

protected:

    FWMEMORY_API bool needDump(size_t supplement) const;

    FWMEMORY_API size_t dump(size_t nbOfBytes);

    FWMEMORY_API void apply(size_t supplement = 0);

    size_t m_minFreeMem;
    size_t m_hysteresisOffset;
};


} // namespace policy

} // namespace fwMemory

#endif // __FWMEMORY_POLICY_VALVEDUMP_HPP__

