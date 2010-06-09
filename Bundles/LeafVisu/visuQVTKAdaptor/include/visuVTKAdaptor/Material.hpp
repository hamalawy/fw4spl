/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_MATERIAL_HPP_
#define _VISUQVTKADAPTOR_MATERIAL_HPP_


#include <fwServices/ObjectMsg.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"


class VISUQVTKADAPTOR_CLASS_API vtkProperty;


namespace visuVTKAdaptor
{



class VISUQVTKADAPTOR_CLASS_API Material: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Material)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API Material() throw();

    VISUQVTKADAPTOR_API virtual ~Material() throw();

    VISUQVTKADAPTOR_API void setVtkProperty(vtkProperty *property);

    VISUQVTKADAPTOR_API vtkProperty *getVtkProperty();

protected:

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    void updateMaterial( ::fwData::Material::sptr material );


    vtkProperty *m_property;
    bool m_manageProperty;

};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_MATERIAL_HPP_
