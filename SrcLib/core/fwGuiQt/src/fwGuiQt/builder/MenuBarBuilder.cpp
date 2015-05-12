/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <QMainWindow>
#include <QMenuBar>

#include <fwGui/registry/macros.hpp>

#include "fwGuiQt/container/QtContainer.hpp"
#include "fwGuiQt/container/QtMenuBarContainer.hpp"
#include "fwGuiQt/builder/MenuBarBuilder.hpp"



fwGuiRegisterMacro(::fwGui::builder::MenuBarBuilder, ::fwGui::builder::IMenuBarBuilder::REGISTRY_KEY);


namespace fwGui
{
namespace builder
{

//-----------------------------------------------------------------------------

MenuBarBuilder::MenuBarBuilder(::fwGui::GuiBaseObject::Key key)
{
}

//-----------------------------------------------------------------------------

MenuBarBuilder::~MenuBarBuilder()
{
}

//-----------------------------------------------------------------------------

void MenuBarBuilder::createMenuBar( ::fwGui::container::fwContainer::sptr parent )
{
    m_parent = ::fwGuiQt::container::QtContainer::dynamicCast(parent);
    SLM_ASSERT("Sorry, the parent container is not a QtContainer", m_parent);
    QMainWindow *window = qobject_cast<QMainWindow*> ( m_parent->getQtContainer() );
    if ( !window )
    {
        window = qobject_cast<QMainWindow*> ( m_parent->getQtContainer()->parent() );
    }
    SLM_ASSERT("Sorry, the parent container must be a QMainWindow", window );
    if (window)
    {
        ::fwGuiQt::container::QtMenuBarContainer::sptr menuBarContainer =
            ::fwGuiQt::container::QtMenuBarContainer::New();
        QMenuBar *menuBar = new QMenuBar(0);
        menuBarContainer->setQtMenuBar(menuBar);
        window->setMenuBar( menuBar );
        m_menuBar = menuBarContainer;
    }
}

//-----------------------------------------------------------------------------

void MenuBarBuilder::destroyMenuBar()
{
    SLM_ASSERT("Sorry, Menu not initialized", m_menuBar);
    SLM_ASSERT("Sorry, the parent container is not a QtContainer", m_parent);
    QMainWindow *window = qobject_cast<QMainWindow*> ( m_parent->getQtContainer() );
    if ( !window )
    {
        window = qobject_cast<QMainWindow*> ( m_parent->getQtContainer()->parent() );
    }
    SLM_ASSERT("Sorry, the parent container must be a QMainWindow", window );
    if (window)
    {
        window->setMenuBar( NULL );
    }
    m_menuBar->destroyContainer();
}

//-----------------------------------------------------------------------------


} // namespace builder
} // namespace fwGui



