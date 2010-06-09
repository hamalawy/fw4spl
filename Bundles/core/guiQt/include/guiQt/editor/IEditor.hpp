/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUIQT_EDITOR_IEDITOR_HPP_
#define _GUIQT_EDITOR_IEDITOR_HPP_

#include <fwTools/Failed.hpp>
#include <fwServices/IService.hpp>

#include <fwQt/IGuiContainer.hpp>

#include "guiQt/config.hpp"
#include <QObject>


namespace guiQt
{

namespace editor
{

/**
 * @brief   Defines the service interface managing the editor service for object.
 * @class   IEditor.
 * @author  IRCAD (Research and Development Team).

 * @date    2009.
 *
 * @todo ACH : Some services of IEditor type do not need a container (example: IOSelectorService that open a dialogue box).
 *  The problem is that the IEditor::starting method create another useless container in this case.
 */

class  GUIQT_CLASS_API IEditor : public  QObject, public ::fwQt::IGuiContainer
{
 
public :
    fwCoreServiceClassDefinitionsMacro ( (IEditor)(::fwServices::IService::Baseclass) ) ;

    /**
     * @brief   Constructor. Do nothing (Just initialize parameters).
     *
     * By default, the container ID is not defined and the container is not locally created.
     */
    GUIQT_API IEditor() throw() ;

    /// Destructor. Do nothing.
    GUIQT_API virtual ~IEditor() throw() ;

protected :

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /// This method is used to find the container ID if it is defined in the configuration element.
    GUIQT_API virtual void configuring() throw( ::fwTools::Failed ) ;

    /**
     * @brief This method retrieves or builds the container.
     *
     * If the container Id does not exist, the container is created (m_isContainerLocallyCreated = true).
     * Else, the method retrieves and sets the container thanks to wxWidgets API (FindWindowById).
     */
    GUIQT_API virtual void starting() throw(::fwTools::Failed);

    /// Stops the service and destroys the m_container if it has been locally created.
    GUIQT_API virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief This method is used to give information about the service. Do nothing.
     * @todo Must be not implemented and class child declaration should be imposed.
     */
    GUIQT_API virtual void info(std::ostream &_sstream ) ;

    /**
     * @brief Update/refresh/execute the service on an observation/notification. Do nothing.
     * @todo Must be not implemented and class child declaration should be imposed.
     */
    GUIQT_API virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed) {};

    /**
     * @brief Update/refresh/execute the service. Do nothing.
     * @todo Must be not implemented and class child declaration should be imposed.
     */
    GUIQT_API virtual void updating() throw(::fwTools::Failed) {};

    ///@}

};

}
}

#endif /*_FWRENDER_RENDERINGSERVICE_HPP_*/


