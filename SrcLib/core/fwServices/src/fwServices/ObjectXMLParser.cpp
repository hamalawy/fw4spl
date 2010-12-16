/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwServices/macros.hpp"
#include "fwServices/helper.hpp"
#include "fwServices/ObjectXMLParser.hpp"


REGISTER_SERVICE( ::fwServices::IXMLParser , ::fwServices::ObjectXMLParser , ::fwTools::Object ) ;


namespace fwServices
{
			
ObjectXMLParser::ObjectXMLParser( )
{}

ObjectXMLParser::~ObjectXMLParser()
{}

//------------------------------------------------------------------------------

bool ObjectXMLParser::refObjectValidator( ::fwRuntime::ConfigurationElement::sptr _cfgElement )
{
    bool isOk = true;

    for(    ::fwRuntime::ConfigurationElement::Iterator configEltIter = _cfgElement->begin() ;
            configEltIter != _cfgElement->end();
            ++configEltIter)
    {
        std::string subElementName = (*configEltIter)->getName();
        if(     subElementName != "service" &&
                subElementName != "serviceList"    )
        {
            OSLM_ERROR("xml subelement \""<< subElementName <<"\" for element object is not supported for the moment when you use a reference on item composite.");
            isOk = false;
        }
    }

    return isOk;
}

//------------------------------------------------------------------------------

void ObjectXMLParser::updating( ) throw(fwTools::Failed)
{
    // Declaration of attributes values
    const std::string OBJECT_BUILD_MODE = "src";
    const std::string BUILD_OBJECT = "new";
    const std::string GET_OBJECT = "ref";

    ::fwRuntime::ConfigurationElement::sptr _cfgElement = this->m_configuration;
    ::fwTools::Object::sptr associatedObject = this->getObject();
    assert( associatedObject ) ;

    for( ::fwRuntime::ConfigurationElement::Iterator configEltIter = _cfgElement->begin() ; !(configEltIter == _cfgElement->end()) ; ++configEltIter)
    {

        if( (*configEltIter)->getName() == "item" )
        {
            ::fwRuntime::ConfigurationElement::sptr itemConfigElem = (*configEltIter);

            // Test build mode
            std::string buildMode = BUILD_OBJECT;

            if ( (*configEltIter)->hasAttribute( OBJECT_BUILD_MODE ) )
            {
                buildMode = itemConfigElem->getExistingAttributeValue( OBJECT_BUILD_MODE );
                OSLM_ASSERT( "Sorry, buildMode \""<< buildMode <<"\" is not supported by the application.", buildMode == BUILD_OBJECT || buildMode == GET_OBJECT );
            }


            SLM_ASSERT( "Sorry, the xml element \"item\" must have an attribute named \"key\" .", itemConfigElem->hasAttribute("key") );
            std::string key = itemConfigElem->getExistingAttributeValue("key");
            SLM_ASSERT( "Sorry, the xml element \"item\" must have an attribute named \"key\" not empty.", ! key.empty() );
            SLM_ASSERT( "Sorry, xml element item must have one (and only one) xml sub-element \"object\".", itemConfigElem->size() == 1 && (*itemConfigElem->begin())->getName() == "object" );

            if( buildMode == BUILD_OBJECT )
            {
                // Test if key already exist in object
                OSLM_ASSERT("Sorry the key "<< key <<" already exists in the object.", associatedObject->getFieldSize( key ) == 0 );

                // Create and manage object config
                ::fwTools::Object::sptr localObj = ::fwServices::New( * ( itemConfigElem->begin() ) ) ;

                // Add object
                associatedObject->setFieldSingleElement( key, localObj);

            }
            else // if( buildMode == GET_OBJECT )
            {
                // Test if key already exist in composite
                OSLM_ASSERT("Sorry the key "<< key <<" not exists in the object.", associatedObject->getFieldSize( key ) == 1 );

                ::fwTools::Object::sptr objRef = associatedObject->getFieldSingleElement< ::fwTools::Object >( key );

                ::fwRuntime::ConfigurationElement::sptr objectConfigElem = ( * ( itemConfigElem->begin() ) );

                OSLM_ASSERT("Sorry, for an item reference, the xml subelement \"object\" does not have attributes.", objectConfigElem->getAttributes().size() == 0 );
                OSLM_ASSERT("Sorry, some subelement of element \"object\" are not supported.", refObjectValidator( objectConfigElem ) );
                ::fwServices::addServicesToObjectFromCfgElem( objRef, objectConfigElem );
            }
        }
    }
}

}

