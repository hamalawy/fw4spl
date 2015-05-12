/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWACTIVITIES_VALIDATOR_RELATEDSTUDY_HPP__
#define __FWACTIVITIES_VALIDATOR_RELATEDSTUDY_HPP__

#include <fwCore/base.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "fwActivities/IValidator.hpp"
#include "fwActivities/config.hpp"

namespace fwActivities
{
namespace validator
{

/**
 * @brief Defines a validator which checks that given series refer to the same study.
 * @class RelatedStudy.
 */
class FWACTIVITIES_CLASS_API RelatedStudy : public ::fwActivities::IValidator
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (RelatedStudy)(::fwActivities::IValidator),
                                            (()),
                                            ::fwActivities::validator::factory::New< RelatedStudy > );


    /// Constructor. Do nothing.
    FWACTIVITIES_API RelatedStudy(::fwActivities::IValidator::Key key);

    /// Destructor. Do nothing.
    FWACTIVITIES_API virtual ~RelatedStudy();

    /**
     * @see ::fwActivities::IValidator::validate
     */
    FWACTIVITIES_API virtual IValidator::ValidationType validate(
        const ::fwActivities::registry::ActivityInfo& activityInfo,
        SPTR(::fwData::Vector) currentSelection ) const;
};

} // namespace validator
} // namespace fwActivities

#endif // __FWACTIVITIES_VALIDATOR_RELATEDSTUDY_HPP__

