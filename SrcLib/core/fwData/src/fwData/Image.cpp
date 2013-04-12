/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <numeric>
#include <functional>
#include <algorithm>
#include <sstream>

#include <fwCore/base.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

#include <climits>
#include <fwTools/DynamicType.hpp>
#include <fwTools/DynamicTypeKeyTypeMapping.hpp>

#include "fwData/registry/macros.hpp"
#include "fwData/Exception.hpp"


#include "fwData/registry/macros.hpp"

#include "fwData/Image.hpp"

//------------------------------------------------------------------------------

fwDataRegisterMacro( ::fwData::Image );

//------------------------------------------------------------------------------

namespace fwData
{

//------------------------------------------------------------------------------

Image::Image(::fwData::Object::Key key) :
        m_type(),
        m_attrWindowCenter(0),
        m_attrWindowWidth(0),
        m_attrNumberOfComponents(1),
        m_dataArray( ::fwData::Array::New() )
{}

//------------------------------------------------------------------------------

Image::~Image() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void Image::shallowCopy(const Object::csptr &_source )
{
    Image::csptr other = Image::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );

    // Assign
    copyInformation( other );

    m_dataArray  = other->m_dataArray;
}

//-----------------------------------------------------------------------------

void Image::deepCopy(const Object::csptr &_source )
{
    Image::csptr other = Image::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
            "Unable to copy" + (_source?_source->getClassname():std::string("<NULL>"))
            + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source );

    // Assign
    copyInformation( other );

    if( other->m_dataArray )
    {
        m_dataArray->deepCopy( other->m_dataArray );
    }
}

//------------------------------------------------------------------------------

::fwData::Array::sptr Image::getDataArray() const
{
    return m_dataArray;
}

//------------------------------------------------------------------------------

void Image::setDataArray(::fwData::Array::sptr array, bool copyArrayInfo)
{
    if( ! array )
    {
        array = ::fwData::Array::New();
    }
    m_dataArray = array;
    if (copyArrayInfo)
    {
        m_size = array->getSize();
        m_type = array->getType();
    }
}

//------------------------------------------------------------------------------


size_t Image::allocate() throw(::fwData::Exception)
{
    if (!m_dataArray)
    {
        m_dataArray = ::fwData::Array::New();
    }

    OSLM_ASSERT( "NumberOfComponents must be > 0", m_attrNumberOfComponents > 0 );
    return m_dataArray->resize(m_type, m_size, m_attrNumberOfComponents, true);
}

//------------------------------------------------------------------------------

size_t Image::allocate(SizeType::value_type x, SizeType::value_type y,  SizeType::value_type z,
                       const ::fwTools::Type &type, size_t numberOfComponents) throw(::fwData::Exception)
{
    m_size = boost::assign::list_of(x)(y)(z);
    m_type = type;
    m_attrNumberOfComponents = numberOfComponents;
    return allocate();
}

//------------------------------------------------------------------------------

size_t Image::allocate(const SizeType &size, const ::fwTools::Type &type, size_t numberOfComponents)
    throw(::fwData::Exception)
{
    m_size = size;
    m_type = type;
    m_attrNumberOfComponents = numberOfComponents;
    return allocate();
}

//------------------------------------------------------------------------------

::fwTools::DynamicType Image::getPixelType() const
{
    typedef std::map<std::string, ::fwTools::DynamicType> DynamicTypeMapType;

    static DynamicTypeMapType dynamicTypeMap = ::boost::assign::map_list_of
        (::fwTools::Type().string() , ::fwTools::DynamicType() )
        ("uint8" , ::fwTools::makeDynamicType<std::string>("unsigned char")  )
        ("uint16", ::fwTools::makeDynamicType<std::string>("unsigned short") )
        ("uint32", ::fwTools::makeDynamicType<std::string>("unsigned int")   )
        ("int8" ,  ::fwTools::makeDynamicType<std::string>("signed char")    )
        ("int16",  ::fwTools::makeDynamicType<std::string>("signed short")   )
        ("int32",  ::fwTools::makeDynamicType<std::string>("signed int")     )
        ("float",  ::fwTools::makeDynamicType<std::string>("float")          )
        ("double", ::fwTools::makeDynamicType<std::string>("double")         )

//special case for dynamic type : 64bits integers was not managed by dynamic type.
#if ( INT_MAX < LONG_MAX )
        ("uint64", ::fwTools::makeDynamicType<std::string>("unsigned long")  )
        ("int64",  ::fwTools::makeDynamicType<std::string>("signed long")    )
#else
        ("uint32", ::fwTools::makeDynamicType<std::string>("unsigned long")  )
        ("int32",  ::fwTools::makeDynamicType<std::string>("signed long")    )
        ("uint64", ::fwTools::DynamicType() )
        ("int64",  ::fwTools::DynamicType() )
#endif
        ;

    ::fwTools::DynamicType dtype = dynamicTypeMap[getType().string()] ;
    return dtype;
}

//------------------------------------------------------------------------------

::fwTools::Type Image::getType() const
{
    return m_type;
}

//------------------------------------------------------------------------------

void Image::setType(::fwTools::Type type)
{
    m_type = type;
}


//------------------------------------------------------------------------------

void Image::setType(const std::string &type)
{
    m_type = ::fwTools::Type(type);
}

//------------------------------------------------------------------------------

void Image::copyInformation( Image::csptr _source )
{
    m_size                   = _source->m_size;
    m_type                   = _source->m_type;
    m_spacing                = _source->m_spacing;
    m_origin                 = _source->m_origin;
    m_attrWindowCenter       = _source->m_attrWindowCenter;
    m_attrWindowWidth        = _source->m_attrWindowWidth;
    m_attrNumberOfComponents = _source->m_attrNumberOfComponents;
}

//------------------------------------------------------------------------------

size_t Image::getNumberOfDimensions() const
{
    return m_size.size();
}


//------------------------------------------------------------------------------

const Image::SpacingType & Image::getSpacing() const
{
    return m_spacing;
}


//------------------------------------------------------------------------------

void Image::setSpacing(const SpacingType &spacing)
{
    m_spacing = spacing;
}

//------------------------------------------------------------------------------

const Image::OriginType & Image::getOrigin() const
{
    return m_origin;
}

//------------------------------------------------------------------------------

void Image::setOrigin(const OriginType &origin)
{
    m_origin = origin;
}

//------------------------------------------------------------------------------

const Image::SizeType & Image::getSize() const
{
    return m_size;
}

//------------------------------------------------------------------------------

void Image::setSize(const SizeType &size)
{
    m_size = size;
}

//------------------------------------------------------------------------------

size_t Image::getSizeInBytes() const
{
    SLM_TRACE_FUNC();

    size_t size = std::accumulate(
                                  m_size.begin(), m_size.end(),
                                  static_cast<size_t>(m_type.sizeOf()) * m_attrNumberOfComponents,
                                  std::multiplies< size_t > () );
    return size;
}


//------------------------------------------------------------------------------

size_t Image::getAllocatedSizeInBytes() const
{
    SLM_TRACE_FUNC();
    size_t size = 0;
    if (m_dataArray)
    {
        size = m_dataArray->getSizeInBytes();
    }
    return size;
}

//------------------------------------------------------------------------------

} // namespace fwData
