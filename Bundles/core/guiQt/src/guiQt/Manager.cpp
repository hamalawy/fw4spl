/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <cassert>
#include <vector>

#include <fwCore/base.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/bundle/runtime.hpp>
#include <fwRuntime/profile/Profile.hpp>

#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>

#include <iostream>

#include "guiQt/Manager.hpp"

#include <QApplication>
#include <QPushButton>
#include <QMainWindow>
#include <QFrame>
#include <QMenuBar>
#include <QDialog>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>

namespace guiQt
{
/**
 * @brief Forward menu event received to the manager in order to update/execute the appropriate service
 * @author IRCAD (Research and Development Team).
 */

/**
 * @brief Application top window
 */


::guiQt::Manager::sptr Manager::m_instance;

//-----------------------------------------------------------------------------
Manager::Manager() throw()
{

}

//-----------------------------------------------------------------------------

Manager::~Manager() throw()
{
    SLM_TRACE_FUNC();
}

void Manager::initialize()
{
  int argc = 1;
  //char* argv[1] = {"fw4splQt"};
  char** argv = NULL;
  int w,h;

//  QApplication app( argc,  argv);

//  QObject::connect(&app, SIGNAL(aboutToQuit()), qApp, SLOT(exit()));
 // QObject::connect(&app, SIGNAL(lastWindowClosed()), this, SLOT(windowClosed()));

//       QObject::connect(m_sliceType, SIGNAL(currentIndexChanged(int)), this, SLOT(sliceTypeChange()));
  /*
  QWidget *widget = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout();
  widget->setLayout(layout);
  widget->resize(screen.width(), screen.height());
  widget->show();
  app.setActiveWindow(widget);*/

  QWidget* mainWindow = new QMainWindow();


  QDesktopWidget *desk = QApplication::desktop();
  QRect screen = desk->screenGeometry(mainWindow);
  mainWindow->resize(screen.width(), screen.height());

  // TESt
  //mainWindow->setAttribute(Qt::WA_QuitOnClose);//Qt::WA_DeleteOnClose
  //app.setActiveWindow(mainWindow);
   qApp->setActiveWindow(mainWindow);

  ::fwServices::OSR::initializeRootObject();

  // a voir plus loin
  mainWindow->show();
 // app.exec();
  qApp->exec();

}


const ::guiQt::Manager::sptr Manager::getDefault() throw()
{
    if(m_instance.get() == 0)
    {
        m_instance = ::guiQt::Manager::NewSptr();
    }
    return m_instance;
}

} // namespace gui
