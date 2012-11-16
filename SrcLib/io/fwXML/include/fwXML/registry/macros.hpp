/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWXML_REGISTRY_MACRO_HPP__
#define __FWXML_REGISTRY_MACRO_HPP__

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/cat.hpp>

#include "fwXML/XML/XMLTranslator.hpp"
#include "fwXML/registry/detail.hpp"

namespace fwXML
{
namespace registry
{

#define fwXMLRegisterMacro( XMLTranslatorClassname, FunctorKey )        \
    static ::fwXML::XMLTranslator::Registrar< XMLTranslatorClassname >  \
    BOOST_PP_CAT( s__factory__record__, __LINE__) ( BOOST_PP_STRINGIZE(FunctorKey) );

} // end namespace registry
} // end namespace fwXML

#endif /*__FWXML_REGISTRY_MACRO_HPP__*/