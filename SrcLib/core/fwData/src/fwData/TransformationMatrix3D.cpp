/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/registry/macros.hpp"
#include "fwData/Exception.hpp"

#include "fwData/TransformationMatrix3D.hpp"

fwDataRegisterMacro( ::fwData::TransformationMatrix3D );

namespace fwData
{

//------------------------------------------------------------------------------

TransformationMatrix3D::TransformationMatrix3D(::fwData::Object::Key key) :  m_vCoefficients(16, 0.)
{
    //default initialization
    for(size_t t = 0; t < MATRIX_SIZE; ++t)
    {
        m_vCoefficients[t*MATRIX_SIZE + t] = 1.;
    }
}

//------------------------------------------------------------------------------

TransformationMatrix3D::~TransformationMatrix3D()
{
}

//-----------------------------------------------------------------------------

void TransformationMatrix3D::shallowCopy(const Object::csptr &_source )
{
    TransformationMatrix3D::csptr other = TransformationMatrix3D::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );
    m_vCoefficients = other->m_vCoefficients;
}

//-----------------------------------------------------------------------------

void TransformationMatrix3D::cachedDeepCopy(const Object::csptr &_source, DeepCopyCacheType &cache)
{
    TransformationMatrix3D::csptr other = TransformationMatrix3D::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source, cache );
    m_vCoefficients = other->m_vCoefficients;
}

//------------------------------------------------------------------------------

double TransformationMatrix3D::getCoefficient(size_t l, size_t c) const
{
    size_t pos = l * MATRIX_SIZE + c;
    assert(pos < m_vCoefficients.size());
    return m_vCoefficients.at(pos);
}

//------------------------------------------------------------------------------

void TransformationMatrix3D::setCoefficient(size_t l, size_t c, TransformationMatrix3D::TM3DType val)
{
    size_t pos = l * MATRIX_SIZE + c;
    assert(pos < m_vCoefficients.size());
    m_vCoefficients.at(pos) = val;
}

//------------------------------------------------------------------------------

} // namespace fwData
