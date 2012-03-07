/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_POINT_HPP_
#define _FWDATA_POINT_HPP_

#include <boost/array.hpp>

#include "fwData/config.hpp"
#include "fwData/Object.hpp"
#include "fwMath/IntrasecTypes.hpp"


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
       ((::fwData::Factory::New< Point > ,() ))
       ((PointFactory ,((float)) ((float)(0.0)) ((float) (0.0)) ))
       ((PointFactory ,((Point::sptr)) ))
       );

    typedef double PointCoordType;

    typedef fwVec3d PointCoordArrayType;
    typedef PointCoordArrayType PointCoordArray; // Remove me

    fwDataObjectMacro();

    /// Defines shallow copy
    FWDATA_API void shallowCopy( Point::csptr _source );

    /// Defines deep copy
    FWDATA_API void deepCopy( Point::csptr _source );

    /// Coordinates of point
    fwGettersSettersDocMacro(Coord, vCoord, fwVec3d, point coordinates. );

protected :

    /// Constructor
    FWDATA_API Point();

    /// Destructor
    FWDATA_API virtual ~Point();

    /// Point factory
    FWDATA_API static Point::sptr PointFactory(float x, float y, float z);
    FWDATA_API static Point::sptr PointFactory( Point::sptr p );

    /// point coordinates
    PointCoordArrayType m_vCoord;

}; // end class Point

} // end namespace fwData

#endif // _FWDATA_POINT_HPP_
