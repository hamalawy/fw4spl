/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOQT_TRIANGULARMESHWRITER_HPP_
#define _IOQT_TRIANGULARMESHWRITER_HPP_

#include <io/IWriter.hpp>
#include <boost/filesystem/path.hpp>

#include "ioQt/config.hpp"

namespace ioData
{
/**
 * @brief   Triangular mesh writer service.
 * @class   TriangularMeshWriterService.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * Service writing a mesh object.
 * @li This service has no specified start and stop method.
 * @li Use setConfiguration(cfg) and configure() methods to configure the mesh filename.
 * @li Use update() to write the mesh and notify observers.
 *
 * Service registered details : \n
 * REGISTER_SERVICE( ::io::IWriter , ::ioData::TriangularMeshWriterService , ::fwData::TriangularMesh )
 */
class  TriangularMeshWriterService : public ::io::IWriter
{


public:

    fwCoreServiceClassDefinitionsMacro ( (TriangularMeshWriterService)(::io::IWriter::Baseclass) ) ;

     /// Super class of writer services
    typedef ::io::IWriter   SuperClass;

    /**
     * @brief   Constructor : does nothing
     */
     TriangularMeshWriterService() ;

    /**
     * @brief   destructor
     */
     ~TriangularMeshWriterService() throw() ;

    /** @name Specified writer service methods ( override from ::io::IWriter )
     * @{
     */

    /**
     * @brief   returns  (filename) extension
     */
     virtual std::vector< std::string > getSupportedExtensions() ;

    /**
     * @brief Configure the image path.
     *
     * This method is used to find
     * the file path  using a file selector.
     */
     void configureWithIHM();
    /// @}

protected:

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief Starting method : default does nothing.
     *
     * This method is used to initialize the service.
     */
     virtual void starting() throw(::fwTools::Failed){};

    /**
     * @brief Stopping method : default does nothing.
     *
     * The stopping method is empty for this service.
     */
     virtual void stopping() throw(::fwTools::Failed){};


    /**
     * @brief Configure service. This method is called by configure() from base service ( ::fwServices::IService )
     *
     * XML configuration sample:
     * @verbatim
    <service type="::ioData::TriangularMeshWriterService">
        <filename>../matrix.trf</filename>
    </service>
     @endverbatim
     *
     * Configure mesh filename.
     */
     virtual void configuring( ) throw(::fwTools::Failed);

    /**
     * @brief Updating method. This method is called by update() from base service ( ::fwServices::IService )
     *
     * This method is used to update the service.
     * The mesh is written with the writer ::fwDataIO::writer::TriangularMeshWriter.
     * Notify writing.
     */
     void updating() throw(::fwTools::Failed);

    /**
     * @brief React on modifications : default does nothing.
     *
     * @note This method is automaticaly called by update( msg ) method from base service ( ::fwServices::IService ).
     *
     * @param[in] _msg information message for modification
     */
     void updating( ::boost::shared_ptr< const ::fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed){};


    /**
     * @brief Info method.
     *
     * This method is used to give
     * informations about the service.
     *
     * @param[out] _sstream output stream
     */
     virtual void info(std::ostream &_sstream ) ;
    /// @}

private :
    ::boost::filesystem::path m_fsMeshPath;
    bool m_bServiceIsConfigured;
};

}

#endif /*_IODATA_TRIANGULARMESHWRITER_HPP_*/
