/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWACTIVITIES_BUILDER_ACTIVITYSERIESINITDATA_HPP__
#define __FWACTIVITIES_BUILDER_ACTIVITYSERIESINITDATA_HPP__

#include <fwCore/base.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "fwActivities/builder/ActivitySeries.hpp"
#include "fwActivities/config.hpp"

namespace fwActivities
{
namespace builder
{

/**
 * @brief Defines an activity builder which instanciates a new requirement if minOccurs and maxOccurs are set to 0.
 * @class ActivitySeriesInitData
 */
class FWACTIVITIES_CLASS_API ActivitySeriesInitData : public ::fwActivities::builder::ActivitySeries
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (ActivitySeriesInitData)(::fwActivities::IBuilder),
                                            (()),
                                            ::fwActivities::builder::factory::New< ActivitySeriesInitData > );

    typedef std::string RegistryKeyType;

    /// Constructor. Do nothing.
    FWACTIVITIES_API ActivitySeriesInitData(::fwActivities::IBuilder::Key key);

    /// Destructor. Do nothing.
    FWACTIVITIES_API virtual ~ActivitySeriesInitData();

    /**
     * @brief Instanciates a new object where requirement maxOccurs and minOccurs are both set to 0.
     * @see ::fwActivities::builder::ActivitySeries::buildData
     */
    FWACTIVITIES_API virtual ::fwMedData::ActivitySeries::sptr buildData(
            const ::fwActivities::registry::ActivityInfo& activityInfo,
            SPTR(::fwData::Vector) currentSelection ) const;
};

} // namespace builder
} // namespace fwActivities

#endif /*__FWACTIVITIES_BUILDER_ACTIVITYSERIESINITDATA_HPP__*/

