/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <fwCore/base.hpp>

#include <exception>

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/ObjectMsg.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <fwData/Boolean.hpp>
#include <fwData/Point.hpp>
#include <fwData/PointList.hpp>

#include <fwServices/ObjectMsg.hpp>
#include <fwComEd/Dictionary.hpp>
#include <fwComEd/ImageMsg.hpp>
#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>

#include "uiMeasurement/action/ShowDistance.hpp"


namespace uiMeasurement
{
namespace action
{

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::uiMeasurement::action::ShowDistance, ::fwData::Image );


//------------------------------------------------------------------------------

ShowDistance::ShowDistance( ) throw()
{
    //addNewHandledEvent( ::fwComEd::ImageMsg::DISTANCE );
}

//------------------------------------------------------------------------------

ShowDistance::~ShowDistance() throw()
{
}

//------------------------------------------------------------------------------

void ShowDistance::info(std::ostream &_sstream )
{
    _sstream << "Action for show distance" << std::endl;
}

//------------------------------------------------------------------------------

void ShowDistance::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    if ( !::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity(image) )
    {
        this->::fwGui::IActionSrv::setIsActive(false);
    }
    else
    {
        ::fwData::Boolean::sptr showDistances = image->getField< ::fwData::Boolean >("ShowDistances", ::fwData::Boolean::New(
                                                                                         true));
        bool isShown = showDistances->value();

        bool toShow = !isShown;
        image->setField("ShowDistances", ::fwData::Boolean::New(toShow));

        // auto manage hide/show : use Field Information instead let gui manage checking
        this->::fwGui::IActionSrv::setIsActive(!toShow);

        ::fwComEd::ImageMsg::sptr msg = ::fwComEd::ImageMsg::New();
        msg->addEvent( ::fwComEd::ImageMsg::DISTANCE );
        ::fwServices::IEditionService::notify(this->getSptr(), image, msg);
    }
}

//------------------------------------------------------------------------------

void ShowDistance::swapping() throw(::fwTools::Failed)
{
    ::fwData::Image::csptr img            = this->getObject< ::fwData::Image >();
    ::fwData::Boolean::sptr showDistances =
        img->getField< ::fwData::Boolean >("ShowDistances", ::fwData::Boolean::New(true));

    this->::fwGui::IActionSrv::setIsActive( !(showDistances->value()) );
}

//------------------------------------------------------------------------------

void ShowDistance::receiving( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::fwComEd::ImageMsg::csptr imgMsg = ::fwComEd::ImageMsg::dynamicConstCast( msg );
    if ( imgMsg && imgMsg->hasEvent( ::fwComEd::ImageMsg::DISTANCE ) )
    {
        ::fwData::Image::csptr img            = this->getObject< ::fwData::Image >();
        ::fwData::Boolean::sptr showDistances = img->getField< ::fwData::Boolean >("ShowDistances", ::fwData::Boolean::New(
                                                                                       true));

        this->::fwGui::IActionSrv::setIsActive( !(showDistances->value()) );
    }
}

//------------------------------------------------------------------------------

void ShowDistance::configuring() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::initialize();
}

//------------------------------------------------------------------------------

void ShowDistance::starting() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::actionServiceStarting();
}

//------------------------------------------------------------------------------

void ShowDistance::stopping() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::actionServiceStopping();
}

//------------------------------------------------------------------------------

} // namespace action
} // namespace uiMeasurement
