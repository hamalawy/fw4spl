/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef GUIQT_ASPECT_IMENU_HPP_
#define GUIQT_ASPECT_IMENU_HPP_

#include "guiQt/config.hpp"


#include <fwServices/IService.hpp>



namespace guiQt
{

namespace aspect
{

/**
 * @brief   Defines the service interface managing GUI aspect, including menu actions and views
 * @class   IMenu.
 * @author  IRCAD (Research and Development Team)

 * @date    2009.
 */
class IMenu : public ::fwServices::IService
{

public :

    fwCoreNonInstanciableClassDefinitionsMacro ( (IMenu)( ::fwServices::IService::Baseclass) ) ;

    /// Default constructor, do nothing.
    IMenu() throw() ;

    /// Default destructor, do nothing.
    virtual ~IMenu() throw() ;

};

}
}

#endif /* GUI_ASPECT_IMENU_HPP_ */
