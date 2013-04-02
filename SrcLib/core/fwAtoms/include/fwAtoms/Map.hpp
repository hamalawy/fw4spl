/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWATOMS_BASE_MAP_HPP_
#define __FWATOMS_BASE_MAP_HPP_


#include <map>
#include <string>

#include "fwAtoms/config.hpp"
#include "fwAtoms/Base.hpp"
#include "fwAtoms/Object.hpp"
#include "fwAtoms/factory/new.hpp"

namespace fwAtoms
{

/**
 *
 * @brief Map is a container for mapping representation.
 *
 * For exemple, this metadata is used to bind :
 * \li std::map
 * \li boost::unordered_map
 * \li fwData::Graph (a part)
 * \li boost graph
 */
class FWATOMS_CLASS_API Map : public Base
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (Map)(::fwAtoms::Base), (()), ::fwAtoms::factory::New< Map >) ;

    typedef std::map<std::string, Base::sptr> MapType;

    typedef MapType::key_type KeyType;
    typedef MapType::mapped_type MappedType;
    typedef MapType::value_type ValueType;
    typedef MapType::iterator IteratorType;
    typedef MapType::const_iterator ConstIteratorType;
    typedef MapType::reverse_iterator ReverseIteratorType;
    typedef MapType::const_reverse_iterator ConstReverseIteratorType;
    typedef MapType::size_type SizeType;


    /// boost_foreach/stl compatibility
    /// @{
    typedef MapType::key_type key_type;
    typedef MapType::mapped_type mapped_type;
    typedef MapType::value_type value_type;
    typedef MapType::iterator iterator;
    typedef MapType::const_iterator const_iterator;
    typedef MapType::reverse_iterator reverse_iterator;
    typedef MapType::const_reverse_iterator const_reverse_iterator;
    typedef MapType::size_type size_type;
    /// @}

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    Map(::fwAtoms::Base::Key key)
    {}

    /**
     * @brief   Destructor
     */
    virtual ~Map()
    {}

    /**
     * @brief Insert a new value in the map.
     * @param key the key, if exist the oldest value is erased.
     * @param value the new value
     */
    std::pair<IteratorType, bool> insert(const std::string &key, Base::sptr &value)
    { return m_value.insert( ValueType(key, value) ); }

    /**
     * @brief retrieve size of map
     * @return the map size
     */
    size_t size() const {return m_value.size();}

    /**
     *@brief Provide an iterator on the first element
     *@return An iterator on the first element, end() if map is empty.
     */
    IteratorType begin(){return m_value.begin();}

    /**
     *@brief Provide a const iterator on the first element
     *@return An iterator on the first element, end() if map is empty.
     */
    ConstIteratorType begin() const{return m_value.begin();}

    /**
     * @brief Return an iterator after the last element in the map
     */
    IteratorType end(){return m_value.end();}

    /**
     * @brief Return a const iterator after the last element in the map
     */
    ConstIteratorType end() const{return m_value.end();}


    /**
     * @brief test if the map is empty
     * @return true if empty
     */
    bool empty() const {return m_value.empty();}

    /**
     * @brief returns atom's map
     */
    const MapType& getValue() const {return m_value;}


    /**
     * @brief find an element in the map
     * @param key the key
     * @return an iterator on value pointed by key if exist, else return
     * Map.end()
     */
    MapType::const_iterator find(const std::string &key) const {return m_value.find(key);}


    /**
     * @brief Access to a member with []
     * @param key a key in the map
     * @return the value associated with the key or an empty base if the key
     *         is not in the map.
     */
    Base::sptr operator[](const std::string &key){return m_value[key];}

    /**
     * @brief Returns a clone object
     */
    FWATOMS_API virtual Base::sptr clone() const;

    /**
     * @brief returns Atom type
     */
    ::fwAtoms::Base::AtomType type() const {return ::fwAtoms::Base::MAP;}


protected:
     MapType m_value;
};

}


#endif /* __FWATOMS_BASE_MAP_HPP_ */

