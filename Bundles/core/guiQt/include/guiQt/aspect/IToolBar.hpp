/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef GUIQT_ASPECT_ITOOLBAR_HPP_
#define GUIQT_ASPECT_ITOOLBAR_HPP_

#include "guiQt/config.hpp"


#include <fwServices/IService.hpp>

namespace guiQt
{

namespace aspect
{

/**
 * @brief   Defines the default toolbar for standard application
 * @class   IToolBar.
 * @author  IRCAD (Research and Development Team)
 * @date    2009.
 */
class  IToolBar : public ::fwServices::IService
{

public :

    fwCoreNonInstanciableClassDefinitionsMacro ( (IToolBar)( ::fwServices::IService::Baseclass) ) ;

    /**
     * @brief Constructor. Do nothing (Just initialize parameters).
     */
     IToolBar() throw() ;

    /**
     * @brief Destructor. Do nothing.
     */
     virtual ~IToolBar() throw() ;

protected :

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief This method create a new ToolBar in frame, and add tools with IToolBar::m_actionIds list.
     */
     virtual void starting() throw( ::fwTools::Failed ) = 0;

    /**
     * @brief This method remove the tools situated in the ToolBar and delete ToolBar.
     */
     virtual void stopping() throw( ::fwTools::Failed ) = 0;

    /**
     * @brief This method is used to update services on notification. Do nothing.
     * @attention An IToolBar service does not received a message.
     */
     virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed) = 0 ;

    /**
     * @brief This method is used to update services. Do nothing.
     */
     virtual void updating() throw(::fwTools::Failed) = 0;

    /**
     * @brief This method is used to configure the class parameters. Do nothing.
    */
     virtual void configuring() throw( ::fwTools::Failed ) = 0;

    /**
     * @brief This method is used to give information about the service. Do nothing.
     * @todo Must be not implemented and class child declaration should be imposed.
     */
     virtual void info(std::ostream &_sstream ) = 0;

    ///@}
};

}
}

#endif