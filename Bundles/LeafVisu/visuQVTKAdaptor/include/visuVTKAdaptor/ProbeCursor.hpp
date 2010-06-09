/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_PROBECURSOR_HPP_
#define _VISUQVTKADAPTOR_PROBECURSOR_HPP_

#include <fwData/TransfertFunction.hpp>

#include "visuVTKAdaptor/config.hpp"
#include <fwComEd/helper/MedicalImageAdaptor.hpp>

class vtkCommand;
class vtkActor;
class vtkActor2D;
class vtkTextMapper;
class vtkPolyData;
class vtkPolyDataMapper;

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API ProbeCursor: public  ::fwComEd::helper::MedicalImageAdaptor,public ::fwRenderVTK::IVtkAdaptorService
{

public:
    fwCoreServiceClassDefinitionsMacro ( (ProbeCursor)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API ProbeCursor() throw();

    VISUQVTKADAPTOR_API virtual ~ProbeCursor() throw();

    VISUQVTKADAPTOR_API void updateView( double world[3] );

    VISUQVTKADAPTOR_API void setVisibility( bool visibility );

    VISUQVTKADAPTOR_API void StartProbeCursor();

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    void buildTextActor();
    void buildPolyData();

    void computeCrossExtremity( const int probeSlice[3] , double worldCross[4][3] );


    double m_priority;

    vtkCommand *m_vtkObserver;

    vtkActor2D * m_textActor; // rendering defect using a vtkTextActor
    vtkTextMapper * m_textMapper;

    vtkPolyData         *m_cursorPolyData;
    vtkPolyDataMapper   *m_cursorMapper;
    vtkActor            *m_cursorActor;

};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_PROBECURSOR_HPP_
