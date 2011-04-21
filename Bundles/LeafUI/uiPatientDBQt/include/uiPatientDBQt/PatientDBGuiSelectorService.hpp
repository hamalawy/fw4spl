#ifndef _UIPATIENTDBQT_PATIENTDBGUISELECTORSERVICE_HPP_
#define _UIPATIENTDBQT_PATIENTDBGUISELECTORSERVICE_HPP_

#include <QPointer>
#include <QObject>
#include <QTreeWidget>

#include <gui/editor/IEditor.hpp>

#include "uiPatientDBQt/config.hpp"


namespace uiPatientDB
{

/**
 * @brief   This editor show the list of patient and acquisition in the PatientDB. It allows to select an acquisition.
 * @class   PatientDBGuiSelectorService.
 * @author  IRCAD (Research and Development Team).
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
    void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed);

    void info( std::ostream &_sstream ) ;

protected slots:

    /**
    * @brief This method is called when the value change.
    */
    void onSelectionChange(QTreeWidgetItem * current, QTreeWidgetItem * previous);

private :

    // The selector panel
    QPointer< QTreeWidget > m_pSelectorPanel;

};

} // namespace uiPatientDB

#endif // _UIPATIENTDBQT_PATIENTDBGUISELECTORSERVICE_HPP_

