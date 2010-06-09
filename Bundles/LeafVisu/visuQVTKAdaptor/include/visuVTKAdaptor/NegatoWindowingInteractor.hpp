/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_NEGATOWINDOWINGINTERACTOR_HPP_
#define _VISUQVTKADAPTOR_NEGATOWINDOWINGINTERACTOR_HPP_

#include <fwData/TransfertFunction.hpp>

#include <fwComEd/helper/MedicalImageAdaptor.hpp>
#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

class vtkCommand;

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API NegatoWindowingInteractor: public ::fwComEd::helper::MedicalImageAdaptor, public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (NegatoWindowingInteractor)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API NegatoWindowingInteractor() throw();

    VISUQVTKADAPTOR_API virtual ~NegatoWindowingInteractor() throw();

    VISUQVTKADAPTOR_API void startWindowing();

    VISUQVTKADAPTOR_API void stopWindowing();

    VISUQVTKADAPTOR_API void updateWindowing( double , double);

    VISUQVTKADAPTOR_API void resetWindowing();

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    vtkCommand *m_vtkObserver;
    double m_initialWindow;
    double m_initialLevel;
    double m_priority;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_NEGATOWINDOWINGINTERACTOR_HPP_
