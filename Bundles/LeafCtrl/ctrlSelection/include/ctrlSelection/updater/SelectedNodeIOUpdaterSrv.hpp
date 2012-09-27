/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _CTRLSELECTION_SELECTEDNODEIOFROMGRAPHUPDATERSRV_HPP_
#define _CTRLSELECTION_SELECTEDNODEIOFROMGRAPHUPDATERSRV_HPP_

#include "ctrlSelection/config.hpp"
#include "ctrlSelection/IUpdaterSrv.hpp"

namespace ctrlSelection
{

namespace updater
{

/**
 * @class  SelectedNodeIOUpdaterSrv
 * @brief  Update the composite related to this service according to port information
 * ( key= portID , value= data in dataNode ), port mode can be "input" or "output"
 * @author IRCAD (Research and Development Team).

 * @date    2009-2010.
 */
class CTRLSELECTION_CLASS_API SelectedNodeIOUpdaterSrv : public ::ctrlSelection::IUpdaterSrv
{

public :

    fwCoreServiceClassDefinitionsMacro ( (SelectedNodeIOUpdaterSrv)(::ctrlSelection::IUpdaterSrv) ) ;

    /// Constructor.  Do nothing.
    CTRLSELECTION_API SelectedNodeIOUpdaterSrv() throw() ;

    /// Destructor. Do nothing.
    CTRLSELECTION_API virtual ~SelectedNodeIOUpdaterSrv() throw() ;

protected:

    /// Implements starting method derived from IService. Do nothing.
    CTRLSELECTION_API virtual void starting()  throw ( ::fwTools::Failed );

    /// Implements stopping method derived from IService. Do nothing.
    CTRLSELECTION_API virtual void stopping()  throw ( ::fwTools::Failed );

    /**
     * @brief Implements configuring method derived from IService.
     *
     * @verbatim
         <service uid="myUpdater" implementation="::ctrlSelection::updater::ResecFromPDBUpdaterSrv" type="::ctrlSelection::IUpdaterSrv" autoComChannel="no">
             <mode>input</mode>
         </service>
      @endverbatim
      The mode must be "input" or "output"
     */
    CTRLSELECTION_API virtual void configuring()  throw ( ::fwTools::Failed );

    /// Implements reconfiguring method derived from IService. Do nothing.
    CTRLSELECTION_API virtual void reconfiguring()  throw ( ::fwTools::Failed );

    /// Implements updating method derived from IService. Do nothing.
    CTRLSELECTION_API virtual void updating() throw ( ::fwTools::Failed );

    /// Implements info method derived from IService. Print classname.
    CTRLSELECTION_API virtual void info( std::ostream &_sstream );

    /**
     * Update the composite related to this service according to node port information
     * ( key= portID , value= data in dataNode ), port mode can be "input" or "output"
     */
    CTRLSELECTION_API virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed );

    /// to sepcify if the updater manage input (upStream=true) or output (default)
    bool  m_upStream;
};

} // updater
} // ctrlSelection

#endif // _CTRLSELECTION_SELECTEDNODEIOFROMGRAPHUPDATERSRV_HPP_
