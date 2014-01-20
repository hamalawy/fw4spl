/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWCOMED_PARSER_OBJECT_HPP_
#define _FWCOMED_PARSER_OBJECT_HPP_

#include <fwTools/Object.hpp>
#include <fwServices/IXMLParser.hpp>
#include <fwServices/AppConfigManager.hpp>

#include "fwComEd/export.hpp"

namespace fwComEd
{
namespace parser
{

/**
 * @class Object
 * @brief Specific ::fwTools::Object parser. Does nothing as no IService method is specialized: IXMLParser base class methods are therefore considered.
 *
 * @note This implementation is only written to avoid the mixing both the service type IXMLParser and the ::fwTools::Object implementation (for homogeneity).
 * 
 */
class FWCOMED_CLASS_API Object : public ::fwServices::IXMLParser
{
public :
    fwCoreServiceClassDefinitionsMacro ( (Object)(::fwServices::IXMLParser) ) ;

    FWCOMED_API Object( ) ;
    FWCOMED_API virtual ~Object() ;

    FWCOMED_API void createConfig( ::fwTools::Object::sptr _obj );
    FWCOMED_API void startConfig();
    FWCOMED_API void updateConfig();
    FWCOMED_API void stopConfig();
    FWCOMED_API void destroyConfig();

protected :

    bool refObjectValidator( ::fwRuntime::ConfigurationElement::csptr _cfgElement );
    void updating() throw( fwTools::Failed );

    std::vector< ::fwServices::AppConfigManager::sptr > m_ctmContainer;
};

} //namespace parser
} //namespace fwComEd

#endif /* _FWCOMED_PARSER_OBJECT_HPP_ */
