/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_INTEGER_HPP_
#define _FWDATA_INTEGER_HPP_

#include "fwData/config.hpp"
#include "fwData/GenericField.hpp"
#include "fwData/Object.hpp"

namespace fwData
{

/**
 * @class 	Integer
 * @brief 	This class contains an integer value
 *
 * Integer object is essentially used as a field in other objects.
 *
 * @author	IRCAD (Research and Development Team).
 * @date	2007-2009.
 */
class FWDATA_CLASS_API Integer : public GenericField< int >
{
public:
	fwCoreClassDefinitionsWithFactoryMacro( (Integer)(::fwData::Object::Baseclass),( ((const int)(0)) ), new Integer) ;
	/**
	 * @brief Constructor.
	 * @param[in] value The initial value.
	 */
	FWDATA_API Integer( const int value = 0 ) throw();

	/**
	 * @brief Destructor.
	 */
	FWDATA_API virtual ~Integer() throw();
};

} // namespace fwData

#endif // _FWDATA_INTEGER_HPP_
