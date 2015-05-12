/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/registry/macros.hpp"
#include "fwData/Exception.hpp"

#include "fwData/Edge.hpp"

fwDataRegisterMacro( ::fwData::Edge );
namespace fwData
{
std::string Edge::NATURE_FLOW = "flow";
std::string Edge::NATURE_DATA = "data";

//------------------------------------------------------------------------------

Edge::Edge( ::fwData::Object::Key key ) :
    m_fromPortIdentifier("not defined"), m_toPortIdentifier("not defined"), m_nature("not defined")
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

Edge::~Edge()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void Edge::setIdentifiers(const std::string & fromPortIndentifier, const std::string & toPortIndentifier)
{
    m_fromPortIdentifier = fromPortIndentifier;
    m_toPortIdentifier   = toPortIndentifier;
}

//------------------------------------------------------------------------------

std::pair<std::string,std::string> Edge::getIdentifiers() const
{
    return make_pair(m_fromPortIdentifier, m_toPortIdentifier);
}

//------------------------------------------------------------------------------

std::string Edge::getFromPortID() const
{
    return m_fromPortIdentifier;
}

//------------------------------------------------------------------------------

std::string Edge::getToPortID() const
{
    return m_toPortIdentifier;
}

//------------------------------------------------------------------------------


std::string Edge::getPortID(bool upStream) const
{
    return upStream ? m_fromPortIdentifier : m_toPortIdentifier;
}

//------------------------------------------------------------------------------

void Edge::setNature(std::string nature)
{
    m_nature = nature;
}

//------------------------------------------------------------------------------

const std::string &Edge::getNature() const
{
    return m_nature;
}

//------------------------------------------------------------------------------
void Edge::shallowCopy(const Object::csptr &_source )
{
    Edge::csptr other = Edge::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );

    this->fieldShallowCopy( _source );
    m_fromPortIdentifier = other->m_fromPortIdentifier;
    m_toPortIdentifier   = other->m_toPortIdentifier;
    m_nature             = other->m_nature;
}

//------------------------------------------------------------------------------

void Edge::cachedDeepCopy(const Object::csptr &_source, DeepCopyCacheType &cache)
{
    Edge::csptr other = Edge::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source, cache );
    m_fromPortIdentifier = other->m_fromPortIdentifier;
    m_toPortIdentifier   = other->m_toPortIdentifier;
    m_nature             = other->m_nature;

}

//------------------------------------------------------------------------------

} // namespace fwData
