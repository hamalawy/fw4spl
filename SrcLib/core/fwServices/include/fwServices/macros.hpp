/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWSERVICES_MACRO_HPP_
#define _FWSERVICES_MACRO_HPP_

#include <fwCore/Demangler.hpp>

#include <fwTools/ClassFactoryRegistry.hpp>
#include <fwTools/ClassRegistrar.hpp>

#include <fwServices/IService.hpp>

namespace fwServices
{

/**
 * @name Macros for declaring service to object association
 */

//@{

/**
 * @brief Service object type association including all string-based registration
 * Associations concern Service-ObjectType are ObjectType-Service. Keys are typeid.
 * @author      IRCAD (Research and Development Team).
 */
#define REGISTER_SERVICE( ServiceType , Service1, Object1 )\
REGISTER_BINDING_ID_CPY( ::fwServices::IService , Service1 , ::fwServices::ObjectServiceKeyType , \
                                                 ::fwServices::ObjectServiceKeyType( ::fwCore::getClassname<ServiceType>(),  ::fwCore::getClassname<Object1>() ) , BOOST_PP_CAT( __LINE__ , A )) ; \
REGISTER_BINDING_ID_CPY( ::fwServices::IService , Service1 , std::string , ::fwCore::TypeDemangler<Service1>().getClassname() , BOOST_PP_CAT( __LINE__ , B ) ) ;

//@}

}

#endif /*MACRO_HPP_*/
