/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __OPKINECT_KINECTSRV_HPP__
#define __OPKINECT_KINECTSRV_HPP__

#include <fwGui/IActionSrv.hpp>

#include "opKinect/Kinect.hpp"
#include "config.hpp"

namespace opKinect
{

/**
 * @brief Service allow to manage opKinect bundle
 */
class KinectSrv : public ::fwGui::IActionSrv
{
public :
    fwCoreServiceClassDefinitionsMacro ( (KinectSrv)(::fwGui::IActionSrv) );

    KinectSrv() throw() ;
    virtual ~KinectSrv() throw() ;

protected:

    /**
     * @brief Used to define the service parameters and analyze its configuration.
     */
    void configuring() throw ( ::fwTools::Failed );

    /**
     * @brief Used to launch the service.
     */
    void starting() throw ( ::fwTools::Failed );

    /**
     * @brief Used to stop the service.
     */
    void stopping() throw ( ::fwTools::Failed );

    /**
     * @brief Called to do an action on the data associated to the service.
     */
    void updating() throw ( ::fwTools::Failed );

    /**
     * @brief Called by a data to notify a service.
     *
     * @param msg : Incoming message
     */
    void receiving( fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed );

    /**
     * @brief info of the class
 */
    void info ( std::ostream &_sstream ) ;

private :
    SPTR(Kinect) m_kinect;
};

} // namespace opKinect


#endif // __OPKINECT_KINECTSRV_HPP__

