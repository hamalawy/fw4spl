/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IOVTK_IMAGEWRITERSERVICE_HPP_
#define _IOVTK_IMAGEWRITERSERVICE_HPP_

#include <string>
#include <boost/filesystem/path.hpp>

#include <io/IWriter.hpp>


#include "ioQVTK/config.hpp"

namespace fwData
{
    class Image;
}


namespace ioVTK
{

/**
 * @brief   VTK image writer service.
 * @class   ImageWriterService.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * Service writing a VTK Image using the vtkIO lib.
 *
 * Service registered details : \n
 * REGISTER_SERVICE( ::io::IWriter , ::ioVTK::ImageWriterService , ::fwData::Image )
 */
class  ImageWriterService : public ::io::IWriter
{

public :
    fwCoreServiceClassDefinitionsMacro ( (ImageWriterService)( ::io::IWriter::Baseclass) ) ;
    /**
     * @brief   Constructor
     *
     */
     ImageWriterService() throw();

    /**
     * @brief   Destructor
     */
     virtual ~ImageWriterService() throw();

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
    <service implementation="::ioVTK::ImageWriterService" type="::io::IWriter">
        <filename id=".../imageTest.vtk"/>
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
    * @brief Save a VTK image.
    * @param[in] _vtkFile ::boost::filesystem::path.
    * @param[out] _pImage ::boost::shared_ptr< ::fwData::Image >.
    * @return bool.
    *
    * This method is used to save an image using the file path.
    * Returns \b true if the image saving is a success and \b false if it fails
    */
    bool saveImage( const ::boost::filesystem::path _vtkFile, ::boost::shared_ptr< ::fwData::Image > _pImage );

    /**
    * @brief the m_bServiceIsConfigured value is \b true
    * if the image path is known.
    */
    bool m_bServiceIsConfigured;

    /**
    * @brief Image path.
    */
    ::boost::filesystem::path m_fsImgPath;

};

} // namespace ioVTK

#endif //_IOVTK_IMAGEWRITERSERVICE_HPP_
