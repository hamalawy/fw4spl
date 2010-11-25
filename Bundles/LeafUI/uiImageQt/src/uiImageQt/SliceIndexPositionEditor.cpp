/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QWidget>
#include <QVBoxLayout>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>

#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>
#include <fwData/Composite.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwCore/base.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>

#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>
#include <fwComEd/ImageMsg.hpp>
#include <fwComEd/Dictionary.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include "uiImageQt/SliceIndexPositionEditor.hpp"

namespace uiImage
{

REGISTER_SERVICE( ::gui::editor::IEditor , ::uiImage::SliceIndexPositionEditor , ::fwData::Image ) ;

const std::string* SliceIndexPositionEditor::SLICE_INDEX_FIELDID[ 3 ] =
{
    &fwComEd::Dictionary::m_sagittalSliceIndexId,
    &fwComEd::Dictionary::m_frontalSliceIndexId,
    &fwComEd::Dictionary::m_axialSliceIndexId
};

//------------------------------------------------------------------------------

SliceIndexPositionEditor::SliceIndexPositionEditor() throw()
{
    addNewHandledEvent( ::fwComEd::ImageMsg::CHANGE_SLICE_TYPE );
    addNewHandledEvent( ::fwComEd::ImageMsg::SLICE_INDEX );
    addNewHandledEvent( ::fwComEd::ImageMsg::BUFFER );
}

//------------------------------------------------------------------------------

SliceIndexPositionEditor::~SliceIndexPositionEditor() throw()
{}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::starting() throw(::fwTools::Failed)
{
    this->create();

    ::fwGuiQt::container::QtContainer::sptr qtContainer =  ::fwGuiQt::container::QtContainer::dynamicCast( this->getContainer() );
    QWidget * const container = qtContainer->getQtContainer();
    assert( container ) ;

    QVBoxLayout* layout = new QVBoxLayout( container );

    m_sliceSelectorPanel = new ::fwGuiQt::SliceSelector( container );
    m_sliceSelectorPanel->setEnable(false);

    ::fwGuiQt::SliceSelector::ChangeIndexCallback changeIndexCallback;
    changeIndexCallback = ::boost::bind( &::uiImage::SliceIndexPositionEditor::sliceIndexNotification, this, _1);
    m_sliceSelectorPanel->setChangeIndexCallback(changeIndexCallback);

    ::fwGuiQt::SliceSelector::ChangeIndexCallback changeTypeCallback;
    changeTypeCallback = ::boost::bind( &::uiImage::SliceIndexPositionEditor::sliceTypeNotification, this, _1);
    m_sliceSelectorPanel->setChangeTypeCallback(changeTypeCallback);
    layout->addWidget( m_sliceSelectorPanel );
    layout->setContentsMargins(0,0,0,0);

    this->updateImageInfos(this->getObject< ::fwData::Image >());
    this->updateSliceType(m_orientation);


    container->setLayout( layout );

    this->updating();
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::stopping() throw(::fwTools::Failed)
{
    if(m_sliceSelectorPanel)
    {
        delete m_sliceSelectorPanel;
        m_sliceSelectorPanel = 0;
    }

    this->getContainer()->clean();
    this->destroy();
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    this->initialize();

    if(m_configuration->hasAttribute("sliceIndex"))
    {
         std::string  orientation = m_configuration->getAttributeValue("sliceIndex");
         if(orientation == "axial" )
         {
             m_orientation = Z_AXIS;
         }
         else if(orientation == "frontal" )
         {
             m_orientation = Y_AXIS;
         }
         else if(orientation == "sagittal" )
         {
             m_orientation = X_AXIS;
         }
    }
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::updating() throw(::fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    bool imageIsValid = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity( image );
    m_sliceSelectorPanel->setEnable(imageIsValid);
    this->updateImageInfos(image);
    this->updateSliceIndex();
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::swapping() throw(::fwTools::Failed)
{
    this->updating();
}
//------------------------------------------------------------------------------

void SliceIndexPositionEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    ::fwComEd::ImageMsg::csptr imageMessage = fwComEd::ImageMsg::dynamicConstCast( _msg );

    if ( imageMessage )
    {
        if ( imageMessage->hasEvent( fwComEd::ImageMsg::BUFFER ) )
        {
            this->updating();
        }
        if ( imageMessage->hasEvent( fwComEd::ImageMsg::SLICE_INDEX ) )
        {
            imageMessage->getSliceIndex( m_axialIndex, m_frontalIndex, m_sagittalIndex);
            this->updateSliceIndex();
        }
        if ( imageMessage->hasEvent( fwComEd::ImageMsg::CHANGE_SLICE_TYPE ) )
        {
            ::fwData::Object::csptr cObjInfo = imageMessage->getDataInfo( ::fwComEd::ImageMsg::CHANGE_SLICE_TYPE );
            ::fwData::Object::sptr objInfo = ::boost::const_pointer_cast< ::fwData::Object > ( cObjInfo );
            ::fwData::Composite::sptr info = ::fwData::Composite::dynamicCast ( objInfo );

            ::fwData::Integer::sptr fromSliceType = ::fwData::Integer::dynamicCast( info->getRefMap()["fromSliceType"] );
            ::fwData::Integer::sptr toSliceType = ::fwData::Integer::dynamicCast( info->getRefMap()["toSliceType"] );

            if( toSliceType->value() == static_cast< int > ( m_orientation ) )
            {
                m_orientation = static_cast< Orientation > ( fromSliceType->value() );
            }
            else if(fromSliceType->value() == static_cast<int>(m_orientation))
            {
                m_orientation = static_cast< Orientation >( toSliceType->value() );
            }
            this->updateSliceType(m_orientation);
        }
    }
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::info( std::ostream &_sstream )
{

}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::updateSliceIndex()
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    // Get Index
    std::string fieldID = *SLICE_INDEX_FIELDID[m_orientation];
    OSLM_ASSERT("Field "<<fieldID<<" is missing", image->getFieldSize( fieldID ) > 0);
    unsigned int index = image->getFieldSingleElement< ::fwData::Integer >( fieldID )->value();

    // Update wxSlider
    int max = image->getSize()[m_orientation]-1;
    m_sliceSelectorPanel->setSliceRange( 0, max );
    m_sliceSelectorPanel->setSliceValue( index );
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::updateSliceType(Orientation type )
{
    // Update Type Choice
    m_sliceSelectorPanel->setTypeSelection( static_cast< int >( type ) );

    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    // Get Index
    std::string fieldID = *SLICE_INDEX_FIELDID[m_orientation];
    OSLM_ASSERT("Field "<<fieldID<<" is missing", image->getFieldSize( fieldID ) > 0);
    unsigned int index = image->getFieldSingleElement< ::fwData::Integer >( fieldID )->value();
    int max = image->getSize()[m_orientation]-1;
    m_sliceSelectorPanel->setSliceRange( 0, max );
    m_sliceSelectorPanel->setSliceValue( index );
    this->updateSliceIndex();
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::sliceIndexNotification( unsigned int index)
{
    // Fire the message
    ::fwComEd::ImageMsg::NewSptr msg;
    msg->setSliceIndex( m_axialIndex, m_frontalIndex, m_sagittalIndex);
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

    SliceIndexPositionEditor::SLICE_INDEX_FIELDID[m_orientation];
    std::string fieldID = *SLICE_INDEX_FIELDID[m_orientation];
    OSLM_ASSERT("Field "<<fieldID<<" is missing", image->getFieldSize( fieldID ) > 0);
    image->getFieldSingleElement< ::fwData::Integer >( fieldID )->value() = index;

    ::fwServices::IEditionService::notify(this->getSptr(),  image, msg);
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::sliceTypeNotification( int _type )
{
    Orientation type = static_cast< Orientation >( _type );
    OSLM_ASSERT("Bad slice type "<<type, type == X_AXIS ||
            type == Y_AXIS ||
            type == Z_AXIS );

    // Change data info
    ::fwData::Composite::NewSptr info;
    ::fwData::Integer::NewSptr fromSliceType;
    ::fwData::Integer::NewSptr toSliceType;
    fromSliceType->value() = static_cast< int > ( m_orientation ) ;
    toSliceType->value() = static_cast< int > ( type ) ;
    info->getRefMap()["fromSliceType"] = fromSliceType;
    info->getRefMap()["toSliceType"] = toSliceType;

    // Change slice type
    m_orientation = type;

    // Fire the message
    ::fwComEd::ImageMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::ImageMsg::CHANGE_SLICE_TYPE, info ) ;
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    ::fwServices::IEditionService::notify(this->getSptr(),  image, msg);
    this->updateSliceIndex();
}
}

