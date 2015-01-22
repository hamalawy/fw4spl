/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IOITK_INRIMAGEWRITERSERVICE_HPP__
#define __IOITK_INRIMAGEWRITERSERVICE_HPP__

#include <boost/filesystem/path.hpp>

#include <io/IWriter.hpp>

#include "ioITK/config.hpp"

namespace fwData
{
class Image;
}

namespace ioITK
{

class IOITK_CLASS_API InrImageWriterService : public ::io::IWriter
{

public :
    fwCoreServiceClassDefinitionsMacro ( (InrImageWriterService)( ::io::IWriter) ) ;

    IOITK_API InrImageWriterService() throw();

    IOITK_API virtual ~InrImageWriterService() throw();

    IOITK_API static void saveImage( const ::boost::filesystem::path &inrFile, const SPTR(::fwData::Image) &image );

protected:

    /// Override
    IOITK_API virtual void starting() throw(::fwTools::Failed);

    /// Override
    IOITK_API virtual void stopping() throw(::fwTools::Failed);

    /// Override
    IOITK_API void updating() throw(::fwTools::Failed);

    /// Override
    virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed) {} ;

    /// Override
    IOITK_API void info(std::ostream &_sstream ) ;

    /// Override
    IOITK_API virtual void configureWithIHM();

    /// Return managed file type, here FILE
    IOITK_API ::io::IOPathType getIOPathType() const;


};

} // namespace ioITK

#endif //__IOITK_INRIMAGEWRITERSERVICE_HPP__
