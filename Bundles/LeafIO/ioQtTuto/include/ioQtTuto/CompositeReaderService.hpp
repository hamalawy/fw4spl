/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOQTTUTO_COMPOSITEREADERSERVICE_HPP_
#define _IOQTTUTO_COMPOSITEREADERSERVICE_HPP_

#include "ioQtTuto/config.hpp"
#include "io/IReader.hpp"

namespace ioTuto
{
/**
 * @brief   Read a composite data structure from Xml file.
 * @class   CompositeReaderService.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 *
 */
class  CompositeReaderService : public ::io::IReader
{
public:
    fwCoreServiceClassDefinitionsMacro ( (CompositeReaderService)( ::io::IReader::Baseclass) ) ;
    /// Constructor. Do nothing (Just initialize parameters).
     CompositeReaderService() throw() ;

    /// Destructor. Do nothing.
     virtual ~CompositeReaderService() throw() ;

protected:

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief   This method initializes class member parameters from configuration elements.
     * @note    Call action base class configuring() method.
     */
     void configuring() throw( ::fwTools::Failed ) ;

    /**
     * @brief  Starting method. Call base class starting() method.
     */
     void starting() throw( ::fwTools::Failed ) ;

    /**
     * @brief  Stopping method. Call base class stopping() method.
     */
     void stopping() throw( ::fwTools::Failed ) ;

    /**
     * @brief Use the ::fwXML::reader::FwXMLObjectReader which propose xml writer services.
     *
     */
     void updating() throw(::fwTools::Failed) ;

    /**
     * @brief React on modifications : does nothing.
     */
     void updating(::boost::shared_ptr< const ::fwServices::ObjectMsg > _msg) throw(::fwTools::Failed) ;

    /**
     * @brief Give a short information about the action.
     */
     void info(std::ostream &_sstream ) ;

    /**
     * @brief Display a selector to choose the path and the name of file to write.
     */
     void configureWithIHM();
    /// @}

private:

    ::boost::filesystem::path m_fsExternalDataPath;

};

} // namespace IOTUTO

#endif // _IOTUTO_COMPOSITEREADERSERVICE_HPP_
