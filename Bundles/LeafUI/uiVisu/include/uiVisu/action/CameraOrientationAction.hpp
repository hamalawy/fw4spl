/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIVISU_ACTION_CAMERAORIENTATIONACTION_HPP__
#define __UIVISU_ACTION_CAMERAORIENTATIONACTION_HPP__

#include <fwTools/Failed.hpp>

#include <fwGui/IActionSrv.hpp>

#include "uiVisu/config.hpp"

namespace uiVisu
{
namespace action
{

/**
 * @brief   This action allows change 3D camera orientation (axial, frontal, sagittal)
 * @class   CameraOrientationAction
 *
 * @date    2010.
 */
class UIVISU_CLASS_API CameraOrientationAction : public ::fwGui::IActionSrv
{

public:

    fwCoreServiceClassDefinitionsMacro ( (CameraOrientationAction)(::fwGui::IActionSrv) );

    /// Constructor. Do nothing.
    UIVISU_API CameraOrientationAction() throw();

    /// Destructor. Do nothing.
    UIVISU_API virtual ~CameraOrientationAction() throw();

protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    /**
     * @brief Configure action.
     * @verbatim
       <service type="::fwGui::IActionSrv" impl="::uiData::action::CameraOrientationAction" autoConnect="no">
        <orientation>frontal</orientation>
       </service>
       @endverbatim
     * - \b orientation : must be axial, frontal or sagittal
     */
    void configuring() throw(fwTools::Failed);

    /// Starts action
    virtual void starting() throw(::fwTools::Failed);

    /// Stops action
    virtual void stopping() throw(::fwTools::Failed);

    /// Does nothing
    virtual void receiving( CSPTR(::fwServices::ObjectMsg) _msg ) throw(::fwTools::Failed);

    /// Notifies message to change camera orientation
    void updating() throw(::fwTools::Failed);

    /// Does nothing
    void swapping() throw(::fwTools::Failed);

    /// Overrides
    virtual void info( std::ostream &_sstream );

private:

    std::string m_orientation;
};

} // action
} // uiVisu

#endif /*__UIVISU_ACTION_CAMERAORIENTATIONACTION_HPP__*/


