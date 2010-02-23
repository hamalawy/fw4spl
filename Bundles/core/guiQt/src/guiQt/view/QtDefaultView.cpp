/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwData/Object.hpp>

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
//     QWidget *mainWidget = qApp->activeWindow();
//     
//     QFrame *viewFrame = new QFrame(mainWidget);
//     QSize s(750, 750);
// 
//     viewFrame->setMinimumSize(s);
//     viewFrame->show();

}

}
}

