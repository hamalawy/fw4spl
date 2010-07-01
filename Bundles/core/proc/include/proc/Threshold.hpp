/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef PROC_THRESHOLD_HPP_
#define PROC_THRESHOLD_HPP_

#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>
#include <fwServices/ObjectMsg.hpp>

#include "proc/IProc.hpp"
#include "proc/config.hpp"

namespace proc
{
/**
 * @brief Message dedicated to the thresholding algorithm
 *
 * This is used, for instance, to notify changes on thresholding parameter so that the GUI editor can refresh controls (see ThresholdEditor)
 *
 * @note The event are also used to name the parameters (input image, thresholds, values, output image)
 */
class PROC_CLASS_API ThresholdMsg : public ::fwServices::ObjectMsg
{
public:
    fwCoreClassDefinitionsWithFactoryMacro((ThresholdMsg)(::fwServices::ObjectMsg), ( () ), new ThresholdMsg );

    /**
     * @name Event identifier dedicated to image thresholding-based processings
     */
    /// @{
    PROC_API static std::string INPUTIMG 	;
    PROC_API static std::string OUTPUTIMG ;
    PROC_API static std::string INSIDEVALUE ;
    PROC_API static std::string OUTSIDEVALUE ;
    PROC_API static std::string UPPERTH ;
    PROC_API static std::string LOWERTH ;
    /// @}

	/// Constructor: does nothing
    PROC_API ThresholdMsg() throw() {};

	/// Destructor: does nothing
    PROC_API virtual ~ThresholdMsg() throw() {};

};


/**
 * @brief Algorithm dedicated to image thresholding
 *
 * Only declares algorithm parameters (i.e. input image, output thresholded image, thresholds, output values) and will invoke the ThresholdAlgorithm on the update() method
 * @note It uses a dispatcher to invoke the ThresholdAlgorithm with the appropriate template parameter, according to the input image point type
 * @note This service is declared to be compliant with object, image (untested), composite (depreciated), process object (depreciated)
 */
class PROC_CLASS_API Threshold : public ::proc::IProc
{
public :

	fwCoreServiceClassDefinitionsMacro ( (Threshold)(::proc::IProc) ) ;

	/// Constructor: declares parameters and affect default values
	PROC_API Threshold() throw() ;

	/// Destructor: does nothing
	PROC_API virtual ~Threshold() throw() ;

    /**
     * @name Some getters
     */

    //@{
	PROC_API ::fwData::Integer::sptr getLowerTh() throw();

	PROC_API ::fwData::Integer::sptr getUpperTh() throw();

	PROC_API ::fwData::Integer::sptr getInsideValue() throw();

	PROC_API ::fwData::Integer::sptr getOutsideValue() throw();
	//@}

protected :

	/// Does nothing special
	PROC_API virtual void doConfigure() throw(::fwTools::Failed) ;
	/// Does nothing special
	PROC_API virtual void doStart() throw(::fwTools::Failed) ;
	/// Trigger thresholding using the ThresholdAlgorithm
	PROC_API virtual void doUpdate() throw(::fwTools::Failed) ;
	/// Does nothing special
	PROC_API virtual void doUpdate(::boost::shared_ptr< const fwServices::ObjectMsg > _mssg) throw(::fwTools::Failed) ;
	/// Does nothing special
	PROC_API virtual void doStop() throw(::fwTools::Failed) ;
	/// React on input image picked points: lower and upper thresholds are automatically computed
	PROC_API void minMaxInteraction();
	/// If true, picking will automatically update thresholds and trigger computations
	bool m_autoRun;
};

} //namespace proc

#endif /* PROC_THRESHOLD_HPP_ */
