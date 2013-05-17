/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_CONTAINER_DICOMSCOORD_HPP__
#define __GDCMIO_CONTAINER_DICOMSCOORD_HPP__

#include <string>
#include <vector>
#include <cmath>

#include <fwData/macros.hpp>

#include "gdcmIO/config.hpp"
#include "gdcmIO/container/DicomCodedAttribute.hpp"
#include "gdcmIO/container/DicomDictionarySR.hpp"

namespace gdcmIO
{
namespace container
{

/**
 * @brief   This class define Spatial COORDinates.
 *
 * @see     PS 3.3 C.18.6
 *
 * @class   DicomSCoord
 */
class GDCMIO_CLASS_API DicomSCoord
{

public :

    GDCMIO_API DicomSCoord();

    GDCMIO_API ~DicomSCoord();

    GDCMIO_API fwGettersMacro(GraphicData, graphicData, std::vector<float>);

    /**
     * @brief   Return number of coordinates.
     */
    GDCMIO_API unsigned int getGraphicDataSize() const;

    GDCMIO_API void setGraphicData(const std::vector< float > & a_coords);

    GDCMIO_API void setGraphicData(const float * a_coords, const unsigned int length);

    /**
     * @brief   Access to graphic data at \p i.
     */
    GDCMIO_API float operator[](const unsigned int i) const;

    GDCMIO_API fwGettersSettersDocMacro(GraphicType, graphicType, std::string, The type of spatial coordinates (eg : "POINT", "CIRCLE"));

    GDCMIO_API void setGraphicType(const DicomDictionarySR::SRGraphicType a_type)
    {
        m_graphicType = DicomDictionarySR::getGraphicTypeString(a_type);
    }

    GDCMIO_API fwGettersSettersDocMacro(Context, context, DicomCodedAttribute, The SCOORD context (eg : Path, Path Vertex, ...));

private :

    /**
     * @brief   Convert a number into graphic data
     *
     * This method convert a floating coordinate into a number with one decimal.
     * This decimal have to be equal to 0 or 5 in order to refer to the side or the middle
     * of a pixel.
     *
     * @see     PS 3.3 C.10.5.1.2
     */
    template <typename T>
    float toGraphicData(const T & a_coord);

    std::vector<float>  m_graphicData;      ///< The spatial coordinates (eg : [x1,y1,x2,y2,...]).

    std::string         m_graphicType;      ///< The SCOORD type (eg : POINT, POLYLINE, CIRCLE).

    DicomCodedAttribute m_context;          ///< The SCOORD context (eg : Path, Path Vertex, ...).

};

//------------------------------------------------------------------------------

template <typename T>
float DicomSCoord::toGraphicData(const T & a_coord)
{
    float   result;

    int     integer_part    = std::floor( a_coord );
    int     sign            = ((integer_part > 0)?1:-1);

    float   decimals        = sign*( sign*a_coord - sign*integer_part ); // <=> abs( abs(a_float) - abs(integer_part) )

    result = (float) integer_part;
    if (decimals > 0.75)
        result += sign;
    else if (decimals > 0.25)
        result += sign * 0.5;

    return result;
}

} //namespace container
} //namespace gdcmIO

#endif /* __GDCMIO_CONTAINER_DICOMSCOORD_HPP__ */
