/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWXML_VISITOR_SINGLE_HPP_
#define _FWXML_VISITOR_SINGLE_HPP_

#include "fwXML/config.hpp"
#include "fwXML/visitor/IVisitor.hpp"

namespace fwData
{
namespace visitor
{

/**
 * @brief Implementation for Visitor Pattern : single Visit
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 */
class FWXML_CLASS_API Single : public  IVisitor
{
public:
    fwCoreNonInstanciableClassDefinitionsMacro( (Single)(IVisitor) ) ;
    /// subclass should override the operation
    FWXML_API virtual void visit( ::fwData::Object::sptr obj)=0;

    /// override processing of childs : ignore them
    FWXML_API virtual void next( ::fwData::Object::sptr src, const ::fwData::Object::FieldMapType &fields);

};

} // namespace visitor

} // namespace fwData

#endif // _FWXML_VISITOR_SINGLE_HPP_
