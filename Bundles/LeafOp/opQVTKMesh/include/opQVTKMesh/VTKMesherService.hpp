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

class  VTKMesherService : public ::opVTKMesh::IMesher
{

public :

    fwCoreServiceClassDefinitionsMacro ( (VTKMesherService)(::opVTKMesh::IMesher::Baseclass) ) ;

     VTKMesherService() throw() ;

     virtual ~VTKMesherService() throw() ;

protected:

     virtual void starting() throw ( ::fwTools::Failed );

     virtual void stopping() throw ( ::fwTools::Failed );

     virtual void updating( fwServices::ObjectMsg::csptr _pMsg ) throw ( ::fwTools::Failed );

     virtual void configuring() throw ( ::fwTools::Failed );

     virtual void updating() throw ( ::fwTools::Failed );

     virtual  void info ( std::ostream &_sstream ) ;

private :

    unsigned int m_reduction;

};

} // namespace opVTKMesh


#endif // _OPVTKMESH_VTKMESHERSERVICE_HPP_
