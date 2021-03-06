/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QWidget>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QVBoxLayout>

#include <fwCore/base.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>

#include <fwTools/fwID.hpp>

#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>
#include <fwData/String.hpp>
#include <fwData/Boolean.hpp>

#include <fwComEd/ImageMsg.hpp>
#include <fwComEd/Dictionary.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IService.hpp>
#include <fwServices/op/Get.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include "uiImageQt/SliceListEditor.hpp"

namespace uiImage
{

fwServicesRegisterMacro( ::gui::editor::IEditor , ::uiImage::SliceListEditor , ::fwData::Image ) ;


SliceListEditor::SliceListEditor() throw()
{
    m_nbSlice = 1;
//    addNewHandledEvent( "SCAN_SHOW" );
}

//------------------------------------------------------------------------------

SliceListEditor::~SliceListEditor() throw()
{}

//------------------------------------------------------------------------------

void SliceListEditor::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->create();

    ::fwGuiQt::container::QtContainer::sptr qtContainer =  ::fwGuiQt::container::QtContainer::dynamicCast( this->getContainer() );
    QWidget* const container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);

    m_dropDownButton = new QPushButton( QObject::tr(">"), container );
    m_dropDownButton->setToolTip( QObject::tr("Manage slice visibility"));
    m_dropDownButton->setMaximumWidth(40);

    m_pDropDownMenu = new QMenu(container);
    QActionGroup * actionGroup = new QActionGroup(m_pDropDownMenu);

    m_oneSliceItem = new QAction(QObject::tr("One slice"), m_pDropDownMenu);
    m_threeSlicesItem = new QAction(QObject::tr("three slices"), m_pDropDownMenu);
//  m_obliqueSliceItem = new QAction(QObject::tr("Oblique slice"), m_pDropDownMenu);
    m_oneSliceItem->setCheckable(true);
    m_threeSlicesItem->setCheckable(true);

    actionGroup->addAction(m_oneSliceItem);
    actionGroup->addAction(m_threeSlicesItem);

    m_pDropDownMenu->addAction(m_oneSliceItem);
    m_pDropDownMenu->addAction(m_threeSlicesItem);
//    m_pDropDownMenu->addAction(m_obliqueSliceItem);
    m_dropDownButton->setMenu(m_pDropDownMenu);

    QObject::connect(m_oneSliceItem, SIGNAL(triggered(bool)), this, SLOT(onChangeSliceMode(bool)));
    QObject::connect(m_threeSlicesItem, SIGNAL(triggered(bool)), this, SLOT(onChangeSliceMode(bool)));
//    QObject::connect(m_obliqueSliceItem, SIGNAL(triggered(bool)), this, SLOT(onChangeSliceMode(bool)));

    QVBoxLayout * vLayout = new QVBoxLayout(container);
    vLayout->addWidget( m_dropDownButton);
    vLayout->setContentsMargins(0,0,0,0);

    m_oneSliceItem->setChecked(m_nbSlice == 1);
    m_threeSlicesItem->setChecked(m_nbSlice == 3);
//  m_obliqueSliceItem->setChecked(m_nbSlice == -1);

    container->setLayout( vLayout );
}

//------------------------------------------------------------------------------

void SliceListEditor::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    QObject::disconnect(m_oneSliceItem, SIGNAL(triggered(bool)), this, SLOT(onChangeSliceMode(bool)));
    QObject::disconnect(m_threeSlicesItem, SIGNAL(triggered(bool)), this, SLOT(onChangeSliceMode(bool)));

    this->getContainer()->clean();
    this->destroy();
}

//------------------------------------------------------------------------------

void SliceListEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    this->initialize();

    std::vector < Configuration > placeInSceneConfig = m_configuration->find("negatoAdaptor");
    SLM_ASSERT("Tag negatoAdaptor required!", !placeInSceneConfig.empty());
    SLM_ASSERT("UID attribute is missing", placeInSceneConfig.at(0)->hasAttribute("uid"));
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

void SliceListEditor::receiving( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::fwComEd::ImageMsg::csptr imageMsg = ::fwComEd::ImageMsg::dynamicConstCast( msg );
    if( imageMsg && imageMsg->hasEvent( "SCAN_SHOW"))
    {
        ::fwData::Object::csptr dataInfo = imageMsg->getDataInfo("SCAN_SHOW");
        SLM_ASSERT("dataInfo is missing", dataInfo);
        SLM_ASSERT("m_relatedServiceId is missing", dataInfo->getField( ::fwComEd::Dictionary::m_relatedServiceId ) );
        std::string servId = dataInfo->getField< ::fwData::String >(::fwComEd::Dictionary::m_relatedServiceId)->value();
        if( servId ==  m_adaptorUID )
        {
            ::fwData::Boolean::csptr isShowScan = ::fwData::Boolean::dynamicConstCast(dataInfo);
            m_dropDownButton->setEnabled(isShowScan->value());
        }
    }
}

//------------------------------------------------------------------------------

void SliceListEditor::info( std::ostream &_sstream )
{
}

//------------------------------------------------------------------------------

void SliceListEditor::onChangeSliceMode( bool checked )
{
    if(::fwTools::fwID::exist(m_adaptorUID))
    {
        ::fwServices::IService::sptr service = ::fwServices::get(m_adaptorUID);
        ::fwData::Image::sptr image = service->getObject< ::fwData::Image >();
        SLM_ASSERT("SliceListEditor adaptorUID " << m_adaptorUID <<" isn't an Adaptor on an Image?" , image);

        ::fwData::Integer::sptr dataInfo = ::fwData::Integer::New();

        if(m_oneSliceItem->isChecked())
        {
            dataInfo->value() = 1;
            m_nbSlice = 1;
        }
        else if(m_threeSlicesItem->isChecked())
        {
            dataInfo->value() = 3;
            m_nbSlice = 3;
        }
        else if(m_obliqueSliceItem->isChecked())
        {
            dataInfo->value() = -1;
            m_nbSlice = -1;
        }
        else
        {
            OSLM_FATAL("Unknown slice mode");
        }
        dataInfo->setField(::fwComEd::Dictionary::m_relatedServiceId ,  ::fwData::String::New( m_adaptorUID ) );
        ::fwComEd::ImageMsg::sptr imageMsg = ::fwComEd::ImageMsg::New();
        imageMsg->addEvent( "SLICE_MODE", dataInfo );
        ::fwServices::IEditionService::notify(this->getSptr(), image, imageMsg);
    }
    else
    {
        OSLM_TRACE("Service "<< m_adaptorUID << " is not yet present.");
    }
}

}

