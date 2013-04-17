/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_PLANE_HPP_
#define _FWDATA_PLANE_HPP_

#include <vector>

#include <fwMath/IntrasecTypes.hpp>

#include "fwData/config.hpp"
#include "fwData/factory/new.hpp"
#include "fwData/Point.hpp"

fwCampAutoDeclareDataMacro((fwData)(Plane), FWDATA_API);
namespace fwData
{
/**
 * @class   Plane
 * @brief   This class defines a plane defined by tree points.
 * @see     ::fwData::Point
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 */
class FWDATA_CLASS_API Plane : public Object
{

public :
    fwCoreClassDefinitionsWithFactoryMacro( (Plane)(::fwData::Object),
        (()), ::fwData::factory::New< Plane >) ;

    fwCampMakeFriendDataMacro((fwData)(Plane));

    typedef ::boost::array< ::fwData::Point::sptr, 3> PointContainer;


    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWDATA_API Plane(::fwData::Object::Key key);

    /// Destructor
    FWDATA_API virtual ~Plane();

    /// Defines shallow copy
    FWDATA_API void shallowCopy( const Object::csptr& _source );

    /// Defines deep copy
    FWDATA_API void cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType &cache);

    /*
     * @brief Compare method
     *
     * Compare plane normal and distance from origin
     */
    FWDATA_API bool operator==( const Plane & _plane ) ;

    /// get the plane coordinate
    fwPlane getPlane() const {return m_plane;};

    /// Re-initialize the plane with 3 points
    FWDATA_API void setValue(::fwData::Point::sptr _point1, ::fwData::Point::sptr _point2, ::fwData::Point::sptr _point3);

    /// Compute plane normal and distance from points coordinates
    FWDATA_API void computePlaneFromPoints();

    /// Points container
    fwGettersSettersDocMacro(Points, vPoints, PointContainer, a container of all points);

    fwGettersSettersDocMacro(IsIntersection, isIntersection, bool, flag if the plane is an intersection one (else an union one));

protected :

    fwPlane m_plane;
    //! Points container
    PointContainer m_vPoints;

    //! flag if the plane is an intersection (else an union)
    bool m_isIntersection;

}; // end class Plane

} // end namespace fwData

#endif // _FWDATA_PLANE_HPP_
