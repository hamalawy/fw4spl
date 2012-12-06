/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIACQUISITIONQT_ORGAN_LIST_EDITOR_HPP_
#define _UIACQUISITIONQT_ORGAN_LIST_EDITOR_HPP_

#include <QPointer>
#include <QObject>

#include <fwTools/Failed.hpp>

#include <fwData/Reconstruction.hpp>

#include <gui/editor/IEditor.hpp>

#include "uiAcquisitionQt/config.hpp"

class QListWidget;
class QCheckBox;
class QListWidgetItem;

namespace uiAcquisition
{

/**
 * @brief   OrganListEditor service.
 * @class   OrganListEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class UIACQUISITIONQT_CLASS_API OrganListEditor :  public QObject, public ::gui::editor::IEditor
{
    Q_OBJECT
public :

    fwCoreServiceClassDefinitionsMacro ( (OrganListEditor)(::gui::editor::IEditor) ) ;

    /// Constructor. Do nothing.
    UIACQUISITIONQT_API OrganListEditor() throw() ;

    /// Destructor. Do nothing.
    UIACQUISITIONQT_API virtual ~OrganListEditor() throw() ;

protected:

    ///This method launches the IEditor::starting method.
    virtual void starting() throw(::fwTools::Failed);

    ///This method launches the IEditor::stopping method.
    virtual void stopping() throw(::fwTools::Failed);

    /// Managment of observations ( overides )
    virtual void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed);

    virtual void updating() throw(::fwTools::Failed);

    virtual void swapping() throw(::fwTools::Failed);

    virtual void configuring() throw(fwTools::Failed);

    /// Overrides
    virtual void info( std::ostream &_sstream ) ;

    typedef std::map< std::string, ::fwData::Reconstruction::sptr > OrganNameReconstruction;

    void updateReconstructions();

protected Q_SLOTS:

    /// Slot called when new current item is setted in m_organChoice
    void onCurrentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );

    void onShowReconstructions(int state);

    void onOrganChoiceVisibility(QListWidgetItem * item);

private:

    void refreshVisibility();
    QPointer< QCheckBox > m_showCheckBox;
    QPointer< QListWidget > m_organChoice;
    OrganNameReconstruction m_map ;

};

} // uiAcquisition

#endif /*_UIACQUISITIONQT_ORGAN_LIST_EDITOR_HPP_*/


