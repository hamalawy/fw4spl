/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIPYTHON_ACTION_STRINGRUN_HPP__
#define __UIPYTHON_ACTION_STRINGRUN_HPP__


#include <fwServices/IService.hpp>

#include <fwGui/IActionSrv.hpp>

#include "uiPython/config.hpp"

namespace uiPython
{
namespace action
{

/**
 * @brief   This action tries to close the window and reset root object.
 * @class   StringRun
 *

 * @date    2009-2010.
 */
class UIPYTHON_CLASS_API StringRun : public ::fwGui::IActionSrv
{

public:

    fwCoreServiceClassDefinitionsMacro ( (StringRun)(::fwGui::IActionSrv) );

    /**
     * @brief Constructor. Do nothing.
     */
    UIPYTHON_API StringRun() throw();

    /**
     * @brief Destructor. Do nothing.
     */
    UIPYTHON_API virtual ~StringRun() throw();

protected:

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
    UIPYTHON_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /**
     * @brief This method is used to configure the service parameters: specifies which services must be started or stopped
     */
    UIPYTHON_API void configuring() throw( ::fwTools::Failed );

    UIPYTHON_API virtual void starting() throw(::fwTools::Failed);

    UIPYTHON_API virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief This method tries to close the window and reset root object.
     */
    UIPYTHON_API virtual void updating() throw(::fwTools::Failed);

    /*
     * @brief This method gives information about the class.
     */
    UIPYTHON_API virtual void info(std::ostream &_sstream );

};


} // namespace action
} // namespace uiPython


#endif /*__UIPYTHON_ACTION_STRINGRUN_HPP__*/
