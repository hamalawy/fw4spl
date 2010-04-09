/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOQVTK_MESHREADERSERVICE_HPP_
#define _IOQVTK_MESHREADERSERVICE_HPP_

#include <string>
#include <boost/filesystem/path.hpp>

#include <io/IReader.hpp>


#include "ioQVTK/config.hpp"

namespace fwData
{
    class TriangularMesh;
}

namespace ioQVTK
{

/**
 * @brief   Mesh reader service.
 * @class   MeshReaderService .
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * Service reading a VTK mesh using the vtkIO lib.
 *
 * Service registered details : \n
 * REGISTER_SERVICE( ::io::IReader , ::ioVTK::MeshReaderService , ::fwData::TriangularMesh )
 */
class  MeshReaderService : public ::io::IReader
{

public :
    fwCoreServiceClassDefinitionsMacro ( (MeshReaderService)( ::io::IReader::Baseclass) ) ;
    /**
     * @brief   Constructor
     *
     */
     MeshReaderService() throw();

    /**
     * @brief   Destructor
     */
     virtual ~MeshReaderService() throw();

    /**
    * @brief Configure the image path.
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
    <service implementation="::ioVTK::MeshReaderService" type="::io::IReader">
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
    * The image is read.
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
    * @brief Load a VTK mesh.
    * @param[in] _vtkFile ::boost::filesystem::path.
    * @param[out] _pTriangularMesh ::boost::shared_ptr< ::fwData::TriangularMesh >.
    * @return bool.
    *
    * This method is used to load a mesh using the file path.
    */
    void loadMesh( const ::boost::filesystem::path _vtkFile, ::boost::shared_ptr< ::fwData::TriangularMesh > _pTriangularMesh );

    /**
     * @brief Notification method.
     *
     * This method is used to notify
     * the mesh services.
     */
    void notificationOfUpdate();

    /**
    * @brief the m_bServiceIsConfigured value is \b true
    * if the mesh path is known.
    */
    bool m_bServiceIsConfigured;

    /**
    * @brief Mesh path.
    */
    ::boost::filesystem::path m_fsMeshPath;

};

} // namespace ioVTK

#endif //_IOVTK_MESHREADERSERVICE_HPP_
