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

#include <QLabel>

#include <QPixmap>


#include "guiQt/view/DefaultView.hpp"

namespace guiQt
{
namespace view
{

REGISTER_SERVICE( ::guiQt::view::IView , ::guiQt::view::DefaultView , ::fwTools::Object ) ;

DefaultView::DefaultView() throw()
{}

DefaultView::~DefaultView() throw()
{}

void DefaultView::configuring() throw( ::fwTools::Failed )
{
  
}

void DefaultView::starting() throw( ::fwTools::Failed )
{
    QWidget *mainWidget = qApp->activeWindow();
    
    QFrame *viewFrame = new QFrame(mainWidget);
    
    //QPushButton *testButton = new QPushButton( "Quit",viewFrame );
  //  testButton->show();
    
    QSize s(750, 750);

    viewFrame->setMinimumSize(s);
    viewFrame->resize(500,500);
   
    // Test view
   // QPixmap image("/home/hchekatt/tux.png");
    //QLabel* tux = new QLabel(viewFrame,0); // this étant un pointeur vers le parent du label.
    //tux->setPixmap(image);
    
    
    viewFrame->show();

}

}
}

