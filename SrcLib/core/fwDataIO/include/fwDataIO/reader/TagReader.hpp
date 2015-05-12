/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATAIO_READER_TAGREADER_HPP__
#define __FWDATAIO_READER_TAGREADER_HPP__

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

#include <fwData/location/SingleFile.hpp>

#include "fwDataIO/config.hpp"
#include "fwDataIO/reader/GenericObjectReader.hpp"

namespace fwData
{
class Tag;
}

namespace fwDataIO
{

namespace reader
{

/**
 * @brief   Tag Reader. Read file format .tag
 * @class   TagReader
 *
 * @date    2009
 *
 * Ircad reader to read a ::fwData::Tag on filesystem which the file format
 * is .tag.
 */
class FWDATAIO_CLASS_API TagReader :
    public GenericObjectReader< ::fwData::Tag >,
    public ::fwData::location::enableSingleFile< IObjectReader >
{

public:
    fwCoreClassDefinitionsWithFactoryMacro( (TagReader)(GenericObjectReader< ::fwData::Tag >),
                                            (()),
                                            ::fwDataIO::reader::factory::New< TagReader >
                                            );
    /// Constructor. Do nothing.
    FWDATAIO_API TagReader(::fwDataIO::reader::IObjectReader::Key key);

    /// Destructor. Do nothing.
    FWDATAIO_API virtual ~TagReader();

    /// Read the file .tag.
    FWDATAIO_API virtual void read();

    /// Defines extension supported by this reader ".tag"
    FWDATAIO_API std::string  extension();

};

} // namespace reader

} // namespace fwDataIO

#endif // __FWDATAIO_READER_TAGREADER_HPP__
