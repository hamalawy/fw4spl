/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATATOOLS_IMAGE_HPP_
#define _FWDATATOOLS_IMAGE_HPP_

#include <fwCore/base.hpp>

#include <fwTools/Type.hpp>

#include <fwData/Image.hpp>

#include <fwMath/Compare.hpp>

#include "fwDataTools/export.hpp"

namespace fwDataTools
{


/**
 * @brief   This class contains helper to generate and compare images.
 * @class   Image.
 * @author  IRCAD (Research and Development Team).
 * @date    2011.
 *
 */
class Image : public ::fwCore::BaseObject
{

public :

    fwCoreClassDefinitionsWithFactoryMacro((Image)( ::fwCore::BaseObject ),
                                           (()),
                                           new Image
                                          );
    fwCoreAllowSharedFromThis();

    /**
     * @brief Initialize 'rand' seed
     */
    FWDATATOOLS_API static void initRand();

    /**
     * @brief Generate an image with the given informations. Buffer is filled with 0.
     * @param image     image to generate
     * @param dim       image dimension (3 for 3D)
     * @param size      vector of image size
     * @param spacing   vector of image spacing
     * @param origin    vector of image origin
     * @param type      image type
     */
    FWDATATOOLS_API static void generateImage(::fwData::Image::sptr image,
                                              ::fwData::Image::SizeType size,
                                              std::vector<double> spacing,
                                              std::vector<double> origin,
                                              ::fwTools::Type type);

    /// Generate an image with random information (size, spacing, ...). Buffer is filled with random values.
    FWDATATOOLS_API static void generateRandomImage(::fwData::Image::sptr image, ::fwTools::Type type);

    /**
     * @brief compare two images.
     * @return true if the images are equal.
     * @note if false, the image difference is written in SLM log
     */
    FWDATATOOLS_API static bool compareImage(   ::fwData::Image::sptr image1,
                                                ::fwData::Image::sptr image2,
                                                 double spacingTolerance = 0,
                                                 double originTolerance = 0,
                                                 std::string errorPrefix = "");

    /**
     * @brief compare image information.
     * @return true if the image has the given information.
     * @note if false, read SLM log
     */
    FWDATATOOLS_API static bool compareImage(::fwData::Image::sptr image1,
                                             ::fwData::Image::SizeType size,
                                             std::vector<double> spacing,
                                             std::vector<double> origin,
                                             ::fwTools::Type type);
    /**
     * @brief compare two arrays.
     * @return true if the arrays are equal.
     */
    FWDATATOOLS_API static bool compareArray(::fwData::Array::sptr buff1, ::fwData::Array::sptr buff2, std::string errorPrefix = "");

    /// Fill array with random value
    FWDATATOOLS_API static void randomizeArray(::fwData::Array::sptr array);

    /**
     * @brief Apply roi to image
     *
     * set input image voxel to zero where roi voxel value is zero
     */
    FWDATATOOLS_API static void applyRoi( ::fwData::Image::sptr image,
                                          ::fwData::Image::sptr roi );

    /**
     * @brief Check if 'imgRoiApplyed' is the result of 'roi' applyed to 'image'
     *
     */
    FWDATATOOLS_API static bool isRoiApplyed( ::fwData::Image::sptr image,
                                              ::fwData::Image::sptr imgRoiApplyed,
                                              ::fwData::Image::sptr roi );

    /**
     * @brief Merge mask in image imgDest: put value 'val' in imgDest when mask value != 0
     */
    template<typename IMG_DEST_TYPE, typename MASK_TYPE>
    void mergeMask(::fwData::Image::sptr imgDest, ::fwData::Image::sptr mask, IMG_DEST_TYPE val );

protected:


    //! @brief Constructor.
    FWDATATOOLS_API Image();

    //! @brief Destructor.
    FWDATATOOLS_API virtual ~Image();

};


//------------------------------------------------------------------------------

template<typename IMG_DEST_TYPE, typename MASK_TYPE>
void Image::mergeMask(::fwData::Image::sptr imgDest, ::fwData::Image::sptr mask, IMG_DEST_TYPE val )
{
    typedef IMG_DEST_TYPE  ImgDestType;
    typedef MASK_TYPE MaskType;
    SLM_ASSERT( "Image dest has not correct type", imgDest->getType().isOfType< ImgDestType >());
    SLM_ASSERT( "Image mask has not correct type", mask->getType().isOfType< MaskType >());

    SLM_ASSERT( "Images have not the same size", imgDest->getSize() == mask->getSize() );
    SLM_ASSERT( "Images have not the same spacing",
            ::fwMath::isContainerEqual(imgDest->getSpacing(), mask->getSpacing()) );
    SLM_ASSERT( "Images have not the same origin",
            ::fwMath::isContainerEqual(imgDest->getOrigin(), mask->getOrigin()) );

    ::fwData::Array::sptr imgData;
    ::fwData::Array::sptr maskData;
    imgData = imgDest->getDataArray();
    maskData = mask->getDataArray();

    ImgDestType *imgIt = imgData->begin<ImgDestType>();
    MaskType *maskIt = maskData->begin<MaskType>();

    const ImgDestType *imgEnd = imgIt + maskData->getNumberOfElements();

    for ( ; imgIt != imgEnd ; ++imgIt, ++maskIt)
    {
        if (*maskIt != 0)
        {
            *imgIt = val;
        }
    }
}

} // namespace fwDataTools

#endif // _FWDATATOOLS_IMAGE_HPP_
