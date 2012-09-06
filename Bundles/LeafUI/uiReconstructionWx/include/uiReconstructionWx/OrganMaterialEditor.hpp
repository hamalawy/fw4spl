/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIRECONSTRUCTIONWX_ORGAN_MATERIAL_EDITOR_HPP_
#define _UIRECONSTRUCTIONWX_ORGAN_MATERIAL_EDITOR_HPP_

#include <wx/slider.h>
#include <wx/clrpicker.h>

#include <fwTools/Failed.hpp>

#include <gui/editor/IEditor.hpp>

#include "uiReconstructionWx/config.hpp"

namespace uiReconstruction
{
/**
 * @brief   OrganMaterialEditor service.
 * @class   OrganMaterialEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class UIRECONSTRUCTIONWX_CLASS_API OrganMaterialEditor : public ::gui::editor::IEditor
{

public :

    fwCoreServiceClassDefinitionsMacro ( (OrganMaterialEditor)(::gui::editor::IEditor) ) ;

    /// Constructor. Do nothing.
    UIRECONSTRUCTIONWX_API OrganMaterialEditor() throw() ;

    /// Destructor. Do nothing.
    UIRECONSTRUCTIONWX_API virtual ~OrganMaterialEditor() throw() ;

protected:


    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

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

    void refreshMaterial( );
    void materialNotification( );
    void onOpacitySlider(wxCommandEvent & event );
    void onColorButton(wxColourPickerEvent & event );

private:

    wxColourPickerCtrl * m_colourButton ;
    wxSlider * m_opacitySlider ;

};

} // uiReconstruction

#endif /*_UIRECONSTRUCTIONWX_ORGAN_MATERIAL_EDITOR_HPP_*/


