/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _OPVTKMESH_ACTION_QVTKMESHCREATION_HPP_
#define _OPVTKMESH_ACTION_QVTKMESHCREATION_HPP_

#include <guiQt/action/IAction.hpp>


#include "opQVTKMesh/config.hpp"

namespace opVTKMesh
{

namespace action
{

class  qVTKMeshCreation : public ::guiQt::action::IAction
{

public :

    fwCoreServiceClassDefinitionsMacro ( (qVTKMeshCreation)(::guiQt::action::IAction::Baseclass) ) ;

     qVTKMeshCreation() throw() ;

     virtual ~qVTKMeshCreation() throw() ;

     void starting() throw ( ::fwTools::Failed );

     void stopping() throw ( ::fwTools::Failed );

     void updating( fwServices::ObjectMsg::csptr _pMsg ) throw ( ::fwTools::Failed );

     void configuring() throw ( ::fwTools::Failed );

     void updating() throw ( ::fwTools::Failed );

     void info ( std::ostream &_sstream ) ;

private :

    std::string m_imageUID;
    std::string m_meshUID;
    unsigned int m_reduction;

};


} // namespace action
} // namespace opVTKMesh


#endif // _OPVTKMESH_ACTION_VTKMESHCREATION_HPP_
