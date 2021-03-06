/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWGUIWX_LOCATIONDIALOG_HPP_
#define _FWGUIWX_LOCATIONDIALOG_HPP_

#include <map>

#include <wx/filedlg.h>

#include <fwCore/base.hpp>
#include <fwGui/dialog/ILocationDialog.hpp>

#include "fwGuiWx/config.hpp"


namespace fwGuiWx
{
namespace dialog
{
/**
 * @brief   Defines the generic file/folder dialog for IHM.
 * @class   LocationDialog
 * 
 * @date    2009-2010.
 *
 */
class FWGUIWX_CLASS_API LocationDialog : public ::fwGui::dialog::ILocationDialog
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (LocationDialog)(::fwGui::dialog::ILocationDialog),
                                            (()),
                                            ::fwGui::factory::New< LocationDialog > );

    FWGUIWX_API LocationDialog(::fwGui::GuiBaseObject::Key key);

    FWGUIWX_API ::fwData::location::ILocation::sptr show();

    FWGUIWX_API ::fwGui::dialog::ILocationDialog& setOption( ::fwGui::dialog::ILocationDialog::Options option);

    /// set the style of location for the dialog
    FWGUIWX_API void setType( ::fwGui::dialog::ILocationDialog::Types type );

    // exemple ( addFilter("images","*.png *.jpg");
    FWGUIWX_API void addFilter(const std::string &filterName, const std::string &wildcardList );

protected:
    unsigned long m_style;
    std::multimap< std::string, std::string > m_filters;
    ::fwGui::dialog::ILocationDialog::Types m_type;

    /// helper to transform m_filters into wx encoding ("BMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif|PNG files (*.png)|*.png"
    wxString fileFilters();

};
} // namespace dialog
} // namespace fwGuiWx

#endif /*_FWGUIWX_LOCATIONDIALOG_HPP_*/


