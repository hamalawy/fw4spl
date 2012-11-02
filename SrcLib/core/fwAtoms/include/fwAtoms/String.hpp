/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWATOMS_BASE_STRING_HPP_
#define _FWATOMS_BASE_STRING_HPP_

#include <string>

#include <fwCamp/Mapper/ValueMapper.hpp>
#include <fwCamp/macros.hpp>

#include "fwAtoms/config.hpp"
#include "fwAtoms/Base.hpp"

fwCampAutoDeclareMacro((fwAtoms)(String), FWATOMS_API);

namespace fwAtoms
{

/**
 * @brief Class represented a String
 *
 */
class FWATOMS_CLASS_API String : public Base
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (String)(::fwAtoms::String), (()), new String ) ;

    FWATOMS_API virtual bool isValue() const {return true;};
    FWATOMS_API virtual bool isString() const {return true;};
    FWATOMS_API virtual std::string getString() const {return m_value;};

    virtual void setString( const std::string value ){m_value=value;};

    /**
     * @brief Construct a new Object represented a string.
     * @param value the internal string.
     *
     */
    FWATOMS_API static String::sptr New(std::string value);

    FWATOMS_API virtual Base::sptr clone();
protected:
    String(){m_value = "";};
    String(const std::string& value);
    std::string m_value;
};

}



#endif /* _FWATOMS_BASE_STRING_HPP_ */
