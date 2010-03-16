/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUIQT_ACTION_STARTERACTIONSERVICE_HPP_
#define _GUIQT_ACTION_STARTERACTIONSERVICE_HPP_

#include <vector>
#include <fwServices/IService.hpp>

#include "guiQt/action/IAction.hpp"


namespace guiQt
{

namespace action
{

/**
 * @brief   This action reset root object. All services are eliminated as well as objects composing the root object.
 * @class   CloseAction.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 */
class  StarterActionService : public ::guiQt::action::IAction
{

public :

    /**
    * @brief Constructor. Do nothing.
    */
    StarterActionService() throw();

    /**
    * @brief Destructor. Do nothing.
    */
    virtual ~StarterActionService() throw();

protected:

    enum ActionType {
        START,
        STOP,
        START_IF_EXISTS,
        STOP_IF_EXISTS,
        DO_NOTHING
    };

    /**
     * @brief This method gives information about the class. Do nothing.
     */
    virtual void info(std::ostream &_sstream ) ;

    /**
     * @brief This method starts-updates or stops the specified services
     */
    void updating()throw (fwTools::Failed);

    /**
     * @brief This method is used to configure the service parameters: specifies which services must be started or stopped
     */
    void configuring() throw( ::fwTools::Failed );

private:
    // vector representing uuid's services that must be started (true) or stopped (false)
    std::vector< std::pair< std::string, ActionType > > m_uuidServices;
};


} // namespace action
} // namespace gui
#endif

