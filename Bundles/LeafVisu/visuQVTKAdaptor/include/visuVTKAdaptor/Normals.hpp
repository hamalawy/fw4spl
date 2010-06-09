/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_NORMALS_HPP_
#define _VISUQVTKADAPTOR_NORMALS_HPP_

#include <fwServices/ObjectMsg.hpp>

#include <vtkCommand.h>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

class vtkActor;
class vtkActor2D;
class vtkAlgorithmOutput;

namespace visuVTKAdaptor
{


class VISUQVTKADAPTOR_CLASS_API Normals: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Normals)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API Normals() throw();
    VISUQVTKADAPTOR_API virtual ~Normals() throw();
    VISUQVTKADAPTOR_API void setMapperInput(vtkAlgorithmOutput *input);

protected:

   VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
   VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
   VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
   VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
   VISUQVTKADAPTOR_API virtual void doUpdate( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);
   VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    vtkActor* getActor();
    void updateNormals();

private:

    vtkAlgorithmOutput *m_mapperInput;

    vtkActor   * m_actor ;
    vtkActor2D * m_ptsIdActor2D ;
    vtkActor2D * m_cellsIdActor2D ;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_NORMALS_HPP_
