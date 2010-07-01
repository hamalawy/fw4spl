/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#include <fwServices/macros.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwData/Image.hpp>
#include <fwData/Composite.hpp>
#include <fwData/Integer.hpp>
#include <fwData/Float.hpp>
#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>
#include <fwComEd/ImageMsg.hpp>
#include <fwComEd/Dictionary.hpp>

#include "ctrlField/ImageSynchronizer.hpp"

namespace ctrlField
{

REGISTER_SERVICE( ::ctrlField::IFieldSynchronizer , ::ctrlField::ImageSynchronizer , ::fwData::Composite ) ;

//-----------------------------------------------------------------------------

ImageSynchronizer::ImageSynchronizer() throw()
{}

//-----------------------------------------------------------------------------

ImageSynchronizer::~ImageSynchronizer() throw()
{}

//-----------------------------------------------------------------------------

void ImageSynchronizer::doStart() throw ( ::fwTools::Failed )
{
	//Checking that optional dynamic fields are initialized: avoid systematic check at observation (update() method)
	for( std::vector< ::fwTools::Object::wptr >::iterator iter = m_objectsToSynchronize.begin() ; iter != m_objectsToSynchronize.end() ; ++iter )
	{
		::fwTools::Object::sptr obj = (*iter).lock();
        bool f1 = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageSliceIndex( ::fwData::Image::dynamicCast(obj) );
    	bool f2 = ::fwComEd::fieldHelper::MedicalImageHelpers::checkOpacity( ::fwData::Image::dynamicCast( obj ) );
	}
}

//-----------------------------------------------------------------------------

void ImageSynchronizer::doStop() throw ( ::fwTools::Failed )
{
}

//-----------------------------------------------------------------------------

void ImageSynchronizer::doConfigure() throw ( ::fwTools::Failed )
{
}

//-----------------------------------------------------------------------------

void ImageSynchronizer::doUpdate() throw ( ::fwTools::Failed )
{
}

//-----------------------------------------------------------------------------

void ImageSynchronizer::doUpdate( std::vector< ::fwTools::Object::wptr > objectsToUpdate, fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed )
{
	::fwTools::Object::sptr srcObj = ::fwData::Image::dynamicCast( _msg->getSubject().lock() );
	assert( srcObj );

    if ( _msg->hasEvent( ::fwComEd::ImageMsg::SLICE_INDEX ) )
    {
    	updateSliceIndex(objectsToUpdate, srcObj);
    }

    if ( _msg->hasEvent( ::fwComEd::ImageMsg::CHANGE_SLICE_TYPE ) )
    {
    	updateSliceType(objectsToUpdate, _msg);
    }

    if ( _msg->hasEvent( ::fwComEd::ImageMsg::OPACITY ) )
    {
    	updateOpacity(objectsToUpdate, srcObj);
    }
}

void ImageSynchronizer::updateSliceIndex(std::vector< ::fwTools::Object::wptr > objectsToUpdate, ::fwTools::Object::sptr srcObj)
{
	//We retrieve fields value of the sender
    unsigned int axialIndex = srcObj->getFieldSingleElement< ::fwData::Integer >( ::fwComEd::Dictionary::m_axialSliceIndexId )->value();
    unsigned int frontalIndex = srcObj->getFieldSingleElement< ::fwData::Integer >( ::fwComEd::Dictionary::m_frontalSliceIndexId )->value();
    unsigned int sagittalIndex = srcObj->getFieldSingleElement< ::fwData::Integer >( ::fwComEd::Dictionary::m_sagittalSliceIndexId )->value();

	//We update (copy) the field of each other element
    for( std::vector< ::fwTools::Object::wptr >::iterator iter = objectsToUpdate.begin() ; iter != objectsToUpdate.end() ; ++iter )
    {

    	::fwTools::Object::sptr destObj = (*iter).lock();
        destObj->getFieldSingleElement< ::fwData::Integer >( ::fwComEd::Dictionary::m_axialSliceIndexId )->value() = axialIndex ;
        destObj->getFieldSingleElement< ::fwData::Integer >( ::fwComEd::Dictionary::m_frontalSliceIndexId )->value() = frontalIndex;
        destObj->getFieldSingleElement< ::fwData::Integer >( ::fwComEd::Dictionary::m_sagittalSliceIndexId )->value() = sagittalIndex;

        //Notification of the modification
        ::fwComEd::ImageMsg::NewSptr notificationMsg;

        notificationMsg->setSliceIndex( ::fwData::Integer::New(axialIndex), ::fwData::Integer::New(frontalIndex),::fwData::Integer::New(sagittalIndex)); //m_axialIndex, m_frontalIndex, m_sagittalIndex);
        ::fwServices::IEditionService::notify(this->getSptr(), destObj , notificationMsg);
    }
}

void ImageSynchronizer::updateOpacity(std::vector< ::fwTools::Object::wptr > objectsToUpdate, ::fwTools::Object::sptr srcObj)
{
	//We retrieve fields value of the sender
	float srcOpacity = srcObj->getFieldSingleElement< ::fwData::Float >( ::fwComEd::Dictionary::m_opacityId )->value();
	assert( 0.0 <= srcOpacity && srcOpacity <= 1.0 );
	//The "complementary" opacity is defined: warning -> can lead to strange effect for more than two objects
	float destOpacity = 1.0 - srcOpacity ;
	OSLM_TRACE("Opacity observed is " << srcOpacity << ": will be affected as " << destOpacity << " to other objects" << std::endl << "If synchronizing more than 2 objects: no specific management");
	//We update (copy) the field of each other element
	for( std::vector< ::fwTools::Object::wptr >::iterator iter = objectsToUpdate.begin() ; iter != objectsToUpdate.end() ; ++iter )
    {
    	//Modification of the corresponding field
    	::fwTools::Object::sptr destObj = (*iter).lock();
    	destObj->getFieldSingleElement< ::fwData::Float >( ::fwComEd::Dictionary::m_opacityId )->value() = destOpacity ;

        //Notification of the modification
        ::fwComEd::ImageMsg::NewSptr notificationMsg;
        notificationMsg->addEvent( ::fwComEd::ImageMsg::OPACITY ) ;
        ::fwServices::IEditionService::notify(this->getSptr(), destObj , notificationMsg);
    }
}

void ImageSynchronizer::updateSliceType(std::vector< ::fwTools::Object::wptr > objectsToUpdate, fwServices::ObjectMsg::csptr _msg)
{
	//Information contained in message
	::fwData::Object::csptr cObjInfo 	= _msg->getDataInfo( ::fwComEd::ImageMsg::CHANGE_SLICE_TYPE );
	::fwData::Object::sptr objInfo 		= ::boost::const_pointer_cast< ::fwData::Object > ( cObjInfo );
	::fwData::Composite::sptr info 		= ::fwData::Composite::dynamicCast ( objInfo );

	int fromSliceType = ::fwData::Integer::dynamicCast( info->getRefMap()["fromSliceType"] )->value();
	int toSliceType =   ::fwData::Integer::dynamicCast( info->getRefMap()["toSliceType"] )->value();

	//Source object
	::fwTools::Object::sptr srcObj = ::fwData::Image::dynamicCast( _msg->getSubject().lock() );

	//Notify slice type modification
	for( std::vector< ::fwTools::Object::wptr >::iterator iter = objectsToUpdate.begin() ; iter != objectsToUpdate.end() ; ++iter )
    {
		//Prepare message content
	    ::fwData::Composite::NewSptr info;
	    ::fwData::Integer::NewSptr fromSliceTypeField;
	    ::fwData::Integer::NewSptr toSliceTypeField;
	    fromSliceTypeField->value() = fromSliceType ;
	    toSliceTypeField->value() = toSliceType ;
	    info->getRefMap()["fromSliceType"] = fromSliceTypeField;
	    info->getRefMap()["toSliceType"] = toSliceTypeField;
	    // Fire the message
		::fwComEd::ImageMsg::NewSptr msg;
		msg->addEvent( ::fwComEd::ImageMsg::CHANGE_SLICE_TYPE, info ) ;
		::fwTools::Object::sptr destObj = (*iter).lock();
		::fwServices::IEditionService::notify(this->getSptr(),  destObj, msg);
    }
}
}
