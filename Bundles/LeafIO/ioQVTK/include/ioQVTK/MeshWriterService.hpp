/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOQVTK_MESHWRITERSERVICE_HPP_
#define _IOQVTK_MESHWRITERSERVICE_HPP_

#include <string>
#include <boost/filesystem/path.hpp>

#include <io/IWriter.hpp>


#include "ioQVTK/config.hpp"

namespace fwData
{
    class TringularMesh;
}


namespace ioVTK
{

/**
 * @brief   VTK mesh writer service.
 * @class   MeshWriterService.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * Service writing a VTK Mesh using the vtkIO lib.
 *
 * Service registered details : \n
 * REGISTER_SERVICE( ::io::IWriter , ::ioVTK::MeshWriterService , ::fwData::TriangularMesh )
 */
class  MeshWriterService : public ::io::IWriter
{

public :
    fwCoreServiceClassDefinitionsMacro ( (MeshWriterService)( ::io::IWriter::Baseclass) ) ;
    /**
     * @brief   Constructor
     *
     */
     MeshWriterService() throw();

    /**
     * @brief   Destructor
     */
     virtual ~MeshWriterService() throw();

    /**
    * @brief Configure the mesh path.
    *
    * This method is used to find
    * the file path  using a file selector.
    */
     virtual void configureWithIHM();


protected:

    /**
    * @brief Configuring method.
    *
    * XML configuration sample:
    * @verbatim
    <service implementation="::ioVTK::MeshWriterService" type="::io::IWriter">
        <filename id=".../meshTest.vtk"/>
    </service>
    @endverbatim
    * This method is used to configure the service.
    */
     virtual void configuring() throw(::fwTools::Failed) ;

    /**
    * @brief Starting method.
    *
    * This method is used to initialize the service.
    */
     virtual void starting() throw(::fwTools::Failed);

    /**
    * @brief Stopping method.
    *
    * The stopping method is empty for this service.
    */
     virtual void stopping() throw(::fwTools::Failed);

    /**
    * @brief Updating method.
    *
    * This method is used to update the service.
    * The mesh is read.
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
    */
     void info(std::ostream &_sstream ) ;


private :

    /**
    * @brief Save a VTK image.
    * @param[in] _vtkFile \b const ::boost::filesystem::path.
    * @param[out] _pTriangularMesh ::boost::shared_ptr< ::fwData::TriangularMesh >.
    *
    * This method is used to save an image using the file path.
    */
    void saveMesh( const ::boost::filesystem::path _vtkFile, ::boost::shared_ptr< ::fwData::TriangularMesh > _pTriangularMesh );

    /**
    * @brief the m_bServiceIsConfigured value is \b true
    * if the image path is known.
    */
    bool m_bServiceIsConfigured;

    /**
    * @brief Mesh path .
    */
    ::boost::filesystem::path m_fsMeshPath;

};

} // namespace ioVTK

#endif //_IOVTK_MESHWRITERSERVICE_HPP_
