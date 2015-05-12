/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GUI_ACTION_QUITACTION_HPP__
#define __GUI_ACTION_QUITACTION_HPP__


#include <fwServices/IService.hpp>

#include <fwGui/IActionSrv.hpp>

#include "gui/config.hpp"

namespace gui
{
namespace action
{

/**
 * @brief   This action tries to close the window and reset root object.
 * @class   QuitAction
 */
class GUI_CLASS_API QuitAction : public ::fwGui::IActionSrv
{

public:

    fwCoreServiceClassDefinitionsMacro ( (QuitAction)(::fwGui::IActionSrv) );

    /**
     * @brief Constructor. Do nothing.
     */
    GUI_API QuitAction() throw();

    /**
     * @brief Destructor. Do nothing.
     */
    GUI_API virtual ~QuitAction() throw();

protected:

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
    GUI_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /**
     * @brief This method is used to configure the service parameters: specifies which services must be started or stopped
     */
    GUI_API void configuring() throw( ::fwTools::Failed );

    GUI_API virtual void starting() throw(::fwTools::Failed);

    GUI_API virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief This method tries to close the window and reset root object.
     */
    GUI_API virtual void updating() throw(::fwTools::Failed);

    /*
     * @brief This method gives information about the class.
     */
    GUI_API virtual void info(std::ostream &_sstream );

};


} // namespace action
} // namespace gui


#endif /*__GUI_ACTION_QUITACTION_HPP__*/
