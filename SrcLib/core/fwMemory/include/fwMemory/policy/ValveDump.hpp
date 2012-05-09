/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2011.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef FWMEMORY_DUMP_HPP_
#define FWMEMORY_DUMP_HPP_

#include <boost/shared_ptr.hpp>

#include <fwCore/base.hpp>

#include "fwMemory/BufferInfo.hpp"
#include "fwMemory/BufferManager.hpp"
#include "fwMemory/IPolicy.hpp"
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
public :

    typedef SPTR(ValveDump) sptr;

    FWMEMORY_API static sptr New();

    FWMEMORY_API ValveDump();

    FWMEMORY_API virtual void allocationRequest( BufferInfo &info, void **buffer, BufferInfo::SizeType size ) ;
    FWMEMORY_API virtual void setRequest( BufferInfo &info, void **buffer, BufferInfo::SizeType size ) ;
    FWMEMORY_API virtual void reallocateRequest( BufferInfo &info, void **buffer, BufferInfo::SizeType newSize ) ;
    FWMEMORY_API virtual void destroyRequest( BufferInfo &info, void **buffer ) ;
    FWMEMORY_API virtual void lockRequest( BufferInfo &info, void **buffer ) ;
    FWMEMORY_API virtual void unlockRequest( BufferInfo &info, void **buffer ) ;

    FWMEMORY_API virtual void dumpSuccess( BufferInfo &info, void **buffer );
    FWMEMORY_API virtual void restoreSuccess( BufferInfo &info, void **buffer );

    FWMEMORY_API void setManager(::fwTools::IBufferManager::sptr manager);

    FWMEMORY_API virtual void refresh();

    FWMEMORY_API bool setParam(const std::string &name, const std::string &value);
    FWMEMORY_API fwMemory::IPolicy::ParamNamesType getParamNames() const;

protected :

    FWMEMORY_API bool needDump(size_t supplement);

    FWMEMORY_API size_t dump(size_t nbOfBytes);

    FWMEMORY_API void apply(size_t supplement = 0);

    size_t m_minFreeMem;
    size_t m_hysteresisOffset;

    ::fwMemory::BufferManager::wptr m_manager;


};


} // namespace policy

} // namespace fwMemory

#endif // FWMEMORY_DUMP_HPP_

