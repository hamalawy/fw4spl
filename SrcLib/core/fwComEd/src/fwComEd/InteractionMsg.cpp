/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <limits>

#include <fwCore/base.hpp>

#include <fwServices/registry/message/macros.hpp>

#include "fwComEd/InteractionMsg.hpp"

fwServicesMessageRegisterMacro( ::fwComEd::InteractionMsg );

namespace fwComEd
{

std::string InteractionMsg::MOUSE_LEFT_UP            = "MOUSE_LEFT_UP";
std::string InteractionMsg::MOUSE_RIGHT_UP           = "MOUSE_RIGHT_UP";
std::string InteractionMsg::MOUSE_MIDDLE_UP          = "MOUSE_MIDDLE_UP";
std::string InteractionMsg::MOUSE_WHEELFORWARD_UP    = "MOUSE_WHEELFORWARD_UP";
std::string InteractionMsg::MOUSE_WHEELBACKWARD_UP   = "MOUSE_WHEELBACKWARD_UP";
std::string InteractionMsg::MOUSE_LEFT_DOWN          = "MOUSE_LEFT_DOWN";
std::string InteractionMsg::MOUSE_RIGHT_DOWN         = "MOUSE_RIGHT_DOWN";
std::string InteractionMsg::MOUSE_MIDDLE_DOWN        = "MOUSE_MIDDLE_DOWN";
std::string InteractionMsg::MOUSE_WHEELFORWARD_DOWN  = "MOUSE_WHEELFORWARD_DOWN";
std::string InteractionMsg::MOUSE_WHEELBACKWARD_DOWN = "MOUSE_WHEELBACKWARD_DOWN";
std::string InteractionMsg::MOUSE_MOVE               = "MOUSE_MOVE";

//-----------------------------------------------------------------------------

InteractionMsg::InteractionMsg(::fwServices::ObjectMsg::Key key) :
        m_eventPoint(::fwData::Point::New()),
        m_modifiersStatus(NONE),
        m_eventTimestamp(0.)
{
}

//-----------------------------------------------------------------------------

InteractionMsg::~InteractionMsg() throw()
{}

//-----------------------------------------------------------------------------

void InteractionMsg::setEvent(std::string event)
{
    SLM_ASSERT("InteractionMsg cannot handle several events in the same message",
            m_eventId2DataInfo.size()==0 );
    addEvent(event);
}

//-----------------------------------------------------------------------------

void InteractionMsg::setModifiersStatus(Modifiers k, bool state)
{
    if (state)
    {
        m_modifiersStatus = static_cast<unsigned char>(m_modifiersStatus | k);
    }
    else
    {
        m_modifiersStatus = static_cast<unsigned char>( m_modifiersStatus & (std::numeric_limits<unsigned char>::max() - k));
    }
}

//-----------------------------------------------------------------------------

bool InteractionMsg::getModifiersStatus(Modifiers k) const
{
    return m_modifiersStatus & k;
}

//-----------------------------------------------------------------------------

void InteractionMsg::setEventPoint(::fwData::Point::csptr point)
{
    SLM_ASSERT("Null point pointer", point);
    m_eventPoint = ::fwData::Object::copy(point);
}

//-----------------------------------------------------------------------------

void InteractionMsg::setEventPoint(PointCoordType x, PointCoordType y, PointCoordType z)
{
    ::fwData::Point::PointCoordArrayType &coords = m_eventPoint->getRefCoord();
    coords[0] = x;
    coords[1] = y;
    coords[2] = z;
}


//-----------------------------------------------------------------------------
::fwData::Point::csptr InteractionMsg::getEventPoint() const
{
    return m_eventPoint;
}

//-----------------------------------------------------------------------------

void InteractionMsg::setEventTimestamp(::fwCore::HiResClock::HiResClockType timestamp)
{
    m_eventTimestamp = timestamp;
}

//-----------------------------------------------------------------------------

::fwCore::HiResClock::HiResClockType InteractionMsg::getEventTimestamp() const
{
    return m_eventTimestamp;
}

//-----------------------------------------------------------------------------

} // namespace fwComEd

