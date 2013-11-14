/* ***** BEGIN LICENSE BLOCK *****
* FW4SPL - Copyright (C) IRCAD, 2009-2013.
* Distributed under the terms of the GNU Lesser General Public License (LGPL) as
* published by the Free Software Foundation.
* ****** END LICENSE BLOCK ****** */

#include <gdcmSegmentReader.h>

#include "gdcmIO/reader/dicom/DicomSegment.hpp"
#include "gdcmIO/helper/GdcmHelper.hpp"

namespace gdcmIO
{
namespace reader
{
namespace dicom
{

//------------------------------------------------------------------------------

DicomSegment::DicomSegment()
{
   SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

DicomSegment::~DicomSegment()
{
   SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

bool DicomSegment::isSurfaceSegmentation()
{
   ::boost::shared_ptr< ::gdcm::SegmentReader > gSegmentReader
       = ::boost::dynamic_pointer_cast< ::gdcm::SegmentReader >( this->getReader() );

   ::gdcm::SegmentReader::SegmentVector::const_iterator it = gSegmentReader->GetSegments().begin();

   if ( (*it)->GetSurfaceCount() > 0 )
       return true;

   return false;
}

//------------------------------------------------------------------------------

void DicomSegment::readSurfaceSegmentation( ::fwData::Reconstruction::sptr  a_reconstruction,
                    ::gdcm::SmartPointer< ::gdcm::Segment > a_segment ) throw (::gdcmIO::exception::Failed)
{
   if (a_segment->GetSurfaceCount() != 1)
       throw ::gdcmIO::exception::Failed("Inappropriate surface count for a segment");

   //*****     Set the reconstruction      *****//
   // Organ Name
   // TODO : if empty, try to get label with DicomDictionarySegment
   a_reconstruction->setOrganName( a_segment->GetSegmentDescription());
   OSLM_TRACE("Organ Name : " << a_reconstruction->getOrganName());

   a_reconstruction->setStructureType(a_segment->GetSegmentLabel());
   OSLM_TRACE("Structure type : " << a_reconstruction->getStructureType());

   // a_reconstruction->setIsAutomatic( ((a_segment->GetSegmentAlgorithmType() == ::gdcm::Segment::AUTOMATIC)? true : false) );
   // OSLM_TRACE("Reconstruction is automatic : " << a_reconstruction->getIsAutomatic());
}

}  // namespace dicom
}  // namespace reader
}  // namespace gdcmIO
