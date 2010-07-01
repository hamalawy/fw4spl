/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#include <fwTools/ClassRegistrar.hpp>
#include <fwTools/Factory.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>

#include <fwData/Float.hpp>
#include <fwData/Integer.hpp>

#include "fwComEd/GenericFieldParser.hpp"

REGISTER_SERVICE( ::fwServices::IXMLParser , ::fwComEd::GenericFieldParser , ::fwData::Float );
REGISTER_SERVICE( ::fwServices::IXMLParser , ::fwComEd::GenericFieldParser , ::fwData::Integer );

namespace fwComEd
{

void GenericFieldParser::updating( ) throw(fwTools::Failed)
{
    ::boost::shared_ptr< ::fwRuntime::ConfigurationElement >_cfgElement = this->m_configuration;
    //Parsing <object id=... type=.. export.... value="myValue" />
    if( _cfgElement->hasAttribute("value") )
    {
    	std::string stringValue = _cfgElement->getExistingAttributeValue("value");
    	if( ::fwData::Float::dynamicCast( this->getObject() ) )
    	{
   			::fwData::Float::sptr myField = this->getObject< ::fwData::Float >();
   			myField->value() = ::boost::lexical_cast< float >(stringValue);
    	}
    	else if ( ::fwData::Integer::dynamicCast( this->getObject() ) )
    	{
    		::fwData::Integer::sptr myField = this->getObject< ::fwData::Integer >();
    		myField->value() = ::boost::lexical_cast< int >(stringValue);
    	}
    }

    //For subelements
    for( ::fwRuntime::ConfigurationElement::Iterator configEltIter = _cfgElement->begin() ; !(configEltIter == _cfgElement->end()) ; ++configEltIter)
    {
        if( (*configEltIter)->getName() == "object" )
        {
            // Configuring the input/output
            ::boost::shared_ptr< fwTools::Object > localObj = fwServices::New( (*configEltIter) ) ;
            assert( localObj ) ;
            assert( (*configEltIter)->hasAttribute("id") ) ;
            std::string _id = (*configEltIter)->getExistingAttributeValue("id") ;
            assert( !this->getObject()->getField(_id) );
            this->getObject()->setFieldSingleElement(_id,localObj);
        }
    }
}

}
