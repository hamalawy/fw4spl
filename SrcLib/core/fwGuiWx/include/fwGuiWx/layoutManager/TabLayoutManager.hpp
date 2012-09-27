/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWGUIWX_LAYOUTMANAGER_TABLAYOUTMANAGER_HPP_
#define _FWGUIWX_LAYOUTMANAGER_TABLAYOUTMANAGER_HPP_

#include <wx/notebook.h>

#include <fwCore/base.hpp>

#include <fwGui/layoutManager/TabLayoutManagerBase.hpp>

#include "fwGuiWx/container/WxContainer.hpp"
#include "fwGuiWx/config.hpp"

namespace fwGui
{

/**
 * @brief   Defines the tab layout manager.
 * @class   TabLayoutManager.
 * @author  IRCAD (Research and Development Team).
 * @date    2009-2010.
 *
 */
class FWGUIWX_CLASS_API TabLayoutManager : public ::fwGui::layoutManager::TabLayoutManagerBase
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (TabLayoutManager)(::fwGui::layoutManager::TabLayoutManagerBase),
                                            (()),
                                            ::fwGui::factory::New< TabLayoutManager > );

    FWGUIWX_API TabLayoutManager(::fwGui::GuiBaseObject::Key key);

    FWGUIWX_API virtual ~TabLayoutManager();

    /**
     * @brief Instantiate layout with parent container.
     * @pre LayoutManager must be initialized before.
     * @pre parent containers must be instanced.
     */
    FWGUIWX_API virtual void createLayout( ::fwGui::container::fwContainer::sptr parent );

    /**
     * @brief Destroy local layout with sub containers.
     * @pre services using this sub containers must be stopped before.
     */
    FWGUIWX_API virtual void destroyLayout();

private:

    ::fwGuiWx::container::WxContainer::sptr m_parentContainer;

    // Layout manager of this view
    wxNotebook * m_notebook;
};

} // namespace fwGui

#endif /*_FWGUIWX_LAYOUTMANAGER_TABLAYOUTMANAGER_HPP_*/


