/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_TAG_HPP_
#define _FWDATA_TAG_HPP_


#include <vector>

#include "fwData/config.hpp"
#include "fwData/PointList.hpp"
#include "fwData/Object.hpp"
#include "fwData/factory/new.hpp"

fwCampAutoDeclareDataMacro((fwData)(Tag), FWDATA_API);

namespace fwData
{
/**
 * @class   Tag
 * @brief   This class represent a tag. A tag contains a list of points.
 *
 * @see ::fwData::PointList
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 */
class FWDATA_CLASS_API Tag : public Object
{

public :
    fwCoreClassDefinitionsWithFactoryMacro( (Tag)(::fwData::Object), (()), ::fwData::factory::New< Tag >) ;

    fwDataObjectMacro();

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWDATA_API Tag(::fwData::Object::Key key);

    /// Destructor
    FWDATA_API virtual ~Tag();

    fwCampMakeFriendDataMacro((fwData)(Tag));

    fwGettersSettersDocMacro(PointList, pointList, ::fwData::PointList::sptr, the list of points )

    fwGettersSettersDocMacro(Type, sType, std::string, the type of tag)

    fwGettersSettersDocMacro(Size, size, double, the size of the tag)

    /// Defines shallow copy
    FWDATA_API void shallowCopy( Tag::csptr source );

    /// Defines deep copy
    FWDATA_API void deepCopy( Tag::csptr source );

protected :

    /// list of points
    ::fwData::PointList::sptr  m_pointList;

    /// type of tag
    std::string m_sType;

    /// size of tag
    double m_size;

}; // end class Tag

} // end namespace fwData


#endif // _FWDATA_TAG_HPP_
