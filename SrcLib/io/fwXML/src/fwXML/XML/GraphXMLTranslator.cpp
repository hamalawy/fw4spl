/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <cstring>

#include <boost/lexical_cast.hpp>

#include <fwTools/UUID.hpp>

#include <fwData/Graph.hpp>
#include <fwData/Node.hpp>

#include "fwXML/ObjectTracker.hpp"
#include "fwXML/XML/GraphXMLTranslator.hpp"
#include "fwXML/XML/EdgeXMLTranslator.hpp"
#include "fwXML/XML/XMLTranslatorHelper.hpp"
#include "fwXML/XML/XMLParser.hpp"
#include "fwXML/XML/XMLTranslatorHelper.hpp"

namespace fwXML
{

GraphXMLTranslator::GraphXMLTranslator() {};

//------------------------------------------------------------------------------

GraphXMLTranslator::~GraphXMLTranslator() {};

//------------------------------------------------------------------------------

xmlNodePtr GraphXMLTranslator::getXMLFrom( ::fwData::Object::sptr obj )
{
    ::fwData::Graph::sptr graph = ::fwData::Graph::dynamicCast(obj);
    SLM_ASSERT("graph not instanced", graph);

    // create master node with className+id
    xmlNodePtr node = XMLTH::MasterNode( obj );

    xmlNodePtr NodeXMList  = XMLTH::homogeneousContainerToXml("Nodes",graph->getCRefNodes().begin(),graph->getCRefNodes().end());
    xmlAddChild(node,NodeXMList);

    // append edges
    xmlNodePtr edgesList = xmlNewNode(NULL,  BAD_CAST "Edges");
    xmlAddChild(node,edgesList);
    ::fwData::Graph::ConnectionContainer::const_iterator i;

    EdgeXMLTranslator translator;
    for ( i= graph->getCRefConnections().begin(); i != graph->getCRefConnections().end() ;++i)
    {
        xmlNodePtr anEdge = XMLTH::toXMLRecursive( (*i).first );
        // hack by adding port ptr
        XMLTH::addProp( anEdge , "fromNode", ::fwTools::UUID::get( (*i).second.first  ) );
        XMLTH::addProp( anEdge , "toNode",   ::fwTools::UUID::get( (*i).second.second ) );
        xmlAddChild(edgesList,anEdge);
    }

    // save edges

    return node;
}

//------------------------------------------------------------------------------

void GraphXMLTranslator::updateDataFromXML( ::fwData::Object::sptr toUpdate,  xmlNodePtr source)
{
    assert( XMLTH::check< ::fwData::Graph >(toUpdate, source) );
    ::fwData::Graph::sptr graph = ::fwData::Graph::dynamicCast(toUpdate);

    // get NODES
    xmlNodePtr NodeXMLList = XMLParser::findChildNamed( source, "Nodes");
    SLM_ASSERT("NodeXMLList not instanced", NodeXMLList); // <Nodes> entry must exist
    XMLTH::containerFromXml(NodeXMLList, std::inserter( graph->getRefNodes(), graph->getRefNodes().begin() ) );

    //assert( graph->getNbNodes() );
    if(graph->getNbNodes() != 0)
    {
        // get Edges
        xmlNodePtr edgesList = XMLParser::findChildNamed( source, "Edges");
        SLM_ASSERT("edgesList not instanced", edgesList); // <Nodes> entry must exist

        xmlNodePtr connectionNode = XMLParser::nextXMLElement(edgesList->children);
        while (connectionNode )
        {
            assert( strcmp((const char *)connectionNode->name,"Edge") == 0 );

            ::fwData::Object::sptr obj = XMLTH::fromXML(connectionNode);
            ::fwData::Edge::sptr  edge = ::fwData::Edge::dynamicCast( obj );
            assert ( edge );

            std::string uuidSrcXML = XMLTH::getProp<std::string>(connectionNode,"fromNode");
            std::string uuidDstXML = XMLTH::getProp<std::string>(connectionNode,"toNode");
            std::string uuidSrc = ObjectTracker::xmlID2RuntimeID( uuidSrcXML );
            std::string uuidDst = ObjectTracker::xmlID2RuntimeID(uuidDstXML );

            ::fwData::Node::sptr srcNode = ::fwData::Node::dynamicCast( ::fwTools::UUID::get(uuidSrc) );
            ::fwData::Node::sptr dstNode = ::fwData::Node::dynamicCast( ::fwTools::UUID::get(uuidDst) );
            SLM_ASSERT("srcNode not instanced", srcNode);
            SLM_ASSERT("dstNode not instanced", dstNode);

            // insert edge
            bool success = graph->addEdge(edge,srcNode, dstNode);
            SLM_ASSERT("success not instanced",success);
            FwCoreNotUsedMacro(success);

            // go to next element
            connectionNode = XMLParser::nextXMLElement(connectionNode->next);
        }
    }
}

//------------------------------------------------------------------------------

}
