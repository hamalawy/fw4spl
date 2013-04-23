/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwTools/BufferObject.hpp>

#include "fwData/ObjectLock.hpp"

namespace fwData
{

//-----------------------------------------------------------------------------

ObjectLock::~ObjectLock()
{
    m_locks.clear();
    m_objects.clear();
}


//-----------------------------------------------------------------------------

ObjectLock::ObjectLock(const ObjectLock & objectLock)
{
    m_objects = objectLock.m_objects;
    m_locks = objectLock.m_locks;
}

//-----------------------------------------------------------------------------

ObjectLock & ObjectLock::operator=(const ObjectLock & objectLock)
{
    m_locks.clear();
    m_objects.clear();

    m_objects = objectLock.m_objects;
    m_locks = objectLock.m_locks;

    return *this;
}

//-----------------------------------------------------------------------------

void ObjectLock::lock( ::fwData::Array::sptr array, LocksType & locks )
{
    if ( array )
    {
        locks.push_back( array->getBufferObject()->lock() ) ;
        m_objects.push_back(array);
    }
}

//-----------------------------------------------------------------------------

void ObjectLock::lock( ::fwData::Image::sptr image, LocksType & locks )
{
    if ( image )
    {
        this->lock( image->getDataArray(), locks ) ;
    }
}

//-----------------------------------------------------------------------------

void ObjectLock::lock( ::fwData::Mesh::sptr mesh, LocksType & locks )
{
    if ( mesh )
    {
        this->lock( mesh->getPointsArray(), locks ) ;
        this->lock( mesh->getCellDataArray(), locks ) ;
        this->lock( mesh->getCellDataOffsetsArray(), locks ) ;
        this->lock( mesh->getCellTypesArray(), locks ) ;

        this->lock( mesh->getCellColorsArray(), locks ) ;
        this->lock( mesh->getPointColorsArray(), locks ) ;
        this->lock( mesh->getCellNormalsArray(), locks ) ;
        this->lock( mesh->getPointNormalsArray(), locks ) ;
    }
}

//-----------------------------------------------------------------------------

void ObjectLock::lock( ::fwData::Reconstruction::sptr rec, LocksType & locks )
{
    if ( rec )
    {
        this->lock( rec->getImage(), locks ) ;
        this->lock( rec->getMesh(), locks ) ;
    }
}

//-----------------------------------------------------------------------------

ObjectLock::ObjectLock( ::fwData::Object::sptr obj )
{
    ::fwData::Image::sptr image = ::fwData::Image::dynamicCast( obj );
    ::fwData::Mesh::sptr mesh = ::fwData::Mesh::dynamicCast( obj );
    ::fwData::Array::sptr array = ::fwData::Array::dynamicCast( obj );

    if( image )
    {
        this->lock( image, m_locks ) ;
    }
    else if ( mesh )
    {
        this->lock( mesh, m_locks ) ;
    }
    else if ( array )
    {
        this->lock( array, m_locks ) ;
    }
}

//-----------------------------------------------------------------------------

} // fwData
