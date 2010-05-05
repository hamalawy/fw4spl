/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIIOQT_EDITOR_IOSELECTORSERVICE_HPP_
#define _UIIOQT_EDITOR_IOSELECTORSERVICE_HPP_

#include <guiQt/editor/IEditor.hpp>

#include "uiIOQt/config.hpp"
 #include <QObject>
#include <QListWidget>

namespace uiIOQt
{
namespace editor
{

/**
 * @brief   Defines the service interface managing the editor service for object.
 * @class   IOSelectorService.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * @todo ACH : Some services of IEditor type do not need a container (example: IOSelectorService that open a dialogue box).
 *  The problem is that the IEditor::starting method creates another useless container in this case.
 */
class  IOSelectorService :  public ::guiQt::editor::IEditor
{

  Q_OBJECT

public :

    /// IOMode enum definition
    typedef enum {
        READER_MODE, /**< this mode allows to configure the service as a reader */
        WRITER_MODE  /**< this mode allows to configure the service as a writer */
    } IOMode;


    fwCoreServiceClassDefinitionsMacro ( (IOSelectorService)( ::guiQt::editor::IEditor::Baseclass) ) ;

    /**
     * @brief   Constructor. Do nothing (Just initialize parameters).
     *
     * By default, the IOSelectorService::m_mode is defined as READER_MODE, and IOSelectorService::m_servicesAreExcluded as true.
     */
     IOSelectorService();

    /// Destructor. Do nothing.
     virtual ~IOSelectorService() throw() ;

    ///Starts the service. Do nothing.
     void starting() throw( ::fwTools::Failed ) ;

    /// Stops the service. Do nothing.
     void stopping() throw( ::fwTools::Failed ) ;

    /**
     * @brief   This method initializes class member parameters from configuration elements.
     *
     * The method verifies if the configuration is well written and retrieves user parameter values.
     * Thanks to this method, IOSelectorService::m_selectedServices value is up to date (cleared before reconfiguring).
     */
     void configuring() throw( ::fwTools::Failed ) ;

    /// Create a dialogue box to provide the user different available readers (writer) for the IOSelector associated objects. Then, the selected reader (writer) is executed.
     void updating() throw( ::fwTools::Failed ) ;

    /// Gives the name of the class. Do nothing.
     void info( std::ostream &_sstream ) ;

    /**
     * @brief This method allows to configure the service in reader or writer mode (set IOSelectorService::m_mode).
     *
     *@param[in] _mode the value can be IOSelectorService::READER_MODE or IOSelectorService::WRITER_MODE.
     */
     void setIOMode( IOMode _mode ) ;
     
     bool extensionSelectionIsCanceled;
     bool m_ok;
     QListWidget *m_list;
     std::string m_selectedString;
     QDialog *box;
     QStringList selection;  
     std::vector< std::pair < std::string, std::string > > availableExtensionsMap;


     
  public slots :
    void startSelectedService();
    void getListIndex();
    void cancel();
    void doubleClickSelection();

private :

    /// Configure the service as writer or reader.
    IOMode m_mode;

    /// Configure if selected services are included or excluded.
    bool m_servicesAreExcluded;

    /**
    * @brief List of services to be included or excluded.
    *
    * @see IOSelectorService::m_servicesAreExcluded.
    */
    std::vector< std::string > m_selectedServices;
};

} // namespace editor

} // namespace uiio

#endif // _UIIO_EDITOR_IOSELECTORSERVICE_HPP_

