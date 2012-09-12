/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <wx/intl.h>
#include <wx/textdlg.h>

#include <fwTools/ClassRegistrar.hpp>
#include <fwData/String.hpp>

#include <fwGuiWx/convert.hpp>

#include "fwGuiWx/dialog/InputDialog.hpp"

REGISTER_BINDING( ::fwGui::dialog::IInputDialog, ::fwGuiWx::dialog::InputDialog, ::fwGui::dialog::IInputDialog::FactoryRegistryKeyType , ::fwGui::dialog::IInputDialog::REGISTRY_KEY );

namespace fwGuiWx
{
namespace dialog
{
InputDialog::InputDialog():m_input(""), m_title(""), m_message("")
{}


InputDialog::~InputDialog()
{}

void InputDialog::setTitle( const std::string &title )
{
    m_title = title;
}

void InputDialog::setMessage( const std::string &msg )
{
    m_message = msg;
}
void InputDialog::setInput(const std::string &text)
{
    m_input = text;
}

/// Get the input text in the input field
std::string InputDialog::getInput()
{
    wxString title = ::fwGuiWx::std2wx(m_title);
    wxString message = ::fwGuiWx::std2wx(m_message);
    wxString inputText = ::fwGuiWx::std2wx(m_input);
    ::fwData::String::NewSptr url;
    wxString text =  wxGetTextFromUser( wxGetTranslation(message), wxGetTranslation(title), wxGetTranslation(inputText));
    return (::fwGuiWx::wx2std(text));
}

} // namespace dialog
} // namespace fwGuiWx



