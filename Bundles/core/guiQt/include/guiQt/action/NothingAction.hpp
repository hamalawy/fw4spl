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
 * @brief   This action does nothing.
 * @class   NothingAction.
 * @author  IRCAD (Research and Development Team).

 * @date    2009.
 */
class  NothingAction : public ::guiQt::action::IAction
{

public :
    
    /**
    * @brief Constructor. Do nothing.
    */
    NothingAction() throw() ;

    /**
    * @brief Destructor. Do nothing.
    */
    virtual ~NothingAction() throw() ;

protected:

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
    virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief This method gives information about the class. Do nothing.
     */
    virtual void info(std::ostream &_sstream ) ;
    
};


} // namespace action
} // namespace gui
