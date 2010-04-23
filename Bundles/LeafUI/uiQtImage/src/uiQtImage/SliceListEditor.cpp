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
#include <fwData/Integer.hpp>
#include <fwData/String.hpp>
#include <fwData/Boolean.hpp>

#include <fwComEd/ImageMsg.hpp>
#include <fwComEd/Dictionary.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IService.hpp>
#include <fwServices/op/Get.hpp>


#include "uiQtImage/SliceListEditor.hpp"

#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>


namespace uiImage
{

REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiImage::SliceListEditor , ::fwData::Image ) ;


SliceListEditor::SliceListEditor() throw()
{
    m_nbSlice = 1;
    m_buttonWidth = 25;
    addNewHandledEvent( "SCAN_SHOW" );
}

//------------------------------------------------------------------------------

SliceListEditor::~SliceListEditor() throw()
{}

//------------------------------------------------------------------------------

void SliceListEditor::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::starting();
    
    QWidget *mainWidget = m_globalUIDToQtContainer.find(this->getUUID())->second;

    m_widget = new QWidget(mainWidget);
    QHBoxLayout *layout = new  QHBoxLayout();
    
//     m_menuBar = new QMenuBar(widget);
//     m_menuBar->setFixedSize(25, 30);
    
//    m_menu = new QMenu(QObject::tr(">"), m_menuBar);
    
//     m_sliceGroup = new QActionGroup(m_menu);
//     m_sliceGroup->setExclusive(true);
//    
//     m_oneSliceItem = new QAction(QObject::tr(" One slice "), m_sliceGroup);
//     m_threeSlicesItem = new QAction(QObject::tr(" Three slice "), m_sliceGroup);
//   
//     m_oneSliceItem->setCheckable(true);
//     m_threeSlicesItem->setCheckable(true);
// 
//     m_menu->addActions(m_sliceGroup->actions());
//     m_menuBar->addMenu(m_menu);
    
  //  widget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
  
   m_button = new QPushButton(QObject::tr(">"),m_widget);
   m_button->setFixedWidth(m_buttonWidth);
  
   m_button->setContextMenuPolicy(Qt::CustomContextMenu);
   QObject::connect(m_button, SIGNAL(clicked()), this, SLOT(createPopUpMenu())); 
  
  
    layout->addWidget( m_widget);
    mainWidget->setLayout(layout);
   
  //QPushButton *b = new QPushButton(QObject::tr(" One slice "), mainWidget);
    
}

void SliceListEditor::createPopUpMenu()
{
  
  std::cout<<" SLOT() \n";

  m_menu = new QMenu(QObject::tr(">"), m_widget);
  
  m_sliceGroup = new QActionGroup(m_menu);
  m_sliceGroup->setExclusive(true);
    
  m_oneSliceItem = new QAction(QObject::tr(" One slice "), m_sliceGroup);
  m_threeSlicesItem = new QAction(QObject::tr(" Three slice "), m_sliceGroup);
  
  m_oneSliceItem->setCheckable(true);
  m_threeSlicesItem->setCheckable(true);
  
  m_menu->addActions(m_sliceGroup->actions());
    
  m_menu->move(m_widget->mapToGlobal(QPoint(m_button->x()+m_buttonWidth, m_button->y())));
  m_menu->show();
}

//------------------------------------------------------------------------------

void SliceListEditor::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::stopping();
}

//------------------------------------------------------------------------------

void SliceListEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::configuring();

    std::vector < Configuration > placeInSceneConfig = m_configuration->find("negatoAdaptor");
  //  SLM_ASSERT("Tag negatoAdaptor required!", !placeInSceneConfig.empty());
   // SLM_ASSERT("UID attribute is missing", placeInSceneConfig.at(0)->hasAttribute("uid"));
    m_adaptorUID = placeInSceneConfig.at(0)->getAttributeValue("uid");

    if (placeInSceneConfig.at(0)->hasAttribute("slices"))
    {
        std::string value(placeInSceneConfig.at(0)->getAttributeValue("slices"));
        m_nbSlice = ::boost::lexical_cast<int >(value.c_str());
    }
    
}

//------------------------------------------------------------------------------

void SliceListEditor::updating() throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void SliceListEditor::swapping() throw(::fwTools::Failed)
{

}
//------------------------------------------------------------------------------

void SliceListEditor::updating( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::fwComEd::ImageMsg::csptr imageMsg = ::fwComEd::ImageMsg::dynamicConstCast( msg );
    if( imageMsg && imageMsg->hasEvent( "SCAN_SHOW"))
    {
        ::fwData::Object::csptr dataInfo = imageMsg->getDataInfo("SCAN_SHOW");
        SLM_ASSERT("dataInfo is missing", dataInfo);
        SLM_ASSERT("m_relatedServiceId is missing", dataInfo->getFieldSize( ::fwComEd::Dictionary::m_relatedServiceId ) );
        std::string servId = dataInfo->getFieldSingleElement< ::fwData::String >(::fwComEd::Dictionary::m_relatedServiceId)->value();
        if( servId ==  m_adaptorUID )
        {
            ::fwData::Boolean::csptr isShowScan = ::fwData::Boolean::dynamicConstCast(dataInfo);
         //   m_dropDownButton->Enable(isShowScan->value());
        }
    }
    
}

//------------------------------------------------------------------------------

void SliceListEditor::info( std::ostream &_sstream )
{
}




}

