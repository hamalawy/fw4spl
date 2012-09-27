/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwMetaData/Blob.hpp"

fwCampImplementMacro((fwMetaData)(Blob))
{
    builder
        .tag("object_version", "1")
        .tag("lib_name", "fwMetaData")
        .base< ::fwMetaData::Base >();
}

namespace fwMetaData
{

Blob::Blob(::fwTools::BufferObject::sptr buffer)
             : m_bufferObject(buffer)
{}

Blob::sptr Blob::New(::fwTools::BufferObject::sptr buffer)
{
    Blob::sptr blob(new Blob(buffer));

    return blob;
}

Blob::sptr Blob::New(void* buffer, unsigned int size)
{
    ::fwTools::BufferObject::sptr bufferObject = ::fwTools::BufferObject::New();
    bufferObject->setBuffer(buffer, size);
    Blob::sptr blob(new Blob(bufferObject));
    return blob;
}

unsigned int Blob::getSize() const
{
    return m_bufferObject->getSize();
}

char* Blob::getBuffer()const
{
    const ::fwTools::BufferObject::Lock& lock = m_bufferObject->lock();
    return static_cast<char *>( lock.getBuffer());
}

::fwTools::BufferObject::sptr Blob::getBufferObject()
{
    return m_bufferObject;
}



Base::sptr Blob::clone()
{
    return this->getSptr();
}

}
