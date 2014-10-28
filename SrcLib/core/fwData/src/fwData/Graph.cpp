/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <utility>


#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Signals.hpp>


#include <boost/foreach.hpp>

#include "fwData/registry/macros.hpp"
#include "fwData/Exception.hpp"

#include "fwData/Edge.hpp"
#include "fwData/Port.hpp"
#include "fwData/Node.hpp"
#include "fwData/Graph.hpp"

fwDataRegisterMacro( ::fwData::Graph );

namespace fwData
{

const bool Graph::UP_STREAM = true;
const bool Graph::DOWN_STREAM = false;

const ::fwCom::Signals::SignalKeyType Graph::s_UPDATED_SIG = "updated";

//------------------------------------------------------------------------------

Graph::Graph(::fwData::Object::Key key) :
    m_sigUpdated(UpdatedSignalType::New())
{
    // Init

    // Register
    m_signals( s_UPDATED_SIG,  m_sigUpdated);

#ifdef COM_LOG
    ::fwCom::HasSignals::m_signals.setID();
#endif

}

//------------------------------------------------------------------------------

Graph::~Graph()
{}

//------------------------------------------------------------------------------

bool Graph::addNode( Node::sptr node)
{
    return m_nodes.insert(node).second;
}

//------------------------------------------------------------------------------

bool Graph::removeNode( Node::csptr node)
{
    // test if connected edge to it
    if ( haveConnectedEdges(node) )
    {
        return false;
    }
    return (m_nodes.erase( Node::constCast(node) ) > 0 );
}

//------------------------------------------------------------------------------

Graph::NodeContainer &Graph::getRefNodes()
{
    return m_nodes;
}

//------------------------------------------------------------------------------

const Graph::NodeContainer &Graph::getCRefNodes() const
{
    return m_nodes;
}

//------------------------------------------------------------------------------

bool Graph::haveConnectedEdges(Node::csptr node ) const
{
    for ( ConnectionContainer::const_iterator i=m_connections.begin() ; i !=  m_connections.end() ; ++i )
    {
        if ( i->second.first == node ||  i->second.second == node)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------

Edge::sptr Graph::makeConnection(
        Node::csptr nodeSource,
        std::string nodeSourceOutputPortID,
        Node::csptr nodeDestination,
        std::string nodeDestinationInputPortID,
        std::string EdgeNature )
{
    ::fwData::Edge::sptr nEdge = ::fwData::Edge::New();
    nEdge->setIdentifiers( nodeSourceOutputPortID, nodeDestinationInputPortID );
    nEdge->setNature( EdgeNature );
    if ( addEdge( nEdge, nodeSource, nodeDestination ) )
    {
          return nEdge;// success return new Edge
    }
    else
    {
        return ::fwData::Edge::sptr(); // failure
    }
}

//------------------------------------------------------------------------------

bool Graph::addEdge(Edge::sptr edge, Node::csptr nodeSource, Node::csptr nodeDestination)
{
    // edge not already recorded
    if (m_connections.find( edge ) !=  m_connections.end() )
    {
        return false; // edge already stored
    }
    // node registred ?
    if (m_nodes.find( Node::constCast(nodeSource) ) ==  m_nodes.end() )
    {
        return false; // node already stored
    }
    // node registred ?
    if ( m_nodes.find( Node::constCast(nodeDestination) ) ==  m_nodes.end() )
    {
        return false; // node already stored
    }

    // test port existance
    Port::sptr sourcePort = nodeSource->findPort( edge->getIdentifiers().first, DOWN_STREAM );
    if ( !sourcePort )
    {
        return false; // port doesn't exist
    }

    // test port existance
    Port::sptr sourceDest= nodeDestination->findPort( edge->getIdentifiers().second , UP_STREAM );
    if ( !sourceDest )
    {
        return false; // port doesn't exist
    }


    // test port compatibility
    if ( sourcePort->getType() !=  sourceDest->getType() )
    {
        return false; // incompatible type
    }

    m_connections[ edge ] = std::make_pair(  Node::constCast(nodeSource), Node::constCast(nodeDestination) );

    return true;
}

//------------------------------------------------------------------------------

bool Graph::removeEdge(Edge::sptr edge)
{
    return ( m_connections.erase(edge) != 0 );
}

//------------------------------------------------------------------------------

Node::sptr Graph::getSourceNode(Edge::sptr edge)
{
    return getNode( edge, UP_STREAM );
}

//------------------------------------------------------------------------------

Node::sptr Graph::getDestinationNode( Edge::sptr edge )
{
    return getNode( edge, DOWN_STREAM );
}

//------------------------------------------------------------------------------

Node::sptr Graph::getNode( Edge::sptr edge, bool upStream )
{
    ConnectionContainer::iterator i;
    i = m_connections.find( edge );

    // check edge is valid ?
    if ( i ==  m_connections.end() )
    {
        return Node::sptr();
    }

    if( upStream )
    {
        return (*i).second.first;
    }
    else
    {
        return (*i).second.second;
    }
}

//------------------------------------------------------------------------------

std::vector< Edge::sptr > Graph::getInputEdges( Node::csptr node )
{
    return getEdges( node, UP_STREAM );
}

//------------------------------------------------------------------------------

std::vector< Edge::sptr > Graph::getOutputEdges( Node::csptr node )
{
    return getEdges( node, DOWN_STREAM );
}

//------------------------------------------------------------------------------

std::vector< Edge::sptr > Graph::getEdges( const Node::csptr &node, bool upStream,
                                           const std::string &nature,
                                           const std::string &portID
                                           )
{
    SLM_ASSERT("Node " + node->getID()  + " not found in graph", m_nodes.find( Node::constCast(node) ) != m_nodes.end());
    SLM_ASSERT("Port " + portID  + " not found on node" + node->getID(),
               portID.empty() || node->findPort(portID, upStream));

    std::vector< Edge::sptr > result;
    result.reserve(4);

    ConnectionContainer::const_iterator end = m_connections.end();
    for ( ConnectionContainer::const_iterator i=m_connections.begin(); i != end; ++i )
    {
        const Edge::sptr &edge = i->first;
        const Node::sptr &nodeFrom = i->second.first;
        const Node::sptr &nodeTo = i->second.second;

        bool isConnectedEdge = ( upStream ? nodeTo : nodeFrom ) == node ;
        if( !isConnectedEdge)
        {
            continue;
        }

        bool isCorrectPort = portID.empty() || portID == ( upStream ? edge->getToPortID() : edge->getFromPortID() );
        if( !isCorrectPort)
        {
            continue;
        }

        bool isCorrectNature =  nature.empty() || edge->getNature() == nature;
        if( !isCorrectNature)
        {
            continue;
        }

        result.push_back( edge );
    }

    return result;
}

//------------------------------------------------------------------------------

std::vector< ::fwData::Node::sptr >
Graph::getNodes(
        const ::fwData::Node::csptr &node,
        bool upStream,
        const std::string &nature,
        const std::string &portID )
{
    std::vector< Edge::sptr > edges;
    edges = getEdges( node, upStream, nature, portID);

    std::vector< Node::sptr > nodes;
    BOOST_FOREACH( Edge::sptr edge, edges )
    {
        Node::sptr distantNode;
        distantNode = ( upStream ? m_connections[edge].first : m_connections[edge].second );

        if( std::find( nodes.begin(), nodes.end(), distantNode ) == nodes.end() )
        {
            nodes.push_back(distantNode);
        }
    }

    return nodes;
}

//------------------------------------------------------------------------------

size_t Graph::getNbNodes() const
{
    return m_nodes.size();
}

//------------------------------------------------------------------------------

size_t Graph::getNbEdges() const
{
    return m_connections.size();
}

//------------------------------------------------------------------------------

const Graph::ConnectionContainer &Graph::getCRefConnections() const
{
    return m_connections;
}

//------------------------------------------------------------------------------

Graph::ConnectionContainer &Graph::getRefConnections()
{
    return m_connections;
}

//------------------------------------------------------------------------------

void Graph::shallowCopy(const Object::csptr &_source )
{
    Graph::csptr other = Graph::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );
    m_nodes = other->m_nodes;
    m_connections = other->m_connections;
}

//------------------------------------------------------------------------------

void Graph::cachedDeepCopy(const Object::csptr &_source, DeepCopyCacheType &cache)
{
    Graph::csptr other = Graph::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source, cache );

    std::map< ::fwData::Node::sptr, ::fwData::Node::sptr > correspondenceBetweenNodes;
    typedef std::pair< Edge::sptr,  std::pair<  Node::sptr,  Node::sptr > > ConnectionContainerElt;

    m_nodes.clear();
    BOOST_FOREACH(const ::fwData::Node::sptr &node, other->m_nodes)
    {
        ::fwData::Node::sptr newNode = ::fwData::Object::copy(node, cache);
        bool addOK =this->addNode(newNode);
        OSLM_ASSERT("Node "<<newNode->getID() <<" can't be added ", addOK );
        FwCoreNotUsedMacro(addOK);
        correspondenceBetweenNodes.insert(std::make_pair(node, newNode));
    }

    m_connections.clear();
    BOOST_FOREACH(const ConnectionContainerElt &connection, other->m_connections)
    {
        // Edge deep copy .
        ::fwData::Edge::sptr newEdge = ::fwData::Object::copy(connection.first, cache);
        ::fwData::Node::sptr oldNode1 = (connection.second).first;
        ::fwData::Node::sptr oldNode2 = (connection.second).second;
        if ((correspondenceBetweenNodes.find(Node::constCast(oldNode1))!= correspondenceBetweenNodes.end())
             && (correspondenceBetweenNodes.find(Node::constCast(oldNode2)) != correspondenceBetweenNodes.end()))
        {
            // Add new Edge
            this->addEdge(newEdge, correspondenceBetweenNodes[oldNode1], correspondenceBetweenNodes[oldNode2]);
        }
    }
    correspondenceBetweenNodes.clear();
}

//------------------------------------------------------------------------------

} // namespace fwData
