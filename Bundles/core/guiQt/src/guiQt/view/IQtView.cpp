/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include <QApplication>
#include <QWidget>
#include <QFrame>
#include "guiQt/view/IQtView.hpp"
#include "guiQt/Manager.hpp"

namespace guiQt
{
namespace view
{


IQtView::IQtView() throw(): m_minWidth(-1), m_minHeight(-1)
{
}

//-----------------------------------------------------------------------------

IQtView::~IQtView() throw()
{
}

//-----------------------------------------------------------------------------

void IQtView::configuring() throw( ::fwTools::Failed )
{
   
}

//-----------------------------------------------------------------------------

void IQtView::info(std::ostream &_sstream )
{
    _sstream << "GUI View with ID = " <<  m_guiContainerId;
}

//-----------------------------------------------------------------------------

void IQtView::starting() throw(::fwTools::Failed)
{
    QWidget *mainWidget = qApp->activeWindow();
    
    QFrame *viewFrame = new QFrame(mainWidget);
    QSize s(750, 750);

    viewFrame->setMinimumSize(s);
    viewFrame->resize(500,500);
    viewFrame->show();

}
//-----------------------------------------------------------------------------

void IQtView::updating() throw(::fwTools::Failed)
{
    SLM_TRACE("IView::updating");
}

//-----------------------------------------------------------------------------

void IQtView::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("An IView service does not received a message." );
}
//-----------------------------------------------------------------------------

void IQtView::stopping() throw(::fwTools::Failed)
{
   
}

//-----------------------------------------------------------------------------

int IQtView::getGuiContainerId()
{
    return m_guiContainerId ;
}

//-----------------------------------------------------------------------------

void IQtView::getWxContainer() const
{
}

//-----------------------------------------------------------------------------

}

}

