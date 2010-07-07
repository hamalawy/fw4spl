/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWGUI_IACTIONSRV_HPP_
#define _FWGUI_IACTIONSRV_HPP_

#include <fwServices/IService.hpp>

#include "fwGui/config.hpp"

#include "fwGui/registrar/ActionRegistrar.hpp"


namespace fwGui
{

/**
 * @brief   Defines the service interface managing the menu items.
 * @class   IActionSrv.
 * @author  IRCAD (Research and Development Team).
 * @date    2009-2010.
 *
 */
class FWGUI_CLASS_API IActionSrv : public ::fwServices::IService
{

public :

    fwCoreServiceClassDefinitionsMacro ( (IActionSrv)(::fwServices::IService) ) ;

protected :
    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;

    /**
     * @brief   Constructor. Initialize default values.
     */
    FWGUI_API IActionSrv() ;

    /**
     * @brief   Destructor. Do nothing.
     */
    FWGUI_API virtual ~IActionSrv() ;

    /// Initialize the layout manager and registrar.
    FWGUI_API void initialize();

private:

    ::fwGui::registrar::ActionRegistrar::sptr    m_registrar;


    /// Flag to hide or disable the actions if the service is stopped
    bool m_hideActions;
};

} // namespace fwGui

#endif /*_FWGUI_IACTIONSRV_HPP_*/


