/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUI_APPLICATION_HPP__
#define __FWGUI_APPLICATION_HPP__

#include <fwCore/base.hpp>

#include "fwGui/IApplication.hpp"
#include "fwGui/config.hpp"

namespace fwGui
{

/**
 * @brief   Gives access to the underlayed application part
 * @class   Application
 *
 * @date    2009-2010.
 *
 */
class FWGUI_CLASS_API Application : public ::fwGui::IApplication
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (Application)(::fwGui::IApplication), (()), Application::factory);

    FWGUI_API static Application::sptr getDefault();

protected:

    /// Application factory, returning the registered instance of Application. Manage a Singleton.
    FWGUI_API static Application::sptr factory();

    Application()
    {
    }
    virtual ~Application()
    {
    }


};

} // namespace fwGui

#endif /*__FWGUI_APPLICATION_HPP__*/


