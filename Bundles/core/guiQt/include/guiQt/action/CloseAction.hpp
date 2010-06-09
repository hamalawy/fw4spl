/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

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
class GUIQT_CLASS_API CloseAction : public ::guiQt::action::IAction
{

public :

    /**
    * @brief Constructor. Do nothing.
    */
	GUIQT_API CloseAction() throw() ;

    /**
    * @brief Destructor. Do nothing.
    */
    GUIQT_API virtual ~CloseAction() throw() ;

protected:

    /**
     * @brief This method is used to reset root object. All services are eliminated as well as objects composing the root object.
     */
    GUIQT_API virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief This method gives information about the class. Do nothing.
     */
    GUIQT_API virtual void info(std::ostream &_sstream ) ;

};


} // namespace action
} // namespace gui


