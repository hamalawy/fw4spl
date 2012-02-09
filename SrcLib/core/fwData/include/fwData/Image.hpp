/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_IMAGE_HPP_
#define _FWDATA_IMAGE_HPP_

#include <vector>

#include <boost/shared_array.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/cstdint.hpp>

#include <fwTools/Type.hpp>
#include <fwTools/DynamicType.hpp>

#include "fwData/Object.hpp"
#include "fwData/Array.hpp"


namespace fwData
{

/**
 * @class   Image
 * @brief   This class defines an image
 *
 * An image contains a buffer and is defined by some parameters (size, spacing, pixel type, ...)
 *
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 */
class FWDATA_CLASS_API Image : public Object
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (Image)(::fwData::Object), (()), ::fwTools::Factory::New< Image > ) ;
    fwCoreAllowSharedFromThis();

    typedef ::fwData::Array::SizeType SizeType;
    typedef std::vector< double > SpacingType;
    typedef std::vector< double > OriginType;

    typedef SizeType::value_type IndexType;
    typedef size_t BufferIndexType;
    typedef ::boost::uint8_t BufferType;

    typedef ::boost::shared_array< BufferType > SharedArray;

    fwDataObjectMacro();

    /// Defines shallow copy
    FWDATA_API void shallowCopy( Image::csptr _source );

    /// Defines deep copy
    FWDATA_API void deepCopy( Image::csptr _source );

    /// image get image information from source. Informations are spacing,origin,size ... expect Fields
    FWDATA_API void copyInformation( Image::csptr _source );


    /**
     * @brief an R/W accessor to the buffer (can involve unserialization/restoration)
     */
    FWDATA_API void * getBuffer() const;

    /// Number of dimension of the image (3 for 3D image)
    FWDATA_API size_t getNumberOfDimensions() const;

    /// Image spacing
    FWDATA_API const SpacingType &getSpacing() const;
    FWDATA_API void setSpacing(const SpacingType &spacing);

    /// Image Origin
    FWDATA_API const OriginType &getOrigin() const;
    FWDATA_API void setOrigin(const OriginType &origin);

    /// Image Size
    FWDATA_API const SizeType &getSize() const;
    FWDATA_API void setSize(const SizeType &size);


    fwGettersSettersDocMacro(WindowCenter, dWindowCenter, double, window level);

    fwGettersSettersDocMacro(WindowWidth, dWindowWidth, double, window width);


    /// Helpers for 3D images
    FWDATA_API void* getPixelBuffer( SizeType::value_type x, SizeType::value_type y, SizeType::value_type z ) const;
    FWDATA_API void* getPixelBuffer( IndexType index ) const;
    FWDATA_API SharedArray getPixelBufferCopy( SizeType::value_type x, SizeType::value_type y, SizeType::value_type z ) const;
    FWDATA_API SharedArray getPixelBufferCopy( IndexType index ) const;

    FWDATA_API void setPixelBuffer( IndexType index , Image::BufferType * pixBuf);

    FWDATA_API static Image::BufferType* getPixelBuffer( Image::BufferType *buffer, IndexType offset, ::boost::uint8_t imagePixelSize );
    FWDATA_API static SharedArray getPixelBufferCopy( Image::BufferType *buffer, IndexType offset, ::boost::uint8_t imagePixelSize );
    FWDATA_API static void  setPixelBuffer( Image::BufferType *destBuffer, const Image::BufferType * pixBuf, IndexType offset, ::boost::uint8_t imagePixelSize );



    FWDATA_API ::fwData::Array::sptr getDataArray() const;
    FWDATA_API void setDataArray(::fwData::Array::sptr array, bool copyArrayInfo = true);

    FWDATA_API ::fwTools::Type getType() const;
    FWDATA_API void setType(::fwTools::Type type);

    /// get a DynamicType for retrocompatibility
    FWDATA_API ::fwTools::DynamicType getPixelType() const;

    FWDATA_API size_t allocate()
        throw(::fwData::Exception);
    FWDATA_API size_t allocate(SizeType::value_type x, SizeType::value_type y,  SizeType::value_type z, const ::fwTools::Type &type)
        throw(::fwData::Exception);
    FWDATA_API size_t allocate(const SizeType &size, const ::fwTools::Type &type)
        throw(::fwData::Exception);


    /// @brief return image size in bytes
    FWDATA_API size_t getSizeInBytes() const;
    size_t getAllocatedSizeInBytes() const;

    /**
     * @brief Get a string containing pixel value
     * @param[in] _image image containing the pixel
     * @param[in] _x x coordinate of the pixel
     * @param[in] _y y coordinate of the pixel
     * @param[in] _z z coordinate of the pixel
     *
     * @return pixel value
     */
    FWDATA_API std::string  getPixelAsString( unsigned int _x, unsigned int _y, unsigned int _z ) const;

protected :

    /**
     * @brief Constructor
     */
    FWDATA_API Image();

    /**
     * @brief Destructor
     */
    FWDATA_API virtual ~Image() throw();


    FWDATA_API void allocateData();

    //! Size of the image (in terms of points)
    SizeType m_size;

    //! type of image pixel
    ::fwTools::Type m_type;

    //! An array on the voxel size of the image
    SpacingType m_spacing;

    //! Origin of the image in 3D repair
    OriginType m_origin;

    /** @{
     * @brief Visu control to adjust contrast image (come from dicom image ?)
     */
    double m_dWindowCenter;
    double m_dWindowWidth;
    // @}

    //! image buffer
    ::fwData::Array::sptr m_dataArray;

};


} // namespace fwData


#endif //_FWDATA_IMAGE_HPP_

