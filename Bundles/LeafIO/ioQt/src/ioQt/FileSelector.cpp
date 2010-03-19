/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <boost/filesystem/operations.hpp>

#include <fwCore/base.hpp>

#include <fwTools/Object.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwComEd/LocationMsg.hpp>
//#include <fwComEd/SingleFileMsg.hpp>

#include "ioQt/FileSelector.hpp"

#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QApplication>
#include <QWidget>
#include <QFrame>
  
#include <fwData/location/SingleFile.hpp>  
#include <iostream>

namespace ioQt
{

//------------------------------------------------------------------------------

// Register a new reader of ::data::Image
REGISTER_SERVICE(::guiQt::editor::IEditor, ::ioQt::FileSelector, ::fwData::location::SingleFile);

//------------------------------------------------------------------------------

FileSelector::FileSelector() throw() :
    m_fsImgPath("")
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

FileSelector::~FileSelector() throw()
{
    SLM_TRACE_FUNC();
    // This method does nothing
}

//------------------------------------------------------------------------------

void FileSelector::configuring() throw ( ::fwTools::Failed )
{
     std::cout<<" configuring() \n";
}


//------------------------------------------------------------------------------

void FileSelector::starting() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    // This method does nothing
}

//------------------------------------------------------------------------------

void FileSelector::stopping() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    // This method does nothing
}

//------------------------------------------------------------------------------

void FileSelector::info( std::ostream &_sstream )
{
    _sstream << "ImageReaderService::info (ToDo :))";
}

//------------------------------------------------------------------------------

void FileSelector::updating() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    
  ::fwData::location::SingleFile::sptr file = this->getObject < ::fwData::location::SingleFile >();

  
 // QWidget *mainWidget = qApp->activeWindow();
    
  QString fileName = QFileDialog::getOpenFileName(0,QObject::tr("Open File"), QDir::currentPath());
  std::cout<<"PATH_FILE : "<<fileName.toStdString()<<"\n";
  

  
  file->setPath(fileName.toStdString());
  // Creation du message
  ::fwComEd::LocationMsg::NewSptr msg;
  msg->addEvent( ::fwComEd::LocationMsg::LOCATION_IS_MODIFIED );
  ::fwServices::IEditionService::notify( this->getSptr(), file, msg);
}

//------------------------------------------------------------------------------

void FileSelector::notificationOfDBUpdate()
{
   
}

//------------------------------------------------------------------------------


} // namespace ioVtk
