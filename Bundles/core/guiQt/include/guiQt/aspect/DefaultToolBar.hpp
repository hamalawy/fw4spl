/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#ifndef GUIQT_DEFAULT_TOOLBAR_HPP_
#define GUIQT_DEFAULT_TOOLBAR_HPP_

#include "guiQt/config.hpp"


#include "guiQt/aspect/IToolBar.hpp"

namespace guiQt
{

namespace aspect
{

/**
 * @brief   Defines the default toolbar for standard application
 * @class   DefaultToolBar.
 * @author  IRCAD (Research and Development Team)

 * @date    2009.
 */
class  DefaultToolBar : public IToolBar
{

public :

    /// Constructor. Do nothing.
     DefaultToolBar();

    /// Destructor. Do nothing.
     virtual ~DefaultToolBar() throw();

protected:

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief This method create a new ToolBar in frame.
     */
     virtual void starting() throw( ::fwTools::Failed ) ;

    /**
     * @brief This method remove the tools situated in the ToolBar and delete ToolBar.
     */
     virtual void stopping() throw( ::fwTools::Failed );

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
     virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /**
     * @brief This method is used to update services. Do nothing.
     */
     virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief This method is used to configure the class parameters. Do nothing.
    */
     virtual void configuring() throw( ::fwTools::Failed );

    /**
     * @brief This method is used to give information about the service. Do nothing.
     */
     virtual void info(std::ostream &_sstream );
    ///@}


};

}

}
#endif
