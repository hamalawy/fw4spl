/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWACTIVITIES_IVALIDATOR_HPP__
#define __FWACTIVITIES_IVALIDATOR_HPP__

#include <boost/enable_shared_from_this.hpp>

#include <fwCore/base.hpp>
#include <fwMedData/ActivitySeries.hpp>

#include "fwActivities/validator/factory/new.hpp"
#include "fwActivities/validator/registry/detail.hpp"
#include "fwActivities/registry/Activities.hpp"

#include "fwActivities/config.hpp"

namespace fwData
{
class Vector;
}

namespace fwActivities
{

/**
 * @brief Base class for all fwActivities validator's classes
 */
class FWACTIVITIES_CLASS_API IValidator : public ::fwCore::BaseObject
{

public:

    /**
     * @brief Defines validation result of an activity.
     * First element tells if the activity is validated or not by the IValidator implementation.
     * Second element may contains information about the result of activity validation.
     */
    typedef std::pair< bool, std::string > ValidationType;

    typedef ::fwActivities::validator::factory::Key Key;

    /**
     * @brief Class used to register a class factory in factory registry.
     * This class defines also the object factory ( 'create' )
     *
     * @tparam T Factory product type
     */
    template <typename T>
    class Registrar
    {
    public:
        Registrar(std::string functorKey)
        {
            ::fwActivities::validator::registry::get()->addFactory( functorKey,
                                                                    &::fwActivities::validator::factory::New<T> );
        }
    };

    fwCoreNonInstanciableClassDefinitionsMacro( (IValidator)(::fwCore::BaseObject) );


    /**
     * @brief Performs validation of given data as inputs for the considered activity.
     * @param[in] activityInfo a structure which contains all the activity configuration
     * @param[in] currentSelection a vector which contains current selected data.
     * @return result of the validation
     *
     * @see IValidator::ValidationType
     */
    FWACTIVITIES_API virtual ValidationType validate(
        const ::fwActivities::registry::ActivityInfo& activityInfo,
        SPTR(::fwData::Vector) currentSelection ) const = 0;

};

} // namespace fwActivities

#endif // __FWACTIVITIES_IVALIDATOR_HPP__

