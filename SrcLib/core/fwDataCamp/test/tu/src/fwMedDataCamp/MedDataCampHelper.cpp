/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <vector>
#include <boost/assign/std/vector.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <fwData/GenericFieldBase.hpp>

#include <fwAtomConversion/RetreiveObjectVisitor.hpp>

#include "fwMedDataCamp/MedDataCampHelper.hpp"

namespace fwMedDataCamp
{
namespace ut
{

//------------------------------------------------------------------------------

void MedDataCampHelper::visitProperties(const std::string& className, const PropertiesNameType& vecProp)
{
    const ::camp::Class& metaClass = ::camp::classByName(className);

    CPPUNIT_ASSERT_EQUAL( vecProp.size(), metaClass.propertyCount()) ;
    BOOST_FOREACH(const std::string& property, vecProp)
    {
        CPPUNIT_ASSERT_MESSAGE("Missing property "+ property+" in fwMedData::Equipment",
                              metaClass.hasProperty(property));
    }
}

//------------------------------------------------------------------------------

void MedDataCampHelper::compareSimplePropertyValue(::fwData::Object::sptr obj,
                                                   const std::string& propertyPath,
                                                   const std::string& value)
{
    ::fwData::GenericFieldBase::sptr field;
    field = ::fwAtomConversion::getSubObject< ::fwData::GenericFieldBase >(obj, propertyPath);
    CPPUNIT_ASSERT_MESSAGE("Retrieve failed for property "+propertyPath, field);
    CPPUNIT_ASSERT_EQUAL( value, field->toString());
}

//------------------------------------------------------------------------------

void MedDataCampHelper::compareObjectPropertyValue(::fwData::Object::sptr obj,
                                                   const std::string& propertyPath,
                                                   ::fwData::Object::sptr value)
{
    ::fwData::Object::sptr subObj;
    subObj = ::fwAtomConversion::getSubObject(obj, propertyPath);
    CPPUNIT_ASSERT_MESSAGE("Retrieve failed for property "+propertyPath, subObj);
    CPPUNIT_ASSERT_MESSAGE("Retrieve property "+propertyPath+" not equal with value", value == subObj);
}

//------------------------------------------------------------------------------


} //namespace ut
} //namespace fwMedDataCamp
