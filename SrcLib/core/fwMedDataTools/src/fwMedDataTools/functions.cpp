/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <string>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <gdcmUIDGenerator.h>

#include "fwMedDataTools/functions.hpp"

namespace fwMedDataTools
{

std::string generatePatientId()
{
    ::gdcm::UIDGenerator uid;
    const char* id = uid.Generate();
    return std::string(id);
}

std::string generateStudyInstanceUid()
{
    using namespace ::boost::posix_time;

    ptime now         = microsec_clock::local_time();
    time_facet* facet = new time_facet("%f%S%M%H%d%m");

    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << now;

    return ss.str();
}

} // namespace fwMedDataTools

