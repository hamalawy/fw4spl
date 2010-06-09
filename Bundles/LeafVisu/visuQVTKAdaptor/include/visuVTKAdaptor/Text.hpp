/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_TEXT_HPP_
#define _VISUQVTKADAPTOR_TEXT_HPP_


#include <string>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

class vtkTextActor;
//class vtkActor2D;
class vtkTextMapper;

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API Text: public ::fwRenderVTK::IVtkAdaptorService
{

public:
    fwCoreServiceClassDefinitionsMacro ( (Text)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API Text() throw();
    VISUQVTKADAPTOR_API virtual ~Text() throw();

    VISUQVTKADAPTOR_API virtual void setText(std::string str);
    VISUQVTKADAPTOR_API virtual std::string getText(){return m_text;};

protected:

    VISUQVTKADAPTOR_API virtual void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API virtual void doStop() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API virtual void doSwap() throw(fwTools::Failed){};
    VISUQVTKADAPTOR_API virtual void doUpdate() throw(fwTools::Failed){};
    VISUQVTKADAPTOR_API virtual void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed){};
    VISUQVTKADAPTOR_API virtual void configuring() throw(fwTools::Failed);


    std::string m_text;

    vtkActor2D * m_actor;
    vtkTextMapper * m_mapper;

};

} //namespace visuVTKAdaptor

#endif //_VISUQVTKADAPTOR_TEXT_HPP_
