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
#include <fwData/ProcessObject.hpp>
#include <fwData/Composite.hpp>
#include <fwData/ValuedPointList.hpp>
#include <fwComEd/ImageMsg.hpp>
#include <fwServices/IEditionService.hpp>


#include <fwTools/IntrinsicTypes.hpp>
#include <fwTools/Dispatcher.hpp>
#include <fwTools/DynamicTypeKeyTypeMapping.hpp>

#include "proc/Threshold.hpp"
#include "proc/ThresholdAlgorithm.hxx"


REGISTER_SERVICE(  ::proc::IProc , ::proc::Threshold , ::fwTools::Object) ;
REGISTER_SERVICE(  ::proc::IProc , ::proc::Threshold , ::fwData::Composite) ;
REGISTER_SERVICE(  ::proc::IProc , ::proc::Threshold , ::fwData::ProcessObject) ;
REGISTER_SERVICE(  ::proc::IProc , ::proc::Threshold , ::fwData::Image) ;

namespace proc
{

std::string ThresholdMsg::INPUTIMG 			= "InImage" ;
std::string ThresholdMsg::OUTPUTIMG 		= "OutImage" ;
std::string ThresholdMsg::INSIDEVALUE 		= "InVal" ;
std::string ThresholdMsg::OUTSIDEVALUE 		= "OutVal" ;
std::string ThresholdMsg::UPPERTH 			= "HighTh" ;
std::string ThresholdMsg::LOWERTH 			= "LowTh" ;


Threshold::Threshold() throw()
{
	//Object type is passed as template parameter, instead of basic string, for the early detection of bad writings
	this->addAttribute< ::fwData::Image >(ThresholdMsg::INPUTIMG);
	this->addAttribute< ::fwData::Image >(ThresholdMsg::OUTPUTIMG);
	//Add an attributes and configure the default value (only used iff no XML configuration for this attribute name)
	this->addAttribute< ::fwData::Integer >(ThresholdMsg::OUTSIDEVALUE)->value()= 0;
	this->addAttribute< ::fwData::Integer >(ThresholdMsg::INSIDEVALUE)->value()=255;
	this->addAttribute< ::fwData::Integer >(ThresholdMsg::LOWERTH)->value()=0;
	this->addAttribute< ::fwData::Integer >(ThresholdMsg::UPPERTH)->value()=500;
}
//-----------------------------------------------------------//
Threshold::~Threshold() throw()
{}

::fwData::Integer::sptr Threshold::getLowerTh() throw()
{
	return this->getAttribute< ::fwData::Integer >(ThresholdMsg::LOWERTH);
}
::fwData::Integer::sptr Threshold::getUpperTh() throw()
{
	return this->getAttribute< ::fwData::Integer >(ThresholdMsg::UPPERTH);
}
::fwData::Integer::sptr Threshold::getInsideValue() throw()
{
	return this->getAttribute< ::fwData::Integer >(ThresholdMsg::INSIDEVALUE);
}
::fwData::Integer::sptr Threshold::getOutsideValue() throw()
{
	return this->getAttribute< ::fwData::Integer >(ThresholdMsg::OUTSIDEVALUE);
}
//-----------------------------------------------------------//
void Threshold::doConfigure() throw(::fwTools::Failed)
{
	OSLM_ASSERT("Configuration must provide autoRun attribute", m_configuration->hasAttribute("autoRun") );
	OSLM_ASSERT("Configuration autoRun attribute must be yes or no", m_configuration->getAttributeValue("autoRun") == "yes" || m_configuration->getAttributeValue("autoRun") == "no" );

	std::string autoRun = m_configuration->getAttributeValue("autoRun");
	if( autoRun == "yes" )
	{
		m_autoRun = true;
	}
	else if( autoRun == "no" )
	{
		m_autoRun = false;
	}
	else
	{
		assert(false);
	}
}
//-----------------------------------------------------------//
void Threshold::doStart() throw(::fwTools::Failed)
{
}
//-----------------------------------------------------------//
void Threshold::doStop() throw(::fwTools::Failed)
{

}
//-----------------------------------------------------------//
void Threshold::doUpdate() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    //Build the ThresholdAlgorithm paramter
    ThresholdAlgorithm::Parameter param;
    param.in 		= this->getAttribute< ::fwData::Integer >(ThresholdMsg::INSIDEVALUE)->value();
    param.out 		= this->getAttribute< ::fwData::Integer >(ThresholdMsg::OUTSIDEVALUE)->value();
    param.low		= this->getAttribute< ::fwData::Integer >(ThresholdMsg::LOWERTH)->value();
    param.high		= this->getAttribute< ::fwData::Integer >(ThresholdMsg::UPPERTH)->value();
    param.imageIn 	= this->getAttribute< ::fwData::Image >(ThresholdMsg::INPUTIMG);
    param.imageOut	= this->getAttribute< ::fwData::Image >(ThresholdMsg::OUTPUTIMG);

    //The appropriate template instance will be invoked (  ThresholdAlgorithm::operator< >(param) ), according to the pixel type of the input image
    ::fwTools::DynamicType type = param.imageIn->getPixelType();
    fwTools::Dispatcher< fwTools::IntrinsicTypes , ThresholdAlgorithm >::invoke( type , param );

    //We must send/notify a message to the "system" to tell it that the OUTPUTIMG has been modified
    ::fwComEd::ImageMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::ImageMsg::NEW_IMAGE ) ;
    msg->addEvent( ::fwComEd::ImageMsg::BUFFER ) ;
    msg->addEvent( ::fwComEd::ImageMsg::REGION ) ;
    msg->addEvent( ::fwComEd::ImageMsg::SPACING ) ;
    msg->addEvent( ::fwComEd::ImageMsg::PIXELTYPE ) ;
    //this->getSptr() : defines the service which is the cause of the modification
    //this->getAttribute< ::fwData::Image >(ThresholdMsg::OUTPUTIMG) : defines the object which has been modified
    //msg : list the modifications
    ::fwServices::IEditionService::notify(this->getSptr(), this->getAttribute< ::fwData::Image >(ThresholdMsg::OUTPUTIMG) , msg);
}
//-----------------------------------------------------------//
void Threshold::doUpdate(::boost::shared_ptr< const fwServices::ObjectMsg > _mssg) throw(::fwTools::Failed)
{
	SLM_TRACE_FUNC();
    if ( _mssg->hasEvent( ::fwComEd::ImageMsg::PICKEDPOINTS ) && _mssg->getSubject().lock().get() == this->getAttribute(ThresholdMsg::INPUTIMG).get())
    {
        SLM_TRACE("Has event ::fwComEd::ImageMsg::PICKEDPOINTS for ThresholdMsg::INPUTIMG");
        ::fwData::ValuedPointList::sptr pointList = this->getAttribute(ThresholdMsg::INPUTIMG)->getFieldSingleElement< ::fwData::ValuedPointList >(::fwComEd::ImageMsg::PICKEDPOINTS );

        //doUpdate();
    	if( !pointList->getRefPoints().empty() )
    	{
    		this->minMaxInteraction();
    	}
    	//    	std::stringstream message;
    	//    	message << "PICKED POINTS: Indices " << std::endl;
    	//    	for(::fwData::ValuedPointList::PointListContainer::iterator iter = pointList->getRefPoints().begin() ; iter != pointList->getRefPoints().end() ; ++iter)
    	//    	{
    	//    		message << "(" << (*iter)->getRefCoord()[0] << "," << (*iter)->getRefCoord()[1] << "," << (*iter)->getRefCoord()[2] << ")= " << (*iter)->getValue() << std::endl;
    	//    	}
    	//    	message << std::endl;
    	//    	SLM_TRACE(message.str());
    }
}
//-----------------------------------------------------------//
void Threshold::minMaxInteraction()
{
	///Compute min max
	::fwData::ValuedPointList::sptr points = this->getAttribute(ThresholdMsg::INPUTIMG)->getFieldSingleElement< ::fwData::ValuedPointList >(::fwComEd::ImageMsg::PICKEDPOINTS );
	::fwData::ValuedPointList::PointListContainer::iterator iter = points->getRefPoints().begin() ;
	double min = (*iter)->getValue();
	double max = min;
	++iter;
	for( ; iter != points->getRefPoints().end() ; ++iter)
	{
		double currentValue = (*iter)->getValue();
		if( currentValue < min )
		{
			min = currentValue ;
		}
		if( currentValue > max )
		{
			max = currentValue ;
		}
	}
	OSLM_TRACE( "Computed (min,max) =("<< min << "," << max << ")")
	///Affect min max to thresholds
	this->getLowerTh()->value()=(int) min;
	this->getUpperTh()->value()=(int) max;
    //We must send/notify a message to the "system" to tell it that parameters have been modified
    ::proc::ThresholdMsg::NewSptr msg;
    msg->addEvent( ::proc::ThresholdMsg::LOWERTH ) ;
    msg->addEvent( ::proc::ThresholdMsg::UPPERTH ) ;
    ::fwServices::IEditionService::notify(this->getSptr(), this->getSptr() , msg);

    ///Automatic computation
    if( m_autoRun )
    {
    	this->update();
    }
}

}
