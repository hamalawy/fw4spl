/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwTools/ClassRegistrar.hpp>


#include "fwData/Composite.hpp"


REGISTER_BINDING_BYCLASSNAME( ::fwTools::Object, ::fwData::Composite,  ::fwData::Composite);

namespace fwData
{


Composite::Composite()
{
    SLM_TRACE_FUNC();
}


Composite::~Composite()
{
    SLM_TRACE_FUNC();
}


Composite &Composite::getRefMap()
{
    return *this;
}


Composite const &Composite::getRefMap() const
{
    return *this;
}

//------------------------------------------------------------------------------

void Composite::shallowCopy( Composite::csptr _source )
{
    ::fwTools::Object::shallowCopyOfChildren( _source );
    this->clear();

     for(    Composite::Container::const_iterator iter = _source->begin();
             iter != _source->end();
             ++iter )
     {
         (*this)[ iter->first ] = iter->second;
     }
}

//------------------------------------------------------------------------------

void Composite::deepCopy( Composite::csptr _source )
{
    ::fwTools::Object::deepCopyOfChildren( _source );

    this->clear();

    for(    Composite::Container::const_iterator iter = _source->begin();
            iter != _source->end();
            ++iter )
    {
        ::fwTools::Object::sptr newObj = ::fwTools::Factory::buildData( iter->second->getClassname() );
        newObj->deepCopy( iter->second );
        (*this)[ iter->first ] = ::fwData::Object::dynamicCast( newObj );
    }
}

//------------------------------------------------------------------------------

}
