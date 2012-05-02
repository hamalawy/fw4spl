/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwTools/Exception.hpp"
#include "fwTools/BufferObject.hpp"

namespace fwTools
{



BufferObject::BufferObject():
    m_buffer(0),
    m_size(0),
    m_count(new long(0)),
    m_bufferManager(::fwTools::IBufferManager::getCurrent()),
    m_allocPolicy(::fwTools::BufferNoAllocPolicy::New())
{
    m_bufferManager->registerBuffer(&m_buffer, m_count);
}

//------------------------------------------------------------------------------

BufferObject::~BufferObject()
{
    OSLM_ASSERT("There is still " << m_count << " locks on this BufferObject (" << this << ")", m_count > 0);
    m_bufferManager->unregisterBuffer(&m_buffer);
    delete m_count;
}

//------------------------------------------------------------------------------

void BufferObject::allocate(SizeType size, ::fwTools::BufferAllocationPolicy::sptr policy)
{
    m_allocPolicy = policy;
    m_size = size;
    if(m_bufferManager->allocateBuffer(&m_buffer, size, policy))
    {
        policy->allocate(m_buffer, size);
    }
}

//------------------------------------------------------------------------------

void BufferObject::reallocate(SizeType size)
{
    m_size = size;
    if(m_bufferManager->reallocateBuffer(&m_buffer, size))
    {
        m_allocPolicy->reallocate(m_buffer, size);
    }
}

//------------------------------------------------------------------------------

void BufferObject::destroy()
{
    if(m_bufferManager->destroyBuffer(&m_buffer))
    {
        m_allocPolicy->destroy(m_buffer);
    }
    m_allocPolicy = ::fwTools::BufferNoAllocPolicy::New();
    m_size = 0;
}

//------------------------------------------------------------------------------

void BufferObject::setBuffer(void *buffer, SizeType size, ::fwTools::BufferAllocationPolicy::sptr policy)
{
    m_allocPolicy = policy;
    m_size   = size;
    m_buffer = buffer;

    m_bufferManager->setBuffer(&m_buffer, size, policy);
}

//------------------------------------------------------------------------------

BufferObject::Lock BufferObject::lock()
{
    return BufferObject::Lock(this->getSptr());
}

//------------------------------------------------------------------------------

BufferObject::ConstLock BufferObject::lock() const
{
    return BufferObject::ConstLock(this->getConstSptr());
}

//------------------------------------------------------------------------------

void BufferObject::swap( BufferObject::sptr _source )
{
    std::swap(m_buffer, _source->m_buffer);
    std::swap(m_size, _source->m_size);
    m_bufferManager.swap(_source->m_bufferManager);
    m_allocPolicy.swap(_source->m_allocPolicy);
}

//------------------------------------------------------------------------------

} //namespace fwTools


