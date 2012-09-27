/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUIQT_EDITOR_DUMMYIEDITOR_HPP_
#define _GUIQT_EDITOR_DUMMYIEDITOR_HPP_

#include <QPointer>
#include <QLabel>

#include <fwTools/Failed.hpp>

#include <gui/editor/IEditor.hpp>

#include "guiQt/config.hpp"

namespace gui
{

namespace editor
{


/**
 * @brief   Defines the service interface managing the basic editor service for object. Do nothing.
 * @class   DummyEditor.
 * @author  IRCAD (Research and Development Team).

 * @date    2009.
 *
 * @todo ACH: This class has been created in order to build test application. Do we remove it now ??
 */
class GUIQT_CLASS_API DummyEditor : public ::gui::editor::IEditor
{

public :


    fwCoreServiceClassDefinitionsMacro ( (DummyEditor)(::gui::editor::IEditor) ) ;

    /// Constructor. Do nothing.
    GUIQT_API DummyEditor() throw() ;

    /// Destructor. Do nothing.
    GUIQT_API virtual ~DummyEditor() throw() ;

protected:

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */
    /**
     * @brief This method launches the IEditor::starting method.
     */
    GUIQT_API virtual void starting() throw( ::fwTools::Failed ) ;

    /**
     * @brief This method launches the IEditor::stopping method.
     */
    GUIQT_API virtual void stopping() throw( ::fwTools::Failed );

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
    GUIQT_API virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed) ;

    /**
     * @brief This method is used to update services. Do nothing.
     */
    GUIQT_API virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief This method is used to configure the class parameters. Do nothing.
    */
    GUIQT_API virtual void configuring() throw( ::fwTools::Failed );

    /**
     * @brief This method is used to give information about the service. Do nothing.
     */
    GUIQT_API virtual void info(std::ostream &_sstream );

    ///@}

private:
    /**
     * @brief optional text
     */
    std::string m_text;

    QPointer< QLabel > m_staticText;
};

}
}

#endif /*_GUIQT_EDITOR_DUMMYIEDITOR_HPP_*/


