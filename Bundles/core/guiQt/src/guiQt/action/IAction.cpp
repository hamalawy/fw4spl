/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/assign/list_of.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>

#include "guiQt/action/IAction.hpp"
#include "guiQt/action/Action.hpp"
#include "guiQt/Manager.hpp"

#include <iostream>

#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QString>

namespace guiQt
{
namespace action
{

IAction::IAction() throw() : ::fwServices::IService()
{
    m_action = new ::guiQt::action::Action("", false, false, false, true);
}

//-----------------------------------------------------------------------------

IAction::~IAction() throw()
{
}

//-----------------------------------------------------------------------------

void IAction::configuring() throw( ::fwTools::Failed )
{
    SLM_TRACE("configuring action") ;
    m_action->configuring(m_configuration);
}

//-----------------------------------------------------------------------------

void IAction::info(std::ostream &_sstream )
{
    m_action->info(_sstream);
}

//-----------------------------------------------------------------------------

void IAction::starting() throw(::fwTools::Failed)
{
    m_action->starting();
}

//-----------------------------------------------------------------------------

void IAction::stopping() throw(::fwTools::Failed)
{
    m_action->stopping();

}

bool IAction::isEnable()
{
    return m_action->isEnable();
}

//-----------------------------------------------------------------------------

void IAction::updating() throw(::fwTools::Failed)
{
    m_action->updating();
}

//-----------------------------------------------------------------------------

void IAction::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    SLM_FATAL("an IAction service does not received a message." );
}

//-----------------------------------------------------------------------------

void IAction::setMenuName(std::string _menuName)
{
    SLM_TRACE_FUNC();
    m_action->setMenuName(_menuName);
}

//-----------------------------------------------------------------------------

std::string IAction::getMenuName()
{
    SLM_TRACE_FUNC();
    return m_action->getMenuName();
}

//-----------------------------------------------------------------------------

int IAction::getId()
{
    SLM_TRACE_FUNC();
    return m_action->getId();
}

//-----------------------------------------------------------------------------

std::string IAction::getNameInMenu()
{
    SLM_TRACE_FUNC();
    return m_action->getNameInMenu();
}

//-----------------------------------------------------------------------------

QAction* IAction::getMenuItem()
{
    SLM_TRACE_FUNC();
    return m_action->getMenuItem();
}

}
}
