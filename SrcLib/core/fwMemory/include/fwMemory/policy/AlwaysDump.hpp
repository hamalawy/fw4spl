/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2011.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef FWMEMORY_ALWAYSDUMP_HPP_
#define FWMEMORY_ALWAYSDUMP_HPP_

#include <boost/shared_ptr.hpp>

#include <fwCore/base.hpp>

#include "fwMemory/BufferInfo.hpp"
#include "fwMemory/IPolicy.hpp"
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
public :

    typedef SPTR(AlwaysDump) sptr;

    FWMEMORY_API static sptr New();

    FWMEMORY_API virtual void allocationRequest( BufferInfo &info, void **buffer, BufferInfo::SizeType size ) ;
    FWMEMORY_API virtual void setRequest( BufferInfo &info, void **buffer, BufferInfo::SizeType size ) ;
    FWMEMORY_API virtual void reallocateRequest( BufferInfo &info, void **buffer, BufferInfo::SizeType newSize ) ;
    FWMEMORY_API virtual void destroyRequest( BufferInfo &info, void **buffer ) ;
    FWMEMORY_API virtual void lockRequest( BufferInfo &info, void **buffer ) ;
    FWMEMORY_API virtual void unlockRequest( BufferInfo &info, void **buffer ) ;

    FWMEMORY_API virtual void dumpSuccess( BufferInfo &info, void **buffer );
    FWMEMORY_API virtual void restoreSuccess( BufferInfo &info, void **buffer );

    FWMEMORY_API virtual void setManager(::fwTools::IBufferManager::sptr manager);

    FWMEMORY_API virtual void refresh();

    FWMEMORY_API bool setParam(const std::string &name, const std::string &value) {return false;};
    FWMEMORY_API fwMemory::IPolicy::ParamNamesType getParamNames() const {return fwMemory::IPolicy::ParamNamesType();};

protected:

    FWMEMORY_API size_t dump();
    FWMEMORY_API void apply();

    ::fwMemory::BufferManager::wptr m_manager;
};


} // namespace policy

} // namespace fwMemory

#endif // FWMEMORY_ALWAYSDUMP_HPP_

