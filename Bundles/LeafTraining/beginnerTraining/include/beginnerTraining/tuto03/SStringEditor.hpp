/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _BEGINNERTRAINING_TUTO03_SSTRINGEDITOR_HPP_
#define _BEGINNERTRAINING_TUTO03_SSTRINGEDITOR_HPP_

// Qt objects
#include <qtextedit.h>
#include <QPointer>

#include <gui/editor/IEditor.hpp>

#include "beginnerTraining/config.hpp"

namespace beginnerTraining
{
namespace tuto03
{

class BEGINNERTRAINING_CLASS_API SStringEditor : public  QObject, public ::gui::editor::IEditor
{
    Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro ( (SStringEditor)(::gui::editor::IEditor) ) ;

    BEGINNERTRAINING_API SStringEditor();

    BEGINNERTRAINING_API virtual ~SStringEditor() throw();

protected Q_SLOTS:

    BEGINNERTRAINING_API void onTextChanged();

protected :

    /// Overrides
    BEGINNERTRAINING_API virtual void configuring() throw ( ::fwTools::Failed );

    /// Overrides
    BEGINNERTRAINING_API virtual void starting() throw ( ::fwTools::Failed );

    /// Overrides
    BEGINNERTRAINING_API virtual void stopping() throw ( ::fwTools::Failed );

    /// Overrides
    BEGINNERTRAINING_API virtual void updating() throw ( ::fwTools::Failed );

    /// Overrides
    BEGINNERTRAINING_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed );

    /// Overrides
    BEGINNERTRAINING_API virtual void swapping() throw ( ::fwTools::Failed );

    /// Method to notify modification on data
    BEGINNERTRAINING_API void notifyMessage();

    /// Text editor manage by the service
    QPointer<QTextEdit> m_textEditor;
};



} // namespace tuto03
} // namespace beginnerTraining

#endif /*_BEGINNERTRAINING_TUTO03_SSTRINGEDITOR_HPP_*/

