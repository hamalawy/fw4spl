/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef GUIQT_ASPECT_IMENUBAR_HPP_
#define GUIQT_ASPECT_IMENUBAR_HPP_

#include "guiQt/config.hpp"


#include <fwServices/IService.hpp>

//#include "gui/export.hpp"

namespace guiQt
{

namespace aspect
{

/**
 * @brief   Defines the service interface managing GUI aspect, including menu actions and views
 * @class   IMenuBar.
 * @author  IRCAD (Research and Development Team)

 * @date    2009.
 */
class GUIQT_CLASS_API IMenuBar : public ::fwServices::IService
{

public :

    fwCoreNonInstanciableClassDefinitionsMacro ( (IMenuBar)( ::fwServices::IService::Baseclass) ) ;

    /// Default constructor, do nothing.
    GUIQT_API IMenuBar() throw() ;

    /// Default destructor, do nothing.
    GUIQT_API virtual ~IMenuBar() throw() ;

};

}
}

#endif /* GUI_ASPECT_IMENUBAR_HPP_ */
