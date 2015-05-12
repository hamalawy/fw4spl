/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <algorithm>

#include <boost/bind.hpp>

#include <fwData/Composite.hpp>

#include <fwServices/IEditionService.hpp>

#include "fwComEd/helper/Field.hpp"

namespace fwComEd
{
namespace helper
{

//-----------------------------------------------------------------------------

Field::Field( ::fwData::Object::sptr object )
    :   m_objectMsg ( ::fwServices::ObjectMsg::New() ),
      m_object ( object )
{
}

//-----------------------------------------------------------------------------

Field::~Field()
{
}

//-----------------------------------------------------------------------------

void Field::setField(const fwData::Object::FieldNameType& name, fwData::Object::sptr obj)
{
    SLM_ASSERT("Field helper need a non-null object pointer", !m_object.expired());
    ::fwData::Object::sptr object = m_object.lock();
    ::fwData::Object::sptr field  = object->getField(name);
    if (!field)
    {
        m_objectMsg->appendAddedField(name, obj);
    }
    else
    {
        m_objectMsg->appendChangedField(name, field, obj);
    }
    object->setField(name, obj);
}

//-----------------------------------------------------------------------------

void Field::setFields( const fwData::Object::FieldMapType& newFields)
{
    SLM_ASSERT("Field helper need a non-null object pointer", !m_object.expired());
    ::fwData::Object::sptr object = m_object.lock();
    const ::fwData::Object::FieldMapType oldFields = object->getFields();
    this->buildMessage(newFields,oldFields);
    object->setFields(newFields);
}

//-----------------------------------------------------------------------------

void Field::updateFields( const fwData::Object::FieldMapType& fieldMap)
{
    SLM_ASSERT("Field helper need a non-null object pointer", !m_object.expired());
    ::fwData::Object::sptr object = m_object.lock();
    const ::fwData::Object::FieldMapType oldFields = object->getFields();
    this->buildMessage(fieldMap,oldFields);
    object->updateFields(fieldMap);
}

//-----------------------------------------------------------------------------

void Field::removeField(const fwData::Object::FieldNameType& name)
{
    SLM_ASSERT("Field helper need a non-null object pointer", !m_object.expired());
    ::fwData::Object::sptr object = m_object.lock();
    ::fwData::Object::sptr field  = object->getField(name);

    if (field)
    {
        m_objectMsg->appendRemovedField(name, field);
    }
    object->removeField(name);
}

//-----------------------------------------------------------------------------

void Field::notify(fwServices::IService::sptr _serviceSource)
{
    SLM_ASSERT("Field helper need a non-null object pointer", !m_object.expired());
    if ( m_objectMsg->getEventIds().size() > 0 )
    {
        ::fwServices::IEditionService::notify( _serviceSource, m_object.lock(), m_objectMsg, true );
    }
    SLM_INFO_IF("The message will not by notified because it has no event.", m_objectMsg->getEventIds().size() == 0);
}

//-----------------------------------------------------------------------------

void Field::buildMessage(
    const ::fwData::Object::FieldMapType &oldFields,
    const ::fwData::Object::FieldMapType &newFields
    )
{
    ::fwData::Object::FieldNameVectorType oldFieldNames;
    ::fwData::Object::FieldNameVectorType newFieldNames;

    std::transform(
        oldFields.begin(), oldFields.end(),
        std::back_inserter(oldFieldNames),
        ::boost::bind(&::fwData::Object::FieldMapType::value_type::first, _1)
        );
    std::transform(
        newFields.begin(), newFields.end(),
        std::back_inserter(newFieldNames),
        ::boost::bind(&::fwData::Object::FieldMapType::value_type::first, _1)
        );

    std::sort(oldFieldNames.begin(), oldFieldNames.end());
    std::sort(newFieldNames.begin(), newFieldNames.end());

    ::fwData::Object::FieldNameVectorType added;   // new - old
    ::fwData::Object::FieldNameVectorType changed; // old & new
    ::fwData::Object::FieldNameVectorType removed; // old - new

    std::set_difference(
        newFieldNames.begin(), newFieldNames.end(),
        oldFieldNames.begin(), oldFieldNames.end(),
        std::back_inserter(added)
        );

    std::set_intersection(
        newFieldNames.begin(), newFieldNames.end(),
        oldFieldNames.begin(), oldFieldNames.end(),
        std::back_inserter(changed)
        );

    std::set_difference(
        oldFieldNames.begin(), oldFieldNames.end(),
        newFieldNames.begin(), newFieldNames.end(),
        std::back_inserter(removed)
        );

    BOOST_FOREACH(const ::fwData::Object::FieldNameVectorType::value_type &fieldName, added)
    {
        m_objectMsg->appendAddedField(fieldName, newFields.find(fieldName)->second);
    }

    BOOST_FOREACH(const ::fwData::Object::FieldNameVectorType::value_type &fieldName, changed)
    {
        m_objectMsg->appendChangedField(
            fieldName,
            oldFields.find(fieldName)->second,
            newFields.find(fieldName)->second
            );
    }

    BOOST_FOREACH(const ::fwData::Object::FieldNameVectorType::value_type &fieldName, changed)
    {
        m_objectMsg->appendRemovedField(fieldName, oldFields.find(fieldName)->second);
    }
}

} // namespace helper
} // namespace fwComEd
