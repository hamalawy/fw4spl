/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <cmath>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

// fwComEd
#include <fwComEd/Dictionary.hpp>

// fwData
#include <fwData/Vector.hpp>
#include <fwData/PointList.hpp>
#include <fwData/Boolean.hpp>

#include "gdcmIO/container/DicomDistance.hpp"
#include "gdcmIO/helper/GdcmHelper.hpp"
#include "gdcmIO/container/DicomDictionarySR.hpp"

namespace gdcmIO
{
namespace container
{

//------------------------------------------------------------------------------

DicomDistance::DicomDistance():
        m_SCoords(),
        m_dists(),
        m_refFrames()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

DicomDistance::~DicomDistance()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void DicomDistance::convertToData(::fwData::Image::sptr a_image)
{
    SLM_TRACE_FUNC();

    const unsigned int nb_distances = this->m_refFrames.size();

    if (nb_distances > 0)
    {
        ::fwData::Vector::sptr vectDist;
        vectDist = a_image->setDefaultField(::fwComEd::Dictionary::m_imageDistancesId, ::fwData::Vector::New());

        const std::string separator = ",";  // Define separator between referenced frame numbers

        // Compute z spacing
        // NOTE : spacing between slice must be regular
        double spacingOnZ = 1;
        if (a_image->getNumberOfDimensions() > 2)
            spacingOnZ = a_image->getSpacing()[2];

        for (unsigned int i = 0; i < nb_distances; ++i)    // For each distance
        {
            // Create a new distance
            ::fwData::PointList::sptr pl = ::fwData::PointList::New();

            // Create 2 points
            ::fwData::Point::sptr pt1 = ::fwData::Point::New();
            ::fwData::Point::sptr pt2 = ::fwData::Point::New();

            // Set x, y coordinates
            const ::gdcmIO::container::DicomSCoord & scoord = m_SCoords[i];

            pt1->getRefCoord()[0] = scoord[0];
            pt1->getRefCoord()[1] = scoord[1];

            pt2->getRefCoord()[0] = scoord[2];
            pt2->getRefCoord()[1] = scoord[3];

            // Set z coordinates
            const std::string & refFrames   = m_refFrames[i];
            const size_t        pos         = refFrames.find( separator );  // Get each slice number

            pt1->getRefCoord()[2] = (atof( refFrames.substr(0, pos).c_str() ) - 1) * spacingOnZ;   // -1 because frame number start at 1 and point at 0
            pt2->getRefCoord()[2] = (atof( refFrames.substr(pos+1).c_str() ) - 1) * spacingOnZ;

            // Add points to the distance
            pl->getRefPoints().push_back( pt1 );
            pl->getRefPoints().push_back( pt2 );

            // Add the distance in the image

            vectDist->getContainer().push_back(pl);

            OSLM_TRACE("new distance : ( "<<pt1->getRefCoord()[0]<<"x"<<pt1->getRefCoord()[1]<<"x"<<pt1->getRefCoord()[2]<<" - " \
                                          <<pt2->getRefCoord()[0]<<"x"<<pt2->getRefCoord()[1]<<"x"<<pt2->getRefCoord()[2]<<" )");
        }

        // force distance to be shown
        a_image->setField("ShowDistances",  ::fwData::Boolean::New(true));
    }
    else
    {
        OSLM_TRACE("Any distance found");
    }
}

//------------------------------------------------------------------------------

void DicomDistance::setFromData(::fwData::Image::csptr a_image) throw (::gdcmIO::exception::Failed)
{
    SLM_TRACE_FUNC();
    ::fwData::Vector::sptr vectDist;
    vectDist = a_image->getField< ::fwData::Vector >(::fwComEd::Dictionary::m_imageDistancesId);
    if (vectDist)
    {
        // Init
        ::fwData::Point::csptr  pt1, pt2;           // Point of the distance
        ::fwData::PointList::PointListContainer vDistance;
        float                   z1, z2;             // z coordinates of poinrts
        std::stringstream       strm;
        const std::string       separator = ",";    // Define separator between referenced frame numbers
        ::gdcmIO::container::DicomSCoord                  scoord;             // Spatial COORDinates which contains x and y coordinates
        float                   graphicData[4];     // x, y coordinates

        // Add a constraint on "z" coordinate
        unsigned int illegalRefFrame    = 0;        // Number of referenced frame number rejected
        unsigned int refFrameMax        = 1;        // maximum required referenced frame number
        float        zSpacingInverse    = 1;
        if (a_image->getNumberOfDimensions() > 2)
        {
            refFrameMax     = a_image->getSize()[2];
            zSpacingInverse = 1. / a_image->getSpacing()[2];
        }

        BOOST_FOREACH(::fwData::Object::sptr obj, *vectDist)
        {
            ::fwData::PointList::sptr pl = ::fwData::PointList::dynamicCast(obj);
            SLM_ASSERT("PointList dynamicCast failed", pl);
            vDistance = pl->getPoints();

            // Set each point
            pt1 = vDistance[0];
            pt2 = vDistance[1];

            graphicData[0] = pt1->getCoord()[0];    graphicData[1] = pt1->getCoord()[1];    z1 = pt1->getCoord()[2] * zSpacingInverse;
            graphicData[2] = pt2->getCoord()[0];    graphicData[3] = pt2->getCoord()[1];    z2 = pt2->getCoord()[2] * zSpacingInverse;

            // If "z" could be a referenced frame
            if (z1 >= 0 && z1 < refFrameMax
             && z2 >= 0 && z2 < refFrameMax)
            {
                // Set SCOORD
                scoord.setGraphicData( graphicData, 4);
                scoord.setGraphicType( DicomDictionarySR::getGraphicTypeString( DicomDictionarySR::POLYLINE ) );
                m_SCoords.push_back( scoord );
                OSLM_TRACE("        SCOORD : " << scoord[0] << "x" << scoord[1] << "x" << scoord[2] << "x" << scoord[3]);

                strm.str("");
                // Convert "z" coordinate to a frame number.
                strm << floor( z1 + 1 + 0.5) << separator << floor( z2 + 1 + 0.5);  // +1 because frame number start at 1
                this->addRefFrame(strm.str());
                OSLM_TRACE("        refFrames : " << strm.str());

                // Compute the distance value (Euclidienne)
                strm.str("");
                strm << sqrt( (scoord[0] - scoord[2]) * (scoord[0] - scoord[2]) + \
                              (scoord[1] - scoord[3]) * (scoord[1] - scoord[3]) + \
                              (z1 - z2) * (z1 - z2) );
                this->addDist(strm.str());
                OSLM_TRACE("        dist : " << strm.str());
            }
            else
            {
                ++illegalRefFrame;
            }
        }

        if (illegalRefFrame > 0)
        {
            strm.str(""); strm << illegalRefFrame << " distance(s) with coordinates out of bounds";
            throw ::gdcmIO::exception::Failed(strm.str());
        }
    }
}

//------------------------------------------------------------------------------

void DicomDistance::addDist(const std::string & a_dist)
{
    this->m_dists.push_back(a_dist);
}

//------------------------------------------------------------------------------

void DicomDistance::addSCoord(const ::gdcmIO::container::DicomSCoord & a_scoord)
{
    this->m_SCoords.push_back(a_scoord);
}

//------------------------------------------------------------------------------

void DicomDistance::addRefFrame(const std::string & a_refFrame)
{
    this->m_refFrames.push_back(a_refFrame);
}

} //namespace container
} //namespace gdcmIO
