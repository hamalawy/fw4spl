/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef FWCOMED_GENERICFIELDPARSER_HPP_
#define FWCOMED_GENERICFIELDPARSER_HPP_

#include <fwTools/Failed.hpp>
#include <fwTools/Object.hpp>

#include <fwRuntime/ConfigurationElement.hpp>

#include <fwServices/IXMLParser.hpp>

#include "fwComEd/export.hpp"

namespace fwComEd
{
/**
 * Parses XML configuration for Float and Integer (declared as GenericField).
 * Practically, it allows to configure their value using the method
 * \code
 *  ::fwTools::Object::sptr ::fwServices::New(::fwRuntime::ConfigurationElement::sptr _elt)
 * \endcode
 * Configurations can look like
 * \code
 * <object id="..." 		type="::fwData::Float" 		value="0.5"		/>
 * <object id="..." 		type="::fwData::Integer" 	value="255"		/>
 * \endcode
 * @note Should be moved to a more central librairy (e.g. fwComEd)
 */
class FWCOMED_CLASS_API GenericFieldParser : public ::fwServices::IXMLParser
{
public :
    fwCoreServiceClassDefinitionsMacro ( (GenericFieldParser)(::fwServices::IXMLParser) ) ;

    /**
     * @brief   Constructor : does nothing.
     */
    FWCOMED_API GenericFieldParser( ) {};

    /**
     * @brief   Destructor : does nothing.
     */
    FWCOMED_API virtual ~GenericFieldParser() {};

    /**
     * @brief Update generic field content
     */
    FWCOMED_API virtual void updating( ) throw(fwTools::Failed) ;

};

} //namespace fwComEd

#endif /* FWCOMED_GENERICFIELDPARSER_HPP_ */
