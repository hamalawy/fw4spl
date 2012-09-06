/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <QDockWidget>
#include <QWidget>
#include <QList>
#include <QLayout>
#include <QMetaObject>

#include "fwGuiQt/container/QtContainer.hpp"

namespace fwGuiQt
{
namespace container
{

//-----------------------------------------------------------------------------

QtContainer::QtContainer() throw() : m_container(0)
{}

//-----------------------------------------------------------------------------

QtContainer::~QtContainer() throw()
{
    SLM_ASSERT( "Error during destruction : The qt container included in this class is still allocated, please call destroyContainer() before.", m_container == 0 );
}

//-----------------------------------------------------------------------------

void QtContainer::clean()
{
    SLM_ASSERT("Sorry, QWidget not yet initialized, cleaning impossible", m_container);

    QObjectList list = m_container->children();
    QWidget *w = NULL;

    Q_FOREACH (QObject *o, list)
    {
        w = qobject_cast<QWidget*>(o);
        if (w)
        {
            w->hide();
            w->setParent(0);
            w->deleteLater();
        }
    }
    m_container->adjustSize();
    if (m_container->layout())
    {
        QLayout* layout = m_container->layout();
        layout->deleteLater();
        m_container->setLayout(0);
        // FIXME destroy layout properly before to set another
        delete layout;
    }
}

//-----------------------------------------------------------------------------

void QtContainer::destroyContainer()
{
    SLM_ASSERT("Sorry, QWidget not yet initialized", m_container);

    QList<QWidget *> allWidgets = m_container->findChildren<QWidget *>();
    OSLM_ASSERT("Container must be empty ( " << allWidgets.count() << " children).", allWidgets.empty());

    m_container->deleteLater();
    m_container = NULL;
}

//-----------------------------------------------------------------------------

void QtContainer::setQtContainer(QWidget *container)
{
    m_container = container;
}

//-----------------------------------------------------------------------------

QWidget* QtContainer::getQtContainer()
{
    return m_container;
}

//-----------------------------------------------------------------------------

bool QtContainer::isShownOnScreen()
{
    SLM_ASSERT("Sorry, QtContainer not yet initialized, cleaning impossible", m_container);
    return m_container->isVisible();
}

//-----------------------------------------------------------------------------

void QtContainer::setVisible(bool isVisible)
{
    SLM_ASSERT("Sorry, QtContainer not yet initialized, cleaning impossible", m_container);
    QWidget* parent = m_container->parentWidget();
    QDockWidget* dock = qobject_cast<QDockWidget*>(parent);
    if(parent)
    {
        if(dock)
        {
            dock->setVisible(isVisible);
        }
    }
    m_container->setVisible(isVisible);
}

//-----------------------------------------------------------------------------

} // namespace container
} // namespace fwGuiQt
