/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2011.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_STRUCTURETRAITSDICTIONARY_HPP__
#define __FWDATA_STRUCTURETRAITSDICTIONARY_HPP__

#include "fwData/StructureTraits.hpp"
#include "fwData/Object.hpp"

namespace fwData
{
/**
 * @class   StructureTraitsDictionary
 * @brief   This class defines a dictionary of structure traits.
 * @author  IRCAD (Research and Development Team).
 * @date    2007-20011.
 */
class FWDATA_CLASS_API StructureTraitsDictionary : public ::fwData::Object
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (StructureTraitsDictionary)(::fwData::Object), (()), ::fwTools::Factory::New< StructureTraitsDictionary >) ;

    typedef std::map< std::string, ::fwData::StructureTraits::sptr > StructureTraitsMapType;

protected :

    /// Constructor
    FWDATA_API StructureTraitsDictionary();

    /// Destructor. Does nothing.
    FWDATA_API virtual ~StructureTraitsDictionary() throw();

    /// Return the structure traits for given type
    FWDATA_API StructureTraits::sptr getStructure(std::string type);

    /// Add a structure
    FWDATA_API void addStructure(StructureTraits::sptr structureTraits);

private:

    /// map of structure traits
    StructureTraitsMapType m_structureTraitsMap;

};

} // namespace fwData

#endif // __FWDATA_STRUCTURETRAITSDICTIONARY_HPP__
