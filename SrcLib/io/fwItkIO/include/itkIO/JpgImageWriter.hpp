/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWITKIO_JPGIMAGEWRITER_HPP_
#define _FWITKIO_JPGIMAGEWRITER_HPP_

#include <boost/filesystem/path.hpp>

#include <fwDataIO/writer/GenericObjectWriter.hpp>
#include <fwData/location/Folder.hpp>
#include <fwTools/ProgressAdviser.hpp>
#include <fwData/Image.hpp>

#include "itkIO/config.hpp"

namespace itkIO
{

class JpgImageWriter :  public ::fwDataIO::writer::GenericObjectWriter< ::fwData::Image >,
                        public ::fwData::location::enableFolder< ::fwDataIO::writer::IObjectWriter >,
                        public ::fwTools::ProgressAdviser
{

public :

    fwCoreClassDefinitionsWithFactoryMacro((JpgImageWriter)( ::fwDataIO::writer::GenericObjectWriter< ::fwData::Image>),
                                           (()),
                                           ::fwDataIO::writer::factory::New< JpgImageWriter >
                                          );
    fwCoreAllowSharedFromThis();

    FWITKIO_API JpgImageWriter(::fwDataIO::writer::IObjectWriter::Key key);

    FWITKIO_API ~JpgImageWriter();

    FWITKIO_API void write();

    FWITKIO_API std::string  extension();
};

} // namespace itkIO

#endif // _FWITKIO_JPGIMAGEWRITER_HPP_
