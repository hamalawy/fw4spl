/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2011-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWCORE_UTIL_FACTORY_HPP__
#define __FWCORE_UTIL_FACTORY_HPP__

#include <map>
#include <string>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
#include <boost/functional/value_factory.hpp>
#include <boost/shared_ptr.hpp>

#include "fwCore/mt/types.hpp"

namespace fwCore
{
namespace util
{

/**
 * @brief FactoryRegistryBase is a class used to store factories
 * @note This class is thread safe.
 */
template < typename FACTORY_SIGNATURE, typename KEY_TYPE = std::string,
         typename FACTORY_HOLDER = ::boost::function< FACTORY_SIGNATURE > >
class FactoryRegistryBase
{
public:
    typedef FACTORY_SIGNATURE FactorySignatureType;
    typedef KEY_TYPE KeyType;

    typedef FACTORY_HOLDER FactoryType;
    typedef std::map< KeyType, FactoryType > RegistryType;
    typedef std::vector<KeyType> KeyVectorType;

    /**
     * @brief Add a factory to the registry.
     */
    void addFactory(const KeyType& name, FactoryType factory)
    {
        // get exclusive access
        ::fwCore::mt::WriteLock lock(m_mutex);
        m_registry[name] = factory;
    }

    /**
     * @brief returns the factory associated with the key.
     */
    virtual FactoryType getFactory(const KeyType& key) const
    {
        // get shared access
        ::fwCore::mt::ReadLock lock(m_mutex);
        typename RegistryType::const_iterator iter = m_registry.find(key);
        FactoryType factory;
        if(iter != m_registry.end())
        {
            factory = iter->second;
        }
        return factory;
    }

    /**
     * @brief returns the registered factory keys.
     */
    virtual KeyVectorType getFactoryKeys() const
    {
        ::fwCore::mt::ReadLock lock(m_mutex);
        KeyVectorType vectKeys;
        std::transform( m_registry.begin(), m_registry.end(),
                std::back_inserter(vectKeys),
                ::boost::bind(& RegistryType::value_type::first,_1) );
        return vectKeys;
    }

protected:

    RegistryType m_registry;
    mutable ::fwCore::mt::ReadWriteMutex m_mutex;
};

/**
 * @brief FactoryRegistry is a class used to store factories and create instance object with these factories.
 */
template <typename F, typename KEY_TYPE = std::string, typename FACTORY_HOLDER = ::boost::function< F > >
class FactoryRegistry;

template< typename RETURN_TYPE, typename KEY_TYPE, typename FACTORY_HOLDER >
class FactoryRegistry< RETURN_TYPE (), KEY_TYPE, FACTORY_HOLDER > :
        public FactoryRegistryBase < RETURN_TYPE (), KEY_TYPE >
{
    typedef RETURN_TYPE (FactorySignatureType)();
    typedef FACTORY_HOLDER FactoryType;
    typedef RETURN_TYPE ReturnType;
    typedef KEY_TYPE KeyType;

public:

    /**
     * @brief Instantiates an object with the factory associated with the specified key.
     * @return Created instance.
     */
    ReturnType create(const KeyType& key) const
    {
        FactoryType factory = this->getFactory(key);

        if(!factory)
        {
            ReturnType obj;
            return obj;
        }
        return factory();
    }
};

template< typename RETURN_TYPE, typename ARG1_TYPE, typename KEY_TYPE, typename FACTORY_HOLDER >
class FactoryRegistry< RETURN_TYPE (ARG1_TYPE), KEY_TYPE, FACTORY_HOLDER > :
            public FactoryRegistryBase < RETURN_TYPE (ARG1_TYPE), KEY_TYPE >
{
    typedef RETURN_TYPE (FactorySignatureType)(ARG1_TYPE);
    typedef FACTORY_HOLDER FactoryType;
    typedef RETURN_TYPE ReturnType;
    typedef ARG1_TYPE Arg1Type;
    typedef KEY_TYPE KeyType;

public:

    /**
     * @brief Instantiates an object with the factory associated with the specified key, passing arg1 to the factory.
     * @return Created instance.
     */
    ReturnType create(const KeyType& key, Arg1Type &arg1) const
    {
        FactoryType factory = this->getFactory(key);
        ReturnType obj;
        if(factory)
        {
            obj = factory(arg1);
        }
        return obj;
    }
};

} //namespace util
} //namespace fwCore

#endif /* __FWCORE_UTIL_FACTORY_HPP__ */
