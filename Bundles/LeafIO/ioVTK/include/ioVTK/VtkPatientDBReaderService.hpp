/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOVTK_VTKPATIENTDBREADERSERVICE_HPP_
#define _IOVTK_VTKPATIENTDBREADERSERVICE_HPP_

#include <string>
#include <boost/filesystem/path.hpp>

#include <io/IReader.hpp>


#include "ioVTK/export.hpp"

namespace fwData
{
    class Image;
}

namespace ioVTK
{

/**
 * @brief   VTK image reader service.
 * @class   VtkPatientDBReaderService.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * Service reading a VTK Image using the vtkIO lib
 * and inserting the image in the PatientDB.
 *
 * Service registered details : \n
 * fwServicesRegisterMacro( ::io::IReader , ::ioVTK::VtkPatientDBReaderService , ::fwData::PatientDB )
 */
class IOVTK_CLASS_API VtkPatientDBReaderService : public ::io::IReader
{

public :
    ~VtkPatientDBReaderService() throw() {}

    fwCoreServiceClassDefinitionsMacro ( (VtkPatientDBReaderService)( ::io::IReader) ) ;

    /**
    * @brief Configure the image path.
    *
    * This method is used to find
    * the file path  using a file selector.
    */
    IOVTK_API virtual void configureWithIHM();


protected:

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

    IOVTK_API virtual ::io::IOPathType getIOPathType() const;

    /**
    * @brief Updating method.
    *
    * This method is used to update the service.
    * The image is read.
    */
    IOVTK_API virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief React on modifications : default does nothing.
     *
     * @note This method is automatically called by update( msg ) method from base service ( ::fwServices::IService ).
     *
     * @param[in] _msg information message for modification
     */
    void receiving( CSPTR(::fwServices::ObjectMsg) _msg ) throw(::fwTools::Failed){};

    /**
    * @brief Info method.
    *
    * This method is used to give
    * informations about the service.
    */
    IOVTK_API void info(std::ostream &_sstream ) ;


private :

    /**
    * @brief Notification method.
    *
    * This method is used to notify
    * the images services.
    */
    void notificationOfDBUpdate();

    /**
    * @brief Create a VTK image.
    * @param[in] _vtkFileDir \b const ::boost::filesystem::path.
    * @param[out] _img ::boost::shared_ptr< ::fwData::Image >.
    * @return bool.
    *
    * This method is used to create an image using the file path.
    * Returns \b true if the image creation is a success and \b false
    * if it fails
    */
    bool createImage( const ::boost::filesystem::path _vtkFileDir, ::boost::shared_ptr< ::fwData::Image > _img );

    /**
    * @brief the m_bServiceIsConfigured value is \b true
    * if the image path is known.
    */
    bool m_bServiceIsConfigured;

    /**
    * @brief Image path .
    */
    ::boost::filesystem::path m_fsImagePath;

};

} // namespace ioVTK

#endif //_IOVTK_VTKPATIENTDBREADERSERVICE_HPP_
