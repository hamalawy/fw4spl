/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwMedData/SeriesDB.hpp>
#include <fwMedData/Series.hpp>

#include <fwCamp/UserObject.hpp>

//------------------------------------------------------------------------------

fwCampImplementDataMacro((fwMedData)(SeriesDB))
{
    builder
        .tag("object_version", "1")
        .tag("lib_name", "fwMedData")
        .base< ::fwData::Object>()
        .property("values", &::fwMedData::SeriesDB::m_attrContainer)
        ;
}
