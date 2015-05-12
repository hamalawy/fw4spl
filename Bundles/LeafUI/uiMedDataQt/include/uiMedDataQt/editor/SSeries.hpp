/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIMEDDATAQT_EDITOR_SSERIES_HPP__
#define __UIMEDDATAQT_EDITOR_SSERIES_HPP__

#include <QObject>
#include <QPointer>

#include <fwCom/Signal.hpp>

#include <gui/editor/IEditor.hpp>

#include "uiMedDataQt/config.hpp"

class QPushButton;

namespace uiMedData
{

namespace widget
{
class PatientEditor;
class StudyEditor;
class EquipmentEditor;
class SeriesEditor;
}

namespace editor
{
/**
 * @brief Shows and edit fwMedData::Series information
 */
class UIMEDDATAQT_CLASS_API SSeries : public QObject,
                                      public ::gui::editor::IEditor
{
Q_OBJECT
public:

    fwCoreServiceClassDefinitionsMacro ( (SSeries)(::gui::editor::IEditor) );

    /// Constructor
    UIMEDDATAQT_API SSeries();

    /// Destructor
    UIMEDDATAQT_API virtual ~SSeries() throw();

    /**
     * @brief Signal type and key triggered when the series has been exported to related series DB.
     * @{ */
    typedef ::fwCom::Signal< void () > SeriesExportedSignalType;
    UIMEDDATAQT_API static const ::fwCom::Signals::SignalKeyType s_SERIES_EXPORTED_SIG;
    /**  @} */

    /**
     * @brief Signal type and key triggered when the service export ability changes
     * @{ */
    typedef ::fwCom::Signal< void ( bool ) > CanExportSignalType;
    UIMEDDATAQT_API static const ::fwCom::Signals::SignalKeyType s_CAN_EXPORT_SIG;
    /**  @} */

    /**
     * @brief Slot triggering export
     * @{ */
    typedef ::fwCom::Slot<void ()> ExportSlotType;
    UIMEDDATAQT_API static const ::fwCom::Slots::SlotKeyType s_EXPORT_SLOT;
    /**  @} */

protected:

    /// Installs GUI : create container and add selector.
    virtual void starting() throw(::fwTools::Failed);

    /// Destroys GUI.
    virtual void stopping() throw(::fwTools::Failed);

    /**
     *
     * @verbatim
       <service uid="selector" impl="::uiMedData::editor::SSeries" type="::gui::editor::IEditor" autoConnect="yes">
        <selectionId>selections</selectionId>
        <selectionMode>single|extended</selectionMode>
        <allowedRemove>yes|no</allowedRemove>
       </service>
       @endverbatim
     * - \b selectionId : defines the fwId of the ::fwData::Vector where the selection will be put or get.
     * - \b selectionMode : defines the selection mode for the series
     * - \b allowedRemove : allows user to remove series
     */
    virtual void configuring() throw (::fwTools::Failed);

    /// Fill selector with the series contained in SeriesDB.
    virtual void updating() throw (::fwTools::Failed);

    /**
     * @brief Manages events (::fwComEd::SeriesDBMsg::ADDED_OBJECTS, ::fwComEd::SeriesDBMsg::REMOVED_OBJECTS)
     *  This method adds/removes series in the selector widget.
     */
    virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

protected Q_SLOTS:

    /// Triggered when export button is clicked
    void onExportClicked();

private:

    /// Widget to allow patient information editing
    QPointer< ::uiMedData::widget::PatientEditor > m_patientEditor;

    /// Widget for study edition/creation
    QPointer< ::uiMedData::widget::StudyEditor > m_studyEditor;

    /// Widget for study edition/creation
    QPointer< ::uiMedData::widget::EquipmentEditor > m_equipmentEditor;

    /// Widget for series edition/creation
    QPointer< ::uiMedData::widget::SeriesEditor > m_seriesEditor;

    /// Widget to export information into target series
    QPointer< QPushButton > m_btnExport;

    /// UID of series to be exported
    std::string m_seriesId;

    /// UID of destination series DB
    std::string m_seriesDBId;

    /// Signal triggered when the series has been exported to the series DB
    SeriesExportedSignalType::sptr m_sigSeriesExported;

    /// Signal triggered when the service export ability changes
    CanExportSignalType::sptr m_sigCanExport;

    /// Slot triggering the export
    ExportSlotType::sptr m_slotExport;
};
} // namespace editor
} // namespace uiMedData

#endif // __UIMEDDATAQT_EDITOR_SSERIES_HPP__

