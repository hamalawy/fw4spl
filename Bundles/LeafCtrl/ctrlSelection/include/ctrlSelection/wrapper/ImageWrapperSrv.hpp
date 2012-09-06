/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _CTRLSELECTION_MANAGER_GRAPHWRAPPERSRV_HPP_
#define _CTRLSELECTION_MANAGER_GRAPHWRAPPERSRV_HPP_

#include <fwServices/IService.hpp>

#include "ctrlSelection/config.hpp"
#include "ctrlSelection/IWrapperSrv.hpp"

namespace ctrlSelection
{

namespace wrapper
{

/**
 * @class  ImageWrapperSrv.
 * @brief  Convert generic message by specific message from Image
 * @author IRCAD (Research and Development Team).

 * @date   2007-2009.
 */
class CTRLSELECTION_CLASS_API ImageWrapperSrv : public ::ctrlSelection::IWrapperSrv
{

public :

    fwCoreServiceClassDefinitionsMacro ( (ImageWrapperSrv)(::ctrlSelection::IWrapperSrv) ) ;

//    /// Constructor.  Do nothing.
    CTRLSELECTION_API ImageWrapperSrv() throw() ;

    /// Destructor. Do nothing.
    CTRLSELECTION_API virtual ~ImageWrapperSrv() throw();

protected:

    /// Implements starting method derived from IService. Do nothing.
   virtual void starting()  throw ( ::fwTools::Failed ) {};

    /// Implements stopping method derived from IService. Do nothing.
    virtual void stopping()  throw ( ::fwTools::Failed ) {};

    /// Implements configuring method derived from IService. Do nothing.
    virtual void configuring()  throw ( ::fwTools::Failed ) {};

    /// Implements reconfiguring method derived from IService. Do nothing.
    virtual void reconfiguring()  throw ( ::fwTools::Failed ) {};

    /// Implements updating method derived from IService. Do nothing.
    virtual void updating() throw ( ::fwTools::Failed ) {};

    /// Implements info method derived from IService. Print classname.
    virtual void info( std::ostream &_sstream ) {};

    /**
     * @brief Convert the Object message with "UPDATED_OBJECT" event sent on Graph by GraphMsg with "NEW_GRAPH" event
     */
    CTRLSELECTION_API virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed );


};

} // wrapper
} // ctrlSelection

#endif // _CTRLSELECTION_MANAGER_MEDICALIMAGEMANAGERSRV_HPP_
