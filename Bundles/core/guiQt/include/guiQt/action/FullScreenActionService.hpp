/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUIQT_ACTION_FULLSCREENACTIONSERVICE_HPP_
#define _GUIQT_ACTION_FULLSCREENACTIONSERVICE_HPP_

#include <fwServices/IService.hpp>

#include "guiQt/action/IAction.hpp"
#include "guiQt/config.hpp"



namespace guiQt
{

namespace action
{

/**
 * @brief   This action shows the current window in full screen mode or restored to its normal state.
 * @class   FullScreenActionService.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 */
class  FullScreenActionService : public ::guiQt::action::IAction
{

public:

    /**
    * @brief Constructor. Do nothing (Just initialize parameters).
    *
    * By default, the m_isFullScreen is defined as false.
    */
     FullScreenActionService() throw();

    /*
     * @brief Destructor. Do nothing.
     */
     virtual ~FullScreenActionService() throw();

protected:

    /**
     * @brief This method is used to configure the class parameters. Do nothing.
     */
     void configuring() throw( ::fwTools::Failed ) ;

    /**
     * @brief This method is used to update services on notification.
     * Show the current window in full screen mode or restored to its normal state.
     */
     void updating() throw(::fwTools::Failed) ;

    /*
     * @brief This method gives information about the class.
     */
     void info(std::ostream &_sstream ) ;

private:

    /// To know if the window is in full-screen mode.
    bool m_isFullScreen;
};
}
}

#endif /* _GUI_ACTION_FULLSCREENACTIONSERVICE_HPP_ */
