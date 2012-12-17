/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

#include <fwCore/base.hpp>

#include <fwData/Image.hpp>

#include <fwMath/IntrasecTypes.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IService.hpp>

#include <fwComEd/InteractionMsg.hpp>
#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>
#include <fwComEd/helper/Image.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include "uiImageQt/ImageInfo.hpp"


namespace uiImage
{

fwServicesRegisterMacro( ::gui::editor::IEditor , ::uiImage::ImageInfo , ::fwData::Image ) ;


ImageInfo::ImageInfo() throw()
{
//    addNewHandledEvent(::fwComEd::InteractionMsg::MOUSE_MOVE);
}

//------------------------------------------------------------------------------

ImageInfo::~ImageInfo() throw()
{}

//------------------------------------------------------------------------------

void ImageInfo::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->::fwGui::IGuiContainerSrv::create();
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

    ::fwGuiQt::container::QtContainer::sptr qtContainer =  ::fwGuiQt::container::QtContainer::dynamicCast( this->getContainer() );
    QWidget* const container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);

    QHBoxLayout* hLayout = new QHBoxLayout();

    QLabel* staticText = new QLabel( tr("intensity:"), container);
    hLayout->addWidget( staticText, 0, Qt::AlignVCenter );

    m_valueText = new QLineEdit( container );
    m_valueText->setReadOnly(true);
    hLayout->addWidget( m_valueText, 1, Qt::AlignVCenter );

    container->setLayout( hLayout );
}

//------------------------------------------------------------------------------

void ImageInfo::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    this->getContainer()->clean();
    this->::fwGui::IGuiContainerSrv::destroy();
}

//------------------------------------------------------------------------------

void ImageInfo::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->::fwGui::IGuiContainerSrv::initialize();
}

//------------------------------------------------------------------------------

void ImageInfo::updating() throw(::fwTools::Failed)
{}

//------------------------------------------------------------------------------

void ImageInfo::swapping() throw(::fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    bool imageIsValid = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity( image );
    m_valueText->setEnabled(imageIsValid);
}

//------------------------------------------------------------------------------

void ImageInfo::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::fwComEd::InteractionMsg::csptr interactionMsg = ::fwComEd::InteractionMsg::dynamicConstCast(_msg);

    if (interactionMsg)
    {
        ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
        bool imageIsValid = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity( image );
        m_valueText->setEnabled(imageIsValid);
        if (imageIsValid)
        {
            ::fwData::Point::csptr point = interactionMsg->getEventPoint();
            SLM_ASSERT("Sorry, the object is null", point);
            if(point)
            {
                fwVec3d  pointCoord = point->getCoord();
                ::fwComEd::helper::Image imageHelper(image);

                std::string intensity = imageHelper.getPixelAsString(pointCoord[0], pointCoord[1], pointCoord[2] );;
                m_valueText->setText(QString::fromStdString(intensity));
            }
        }
    }
}

//------------------------------------------------------------------------------

void ImageInfo::info( std::ostream &_sstream )
{
    _sstream << "Image Info Editor";
}

//------------------------------------------------------------------------------
}

