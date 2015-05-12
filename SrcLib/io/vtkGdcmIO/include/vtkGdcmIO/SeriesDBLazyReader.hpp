/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VTKGDCMIO_SERIESDBLAZYREADER_HPP__
#define __VTKGDCMIO_SERIESDBLAZYREADER_HPP__

#include <gdcmScanner.h>

#include <fwCore/macros.hpp>

#include <fwTools/ProgressAdviser.hpp>

#include <fwData/Image.hpp>
#include <fwData/location/Folder.hpp>
#include <fwData/location/MultiFiles.hpp>

#include <fwDataIO/reader/GenericObjectReader.hpp>

#include "vtkGdcmIO/config.hpp"

namespace fwMedData
{
class SeriesDB;
class Equipment;
class ImageSeries;
class Patient;
class Series;
class SeriesDB;
class Study;
}

namespace vtkGdcmIO
{

/**
 * @class SeriesDBLazyReader
 *
 * @brief Reads DICOM data from a directory path in order to create a SeriesDB object in lazy mode.
 */
class SeriesDBLazyReader : public ::fwDataIO::reader::GenericObjectReader< ::fwMedData::SeriesDB >,
                           public ::fwData::location::enableFolder< ::fwDataIO::reader::IObjectReader >,
                           public ::fwData::location::enableMultiFiles< ::fwDataIO::reader::IObjectReader >,
                           public ::fwTools::ProgressAdviser
{

public:

    typedef std::vector< std::string > SeriesFilesType;

    typedef std::map< std::string, SeriesFilesType > MapSeriesType;

    fwCoreClassDefinitionsWithFactoryMacro(
        (SeriesDBLazyReader)( ::fwDataIO::reader::GenericObjectReader< ::fwMedData::SeriesDB >),
        (()),
        ::fwDataIO::reader::factory::New< SeriesDBLazyReader >
        );

    fwCoreAllowSharedFromThis();

    /// Does nothing
    VTKGDCMIO_API SeriesDBLazyReader(::fwDataIO::reader::IObjectReader::Key key);

    /// Does nothing
    VTKGDCMIO_API ~SeriesDBLazyReader();

    /// Reads DICOM data from configured path and fills SeriesDB object. Use lazy reading process to read images.
    VTKGDCMIO_API void read();

private:

    /**
     * @brief Creates a SeriesDB from the data contained in the given directory path.
     *
     * @param directory path from which data is read
     *
     * @return a new SeriesDB object
     */
    SPTR( ::fwMedData::SeriesDB ) createSeriesDB( const ::boost::filesystem::path &dicomDir );

    /**
     * @brief Fills the related SeriesDB object with DICOM data extracted from the given files.
     *
     * @param seriesDB SeriesDB object to be filled with DICOM data
     * @param filenames files to extract DICOM data from
     */
    void addSeries( const SPTR( ::fwMedData::SeriesDB ) &seriesDB, const std::vector< std::string > &filenames);

    /// Tries estimating Z spacing of an image dicom, returns 0 if it is not a success
    double computeZSpacing( const SeriesFilesType & seriesFiles );

    /// Searches and sets image information: window/center, correct spacing after preprocessImage(not read image buffer)
    void fillImage( gdcm::Scanner & scanner, const SeriesFilesType & seriesFiles,
                    const std::string & dcmFile, ::fwData::Image::sptr img );

    /// Searches and sets another image information ( origin, spacing, type, size, nbComponent )
    void preprocessImage( const ::fwData::Image::sptr & img, const SeriesFilesType & files );

    /// Searches and sets equipment information
    void fillEquipment( gdcm::Scanner & scanner, const std::string & dcmFile, SPTR( ::fwMedData::Equipment ) equipment);

    /// Searches and sets study information
    void fillStudy( gdcm::Scanner & scanner, const std::string & dcmFile, SPTR( ::fwMedData::Study ) study );

    /// Searches and sets patient information
    void fillPatient( gdcm::Scanner & scanner, const std::string & dcmFile, SPTR( ::fwMedData::Patient ) patient );

    /// Searches and sets series information
    void fillSeries( gdcm::Scanner & scanner, const std::string & dcmFile, SPTR( ::fwMedData::Series ) series );

    /// Select some dicom tags and scan information in all filenames
    void scanFiles( gdcm::Scanner & scanner, const std::vector< std::string > & filenames );
};

} // namespace vtkGdcmIO

#endif // __VTKGDCMIO_SERIESDBLAZYREADER_HPP__
