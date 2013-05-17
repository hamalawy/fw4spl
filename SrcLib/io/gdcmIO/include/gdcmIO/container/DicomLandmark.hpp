/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_CONTAINER_DICOMLANDMARK_HPP__
#define __GDCMIO_CONTAINER_DICOMLANDMARK_HPP__

#include <string>
#include <vector>

#include <fwData/Image.hpp>
#include <fwData/macros.hpp>

#include "gdcmIO/config.hpp"
#include "gdcmIO/exception/Failed.hpp"
#include "gdcmIO/container/DicomSCoord.hpp"

namespace gdcmIO
{
namespace container
{

/**
 * @brief   This class define a container of landmarks to translate
 * in DICOM/FW4SPL form.
 *
 * @class   DicomLandmark
 */
class GDCMIO_CLASS_API DicomLandmark
{

public :

    typedef std::vector< ::gdcmIO::container::DicomSCoord > SCoordsType;

    GDCMIO_API DicomLandmark();

    GDCMIO_API ~DicomLandmark();

    /**
     * \brief   Set landmark(s) data in an image.
     *
     * \param   a_image Image where landmark(s) could be inserted.
     */
    GDCMIO_API void convertToData(::fwData::Image::sptr a_image);

    /**
     * \brief   Get landmark(s) data from an image and convert it to a closed DICOM form.
     *
     * \param   a_image Image which can contain landmark(s).
     */
    GDCMIO_API void setFromData(::fwData::Image::csptr a_image) throw (::gdcmIO::exception::Failed);

    GDCMIO_API fwGettersSettersDocMacro(Labels, labels, std::vector<std::string>, Labels container (eg : label for each landmark : "Label_1"));

    GDCMIO_API fwGettersSettersDocMacro(SCoords, SCoords, SCoordsType, Spatial coordinates container (eg : coordinates on x, y axis));

    GDCMIO_API fwGettersSettersDocMacro(RefFrames, refFrames, std::vector<unsigned int>, Slice numbers container (equal to coordinate on z axis));

    /**
     * @brief   Add to DicomLandmark the label of one landmark.
     */
    GDCMIO_API void addLabel(const std::string & a_label);

    /**
     * @brief   Add to DicomLandmark the SCOORD of one landmark.
     */
    GDCMIO_API void addSCoord(const ::gdcmIO::container::DicomSCoord & a_point);

    /**
     * @brief   Add to DicomLandmark the referenced frame number of one landmark.
     */
    GDCMIO_API void addRefFrame(const unsigned int a_refFrame);

private :

    std::vector<std::string>    m_labels;       ///< Labels container (e.g: label for each landmark : "Label_1")

    SCoordsType                 m_SCoords;      ///< Coordinates container (equal to coordinates on x, y axis).
                                                ///< (e.g: coordinates for each landmark : "175.0,23.5").

    std::vector<unsigned int>   m_refFrames;    ///< Slice numbers container (equal to coordinate on z axis)
};

} //namespace container
} //namespace gdcmIO

#endif /* __GDCMIO_CONTAINER_DICOMLANDMARK_HPP__ */
