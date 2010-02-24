/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwData/Object.hpp>
#include <QPushButton>

#include <QApplication>
#include <QFrame>
#include "guiQt/view/QtDefaultView.hpp"

namespace guiQt
{
namespace view
{

REGISTER_SERVICE( ::guiQt::view::IQtView , ::guiQt::view::QtDefaultView , ::fwTools::Object ) ;

QtDefaultView::QtDefaultView() throw()
{}

QtDefaultView::~QtDefaultView() throw()
{}

void QtDefaultView::configuring() throw( ::fwTools::Failed )
{
  
}

void QtDefaultView::starting() throw( ::fwTools::Failed )
{
    std::cout<<" \n\n---> IQtView::Starting() \n\n";  

 // guiQt::view::IQtView::configuring();
     std::cout<<" \n\n---> AFTER conf \n\n";  
 
    QWidget *mainWidget = qApp->activeWindow();
    
    QFrame *viewFrame = new QFrame(mainWidget);
    
    QPushButton *testButton = new QPushButton( "Quit",viewFrame );
   // QObject::connect( testButton, SIGNAL(clicked()), qApp, SLOT(quit()) );
    testButton->show();
    QSize s(750, 750);

    viewFrame->setMinimumSize(s);
    viewFrame->resize(500,500);
    viewFrame->show();

}

}
}

