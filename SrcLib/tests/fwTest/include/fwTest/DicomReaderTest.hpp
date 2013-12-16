/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWTEST__DICOMREADERTEST_HPP__
#define __FWTEST__DICOMREADERTEST_HPP__

#include "fwTest/config.hpp"

namespace fwMedData
{
    class ImageSeries;
}

namespace fwTest
{

/**
 * @class   DicomReaderTest
 * @brief   This class defines some tools to check dicom manipulations during tests
 * 
 * @date    2007-2012.
 */
class FWTEST_CLASS_API DicomReaderTest
{

public:

    /**
     * @brief Method used to verify if a specific dicom file (stored on the
     * test database) is well read
     *
     * This file is located here on test database : "fw4spl/Patient/Dicom/ACHGenou",
     * its represents a CT image of a knee ( 400 slices ).
     */
    FWTEST_API static bool checkSeriesACHGenou( const SPTR(::fwMedData::ImageSeries) &series );
};




} // namespace fwTest

#endif // __FWTEST__DICOMREADERTEST_HPP__
