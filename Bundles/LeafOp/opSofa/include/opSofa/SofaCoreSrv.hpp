/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __OPSOFA_SOFACORESRV_HPP__
#define __OPSOFA_SOFACORESRV_HPP__

#include <boost/shared_ptr.hpp>

#include <fwGui/IActionSrv.hpp>

#include "config.hpp"
#include "opSofa/SofaBusiness.hpp"

namespace opSofa
{


/**
 * @brief Service allow to manage opSofa bundle
 */
class SofaCoreSrv : public ::fwGui::IActionSrv
{

public :
    fwCoreServiceClassDefinitionsMacro ( (SofaCoreSrv)(::fwGui::IActionSrv) );

    SofaCoreSrv() throw() ;
    virtual ~SofaCoreSrv() throw() ;

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
     * @param _pMsg : Incoming message
     */
    void receiving( ::fwServices::ObjectMsg::csptr _pMsg ) throw ( ::fwTools::Failed );

    /**
     * @brief info of the class
     */
    void info ( std::ostream &_sstream ) ;

    /**
     * @brief Add a triangularMesh to the model series data
     *
     * @param meshPath : path to the mesh
     * @param meshName : name of the mesh
     */
    void addMesh(std::string, std::string);

private :

    /**
     * @brief SofaBusiness pointer
     */
    ::boost::shared_ptr< SofaBusiness > m_sofa;
};

} // namespace opSofa


#endif // __OPSOFA_SOFACORESRV_HPP__

