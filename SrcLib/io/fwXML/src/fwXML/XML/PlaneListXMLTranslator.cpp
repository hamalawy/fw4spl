/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>
//#include <fwData/String.hpp>

#include "fwXML/XML/PlaneListXMLTranslator.hpp"
#include "fwXML/XML/XMLParser.hpp"
#include "fwXML/XML/XMLTranslatorHelper.hpp"
#include "fwXML/Serializer.hpp"

namespace fwXML
{

//------------------------------------------------------------------------------

PlaneListXMLTranslator::PlaneListXMLTranslator() {};

//------------------------------------------------------------------------------

PlaneListXMLTranslator::~PlaneListXMLTranslator() {};

//------------------------------------------------------------------------------

xmlNodePtr PlaneListXMLTranslator::getXMLFrom( ::fwData::Object::sptr obj )
{

    ::fwData::PlaneList::sptr pPlaneList = ::fwData::PlaneList::dynamicCast(obj);
    SLM_ASSERT("pPlaneList not instanced", pPlaneList);

    // create master node with className+id
    xmlNodePtr masterNode = XMLTranslatorHelper::MasterNode( obj );

    if( ! pPlaneList->getCRefPlanes().empty() )
    {
        xmlNodePtr planesNode =
                XMLTH::homogeneousContainerToXml("planes", pPlaneList->getCRefPlanes().begin(),  pPlaneList->getCRefPlanes().end() );
        xmlAddChild( masterNode , planesNode);
    }

    return masterNode;

}

//------------------------------------------------------------------------------

void PlaneListXMLTranslator::updateDataFromXML( ::fwData::Object::sptr toUpdate,  xmlNodePtr source)
{
    SLM_ASSERT("toUpdate not instanced", toUpdate); // object should exist
    SLM_ASSERT("source not instanced", source);
    //get its label
    ::fwData::PlaneList::sptr pPlaneList = ::fwData::PlaneList::dynamicCast(toUpdate);
    pPlaneList->getRefPlanes().clear();

    xmlNodePtr planesNode = XMLParser::getChildrenXMLElement( source );
    // If the plane list is not empty
    if ( planesNode  )
    {
        assert(  !xmlStrcmp( planesNode->name, BAD_CAST  "planes") );
        xmlNodePtr planeNode = XMLParser::nextXMLElement( planesNode->children );
        SLM_ASSERT("planeNode not instanced", planeNode);
        while ( planeNode )
        {
            std::string nodeName = (const char *) planeNode->name;
            if ( nodeName == "Plane" ) // className
            {
                // Load Plane
                ::fwData::Object::sptr valueObj;
                valueObj = Serializer().ObjectsFromXml( planeNode, true );

                // Add plane in the vector
                SLM_ASSERT("valueObj not instanced", valueObj);
                assert( ::fwData::Plane::dynamicCast( valueObj ) );
                pPlaneList->getRefPlanes().push_back( ::fwData::Plane::dynamicCast( valueObj ) );
            }
            planeNode = XMLParser::nextXMLElement( planeNode->next );
        }
    }
}

}
