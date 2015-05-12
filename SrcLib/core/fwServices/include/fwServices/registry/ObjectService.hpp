/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSERVICES_REGISTRY_OBJECTSERVICE_HPP__
#define __FWSERVICES_REGISTRY_OBJECTSERVICE_HPP__

#include <map>
#include <vector>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/multiset_of.hpp>

#include <fwCore/mt/types.hpp>
#include <fwCore/base.hpp>
#include <fwCore/LogicStamp.hpp>

#include <fwTools/Failed.hpp>
#include <fwTools/Object.hpp>

#include "fwServices/config.hpp"
#include "fwServices/IService.hpp"

namespace fwServices
{
namespace registry
{

/**
 * @class ObjectService
 *
 * @brief maintain the relation between objects and services
 *
 * The API of ObjectService should not be directly invoked,
 * fwServices/op methods (i.e. add, get,...) should be used instead
 *
 *
 */
class FWSERVICES_CLASS_API ObjectService : public ::fwCore::BaseObject
{
public:

    fwCoreNonInstanciableClassDefinitionsMacro( (ObjectService)(::fwCore::BaseObject) );

    /**
     * @brief Service container
     * keeps relation between objects identifiers and attached services
     */
    typedef ::boost::bimaps::bimap<
            ::boost::bimaps::multiset_of< ::fwCore::LogicStamp::LogicStampType >,
            ::boost::bimaps::unordered_set_of< ::fwServices::IService::sptr >
            > ServiceContainerType;


    typedef std::vector< SPTR( ::fwData::Object ) >       ObjectVectorType;
    typedef std::vector< SPTR( ::fwServices::IService ) > ServiceVectorType;

    /// Return some informations contain in the registry
    FWSERVICES_API std::string getRegistryInformation() const;

    /**
     * @name Management of registrations
     */

    //@{

    /**
     * @brief Register the service (service) for the object (obj)
     * It also updates IService::m_associatedObject of service to point to obj
     * removal at obj destruction.
     */
    FWSERVICES_API void registerService( ::fwData::Object::sptr obj, ::fwServices::IService::sptr service );

    //@}

    /**
     * @name Management of unregistrations
     */

    //@{

    /**
     * @brief Remove the service (service) from the m_container
     * It invokes service stop method first as well as related observations removal.
     */
    FWSERVICES_API void unregisterService(  ::fwServices::IService::sptr service );

    /**
     * @brief container manipulator (Helper)
     * remove service from ObjectService containers, do not perform any "semantic action" like
     * stop service, remove comm channel
     * @note (internal use) use with care
     */
    void  removeFromContainer( ::fwServices::IService::sptr service );

    //@}

    /**
     * @name Some useful getters
     */

    //@{

    /**
     * @brief Return a container with all services of type SERVICE registered in m_container
     * @note Services may be associated to different object
     */
    template<class SERVICE>
    std::vector< SPTR(SERVICE) > getServices() const;

    /**
     * @brief Return a container with all services of type SERVICE associated to obj in m_container
     */
    template<class SERVICE>
    std::vector< SPTR(SERVICE) > getServices(::fwData::Object::sptr obj) const;

    /**
     * @brief Return registered services matching serviceType
     * @note Should be optimized
     * @note Invoke getServices( ::fwData::Object::sptr , const std::string & ) for each registered object
     *
     */
    FWSERVICES_API ServiceVectorType getServices( const std::string & serviceType ) const;

    /**
     * @brief Return a container of services of type serviceType which are attached to obj
     *
     */
    FWSERVICES_API ServiceVectorType getServices( ::fwData::Object::sptr obj, const std::string & serviceType ) const;

    /**
     * @brief return a vector containing all services associated with the object obj
     *
     */
    FWSERVICES_API ServiceVectorType getServices( ::fwData::Object::sptr obj ) const;

    /**
     * @brief Return a container with all objects associated with a service of type SERVICE in m_container
     */
    template<class SERVICE>
    ObjectVectorType getObjects() const;


    /**
     * @brief Return a container with all objects registered in m_container
     */
    FWSERVICES_API ObjectVectorType getObjects() const;

    //@}

    /**
     * @name Some useful getters
     */

    //@{
    /**
     * @brief return true is obj has at least one service of type srvType
     *
     */
    FWSERVICES_API bool has( ::fwData::Object::sptr obj, const std::string &srvType) const;

    /**
     * @brief return true if key is still present in OSR
     */
    FWSERVICES_API bool hasKey( ::fwCore::LogicStamp::csptr key ) const;

    //@}


    /**
     * @name Misc.
     */

    //@{

    /**
     * @brief Move service (service) to object objDst in the m_container
     *
     */
    FWSERVICES_API void swapService( ::fwData::Object::sptr objDst, ::fwServices::IService::sptr service );

    //@}

protected:

    /**
     * @brief Object to service associations container
     * @note An object can be registered without services
     * @warning Do not use smart pointers for ::fwData::Object, otherwise they will never destroy
     */
    ServiceContainerType m_container;

    mutable ::fwCore::mt::ReadWriteMutex m_containerMutex;

private:

    /**
     * @brief Register the service (service) for the object (obj)
     * It also updates IService::m_associatedObject of service to point to obj
     * removal at obj destruction.
     * @warning not thread-safe
     */
    void internalRegisterService( ::fwData::Object::sptr obj, ::fwServices::IService::sptr service );

};

} // namespace registry



/**
 * @brief OSR wraps main Object-Service registry access
 */
namespace OSR
{

/**
 * @brief returns the Object-Service registry main instance
 */
FWSERVICES_API ::fwServices::registry::ObjectService::sptr get();

/**
 * @brief Wraps ObjectService::getObjects
 */
FWSERVICES_API ::fwServices::registry::ObjectService::ObjectVectorType getObjects();

/**
 * @brief Wraps ObjectService::getRegistryInformation
 */
FWSERVICES_API std::string getRegistryInformation();

/**
 * @brief Wraps ObjectService::getServices
 */
template<class SERVICE>
std::vector< SPTR(SERVICE) > getServices();

/**
 * @brief Wraps ObjectService::getServices
 */
template<class SERVICE>
std::vector< SPTR(SERVICE) > getServices(::fwData::Object::sptr obj);

/**
 * @brief Wraps ObjectService::getServices
 */
FWSERVICES_API ::fwServices::registry::ObjectService::ServiceVectorType getServices( const std::string & serviceType );

/**
 * @brief Wraps ObjectService::getServices
 */
FWSERVICES_API ::fwServices::registry::ObjectService::ServiceVectorType getServices( ::fwData::Object::sptr obj,
                                                                                     const std::string & serviceType );

/**
 * @brief Wraps ObjectService::getServices
 */
FWSERVICES_API ::fwServices::registry::ObjectService::ServiceVectorType getServices( ::fwData::Object::sptr obj );

/**
 * @brief Wraps ObjectService::has
 */
FWSERVICES_API bool has( ::fwData::Object::sptr obj, const std::string &srvType);

/**
 * @brief Wraps ObjectService::registerService
 */
FWSERVICES_API void registerService( ::fwData::Object::sptr obj, ::fwServices::IService::sptr service );

/**
 * @brief Wraps ObjectService::swapService
 */
FWSERVICES_API void swapService( ::fwData::Object::sptr objDst, ::fwServices::IService::sptr service );

/**
 * @brief Wraps ObjectService::unregisterService
 */
FWSERVICES_API void unregisterService(  ::fwServices::IService::sptr service );

} // namespace OSR

} // namespace fwServices

#include "fwServices/registry/ObjectService.hxx"

#endif // __FWSERVICES_REGISTRY_OBJECTSERVICE_HPP__


