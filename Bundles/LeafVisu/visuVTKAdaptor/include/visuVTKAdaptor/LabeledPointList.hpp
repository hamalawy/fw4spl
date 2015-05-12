/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_LABELEDPOINTLIST_HPP__
#define __VISUVTKADAPTOR_LABELEDPOINTLIST_HPP__


#include <fwServices/ObjectMsg.hpp>
#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

class vtkCommand;

namespace visuVTKAdaptor
{


/**
 * @brief Adaptor to display a point list
 */
class VISUVTKADAPTOR_CLASS_API LabeledPointList : public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (LabeledPointList)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API LabeledPointList() throw();

    VISUVTKADAPTOR_API virtual ~LabeledPointList() throw();

protected:

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API virtual void doReceive( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    std::list< ::fwRenderVTK::IVtkAdaptorService::sptr > m_subServices;

    vtkCommand * m_rightButtonCommand;

    bool m_needSubservicesDeletion;

};




} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_LABELEDPOINTLIST_HPP__
