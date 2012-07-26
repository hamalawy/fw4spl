/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/registry/message/macros.hpp>

#include "fwComEd/VideoMsg.hpp"

fwServicesMessageRegisterMacro( ::fwComEd::VideoMsg );

namespace fwComEd
{

//-----------------------------------------------------------------------------

std::string VideoMsg::VIDEO_IS_REFRESHED="VIDEO_IS_REFRESHED";
std::string VideoMsg::VIDEO_IS_INITIALIZED="VIDEO_IS_INITIALIZED";

//-----------------------------------------------------------------------------

VideoMsg::VideoMsg(::fwServices::ObjectMsg::Key key)
{}

//-----------------------------------------------------------------------------

VideoMsg::~VideoMsg() throw()
{}

//-----------------------------------------------------------------------------

} // namespace fwComEd

