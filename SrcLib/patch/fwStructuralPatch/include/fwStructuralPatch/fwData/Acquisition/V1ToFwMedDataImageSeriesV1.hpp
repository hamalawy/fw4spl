/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSTRUCTURALPATCH_FWDATA_ACQUISITION_V1TOFWMEDDATAIMAGESERIESV1_HPP__
#define __FWSTRUCTURALPATCH_FWDATA_ACQUISITION_V1TOFWMEDDATAIMAGESERIESV1_HPP__

#include <fwAtomsPatch/IStructuralPatch.hpp>
#include "fwStructuralPatch/config.hpp"

namespace fwStructuralPatch
{

namespace fwData
{

namespace Acquisition
{

/// Transforms an acquisition in version '1' to an image series in version '1'.
class FWSTRUCTURALPATCH_CLASS_API V1ToFwMedDataImageSeriesV1 : public ::fwAtomsPatch::IStructuralPatch
{
public:
    fwCoreClassDefinitionsWithFactoryMacro(
            (V1ToFwMedDataImageSeriesV1)(::fwStructuralPatch::fwData::Acquisition::V1ToFwMedDataImageSeriesV1),
            (()),
            new V1ToFwMedDataImageSeriesV1);

    /// Constructor
    FWSTRUCTURALPATCH_API V1ToFwMedDataImageSeriesV1();

    /// Destructor
    FWSTRUCTURALPATCH_API ~V1ToFwMedDataImageSeriesV1();

    /// Copy constructor
    FWSTRUCTURALPATCH_API V1ToFwMedDataImageSeriesV1( const V1ToFwMedDataImageSeriesV1 &cpy );

    /**
     * @brief Applies patch
     *
     * Removes attributes from acquisition, changes 'uid' attribute to 'instance_uid', adds attributes 'modality',
     * 'patient', 'equipment', 'performing_physicians_name', 'time' and 'date'.
     */
    FWSTRUCTURALPATCH_API virtual void apply(
            const ::fwAtoms::Object::sptr& previous,
            const ::fwAtoms::Object::sptr& current,
            ::fwAtomsPatch::IPatch::NewVersionsType& newVersions);

};


} // namespace Acquisition

} // namespace fwData

} // namespace fwStructuralPatch

#endif /* __FWSTRUCTURALPATCH_FWDATA_ACQUISITION_V1TOFWMEDDATAIMAGESERIESV1_HPP__ */

