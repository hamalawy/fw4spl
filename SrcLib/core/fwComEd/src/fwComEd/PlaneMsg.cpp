/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/registry/message/macros.hpp>

#include "fwComEd/PlaneMsg.hpp"

fwServicesMessageRegisterMacro( ::fwComEd::PlaneMsg );

namespace fwComEd
{

//-----------------------------------------------------------------------------

std::string PlaneMsg::PLANE_MODIFIED          = "PLANE_MODIFIED";
std::string PlaneMsg::START_PLANE_INTERACTION = "START_PLANE_INTERACTION";
std::string PlaneMsg::DESELECT_PLANE          = "DESELECT_PLANE";
std::string PlaneMsg::WAS_SELECTED            = "WAS_SELECTED";
std::string PlaneMsg::WAS_DESELECTED          = "WAS_DESELECTED";

//-----------------------------------------------------------------------------

PlaneMsg::PlaneMsg(::fwServices::ObjectMsg::Key key)
{
}

//-----------------------------------------------------------------------------

PlaneMsg::~PlaneMsg() throw()
{
}

//-----------------------------------------------------------------------------

} // namespace fwComEd


