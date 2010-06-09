/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _OPVTKMESH_VTKMESHERSERVICE_HPP_
#define _OPVTKMESH_VTKMESHERSERVICE_HPP_

#include "opQVTKMesh/IMesher.hpp"
#include "opQVTKMesh/config.hpp"

namespace opVTKMesh
{

class  OPQVTKMESH_CLASS_API VTKMesherService : public ::opVTKMesh::IMesher
{

public :

    fwCoreServiceClassDefinitionsMacro ( (VTKMesherService)(::opVTKMesh::IMesher::Baseclass) ) ;

    OPQVTKMESH_API VTKMesherService() throw() ;

    OPQVTKMESH_API virtual ~VTKMesherService() throw() ;

protected:

    OPQVTKMESH_API virtual void starting() throw ( ::fwTools::Failed );

    OPQVTKMESH_API virtual void stopping() throw ( ::fwTools::Failed );

    OPQVTKMESH_API virtual void updating( fwServices::ObjectMsg::csptr _pMsg ) throw ( ::fwTools::Failed );

    OPQVTKMESH_API virtual void configuring() throw ( ::fwTools::Failed );

    OPQVTKMESH_API virtual void updating() throw ( ::fwTools::Failed );

    OPQVTKMESH_API virtual  void info ( std::ostream &_sstream ) ;

private :

    unsigned int m_reduction;

};

} // namespace opVTKMesh


#endif // _OPVTKMESH_VTKMESHERSERVICE_HPP_
