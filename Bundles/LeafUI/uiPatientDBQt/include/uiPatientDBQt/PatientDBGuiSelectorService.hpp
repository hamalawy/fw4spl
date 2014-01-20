/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIPATIENTDBQT_PATIENTDBGUISELECTORSERVICE_HPP_
#define _UIPATIENTDBQT_PATIENTDBGUISELECTORSERVICE_HPP_

#include <QPointer>
#include <QObject>
#include <QTreeWidget>
#include <QMetaType>

#include <fwData/Patient.hpp>
#include <fwData/PatientDB.hpp>

#include <gui/editor/IEditor.hpp>

#include "uiPatientDBQt/config.hpp"

Q_DECLARE_METATYPE( QList<int> );

namespace uiPatientDB
{

/**
 * @brief   This editor show the list of patient and acquisition in the PatientDB. It allows to select an acquisition.
 * @class   PatientDBGuiSelectorService
 * 
 * @date    2010.
 */
class UIPATIENTDBQT_CLASS_API PatientDBGuiSelectorService : public QObject, public ::gui::editor::IEditor
{
    Q_OBJECT
public :
    fwCoreServiceClassDefinitionsMacro ( (PatientDBGuiSelectorService)(::gui::editor::IEditor) ) ;

    /// Constructor
    UIPATIENTDBQT_API PatientDBGuiSelectorService();

    /// Destructor
    UIPATIENTDBQT_API virtual ~PatientDBGuiSelectorService() throw() ;

protected:

    void starting() throw(::fwTools::Failed);

    void stopping() throw(::fwTools::Failed);

    void configuring() throw (::fwTools::Failed);

    /// Updates patients list
    void updating() throw (::fwTools::Failed);

    /// Receives message and call updating()
    void receiving( ::boost::shared_ptr< const ::fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed);

    void info( std::ostream &_sstream ) ;

    /// Updated the PatientDB m_imageSelectedId field with the last added image of the patient given by its index in PatientDB
    void selectLastAddedImage(int patientIndex);

    /// Receives event sended by m_pSelectorPanel
    bool eventFilter(QObject *object, QEvent *event);

    /// Erases selected data
    void erase();

    /// Helper to erase selected acquisition
    static void eraseSelectedAcquisition( ::fwData::PatientDB::sptr pdb );

    /// Helper to erase selected patient
    static void eraseSelectedPatient( ::fwData::PatientDB::sptr pdb );

protected Q_SLOTS:

    /**
    * @brief This method is called when the value change.
    */
    void onSelectionChange(QTreeWidgetItem * current, QTreeWidgetItem * previous);

    /// This method is called when an item is double cliked, and open a comment editor
    void onItemDoubleClicked ( QTreeWidgetItem * item, int column );

private :

    // The selector panel
    QPointer< QTreeWidget > m_pSelectorPanel;

};

} // namespace uiPatientDB

#endif // _UIPATIENTDBQT_PATIENTDBGUISELECTORSERVICE_HPP_

