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
#include <QHBoxLayout>
#include <QVBoxLayout>	


REGISTER_SERVICE( ::guiQt::aspect::IMenuBar , ::guiQt::aspect::DefaultMenuBar , ::fwTools::Object );

namespace guiQt
{

namespace aspect
{


//-----------------------------------------------------------------------------

DefaultMenuBar::DefaultMenuBar() throw()
{}

//-----------------------------------------------------------------------------

DefaultMenuBar::~DefaultMenuBar() throw()
{}

//-----------------------------------------------------------------------------

void DefaultMenuBar::configuring() throw( ::fwTools::Failed )
{
}

//-----------------------------------------------------------------------------
// TODO : spacer
void DefaultMenuBar::starting() throw( ::fwTools::Failed )
{
  QWidget *mainWidget = qApp->activeWindow();

  QMenuBar * menuBar = new QMenuBar(mainWidget);
 // menuBar->resize(950,30);
  
//   QString *string = new QString("TestMenuBar");
  // menuBar->setObjectName(this->getUUID().c_str());

  menuBar->show();  //sinon ajout menu incorrete
  
     QLayout *layout;// = new QHBoxLayout();
     
     if(mainWidget->layout()!=0)
        std::cout<<"=====> Layout OK \n";
     else
       std::cout<<"=====> Layout EMPTY \n";
     
     layout = mainWidget->layout();
     
    layout->setMenuBar(menuBar);

   // (mainWidget->layout())->addWidget(menuBar);
 //   (mainWidget->layout())->setMenuBar(menuBar);



}

//-----------------------------------------------------------------------------

void DefaultMenuBar::stopping() throw( ::fwTools::Failed )
{
   
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::updating() throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : an DefaultMenuBar service does not be updated." );
}

//-----------------------------------------------------------------------------

void DefaultMenuBar::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("ACH : an DefaultMenuBar service does not received a message." );
}

//-----------------------------------------------------------------------------

}

}
