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
class  GUIQT_CLASS_API DefaultToolBar : public IToolBar
{

public :

    /// Constructor. Do nothing.
    GUIQT_API DefaultToolBar();

    /// Destructor. Do nothing.
    GUIQT_API virtual ~DefaultToolBar() throw();

protected:

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief This method create a new ToolBar in frame.
     */
    GUIQT_API virtual void starting() throw( ::fwTools::Failed ) ;

    /**
     * @brief This method remove the tools situated in the ToolBar and delete ToolBar.
     */
    GUIQT_API virtual void stopping() throw( ::fwTools::Failed );

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
    GUIQT_API virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /**
     * @brief This method is used to update services. Do nothing.
     */
    GUIQT_API virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief This method is used to configure the class parameters. Do nothing.
    */
    GUIQT_API virtual void configuring() throw( ::fwTools::Failed );

    /**
     * @brief This method is used to give information about the service. Do nothing.
     */
    GUIQT_API virtual void info(std::ostream &_sstream );
    ///@}


};

}

}
#endif
