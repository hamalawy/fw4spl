/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_POINT_HPP_
#define _FWDATA_POINT_HPP_

#include <fwMath/IntrasecTypes.hpp>

#include "fwData/config.hpp"
#include "fwData/Object.hpp"
#include "fwData/factory/new.hpp"


namespace fwData
{
/**
 * @class   Point
 * @brief   This class define a 3D point.
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 */


class FWDATA_CLASS_API Point : public Object
{

public :

    fwCoreClassDefinitionsWithNFactoriesMacro( (Point)(::fwData::Object),
       ((::fwData::factory::New< Point > ,() ))
       ((PointFactory ,((float)) ((float)(0.0)) ((float) (0.0)) ))
       ((PointFactory ,((Point::sptr)) ))
       );

    typedef double PointCoordType;
    typedef fwVec3d PointCoordArrayType;

    fwDataObjectMacro();

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWDATA_API Point(::fwData::Object::Key key);

    /// Destructor
    FWDATA_API virtual ~Point();

    /// Defines shallow copy
    FWDATA_API void shallowCopy( Point::csptr _source );

    /// Defines deep copy
    FWDATA_API void deepCopy( Point::csptr _source );

    /// Coordinates of point
    fwGettersSettersDocMacro(Coord, vCoord, fwVec3d, point coordinates. );

protected :

    /// Point factory
    FWDATA_API static Point::sptr PointFactory(float x, float y, float z);
    FWDATA_API static Point::sptr PointFactory( Point::sptr p );

    /// point coordinates
    PointCoordArrayType m_vCoord;

}; // end class Point

} // end namespace fwData

#endif // _FWDATA_POINT_HPP_
