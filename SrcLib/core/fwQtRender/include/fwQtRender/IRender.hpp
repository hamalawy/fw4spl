/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWQTRENDER_IRENDER_HPP_
#define _FWQTRENDER_IRENDER_HPP_

#include <fwTools/Failed.hpp>
#include <fwServices/IService.hpp>

#include <fwQt/IGuiContainer.hpp>

#include "fwQtRender/config.hpp"

/*
A enregistrer en temps que sevie dans Bundles/core/visu/rc/plugin.xml
==> a creer un visu pour Qt
*/
/**
 * @brief   The namespace fwRender contains the base interface for rendering services.
 * @namespace   fwRender
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 *
 */
namespace fwQtRender
{

/**
 * @brief   Defines the service interface managing the rendering service for object.
 * @class   IRender.
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 *
 * Use the base service methods :
 * @li The service is configured with methods setConfiguration(cfg) and configure()
 * @li The method start() initialize the service
 * @li Use update() method to update data
 * @li To react on modifications use update(msg)
 * @li Finally we must call stop() before deleting the service
 *
 */
class  IRender : public ::fwQt::IGuiContainer
{

public :
    fwCoreServiceClassDefinitionsMacro ( (IRender)(::fwServices::IService::Baseclass) ) ;

    /// Clock rate identifiant (for persistance)
    static const std::string ClockRateId ;

    /**
     * @brief   Constructor. Initialize default values.
     */
     IRender() throw() ;

    /**
     * @brief   Destructor. Do nothing.
     */
     virtual ~IRender() throw() ;



    /**
     * @name    Helper service methods
     */
    //@{

    /**
     * @brief Configure the service (use it in configuring, before start).
     *
     * Configure clock rate if it's specified in the configuration.
     */
     void initClockRate();


    /**
     * @brief Initialize the Render.
     *
     * This method initializes the gui container ( create a new frame if the container is not configured).
     * It starts timer if clock is configured and register communication channel.
     *
     * @note This method should be called in starting() method by concrete services.
     */
     void initRender();

    /**
     * @brief Stop the timer and destroy the container if they where configured.
     *
     * @note This method should be called in stopping() method by concrete services.
     */
     void stopRender();
    //@}



    /**
     * @name    Standard service methods
     */
    //@{

    /**
     * @brief Info method.
     *
     * This method is used to give
     * informations about the service clock rate.
     *
     * @param[out] _sstream output stream
     */
     virtual void info(std::ostream &_sstream ) ;

    //@}

    /**
     * @name    Specific service methods for visualization management
     */
    //@{

    /**
     * @brief   defines the clock rate
     *
     * @param[in] _rate clock rate
     */
     virtual void setClockRate(int _rate) ;

    /**
     * @brief   retrieve the clock rate
     *
     * @li m_clockRate.first == true if clock rate is configured
     * @li m_clockRate.second = clock rate value
     */
      virtual std::pair< bool , int > getClockRate() ;

    //@}

protected :


    /**
     * @brief gui container indentifier
     *
     * @li m_clockRate.first == true if clock rate is configured
     * @li m_clockRate.second = clock rate value
     **/
    std::pair< bool , int > m_clockRate ;

    /// Clock used to refresh view
 //   ::fwWX::Timer<>::ptr m_timer ;

};

} // namespace fwRenderVTK

#endif /*_FWRENDER_IRENDER_HPP_*/


