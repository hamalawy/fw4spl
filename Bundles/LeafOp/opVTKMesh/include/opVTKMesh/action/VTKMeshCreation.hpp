/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _OPVTKMESH_ACTION_VTKMESHCREATION_HPP_
#define _OPVTKMESH_ACTION_VTKMESHCREATION_HPP_

#include <fwGui/IActionSrv.hpp>

#include "opVTKMesh/config.hpp"

namespace opVTKMesh
{

namespace action
{

/**
* @brief Create a mesh from an image with VTK
**/
class OPVTKMESH_CLASS_API VTKMeshCreation : public ::fwGui::IActionSrv
{

public :

    fwCoreServiceClassDefinitionsMacro ( (VTKMeshCreation)(::fwGui::IActionSrv) ) ;

    OPVTKMESH_API VTKMeshCreation() throw() ;

    OPVTKMESH_API virtual ~VTKMeshCreation() throw() ;

protected:

    OPVTKMESH_API void starting() throw ( ::fwTools::Failed );

    OPVTKMESH_API void stopping() throw ( ::fwTools::Failed );

    OPVTKMESH_API void receiving( ::fwServices::ObjectMsg::csptr _pMsg ) throw ( ::fwTools::Failed );

    OPVTKMESH_API void configuring() throw ( ::fwTools::Failed );

    OPVTKMESH_API void updating() throw ( ::fwTools::Failed );

    OPVTKMESH_API void info ( std::ostream &_sstream ) ;

private :

    std::string m_imageUID;
    std::string m_meshUID;
    unsigned int m_reduction;
};


} // namespace action
} // namespace opVTKMesh


#endif // _OPVTKMESH_ACTION_VTKMESHCREATION_HPP_
