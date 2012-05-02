/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/convenience.hpp>

#include <stdio.h>
#include <string.h>

#include <fwServices/Base.hpp>
#include <fwDataIO/writer/IObjectWriter.hpp>
#include <fwDataIO/reader/IObjectReader.hpp>
#include <fwData/Composite.hpp>

#include "fwXML/XML/ImageXMLTranslator.hpp"
#include "fwXML/XML/XMLParser.hpp"
#include "fwXML/XML/GenericXMLTranslator.hpp"
#include "fwXML/boostSerializer/IFSBoostSerialize.hpp"
#include "fwXML/boostSerializer/Image.hpp"
#include "fwXML/XML/XMLTranslatorHelper.hpp"
#include "fwXML/IFileFormatService.hpp"
#include "fwXML/Serializer.hpp"

namespace fwXML
{

ImageXMLTranslator::ImageXMLTranslator() {};

//------------------------------------------------------------------------------

ImageXMLTranslator::~ImageXMLTranslator() {};

//------------------------------------------------------------------------------

xmlNodePtr ImageXMLTranslator::getXMLFrom( ::fwData::Object::sptr obj )
{
    // call default xml representation
    GenericXMLTranslator< ::fwData::Image > img2xmlbase;
    xmlNodePtr node = img2xmlbase.getXMLFrom(obj);

    ::fwData::Image::sptr pImage = ::fwData::Image::dynamicCast(obj);
    SLM_ASSERT("Object is not an image", pImage);

    ::fwData::Composite::NewSptr arrays;
    if(pImage->getDataArray())
    {
        (*arrays)["DataArray"]   = pImage->getDataArray();
    }

    xmlNodePtr arraysNode = XMLTH::toXMLRecursive(arrays);
    xmlAddChild( node, arraysNode);
    return node;
}

//------------------------------------------------------------------------------

void ImageXMLTranslator::updateDataFromXML( ::fwData::Object::sptr toUpdate,  xmlNodePtr source)
{
    ::fwData::Image::sptr pImage = ::fwData::Image::dynamicCast(toUpdate);
    SLM_ASSERT("Object is not an image", pImage);

    GenericXMLTranslator< ::fwData::Image > img2xmlbase;
    img2xmlbase.updateDataFromXML(toUpdate, source);

    xmlNodePtr compositeNode = XMLParser::findChildNamed( source, std::string("Composite") );

    ::fwData::Object::sptr obj;
    obj = Serializer().ObjectsFromXml( compositeNode, true );
    SLM_ASSERT("obj not instanced", obj);
    ::fwData::Composite::sptr arrays = ::fwData::Composite::dynamicCast(obj);
    SLM_ASSERT("composite not instanced", arrays);

    ::fwData::Composite::iterator iterDataArray = arrays->find("DataArray");
    if(iterDataArray != arrays->end())
    {
        ::fwData::Array::sptr arraySource = ::fwData::Array::dynamicCast(iterDataArray->second);
        if(arraySource)
        {
            ::fwData::Array::sptr array = pImage->getDataArray();
            array->swap(arraySource);
        }
    }

}

//------------------------------------------------------------------------------

}
