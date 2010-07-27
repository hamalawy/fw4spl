/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <QMainWindow>
#include <QToolBar>

#include <fwTools/ClassRegistrar.hpp>

#include "fwGuiQt/container/QtContainer.hpp"
#include "fwGuiQt/container/QtToolBarContainer.hpp"
#include "fwGuiQt/builder/ToolBarBuilder.hpp"



REGISTER_BINDING( ::fwGui::builder::IToolBarBuilder,
        ::fwGui::builder::ToolBarBuilder,
        ::fwGui::builder::IToolBarBuilder::RegistryKeyType,
        ::fwGui::builder::IToolBarBuilder::REGISTRY_KEY );


namespace fwGui
{
namespace builder
{

//-----------------------------------------------------------------------------

ToolBarBuilder::ToolBarBuilder()
{}

//-----------------------------------------------------------------------------

ToolBarBuilder::~ToolBarBuilder()
{}

//-----------------------------------------------------------------------------

void ToolBarBuilder::createToolBar( ::fwGui::fwContainer::sptr parent )
{
    m_parent = ::fwGuiQt::container::QtContainer::dynamicCast(parent);
    SLM_ASSERT("Sorry, the parent container is not a QtContainer", m_parent);
    QMainWindow *window = qobject_cast<QMainWindow*> ( m_parent->getQtContainer() ) ;
    if ( !window )
    {
        window = qobject_cast<QMainWindow*> ( m_parent->getQtContainer()->parent() ) ;
    }
    SLM_ASSERT("Sorry, the parent container must be a QMainWindow", window ) ;
    if (window)
    {
        ::fwGuiQt::container::QtToolBarContainer::sptr toolBarContainer = ::fwGuiQt::container::QtToolBarContainer::New();
        QToolBar *toolBar = new QToolBar(0);
        toolBar->setIconSize( QSize(m_toolBitmapSize.first, m_toolBitmapSize.second) );
        toolBar->setFloatable(false);
        toolBarContainer->setQtToolBar(toolBar);
        window->addToolBar( toolBar );
        m_toolBar = toolBarContainer;
    }
}

//-----------------------------------------------------------------------------

void ToolBarBuilder::destroyToolBar()
{
    SLM_ASSERT("Sorry, Tool not initialized", m_toolBar);
    SLM_ASSERT("Sorry, the parent container is not a QtContainer", m_parent);
    QMainWindow *window = qobject_cast<QMainWindow*> ( m_parent->getQtContainer() ) ;
    if ( !window )
    {
        window = qobject_cast<QMainWindow*> ( m_parent->getQtContainer()->parent() ) ;
    }
    SLM_ASSERT("Sorry, the parent container must be a QMainWindow", window ) ;
    if (window)
    {
        ::fwGuiQt::container::QtToolBarContainer::sptr qtToolBar = ::fwGuiQt::container::QtToolBarContainer::dynamicCast(m_toolBar);
        QToolBar * toolbar = qtToolBar->getQtToolBar();
        window->removeToolBar( toolbar );
    }
    m_toolBar->clean();
}

//-----------------------------------------------------------------------------


} // namespace builder
} // namespace fwGui



