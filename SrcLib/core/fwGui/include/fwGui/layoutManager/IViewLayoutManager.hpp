/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

/**
 * @file fwGui/IViewLayoutManagerBase.hpp
 * @brief This file defines the interface of the base class for managing a layout geometry.
 *
 * @author IRCAD (Research and Development Team).
 * @date 2009-2010
 */

#ifndef _FWGUI_LAYOUTMANAGER_ILAYOUTMANAGER_HPP_
#define _FWGUI_LAYOUTMANAGER_ILAYOUTMANAGER_HPP_

#include <fwRuntime/ConfigurationElement.hpp>

#include "fwGui/GuiBaseObject.hpp"
#include "fwGui/container/fwContainer.hpp"
#include "fwGui/config.hpp"

namespace fwGui
{
namespace layoutManager
{

/**
 * @brief   Defines the generic layout manager for IHM.
 * @class   IViewLayoutManager.
 * @author  IRCAD (Research and Development Team).
 * @date    2009-2010.
 *
 */
class FWGUI_CLASS_API IViewLayoutManager : public ::fwGui::GuiBaseObject
{
public:

    fwCoreNonInstanciableClassDefinitionsMacro( (IViewLayoutManager)(::fwGui::GuiBaseObject) )

    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;

    /// Constructor. Do nothing.
    FWGUI_API IViewLayoutManager();

    /// Destructor. Do nothing.
    FWGUI_API virtual ~IViewLayoutManager();

    /**
     * @brief Configure the layout before creation.
     */
    FWGUI_API virtual void initialize( ConfigurationType configuration) = 0;

    /**
     * @brief Instantiate layout with parent container.
     * @pre LayoutManager must be initialized before.
     * @pre parent containers must be instanced.
     */
    FWGUI_API virtual void createLayout( ::fwGui::container::fwContainer::sptr parent ) = 0;

    /**
     * @brief Destroy local layout with sub containers.
     * @pre services using this sub containers must be stopped before.
     */
    FWGUI_API virtual void destroyLayout() = 0;

    /**
     * @brief Returns all sub containers managed by this layout.
     */
    FWGUI_API virtual std::vector< ::fwGui::container::fwContainer::sptr > getSubViews();

protected:

    /**
     * @brief Helper to destroy local sub views.
     */
    FWGUI_API virtual void destroySubViews();

    /// All sub containers managed by this layout.
    std::vector< ::fwGui::container::fwContainer::sptr > m_subViews;

};

} // namespace layoutManager
} // namespace fwGui

#endif /*_FWGUI_LAYOUTMANAGER_ILAYOUTMANAGER_HPP_*/


