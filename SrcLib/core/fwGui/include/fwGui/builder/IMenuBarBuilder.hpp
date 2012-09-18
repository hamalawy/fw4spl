/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWGUI_BUILDER_IMENUBARBUILDER_HPP_
#define _FWGUI_BUILDER_IMENUBARBUILDER_HPP_

#include <fwRuntime/ConfigurationElement.hpp>

#include "fwGui/GuiBaseObject.hpp"
#include "fwGui/container/fwMenuBar.hpp"
#include "fwGui/container/fwContainer.hpp"
#include "fwGui/config.hpp"

namespace fwGui
{
namespace builder
{

/**
 * @brief   Defines the interface class  for the menubar builder.
 * @class   IMenuBarBuilder.
 * @author  IRCAD (Research and Development Team).
 * @date    2009-2010.
 *
 */
class FWGUI_CLASS_API IMenuBarBuilder : public ::fwGui::GuiBaseObject
{
public:
    fwCoreNonInstanciableClassDefinitionsMacro( (IMenuBarBuilder)(::fwGui::GuiBaseObject) )

    typedef std::string RegistryKeyType;

    FWGUI_API const static RegistryKeyType REGISTRY_KEY;

    /// Constructor. Do nothing.
    FWGUI_API IMenuBarBuilder();

    /// Destructor. Do nothing.
    FWGUI_API virtual ~IMenuBarBuilder();

    /**
     * @brief Returns the builded menu bar.
     */
    FWGUI_API virtual ::fwGui::container::fwMenuBar::sptr getMenuBar();

    /**
     * @brief Configure the layout before creation.
     */
    FWGUI_API virtual void initialize( ::fwRuntime::ConfigurationElement::sptr configuration);

    /**
     * @brief Instantiate layout with parent menuBar.
     * @pre LayoutManager must be initialized before.
     * @pre parent menuBar must be instanced.
     */
    FWGUI_API virtual void createMenuBar( ::fwGui::container::fwContainer::sptr parent ) = 0;

    /**
     * @brief Destroy local layout with sub containers.
     * @pre services using this sub containers must be stopped before.
     */
    FWGUI_API virtual void destroyMenuBar() = 0;

protected:

    /// MenuBar.
    ::fwGui::container::fwMenuBar::sptr m_menuBar;

};

} // namespace builder
} // namespace fwGui

#endif /*_FWGUI_BUILDER_IMENUBARBUILDER_HPP_*/


