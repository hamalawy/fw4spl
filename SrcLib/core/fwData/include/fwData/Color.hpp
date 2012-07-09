/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_COLOR_HPP_
#define _FWDATA_COLOR_HPP_

#include <boost/array.hpp>

#include "fwData/config.hpp"
#include "fwData/Object.hpp"
#include "fwData/factory/new.hpp"

fwCampAutoDeclareDataMacro((fwData)(Color), FWDATA_API);

namespace fwData
{
/**
 * @class   Color
 * @brief   This class defines color object.
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 */

class FWDATA_CLASS_API Color : public Object
{

public :

    typedef float ColorType;
    typedef ::boost::array<ColorType,4> ColorArray;

    fwCoreClassDefinitionsWithNFactoriesMacro( (Color)(::fwData::Object),
       ((::fwData::factory::New< Color > ,() ))
       ((ColorFactory ,((ColorType)) ((ColorType)(1.0)) ((ColorType) (1.0)) ((ColorType)(1.0)) ))
       );

    fwDataObjectMacro();
	fwCampMakeFriendDataMacro((fwData)(Color));

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWDATA_API Color( ::fwData::Object::Key key );

    /// Destructor
    FWDATA_API virtual ~Color();

    /// Defines shallow copy
    FWDATA_API void shallowCopy( Color::csptr _source );

    /// Defines deep copy
    FWDATA_API void deepCopy( Color::csptr _source );

    fwGettersSettersDocMacro(RGBA, vRGBA, ColorArray, the array of color values (red, green, blue, alpha).);

    ///@{
    ///@brief set RGBA color
    ///@param[in] red
    ///@param[in] green
    ///@param[in] blue
    ///@param[in] alpha
    FWDATA_API void setRGBA( const ColorType red, const ColorType green, const ColorType blue, const ColorType alpha=1.0 );

    ///@brief set RGBA from hexadecimal format (#ffffff)
    ///@param[in] hexaColor c hexadecimal format (#ffffff)
    FWDATA_API void setRGBA( std::string hexaColor );
    ///@}

    /** @name color attributes accessor */
    /**@brief Get editable reference to color attributes
     * @{
     */
    FWDATA_API ColorType  &red();
    FWDATA_API ColorType  &green();
    FWDATA_API ColorType  &blue();
    FWDATA_API ColorType  &alpha();
    /// @}

    /**@brief Get read-only reference to color attributes
     * @{
     */
    FWDATA_API const ColorType  &red()   const ;
    FWDATA_API const ColorType  &green() const ;
    FWDATA_API const ColorType  &blue()  const ;
    FWDATA_API const ColorType  &alpha() const;
    //@}

protected :


    FWDATA_API static sptr ColorFactory(ColorType red, ColorType green, ColorType blue, ColorType alpha);

    //! RGBA of the image (in terms of points)
    ColorArray  m_vRGBA;



}; // end class Color

} // end namespace fwData


#endif // _FWDATA_COLOR_HPP_
