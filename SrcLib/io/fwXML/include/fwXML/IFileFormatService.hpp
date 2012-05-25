/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#ifndef _FWWXML_IFILEFORMATSERVICE_HPP_
#define _FWWXML_IFILEFORMATSERVICE_HPP_

#include <boost/filesystem/path.hpp>

#include <fwServices/IService.hpp>
#include <fwTools/ProgressAdviser.hpp>

#include "fwXML/config.hpp"
#include "fwXML/FSLocation.hpp"

namespace fwDataIO
{
namespace reader
{
class IObjectReader;
}
namespace writer
{
class IObjectWriter;
}
}


namespace fwXML
{
/**
 * @brief The purpose of this class is to provide an io service to native data that are not stored in xml way aka .trian .inr.gz
 */
class FWXML_CLASS_API IFileFormatService : public ::fwServices::IService, public FSLocation,
                                              public ::fwTools::ProgressAdviser
{
public:
    fwCoreNonInstanciableClassDefinitionsMacro ( (IFileFormatService)(::fwTools::Object) ) ;
    FWXML_API IFileFormatService();

    FWXML_API virtual ~IFileFormatService();

    /// load image from this->rootFolder()/ this->localFolder()  / this->filename() location
    FWXML_API virtual void load() = 0;

    /// save image to this->rootFolder()/ this->localFolder()  / this->filename() location
    FWXML_API virtual void save() = 0;

    // ovveride
    FWXML_API virtual std::string getPersistanceId();


    /**
     * @brief allow to change reader policy
     */
    FWXML_API void setReader( SPTR(::fwDataIO::reader::IObjectReader) _reader);
    FWXML_API void setWriter( SPTR(::fwDataIO::writer::IObjectWriter) _writer);

    /// RW accessor policy
    FWXML_API  SPTR(::fwDataIO::reader::IObjectReader) getReader();
    FWXML_API  SPTR(::fwDataIO::writer::IObjectWriter) getWriter();


protected :

    virtual void starting() throw(::fwTools::Failed) {} ;
    virtual void stopping() throw(::fwTools::Failed) {} ;
    virtual void configuring() throw(::fwTools::Failed) {} ;
    virtual void updating() throw(::fwTools::Failed) {} ;
    virtual void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed) {} ;

    // Override
    FWXML_API virtual void info(std::ostream &_sstream );

    SPTR(::fwDataIO::reader::IObjectReader) m_reader;
    SPTR(::fwDataIO::writer::IObjectWriter) m_writer;
};

}

#endif /* _FWWXML_IFILEFORMATSERVICE_HPP_*/
