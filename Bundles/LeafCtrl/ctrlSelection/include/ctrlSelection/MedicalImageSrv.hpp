/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _CTRLSELECTION_MEDICALIMAGESRV_HPP_
#define _CTRLSELECTION_MEDICALIMAGESRV_HPP_

#include <fwServices/IService.hpp>
#include <fwServices/IController.hpp>

#include "ctrlSelection/config.hpp"

namespace ctrlSelection
{


/**
 * @class  MedicalImageSrv.
 * @brief  This service convert its attached image to a medical image by adding specific fields.
 * @author IRCAD (Research and Development Team).

 * @date   2010.
 */
class CTRLSELECTION_CLASS_API MedicalImageSrv : public ::fwServices::IController
{

public :

    fwCoreServiceClassDefinitionsMacro ( (MedicalImageSrv)(::fwServices::IController) ) ;


    CTRLSELECTION_API MedicalImageSrv() throw() ;

    CTRLSELECTION_API virtual ~MedicalImageSrv() throw() ;

protected:

    /// Implements starting method derived from IService. Convert the image.
    CTRLSELECTION_API virtual void starting()  throw ( ::fwTools::Failed );

    /// Implements stopping method derived from IService. Do nothing.
    CTRLSELECTION_API virtual void stopping()  throw ( ::fwTools::Failed );

    /// Implements swapping method derived from IService. Convert the image.
    CTRLSELECTION_API virtual void swapping()  throw ( ::fwTools::Failed );

    /// Implements configuring method derived from IService. Do nothing.
    CTRLSELECTION_API virtual void configuring()  throw ( ::fwTools::Failed );

    /// Implements reconfiguring method derived from IService. Do nothing.
    CTRLSELECTION_API virtual void reconfiguring()  throw ( ::fwTools::Failed );

    /// Implements updating method derived from IService. Do nothing.
    CTRLSELECTION_API virtual void updating() throw ( ::fwTools::Failed );

    /// Implements info method derived from IService. Print classname.
    CTRLSELECTION_API virtual void info( std::ostream &_sstream );

    CTRLSELECTION_API virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed );

private :

    void convertImage();

    std::vector< std::string > m_imageCompositeKeys;
};

} // ctrlSelection

#endif // _CTRLSELECTION_MEDICALIMAGESRV_HPP_
