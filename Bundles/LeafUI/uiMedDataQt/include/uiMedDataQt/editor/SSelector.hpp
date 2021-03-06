/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIMEDDATAQT_EDITOR_SSELECTOR_HPP_
#define _UIMEDDATAQT_EDITOR_SSELECTOR_HPP_

#include <QAbstractItemView>
#include <QPointer>
#include <QObject>
#include <QVector>

#include <fwData/Vector.hpp>

#include <gui/editor/IEditor.hpp>

#include "uiMedDataQt/config.hpp"
#include "uiMedDataQt/widget/Selector.hpp"


namespace uiMedData
{
namespace editor
{
/**
 * @brief   This editor shows information about the medical data. It allows to manipulate
 *          (select, erase, ...) studies and series.
 * @class   SSelector
 * 
 * @date    2013.
 */
class UIMEDDATAQT_CLASS_API SSelector : public QObject, public ::gui::editor::IEditor
{
    Q_OBJECT
public :
    fwCoreServiceClassDefinitionsMacro ( (SSelector)(::gui::editor::IEditor) ) ;

    /// Constructor
    UIMEDDATAQT_API SSelector();

    /// Destructor
    UIMEDDATAQT_API virtual ~SSelector() throw() ;

    typedef ::fwCom::Signal< void ( SPTR( ::fwMedData::Series ) ) > SeriesDoubleClickedSignalType;

    /// Key in m_signals map of signal m_sigSeriesDoubleClicked
    UIMEDDATAQT_API static const ::fwCom::Signals::SignalKeyType s_SERIES_DOUBLE_CLICKED_SIG;


protected:

    /// Installs GUI : create container and add selector.
    virtual void starting() throw(::fwTools::Failed);

    /// Destroys GUI.
    virtual void stopping() throw(::fwTools::Failed);

    /**
     *
     * @verbatim
     <service uid="selector" impl="::uiMedData::editor::SSelector" type="::gui::editor::IEditor" autoConnect="yes">
        <selectionId>selections</selectionId>
        <selectionMode>single|extended</selectionMode>
        <allowedRemove>yes|no</allowedRemove>
        <insertMode>yes|no</insertMode>
        <icons>
            <icon series="::fwMedData::ImageSeries" icon="Bundles/media_0-1/icons/ImageSeries.svg" />
            <icon series="::fwMedData::ModelSeries" icon="Bundles/media_0-1/icons/ModelSeries.svg" />
        </icons>
     </service>
     @endverbatim
     * - \b selectionId : defines the fwId of the ::fwData::Vector where the selection will be put or get.
     * - \b selectionMode : defines the selection mode for the series
     * - \b allowedRemove : allows user to remove series
     * - \b insertMode : only allows selection of uiMedData::InsertSeries
     * - \b icons : defines the icon to associate for a series
     *    - \b series : the series classname
     *    - \b icon : the icon path

     */
    virtual void configuring() throw (::fwTools::Failed);

    /// Fill selector with the series contained in SeriesDB.
    virtual void updating() throw (::fwTools::Failed);

    /**
     * @brief Manages events (::fwComEd::SeriesDBMsg::ADDED_OBJECTS, ::fwComEd::SeriesDBMsg::REMOVED_OBJECTS)
     *  This method adds/removes series in the selector widget.
     */
    virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    virtual void info( std::ostream &_sstream );



protected Q_SLOTS:

    /**
     * @brief Manages the selection vector according to selection/deselection.
     * @param[in] selection series to add in selection vector.
     * @param[in] deselection series to remove from selection vector.
     */
    void onSelectedSeries(QVector< ::fwMedData::Series::sptr > selection,
                          QVector< ::fwMedData::Series::sptr > deselection);

    /**
     * @brief Send a 'seriesDoubleClicked' signal when the user double click on a series. This signal holds the
     * clicked series.
     * @param[in] index index of the clicked item in the selector.
     * @todo  Manages double click on a study.
     */
    void onDoubleClick(const QModelIndex &index);

    /**
     * @brief Removes series from seriesDB and notify.
     * @param[in] selection series to remove from seriesDB.
     */
    void onRemoveSeries(QVector< ::fwMedData::Series::sptr > selection);

private :

    /// Returns current selection vector given by its fwID m_selectionId.
    ::fwData::Vector::sptr getSelection();

    /// fwID of the vector of selections
    std::string m_selectionId;

    /// Selector widget.
    QPointer< ::uiMedData::widget::Selector > m_selectorWidget;

    /// Signal emitted when there is a double click on a series
    SeriesDoubleClickedSignalType::sptr m_sigSeriesDoubleClicked;
    
    /// Map containing the specified icons for a series (map\<series classname, icon path\>)
    ::uiMedData::widget::Selector::SeriesIconType m_seriesIcons;

    /// Permits the user to remove series. Default is yes
    bool m_allowedRemove;

    /// Change the behaviour of the treeview selection mode
    QAbstractItemView::SelectionMode m_selectionMode;

    /// If true, allows selection of uiMedData::InsertSeries only
    bool m_insertMode;

};
} // namespace editor
} // namespace uiMedData

#endif // _UIMEDDATAQT_EDITOR_SSELECTOR_HPP_

