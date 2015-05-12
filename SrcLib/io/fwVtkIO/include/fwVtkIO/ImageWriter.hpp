/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWVTKIO_IMAGEWRITER_HPP__
#define __FWVTKIO_IMAGEWRITER_HPP__

#include <boost/filesystem/path.hpp>

#include <vtkActor.h>

#include <fwDataIO/writer/GenericObjectWriter.hpp>
#include <fwData/location/SingleFile.hpp>
#include <fwTools/ProgressAdviser.hpp>
#include <fwData/Image.hpp>

#include "fwVtkIO/config.hpp"

namespace fwVtkIO
{

/**
 * @brief   Write an image.
 * @class   ImageWriter
 *
 * @date    2009.
 *
 * Write a VTK Image using the VTK lib
 */
class ImageWriter : public ::fwDataIO::writer::GenericObjectWriter< ::fwData::Image >,
                    public ::fwData::location::enableSingleFile< ::fwDataIO::writer::IObjectWriter >,
                    public ::fwTools::ProgressAdviser
{

public:

    fwCoreClassDefinitionsWithFactoryMacro((ImageWriter)( ::fwDataIO::writer::GenericObjectWriter< ::fwData::Image >),
                                           (()),
                                           ::fwDataIO::writer::factory::New< ImageWriter >
                                           );
    fwCoreAllowSharedFromThis();

    //! @brief Constructor.
    FWVTKIO_API ImageWriter(::fwDataIO::writer::IObjectWriter::Key key);

    //! @brief Destructor.
    FWVTKIO_API ~ImageWriter();

    //! @brief Writing operator.
    FWVTKIO_API void write();

    /// @return ".vtk"
    FWVTKIO_API std::string extension();
};

} // namespace fwVtkIO

#endif // __FWVTKIO_IMAGEWRITER_HPP__
