/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */



#ifndef _FWWX_CONVERT_HPP_
#define _FWWX_CONVERT_HPP_

#include <string>

#ifdef __WXMSW__
#   include <wx/msw/winundef.h>
#endif
#include <wx/string.h>

#include "fwWX/config.hpp"

#ifdef __WXMAC__
#include <wx/event.h>
wxDECLARE_EVENT(wxEventFwOpen, wxCommandEvent);
#define wxIDEventFwOpen 27001
#endif

namespace fwWX
{
    FWWX_API wxString std2wx( const std::string & value );
    FWWX_API std::string wx2std( const wxString & value );
}
#endif
