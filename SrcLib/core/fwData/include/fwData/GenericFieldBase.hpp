/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_GENERICFIELDBASE_HPP_
#define _FWDATA_GENERICFIELDBASE_HPP_

#include <string>
#include <ostream>

#include <fwCore/base.hpp>


#include "fwData/config.hpp"
#include "fwData/Object.hpp"

namespace fwData
{

/**
 * @class   GenericFieldBase
 *
 * 
 * @date    2009-2010.
 */
class FWDATA_CLASS_API GenericFieldBase : public Object
{
public:

    fwCoreNonInstanciableClassDefinitionsMacro( (GenericFieldBase) );

    FWDATA_API virtual bool operator== (const GenericFieldBase &lf ) = 0;
    FWDATA_API virtual bool operator!= (const GenericFieldBase &lf ) = 0;
    FWDATA_API virtual bool operator<  (const GenericFieldBase &lf ) = 0;
    FWDATA_API virtual bool operator>  (const GenericFieldBase &lf ) = 0;
    FWDATA_API virtual bool operator<= (const GenericFieldBase &lf ) = 0;
    FWDATA_API virtual bool operator>= (const GenericFieldBase &lf ) = 0;
    FWDATA_API friend std::ostream& operator<<( std::ostream &, const GenericFieldBase & );

    FWDATA_API virtual ::std::string toString() const = 0;
    FWDATA_API virtual void fromString(const ::std::string &_value) = 0;


private :
    FWDATA_API virtual std::ostream & toOStream( std::ostream &_os ) const = 0;
};

} // namespace fwData

#endif // _FWDATA_GENERICFIELDBASE_HPP_
