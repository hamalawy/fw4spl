/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIIO_ACTION_EXPORTACQUISITION_HPP_
#define _UIIO_ACTION_EXPORTACQUISITION_HPP_

#include <fwGui/IActionSrv.hpp>

#include "uiIO/config.hpp"

namespace uiIO
{
namespace action
{
/**
 * @brief   Action to export the selected acquisition of a patientDB.
 * @class   ExportAcquisition.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * @see     ::fwGui::IActionSrv
 */
class UIIO_CLASS_API ExportAcquisition : public ::fwGui::IActionSrv
{
public:

    fwCoreServiceClassDefinitionsMacro ( (ExportAcquisition)(::fwGui::IActionSrv) ) ;
    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;

    /// Constructor. Do nothing (Just initialize parameters).
    UIIO_API ExportAcquisition() throw() ;

    /// Destructor. Do nothing.
    UIIO_API virtual ~ExportAcquisition() throw() ;

protected:

    /**
     * @brief   This method initializes class member parameters from configuration elements.
     * @note    Call action base class configuring() method.
     */
    UIIO_API void configuring() throw( ::fwTools::Failed ) ;

    /**
     * @brief  Starting method. Call base class starting() method.
     */
    UIIO_API void starting() throw( ::fwTools::Failed ) ;

    /**
     * @brief  Stopping method. Call base class stopping() method.
     */
    UIIO_API void stopping() throw( ::fwTools::Failed ) ;

    /**
     * @brief  Export the acquition.
     *
     * Use the IOSelectorService which propose all writer services for acquisition.
     */
    UIIO_API void updating() throw(::fwTools::Failed) ;

    /**
     * @brief React on modifications : does nothing.
     */
    UIIO_API void updating(::boost::shared_ptr< const ::fwServices::ObjectMsg > _msg) throw(::fwTools::Failed) ;

    /// Gives a short information about the action.
    UIIO_API void info(std::ostream &_sstream ) ;

private:

    std::string m_ioSelectorSrvConfig;
};

} // namespace action
} // namespace uiIO

#endif // _UIIO_ACTION_EXPORTACQUISITION_HPP_
