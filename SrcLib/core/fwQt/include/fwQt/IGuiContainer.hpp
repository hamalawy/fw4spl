/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWQT_IGUICONTAINER_HPP_
#define _FWQT_IGUICONTAINER_HPP_

#include <QMainWindow>

#include <fwServices/IService.hpp>

#include "fwQt/config.hpp"

/**
 * @brief   The namespace fwQt contains the base interface for Qt services and some specifics Qt components.
 * @namespace   fwQt
 * @author  IRCAD (Research and Development Team).
 * @date    2009-2010.
 *
 */
namespace fwQt
{

/**
 * @brief   Defines the service interface managing the layout.
 * @class   IGuiContainer.
 * @author  IRCAD (Research and Development Team).
 * @date    2009-2010.
 *
 */
class  FWQT_CLASS_API IGuiContainer : public fwServices::IService
{

public :

    typedef std::map< std::string , QWidget* > ContainerMapType;

    fwCoreServiceClassDefinitionsMacro ( (IGuiContainer)(::fwServices::IService::Baseclass) ) ;



    /**
     * @brief   Constructor. Initialize default values.
     */
    FWQT_API IGuiContainer() throw() ;

    /**
     * @brief   Destructor. Do nothing.
     */
    FWQT_API virtual ~IGuiContainer() throw() ;

    /**
     * @name    Helper service methods
     */
    //@{

    /**
     * @brief Returns parent QWidget
     */
    FWQT_API virtual QWidget* getQtContainer() ;

    /**
     * @brief Configure parent container associated wit the service (use it before start).
     * If any container has been reserved for this service, parent container will be locally created in a new QWidget.
     */
    FWQT_API void initGuiParentContainer();

    /**
     * @brief Reset parent QWidget and destroy it if parent container has been locally created.
     */
    FWQT_API void resetGuiParentContainer();
    /**
     * @brief Registers associate container with specific service's uid in the global and local UID-Container map.
     */
    FWQT_API void registerQtContainer(std::string uid , QWidget* container);

    /**
     * @brief Unregisters all container in global and local containers (with service UID associated), and stop associated service.
     */
    FWQT_API void unregisterQtContainer(std::string uid);

    /**
     * @brief Unregisters all container in global and local containers, and stop associated services.
     */
    FWQT_API  void unregisterAllQtContainer();

    /**
     * @brief Registers container in global container (with service UID associated).
     */
    FWQT_API static void registerGlobalQtContainer(std::string uid, QWidget* container);

    /**
     * @brief Unregisters container in global container (with service UID associated).
     */
    FWQT_API static void unregisterGlobalQtContainer(std::string uid);

    /**
     * @brief Returns QWidget associate with service uid.
     */
    FWQT_API static QWidget* getQtContainer(std::string uid);

    //@}

protected :

    // Parent QtContainer
    QWidget* m_container;

    FWQT_API static ContainerMapType m_globalUIDToQtContainer;

    /**
     * @brief Internal sub wxContainer created for specific uid's service
     */
    ContainerMapType m_subUIDToQtContainer;

    /// flag if the container is locally created (ie no parent container associated with this service)
    bool m_isContainerLocallyCreated ;

};

} // namespace fwQt

#endif /*_FWQT_IGUICONTAINER_HPP_*/


