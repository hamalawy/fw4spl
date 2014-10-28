/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VTKGDCMIO_SERIESDBREADER_HPP__
#define __VTKGDCMIO_SERIESDBREADER_HPP__


#include <fwTools/ProgressAdviser.hpp>
#include <fwDataIO/reader/GenericObjectReader.hpp>
#include <fwData/location/Folder.hpp>
#include <fwData/location/MultiFiles.hpp>

#include "vtkGdcmIO/config.hpp"
namespace fwMedData
{
class SeriesDB;
}


namespace vtkGdcmIO
{

/**
 * @class SeriesDBReader
 *
 * @brief Reads DICOM data from a directory path in order to create a SeriesDB object.
 */
class SeriesDBReader : public ::fwDataIO::reader::GenericObjectReader< ::fwMedData::SeriesDB >,
                             public ::fwData::location::enableFolder< ::fwDataIO::reader::IObjectReader > ,
                             public ::fwData::location::enableMultiFiles< ::fwDataIO::reader::IObjectReader > ,
                             public ::fwTools::ProgressAdviser
{

public :

    fwCoreClassDefinitionsWithFactoryMacro(
                    (SeriesDBReader)( ::fwDataIO::reader::GenericObjectReader< ::fwMedData::SeriesDB >),
                    (()),
                    ::fwDataIO::reader::factory::New< SeriesDBReader >
                    );
    fwCoreAllowSharedFromThis();

    VTKGDCMIO_API SeriesDBReader(::fwDataIO::reader::IObjectReader::Key key);

    VTKGDCMIO_API ~SeriesDBReader();

    /**
     * @brief Reads DICOM data from configured path and fills SeriesDB object.
     */
    VTKGDCMIO_API void read();

private :

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

};

} // namespace vtkGdcmIO

#endif /*__VTKGDCMIO_SERIESDBREADER_HPP__*/
