/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWITKIO_IMAGEREADER_HPP_
#define _FWITKIO_IMAGEREADER_HPP_

#include <fwDataIO/reader/GenericObjectReader.hpp>
#include <fwData/location/SingleFile.hpp>
#include <fwData/Image.hpp>

#include <fwTools/ProgressAdviser.hpp>

#include "fwItkIO/config.hpp"

namespace fwItkIO
{

class ImageReader : public ::fwDataIO::reader::GenericObjectReader< ::fwData::Image >,
                    public ::fwData::location::enableSingleFile< ::fwDataIO::reader::IObjectReader >,
                    public ::fwTools::ProgressAdviser
{

public :

    fwCoreClassDefinitionsWithFactoryMacro((ImageReader)(::fwDataIO::reader::GenericObjectReader< ::fwData::Image>),
                                            (()),
                                            ::fwDataIO::reader::factory::New< ImageReader >
                                           );
    fwCoreAllowSharedFromThis();

    FWITKIO_API ImageReader(::fwDataIO::reader::IObjectReader::Key key);

    FWITKIO_API ~ImageReader();

    FWITKIO_API void read();
};

} // namespace fwItkIO

#endif // _FWITKIO_IMAGEREADER_HPP_
