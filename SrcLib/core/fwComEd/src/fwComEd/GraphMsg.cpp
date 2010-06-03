/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwTools/ClassRegistrar.hpp>

#include "fwComEd/GraphMsg.hpp"

REGISTER_BINDING_BYCLASSNAME( ::fwTools::Object, ::fwComEd::GraphMsg, ::fwComEd::GraphMsg );

namespace fwComEd
{

//-----------------------------------------------------------------------------

std::string GraphMsg::NEW_GRAPH            = "NEW_GRAPH";
std::string GraphMsg::ADD_NODE             = "ADD_NODE";
std::string GraphMsg::REMOVE_NODE          = "REMOVE_NODE";
std::string GraphMsg::ADD_EDGE             = "ADD_EDGE";
std::string GraphMsg::REMOVE_EDGE          = "REMOVE_EDGE";
std::string GraphMsg::SELECTED_NODE        = "SELECTED_NODE";
std::string GraphMsg::UNSELECTED_NODE      = "UNSELECTED_NODE";
std::string GraphMsg::SELECTED_OPERATOR    = "SELECTED_OPERATOR";
std::string GraphMsg::EXECUTE_GRAPH        = "EXECUTE_GRAPH";
std::string GraphMsg::CHANGED_NODE_STATE   = "CHANGED_NODE_STATE";

//-----------------------------------------------------------------------------

GraphMsg::GraphMsg() throw()
{}

//-----------------------------------------------------------------------------

GraphMsg::~GraphMsg() throw()
{}

//-----------------------------------------------------------------------------


void GraphMsg::addedNode( ::fwData::Node::sptr node )
{
    addEvent( GraphMsg::ADD_NODE , node );
}

//-----------------------------------------------------------------------------

::fwData::Node::csptr GraphMsg::getAddedNode() const
{
    return ::fwData::Node::dynamicConstCast( getDataInfo( GraphMsg::ADD_NODE ) );
}

//-----------------------------------------------------------------------------

void GraphMsg::removedNode( ::fwData::Node::sptr node )
{
    addEvent( GraphMsg::REMOVE_NODE , node );
}

//-----------------------------------------------------------------------------

::fwData::Node::csptr GraphMsg::getRemovedNode() const
{
    return ::fwData::Node::dynamicConstCast( getDataInfo( GraphMsg::REMOVE_NODE ) );
}

//-----------------------------------------------------------------------------

void GraphMsg::selectedNode( ::fwData::Node::sptr node )
{
    addEvent( GraphMsg::SELECTED_NODE , node );
}

//-----------------------------------------------------------------------------

::fwData::Node::csptr GraphMsg::getSelectedNode() const
{
    return ::fwData::Node::dynamicConstCast( getDataInfo( GraphMsg::SELECTED_NODE ) );
}

//-----------------------------------------------------------------------------

void GraphMsg::unselectedNode( ::fwData::Node::sptr node )
{
    addEvent( GraphMsg::UNSELECTED_NODE , node );
}

//-----------------------------------------------------------------------------

::fwData::Node::csptr GraphMsg::getUnselectedNode() const
{
    return ::fwData::Node::dynamicConstCast( getDataInfo( GraphMsg::UNSELECTED_NODE ) );
}

//-----------------------------------------------------------------------------

} // namespace fwComEd

