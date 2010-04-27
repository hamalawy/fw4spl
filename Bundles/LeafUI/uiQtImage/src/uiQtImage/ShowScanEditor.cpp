/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>

#include <fwTools/Object.hpp>
#include <fwTools/UUID.hpp>

#include <fwData/Image.hpp>
#include <fwData/Boolean.hpp>
#include <fwData/String.hpp>

#include <fwComEd/ImageMsg.hpp>
#include <fwComEd/Dictionary.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IService.hpp>
#include <fwServices/op/Get.hpp>

#include "uiQtImage/ShowScanEditor.hpp"

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QImage>
#include <QHBoxLayout>

namespace uiImage
{

REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiImage::ShowScanEditor , ::fwData::Image ) ;


ShowScanEditor::ShowScanEditor() throw(): m_scanAreShown(true), m_buttonWidth(50)
{
}

//------------------------------------------------------------------------------

ShowScanEditor::~ShowScanEditor() throw()
{}

//------------------------------------------------------------------------------

void ShowScanEditor::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::starting();

   // QWidget *mainWidget = m_globalUIDToQtContainer.find(this->getUUID())->second;
    QWidget *widget = new QWidget(m_container);
    
    QHBoxLayout *layout = new  QHBoxLayout();

    icon = QIcon();
    QString str = QObject::tr("Bundles/uiQtImage_") + QObject::tr(UIQTIMAGE_VER) + QObject::tr("/sliceShow.png");

    m_imageShowScan = QImage(str);
    m_imageHideScan = QImage( QObject::tr("Bundles/uiQtImage_") + QObject::tr(UIQTIMAGE_VER) + QObject::tr("/sliceHide.png"));

    m_showScanButton = new QPushButton(widget);
    m_showScanButton->setFixedWidth(m_buttonWidth);
//    m_showScanButton->setFixedWidth(m_buttonWidth);

    icon.addPixmap(QPixmap::fromImage(m_imageShowScan), QIcon::Normal);
    m_showScanButton->setIcon(icon);
    m_showScanButton->setIconSize(QSize(m_showScanButton->width(), m_showScanButton->height()));
    QObject::connect(m_showScanButton, SIGNAL(clicked()), this, SLOT(changeScanMode())); 
    
    m_showScanButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    
    layout->addWidget( widget);
    m_container->setLayout(layout);
}


void ShowScanEditor::changeScanMode()
{
  if(::fwTools::UUID::exist(m_adaptorUID, ::fwTools::UUID::SIMPLE ))
  {
      if(m_scanAreShown)
      {
	m_scanAreShown=false;
	icon.addPixmap(QPixmap::fromImage(m_imageHideScan), QIcon::Normal);
	m_showScanButton->setIcon(icon);
      }
      else
      {
	m_scanAreShown=true;
	icon.addPixmap(QPixmap::fromImage(m_imageShowScan), QIcon::Normal);
	m_showScanButton->setIcon(icon);
      }
	::fwServices::IService::sptr service = ::fwServices::get(m_adaptorUID);
        ::fwData::Image::sptr image = service->getObject< ::fwData::Image >();
        SLM_ASSERT("ShowScanEditor adaptorUID " << m_adaptorUID <<" isn't an Adaptor on an Image?" , image);

        ::fwData::Boolean::NewSptr dataInfo;
        dataInfo->value() = m_scanAreShown;

        dataInfo->setFieldSingleElement(::fwComEd::Dictionary::m_relatedServiceId ,  ::fwData::String::NewSptr( m_adaptorUID ) );
        ::fwComEd::ImageMsg::NewSptr imageMsg;
        imageMsg->addEvent( "SCAN_SHOW", dataInfo );
        ::fwServices::IEditionService::notify(this->getSptr(), image, imageMsg);
  }
}

//------------------------------------------------------------------------------

void ShowScanEditor::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
   // m_container->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &ShowScanEditor::onChangeScanMode, this, m_showScanButton->GetId());

    ::guiQt::editor::IEditor::stopping();
}

//------------------------------------------------------------------------------

void ShowScanEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::configuring();

    std::vector < Configuration > placeInSceneConfig = m_configuration->find("negatoAdaptor");
    SLM_ASSERT("Tag negatoAdaptor required!", !placeInSceneConfig.empty());
    SLM_ASSERT("UID attribute is missing", placeInSceneConfig.at(0)->hasAttribute("uid"));
    m_adaptorUID = placeInSceneConfig.at(0)->getAttributeValue("uid");

}

//------------------------------------------------------------------------------

void ShowScanEditor::updating() throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void ShowScanEditor::swapping() throw(::fwTools::Failed)
{
}
//------------------------------------------------------------------------------

void ShowScanEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void ShowScanEditor::info( std::ostream &_sstream )
{
}

}

