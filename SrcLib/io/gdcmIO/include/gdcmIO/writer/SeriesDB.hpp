/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_WRITER_SERIESDB_HPP__
#define __GDCMIO_WRITER_SERIESDB_HPP__

#include <fwTools/ProgressAdviser.hpp>

#include <fwMedData/SeriesDB.hpp>
#include <fwData/location/Folder.hpp>

#include <fwDataIO/writer/GenericObjectWriter.hpp>

#include "gdcmIO/config.hpp"

namespace gdcmIO
{

namespace writer
{

/**
 * @brief   This class manages patient writing, in DICOM file format.
 *
 * @class   SeriesDB
 */
class SeriesDB : public ::fwDataIO::writer::GenericObjectWriter< ::fwMedData::SeriesDB >,
                                    public ::fwData::location::enableFolder< ::fwDataIO::writer::IObjectWriter >,
                                    public ::fwTools::ProgressAdviser
{

public :

    fwCoreClassDefinitionsWithFactoryMacro( (SeriesDB)( ::fwDataIO::writer::GenericObjectWriter< ::fwMedData::SeriesDB >),
                                            (()),
                                            ::fwDataIO::writer::factory::New< SeriesDB >
                                          );


    GDCMIO_API SeriesDB(::fwDataIO::writer::IObjectWriter::Key key);

    GDCMIO_API ~SeriesDB();

    /**
     * @brief    Manage writing tools to save all patients.
     *
     * It launches DicomGlobalManager for each patient.
     */
    GDCMIO_API void write();

    /**
     * Overriding
     * @brief   Do nothing
     */
    GDCMIO_API std::string extension();
};

}   // namespace writer

}   // namespace gdcmIO

#endif // __GDCMIO_WRITER_SERIESDB_HPP__
