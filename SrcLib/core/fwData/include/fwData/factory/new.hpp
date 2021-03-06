/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_FACTORY_NEW_HPP__
#define __FWDATA_FACTORY_NEW_HPP__

#include <string>

#include <boost/make_shared.hpp>

#include <fwTools/macros.hpp>
#include <fwTools/DynamicAttributes.hxx>

#include "fwData/config.hpp"
#include "fwData/registry/detail.hpp"

namespace fwData
{

class Object;

namespace factory
{


template<class CLASSNAME > SPTR( CLASSNAME )  New();

/**
 * @brief Key class used to restrict access to Object construction.
 * See http://www.drdobbs.com/184402053
 */
class Key
{
    template<typename CLASSNAME>
    friend SPTR( CLASSNAME ) fwData::factory::New();

    Key(){};
};


FWDATA_API SPTR( ::fwData::Object ) New( const ::fwData::registry::KeyType & classname );


template<class CLASSNAME > SPTR( CLASSNAME )  New()
{
    SPTR(CLASSNAME) obj = ::boost::make_shared< CLASSNAME >( Key() );

    ::fwTools::DynamicAttributesBase *dynAttr = obj.get();
    dynAttr->__FWTOOLS_ATTRIBUTES_REGISTER_FUNC_NAME();

#ifdef COM_LOG
    obj->setID(obj->getID());
#endif

    return obj;
}

} // namespace factory

} // namespace fwData

#endif /* __FWDATA_FACTORY_NEW_HPP__ */


