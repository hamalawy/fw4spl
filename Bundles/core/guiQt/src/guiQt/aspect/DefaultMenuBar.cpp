/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <fwTools/UUID.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/bundle/runtime.hpp>

#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

//#include "guiQt/action/IAction.hpp"
#include "guiQt/aspect/IMenu.hpp"
#include "guiQt/aspect/IAspect.hpp"
#include "guiQt/aspect/DefaultMenuBar.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>


REGISTER_SERVICE( ::guiQt::aspect::IMenuBar , ::guiQt::aspect::DefaultMenuBar , ::fwTools::Object );

namespace guiQt
{

namespace aspect
{


//-----------------------------------------------------------------------------

DefaultMenuBar::DefaultMenuBar() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

DefaultMenuBar::~DefaultMenuBar() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::configuring() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------
// TODO : spacer
void DefaultMenuBar::starting() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    QWidget *mainWidget = qApp->activeWindow();
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(mainWidget);

    QMenuBar * menuBar = new QMenuBar(mainWindow);
    menuBar->show();  //sinon ajout menu incorrete

    mainWindow->setMenuBar(menuBar);
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::stopping() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    SLM_FATAL("ACH : an DefaultMenuBar service does not be updated." );
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    SLM_FATAL("ACH : an DefaultMenuBar service does not received a message." );
}

//-----------------------------------------------------------------------------

}

}
