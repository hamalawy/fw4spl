/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QMenuBar>

#include "fwGuiQt/container/QtMenuBarContainer.hpp"

namespace fwGuiQt
{
namespace container
{

//-----------------------------------------------------------------------------

QtMenuBarContainer::QtMenuBarContainer(::fwGui::GuiBaseObject::Key key) throw() : m_menuBar(0)
{}

//-----------------------------------------------------------------------------

QtMenuBarContainer::~QtMenuBarContainer() throw()
{}

//-----------------------------------------------------------------------------

void QtMenuBarContainer::clean()
{
    SLM_ASSERT("Sorry, Qt MenuBar not yet initialized, cleaning impossible", m_menuBar);

    m_menuBar->clear();

    QList<QMenu *> list = m_menuBar->findChildren<QMenu *>();
    Q_FOREACH (QMenu *m, list)
    {
        m->hide();
        delete m;
    }
}

//-----------------------------------------------------------------------------

void QtMenuBarContainer::destroyContainer()
{
    SLM_ASSERT("Sorry, Qt MenuBar not yet initialized", m_menuBar);

    QList<QMenu *> allMenu = m_menuBar->findChildren<QMenu *>();
    OSLM_ASSERT("MenuBar container must be empty ( " << allMenu.count() << " menus).", allMenu.empty());

    m_menuBar->hide();
    delete m_menuBar;
}

//-----------------------------------------------------------------------------

void QtMenuBarContainer::setQtMenuBar(QMenuBar *menuBar)
{
    this->m_menuBar = menuBar;
}

//-----------------------------------------------------------------------------

QMenuBar* QtMenuBarContainer::getQtMenuBar()
{
    return this->m_menuBar;
}

//-----------------------------------------------------------------------------

} // namespace container
} // namespace fwGuiQt
