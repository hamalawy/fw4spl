/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef GRAPHXMLTRANSLATOR_HPP_
#define GRAPHXMLTRANSLATOR_HPP_

#include <fwData/Graph.hpp>
#include "fwXML/XML/XMLTranslator.hpp"




namespace fwXML
{

class FWXML_CLASS_API GraphXMLTranslator : public XMLTranslator
{
public:

    /// trivial constructor
    FWXML_API GraphXMLTranslator() ;

    /// trivial destructor
    FWXML_API virtual ~GraphXMLTranslator() ;

    /// get XML from current object
    FWXML_API xmlNodePtr getXMLFrom( ::fwData::Object::sptr obj );


    /// get Object from an XML Graph
    FWXML_API void updateDataFromXML( ::fwData::Object::sptr toUpdate,  xmlNodePtr source);

protected :

};

}

#endif /*GRAPHXMLTRANSLATOR_HPP_*/
