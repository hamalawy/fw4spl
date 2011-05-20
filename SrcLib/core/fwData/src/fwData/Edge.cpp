/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwTools/ClassRegistrar.hpp>

#include "fwData/Edge.hpp"

REGISTER_BINDING_BYCLASSNAME( ::fwTools::Object, ::fwData::Edge, ::fwData::Edge );
namespace fwData {
std::string Edge::NATURE_FLOW = "flow";
std::string Edge::NATURE_DATA = "data";

//------------------------------------------------------------------------------

Edge::Edge() : m_fromPortIdentifier("not defined"), m_toPortIdentifier("not defined"), m_nature("not defined")
{
    // TODO Auto-generated constructor stub
}

//------------------------------------------------------------------------------

Edge::~Edge()
{
    // TODO Auto-generated destructor stub
}

//------------------------------------------------------------------------------

void Edge::setIdentifiers(const std::string & fromPortIndentifier, const std::string & toPortIndentifier)
{
    m_fromPortIdentifier = fromPortIndentifier;
    m_toPortIdentifier = toPortIndentifier;
}

//------------------------------------------------------------------------------

std::pair<std::string,std::string> Edge::getIdentifiers() const
{
    return make_pair(m_fromPortIdentifier , m_toPortIdentifier);
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
    return upStream?m_fromPortIdentifier:m_toPortIdentifier;
}

//------------------------------------------------------------------------------

void Edge::setNature(std::string nature)
{
    m_nature =  nature;
}

//------------------------------------------------------------------------------

const std::string &Edge::getNature() const
{
    return m_nature;
}

//------------------------------------------------------------------------------
void Edge::shallowCopy( Edge::csptr _source )
{

    ::fwTools::Object::shallowCopyOfChildren( _source );
    this->m_fromPortIdentifier = _source->m_fromPortIdentifier;
    this->m_toPortIdentifier = _source->m_toPortIdentifier;
    this->m_nature = _source->m_nature;
}

//------------------------------------------------------------------------------

void Edge::deepCopy( Edge::csptr _source )
{
    ::fwTools::Object::deepCopyOfChildren( _source );
    this->m_fromPortIdentifier = _source->m_fromPortIdentifier;
    this->m_toPortIdentifier = _source->m_toPortIdentifier;
    this->m_nature = _source->m_nature;

}

//------------------------------------------------------------------------------

} // namespace fwData
