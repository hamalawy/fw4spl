/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWCOMED_HELPER_ARRAY_HPP__
#define __FWCOMED_HELPER_ARRAY_HPP__

#include <fwTools/Type.hpp>
#include <fwMemory/BufferObject.hpp>

#include <fwData/Array.hpp>

#include "fwComEd/config.hpp"

namespace fwComEd
{

namespace helper
{

/**
 * @class   Array
 * @brief   Helper to manage array buffer. Lock the buffer before to modify it.
 *
 * @date    2012
 */
class FWCOMED_CLASS_API Array
{

public:
    fwCoreClassDefinitionsWithFactoryMacro((Array), (((::fwData::Array::sptr))), new Array );


    FWCOMED_API Array( ::fwData::Array::sptr array );

    FWCOMED_API virtual ~Array();

    /**
     * @brief Setter for one item components of the array
     *
     * @param id Item id
     * @param value Valid buffer of elements of type \<m_type\> with a length equal to \<m_nbOfComponents\> to be copied to array 'id'
     */
    FWCOMED_API virtual void setItem(const ::fwData::Array::IndexType &id, const void *value);

    /**
     * @brief Setter for one item component of the array
     *
     * @param id Item id
     * @param component Component id to write data in.
     * @param value Valid buffer of elements of type \<m_type\> with a length equal to \<m_nbOfComponents\> to be copied to array 'id', component n° 'component'
     *
     */
    FWCOMED_API virtual void setItem(const ::fwData::Array::IndexType &id, const size_t component, const void *value);

    ///
    /**
     * @brief Getter for a buffer item. pointer to the requested item in the buffer
     *
     * @param id Item id
     * @param component Component id
     *
     * @return Pointer to the requested item in the buffer
     */
    FWCOMED_API virtual void* getItem(const ::fwData::Array::IndexType &id, const size_t component = 0);

    /**
     * @brief Typed version of getItem
     *
     * @tparam T Type in which the pointer will be returned
     * @param id Item id
     * @param component Component id
     *
     * @return Array buffer pointer casted to T
     */
    template< typename T > T* getItem(const ::fwData::Array::IndexType &id, const size_t component = 0);

    /**
     * @brief Copies the data into the buffer pointed by <value>
     *
     * @param id Item id
     * @param[out] value Buffer to write into
     */
    FWCOMED_API virtual void getItem(const ::fwData::Array::IndexType &id, void *value) const;
    FWCOMED_API virtual void getItem(const ::fwData::Array::IndexType &id, const size_t component, void *value) const;

    /**
     * @brief Getter for the array buffer
     *
     * @return Array's buffer, if exists, else NULL
     * @{
     */
    FWCOMED_API virtual void *getBuffer();
    FWCOMED_API virtual const void *getBuffer() const;
    ///@}

    /**
     * @brief Setter for the array buffer.
     *
     * An existing buffer will be released if the array own it.
     *
     * @param buf            Buffer to set as Array's buffer
     * @param takeOwnership  if true, the Array will manage allocation and destroy the buffer when needed.
     * @param type           Type of the array view
     * @param size           Size of the array view
     * @param nbOfComponents Number of components of the array view, Min value : 1
     */
    FWCOMED_API void setBuffer( void *buf, bool takeOwnership,
                                const ::fwTools::Type &type,
                                const ::fwData::Array::SizeType &size,
                                size_t nbOfComponents );

    /// Returns the begining/end of the buffer interpreted as a char buffer
    FWCOMED_API virtual char* begin();
    FWCOMED_API virtual char* end();
    FWCOMED_API virtual const char* begin() const;
    FWCOMED_API virtual const char* end() const;


    /// Returns the begining/end of the buffer, casted to T
    template< typename T > T* begin();
    template< typename T > T* end();

    /**
     * @brief Get a pointer to the value described by given parameters
     *
     * @param id Item id
     * @param component Item component id
     * @param sizeOfType size of a component
     *
     * @return buffer item pointer
     * @{
     */
    FWCOMED_API char *getBufferPtr( const ::fwData::Array::IndexType &id, size_t component, size_t sizeOfType );
    FWCOMED_API const char *getBufferPtr( const ::fwData::Array::IndexType &id, size_t component,
                                          size_t sizeOfType ) const;
    ///@}

    /// Returns a copy of current lock on array
    FWCOMED_API ::fwMemory::BufferObject::Lock getLock() const;

protected:

    /**
     * @brief Protected setter for the array buffer.
     * An existing buffer will be released if the array own it.
     *
     * @param buf Buffer to set as Array's buffer
     * @param takeOwnership if true, the Array will manage allocation and destroy the buffer when needed.
     */
    FWCOMED_API virtual void setBuffer(void *buf, bool takeOwnership = false);


    ::fwData::Array::sptr m_array;
    ::fwMemory::BufferObject::Lock m_lock;

};

template< typename T >
T* Array::begin()
{
    return static_cast<T*>(this->getBuffer());
}

template< typename T >
T* Array::end()
{
    return reinterpret_cast<T*> (static_cast<char*>(this->getBuffer()) + m_array->getSizeInBytes());
}

template< typename T >
T* Array::getItem(const  ::fwData::Array::IndexType &id, const size_t component)
{
    return static_cast<T*> (this->getItem(id, component));
}


} // namespace helper

} // namespace fwData

#endif // __FWCOMED_HELPER_ARRAY_HPP__
