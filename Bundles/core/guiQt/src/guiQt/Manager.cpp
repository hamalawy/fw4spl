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
	
}


void Manager::initialize()
{
  int argc = 1;
  char* argv[1] = {"fw4splQt"};
  
  QApplication app( argc,  argv);
  
  QWidget widget;
  widget.resize(950, 750);
  widget.show();
   
  app.setActiveWindow(&widget);

  ::fwServices::OSR::initializeRootObject();
   
  app.exec();
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
