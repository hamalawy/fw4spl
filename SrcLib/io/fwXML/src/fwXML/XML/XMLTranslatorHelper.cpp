/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwTools/UUID.hpp>
#include <fwData/Object.hpp>
#include <fwXML/visitor/accept.hpp>

#include "fwXML/ObjectTracker.hpp"
#include "fwXML/XML/XMLTranslatorHelper.hpp"
#include "fwXML/XML/TrivialXMLTranslator.hpp"
#include "fwXML/visitor/SerializeXML.hpp"

namespace fwXML
{

XMLTranslatorHelper::XMLTranslatorHelper()
{}

//------------------------------------------------------------------------------

XMLTranslatorHelper::~XMLTranslatorHelper()
{}

//------------------------------------------------------------------------------

xmlNodePtr XMLTranslatorHelper::MasterNode( ::fwData::Object::sptr obj )
{
    // create node with className
    xmlNodePtr node = xmlNewNode(NULL, xmlStrdup( BAD_CAST obj->getLeafClassname().c_str() ) );

    // append an unique id to its objects
    std::string id = ::fwTools::UUID::get(obj);

    xmlNewProp( node, BAD_CAST "id", xmlStrdup( BAD_CAST  id.c_str() ));
    xmlNewProp( node, BAD_CAST "class", xmlStrdup( BAD_CAST  obj->getRootedClassname().c_str() ));

    return node;
}

//------------------------------------------------------------------------------

xmlNodePtr XMLTranslatorHelper::toXMLRecursive( ::fwData::Object::sptr obj )
{
    ::visitor::SerializeXML visitor;
    ::fwData::visitor::accept( obj , &visitor );
    return visitor.m_correspondance[ obj ];
}

//------------------------------------------------------------------------------

void XMLTranslatorHelper::fromXML( ::fwData::Object::sptr toUpdate, xmlNodePtr source )
{
    const std::string nameInXML = (const char*)source->name;
    SLM_ASSERT("XML node not correspond to object classname", toUpdate->getLeafClassname() ==  nameInXML );

    ::fwXML::XMLTranslator::sptr translator;
    translator = ::fwTools::ClassFactoryRegistry::create< ::fwXML::XMLTranslator  >(  toUpdate->getRootedClassname() );

    if ( translator.get() )
    {
        translator->updateDataFromXML(toUpdate,source);
    }
    else
    {
        OSLM_WARN("No XML Translator for " << toUpdate->getLeafClassname() << " Object UnModified");
    }
}

//------------------------------------------------------------------------------

::fwData::Object::sptr XMLTranslatorHelper::fromXML( xmlNodePtr source )
{
    OSLM_TRACE(" fromXML(xmlNode) with xmlNode->name=" << (const char*)source->name << " addr=" << source );

    std::string className = ObjectTracker::getClassname(source);
    std::string id        = ObjectTracker::getID(source);

    // !!! NOTE HERE WE DO NOT PERFORME TRANSLATION ID OLD -> NEW !!!

    // dot not create duplicate object
    ::fwData::Object::sptr obj;
    if ( ObjectTracker::isAlreadyInstanciated( id ) )
    {
        obj = ObjectTracker::buildObject( className, id ); // use previous one
    }
    else
    {
        obj = ObjectTracker::buildObject( className, id ); // create one with a new id
        fromXML(obj,source); //then fill with xml

        // fill Attributes.
        xmlNodePtr attributesNode   = XMLParser::findChildNamed( source, "Attributes");
        if ( attributesNode )
        {
            xmlNodePtr elementNode = XMLParser::nextXMLElement(attributesNode->children);
            while (elementNode)
            {
                std::string nodeName = (const char *) elementNode->name;
                if ( nodeName == "element" )
                {
                    xmlNodePtr keyNode   = XMLParser::findChildNamed( elementNode, "key");
                    xmlNodePtr valueNode = XMLParser::findChildNamed( elementNode, "value");
                    SLM_ASSERT("keyNode not instanced", keyNode);
                    SLM_ASSERT("valueNode not instanced", valueNode);
                    OSLM_INFO( "CompositeXMLTranslator::updateDataFromXML"  << BAD_CAST xmlNodeGetContent(keyNode) );

                    std::string key ( (char *)xmlNodeGetContent(keyNode)) ;

                    xmlNodePtr ConcretevalueNode = XMLParser::getChildrenXMLElement( valueNode );
                    SLM_ASSERT("ConcretevalueNode not instanced", ConcretevalueNode);

                    ::fwData::Object::sptr valueObj;
                    std::string className = ObjectTracker::getClassname(ConcretevalueNode);
                    std::string id        = ObjectTracker::getID(ConcretevalueNode);
                    valueObj = ObjectTracker::buildObject( className, id ); // create one with a new id
                    fromXML(valueObj, ConcretevalueNode);

                    SLM_ASSERT("valueObj not instanced", valueObj);

                    OSLM_ASSERT("Sorry, attribute " << key << " already exists.", ! obj->getField(key) );
                    obj->setField( key, valueObj );
                }
                elementNode = XMLParser::nextXMLElement( elementNode->next );
            }
        }
        // fill DynamicAttributes.
        xmlNodePtr dynamicAttributesNode   = XMLParser::findChildNamed( source, "DynamicAttributes");
        if (dynamicAttributesNode )
        {
            xmlNodePtr elementNode = XMLParser::nextXMLElement(attributesNode->children);
            while (elementNode)
            {
                std::string nodeName = (const char *) source->name;
                if ( nodeName == "element" )
                {
                    xmlNodePtr keyNode   = XMLParser::findChildNamed( elementNode, "key");
                    xmlNodePtr valueNode = XMLParser::findChildNamed( elementNode, "value");
                    SLM_ASSERT("keyNode not instanced", keyNode);
                    SLM_ASSERT("valueNode not instanced", valueNode);
                    OSLM_INFO( "CompositeXMLTranslator::updateDataFromXML"  << BAD_CAST xmlNodeGetContent(keyNode) );

                    std::string key ( (char *)xmlNodeGetContent(keyNode)) ;

                    xmlNodePtr ConcretevalueNode = XMLParser::getChildrenXMLElement( valueNode );
                    SLM_ASSERT("ConcretevalueNode not instanced", ConcretevalueNode);

                    ::fwData::Object::sptr valueObj;
                    std::string className = ObjectTracker::getClassname(ConcretevalueNode);
                    std::string id        = ObjectTracker::getID(ConcretevalueNode);
                    valueObj = ObjectTracker::buildObject( className, id ); // create one with a new id
                    fromXML(valueObj, ConcretevalueNode);

                    SLM_ASSERT("valueObj not instanced", valueObj);

                    if(obj->hasAttribute(key))
                    {
                        obj->setAttribute( key, valueObj );
                    }
                }
                elementNode = XMLParser::nextXMLElement( elementNode->next );
            }
        }
    }
    return obj;
}

//------------------------------------------------------------------------------

xmlNodePtr XMLTranslatorHelper::newElement( const  std::string &name, bool value )
{
    xmlNodePtr result = xmlNewNode( NULL, xmlStrdup( BAD_CAST name.c_str() )  );
    xmlNodeAddContent( result,   BAD_CAST (value?"1":"0")  );
    return result;
}


//------------------------------------------------------------------------------

bool XMLTranslatorHelper::getElement( xmlNodePtr node )
{
    std::string str = XMLParser::getTextValue (node);
    return (str == "1");
}

//------------------------------------------------------------------------------

void XMLTranslatorHelper::addAttribute( xmlNodePtr masterNode, const std::string & name, ::fwData::Object::sptr obj, bool isMandatory )
{
    if( obj )
    {
        xmlNodePtr node = XMLTH::toXMLRecursive( obj );
        xmlNodePtr fatherNode = xmlNewNode(NULL, BAD_CAST name.c_str() );
        xmlAddChild( fatherNode, node);
        xmlAddChild( masterNode , fatherNode );
    }
    else if ( isMandatory )
    {
        FW_RAISE("Sorry, attribute " << name << " is mandatory.");
    }
}

//------------------------------------------------------------------------------

} // end namespace
