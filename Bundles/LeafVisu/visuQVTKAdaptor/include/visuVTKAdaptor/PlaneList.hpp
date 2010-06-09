/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_PLANELIST_HPP_
#define _VISUQVTKADAPTOR_PLANELIST_HPP_

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/MeshFactory.hpp"


class vtkCommand;

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API PlaneList: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (PlaneList)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API PlaneList() throw();

    VISUQVTKADAPTOR_API virtual ~PlaneList() throw();

    VISUQVTKADAPTOR_API void setPlaneCollectionId(::fwRenderVTK::VtkRenderService::VtkObjectIdType id) { m_planeCollectionId = id; }

    VISUQVTKADAPTOR_API ::fwRenderVTK::VtkRenderService::VtkObjectIdType getPlaneCollectionId() { return m_planeCollectionId; }

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);



    vtkCommand * m_rightButtonCommand;
    ::fwRenderVTK::VtkRenderService::VtkObjectIdType m_planeCollectionId;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_PLANELIST_HPP_
