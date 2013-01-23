/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWXML_XML_TRIANGULARMESHXMLTRANSLATOR_HPP__
#define __FWXML_XML_TRIANGULARMESHXMLTRANSLATOR_HPP__

#include <boost/lexical_cast.hpp>

#include <fwData/TriangularMesh.hpp>

#include "fwXML/XML/XMLTranslator.hpp"

namespace fwXML
{

class FWXML_CLASS_API TriangularMeshXMLTranslator : public XMLTranslator
{
public:

    /// trivial constructor
    FWXML_API TriangularMeshXMLTranslator() ;

    /// trivial destructor
    FWXML_API virtual ~TriangularMeshXMLTranslator() ;

    /// get XML from current object
    FWXML_API xmlNodePtr getXMLFrom( ::fwData::Object::sptr obj );


    /// get Object from an XML node
    FWXML_API void updateDataFromXML( ::fwData::Object::sptr toUpdate,  xmlNodePtr source);

protected :

    /// manage extra serialization : read image from path extracted from filename attribut in \<Buffer\> xml node
    FWXML_API void manageLoadingBuffer( xmlNodePtr boostXMLBuffer, ::fwData::TriangularMesh::sptr );

    /// manage extra serialization : only inform \<Buffer filename attribut /\>
    FWXML_API void manageSavingBuffer( xmlNodePtr boostXMLBuffer, ::fwData::TriangularMesh::sptr );
};

}

#endif /*__FWXML_XML_TRIANGULARMESHXMLTRANSLATOR_HPP__*/
