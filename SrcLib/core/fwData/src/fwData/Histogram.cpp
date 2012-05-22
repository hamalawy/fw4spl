/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/registry/macros.hpp"

#include <fwData/Image.hpp>

#include "fwData/Histogram.hpp"


fwDataRegisterMacro( ::fwData::Histogram );


namespace fwData
{

Histogram::Histogram()
{
    SLM_TRACE_FUNC();
    m_binsWidth = 1;
    m_minValue  = 0;
    m_maxValue  = 100;
}

//------------------------------------------------------------------------------

Histogram::~Histogram()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void Histogram::shallowCopy( Histogram::csptr _source )
{
    this->fieldShallowCopy( _source );
    m_values = _source->m_values;
    m_minValue = _source->m_minValue;
    m_maxValue = _source->m_maxValue;
    m_binsWidth = _source->m_binsWidth;
}

//------------------------------------------------------------------------------

void Histogram::deepCopy( Histogram::csptr _source )
{
    this->fieldDeepCopy( _source );

    m_minValue = _source->m_minValue;
    m_maxValue = _source->m_maxValue;
    m_binsWidth = _source->m_binsWidth;

    m_values.clear();

    BOOST_FOREACH( long value, _source->m_values )
    {
        m_values.push_back( value );
    }
}

//------------------------------------------------------------------------------

void Histogram::addPixel( float _pixel )
{
    if( this->isInRange( _pixel ) )
    {
        int index = static_cast<int>(( _pixel - m_minValue ) / m_binsWidth);
        m_values[ index ]++;
    }
}

//------------------------------------------------------------------------------

void Histogram::initialize( float _min, float _max, float _binsWidth )
{
    SLM_ASSERT("The minimum value can't be greater than the maximum value", _min <= _max);

    m_minValue = _min;
    m_maxValue = _max;
    m_binsWidth = _binsWidth;

    m_values.clear();

    if( m_binsWidth != 0 )
    {
        int newSize = static_cast<int>(( m_maxValue - m_minValue ) / m_binsWidth);
        m_values.resize( newSize + 1, 0 );
    }
}

//------------------------------------------------------------------------------

long Histogram::getNbPixels( float _min, float _max )
{
    SLM_ASSERT("The minimum value can't be greater than the maximum value", _min < _max);

    size_t indexMin = 0;
    if( _min >= m_minValue )
    {
        indexMin = static_cast<size_t>(( _min - m_minValue ) / m_binsWidth);
    }
    size_t indexMax = m_values.size();
    if( _max <= m_maxValue )
    {
        indexMax = static_cast<size_t>(( _max - m_minValue ) / m_binsWidth);
    }
    long nbPixels = 0;

    while( indexMin < indexMax )
    {
        nbPixels += m_values.at( indexMin++ );
    }

    return nbPixels;
}

//------------------------------------------------------------------------------

bool Histogram::isInRange( float _pixel )
{
    return ( _pixel >= m_minValue && _pixel <= m_maxValue );
}

} // namespace fwData

