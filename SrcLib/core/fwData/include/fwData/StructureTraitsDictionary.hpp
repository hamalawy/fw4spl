/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2011.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_STRUCTURETRAITSDICTIONARY_HPP__
#define __FWDATA_STRUCTURETRAITSDICTIONARY_HPP__

#include "fwData/StructureTraits.hpp"
#include "fwData/Object.hpp"
#include "fwData/factory/new.hpp"

fwCampAutoDeclareDataMacro((fwData)(StructureTraitsDictionary), FWDATA_API);

namespace fwData
{
/**
 * @class   StructureTraitsDictionary
 * @brief   This class defines a dictionary of structure traits.
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2011.
 *
 * @note The dictionary cannot contain two structure with the same type.
 * @see StructureTraits
 */
class FWDATA_CLASS_API StructureTraitsDictionary : public ::fwData::Object
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (StructureTraitsDictionary)(::fwData::Object), (()), ::fwData::factory::New< StructureTraitsDictionary >) ;
    fwCampMakeFriendDataMacro((fwData)(StructureTraitsDictionary));

    typedef std::vector<std::string> StructureTypeNameContainer;

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWDATA_API StructureTraitsDictionary(::fwData::Object::Key key);

    /// Destructor. Does nothing.
    FWDATA_API virtual ~StructureTraitsDictionary();


    /**
     * @brief Add a structure in dictionary
     * @param [in] structureTraits a StructureTraits correctly initialized.
     * @pre A structure with the same type cannot exist in dictionary.
     * @pre Only structure of class 'Lesion' or 'Functional' can have attachment.
     * @pre Structure attachment must be of class 'Organ' and must exist in dictionary.
     * @pre Structure type cannot contain space.
     */
    FWDATA_API void addStructure(StructureTraits::sptr structureTraits);

    /**
     * @brief Return the structure traits for given type
     * @pre given 'type' must exist in dictionary
     */
    FWDATA_API StructureTraits::sptr getStructure(std::string type);

    /// Return all array names stock in the structureTraits-map
    FWDATA_API StructureTypeNameContainer getStructureTypeNames() const;

    /// Defines shallow copy
    FWDATA_API void shallowCopy( StructureTraitsDictionary::csptr _source );

    /// Defines deep copy
    FWDATA_API void deepCopy( StructureTraitsDictionary::csptr _source );

private:

    typedef std::map< std::string, ::fwData::StructureTraits::sptr > StructureTraitsMapType;

    /// map to register structure traits. Map key is structure type.
    StructureTraitsMapType m_structureTraitsMap;

};

} // namespace fwData

#endif // __FWDATA_STRUCTURETRAITSDICTIONARY_HPP__
