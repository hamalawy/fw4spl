/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIVISUWX_SNAPSHOTEDITOR_HPP_
#define _UIVISUWX_SNAPSHOTEDITOR_HPP_

#include <fwTools/Failed.hpp>
#include <gui/editor/IEditor.hpp>

#include "uiVisuWx/config.hpp"

namespace uiVisu
{

/**
 * @brief   SnapshotEditor service is represented by a button. It allows to snap shot a generic scene.
 * @class   SnapshotEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class UIVISUWX_CLASS_API SnapshotEditor : public ::gui::editor::IEditor
{

public :

    fwCoreServiceClassDefinitionsMacro ( (SnapshotEditor)(::gui::editor::IEditor) ) ;

    /// Constructor. Do nothing.
    UIVISUWX_API SnapshotEditor() throw() ;

    /// Destructor. Do nothing.
    UIVISUWX_API virtual ~SnapshotEditor() throw() ;

protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    /**
     * @brief Install the layout.
     */
    virtual void starting() throw(::fwTools::Failed);

    /**
     * @brief Destroy the layout.
     */
    virtual void stopping() throw(::fwTools::Failed);

    /// Do nothing
    virtual void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed);

    /// Do nothing
    virtual void updating() throw(::fwTools::Failed);

    /// Do nothing
    virtual void swapping() throw(::fwTools::Failed);

    /**
     * @brief Configure the editor.
     *
     * Example of configuration
     * @verbatim
    <service uid="snapshotEditor" type="::gui::editor::IEditor" implementation="::uiVisu::SnapshotEditor" autoComChannel="no">
        <snap>
            <scene uid="genericScene" />
        </snap>
    </service>
       @endverbatim
       \b genericScene is the uid of the ::fwRenderVTK::VtkRenderService representing the generic scene which will be printed.
     */
    virtual void configuring() throw(fwTools::Failed);

    /// Overrides
    virtual void info( std::ostream &_sstream ) ;

    /**
     * @brief Show a file dialog and notify the scene must be printed.
     */
    void onSnapButton( wxCommandEvent& event );

private:
    std::string requestFileName();

    std::vector< std::string > m_scenesUID;
    int m_idSnapButton;

};

} // uiVisu

#endif /*_UIVISUWX_SNAPSHOTEDITOR_HPP_*/


