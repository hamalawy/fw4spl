/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_NEGATOMPR_HPP_
#define _VISUQVTKADAPTOR_NEGATOMPR_HPP_

#include <boost/logic/tribool.hpp>

#include <fwComEd/helper/MedicalImageAdaptor.hpp>
#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

namespace visuVTKAdaptor
{

class VISUQVTKADAPTOR_CLASS_API NegatoMPR: public ::fwComEd::helper::MedicalImageAdaptor,public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (NegatoMPR)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API NegatoMPR() throw();

    VISUQVTKADAPTOR_API virtual ~NegatoMPR() throw();

    typedef enum
    {
        NO_SLICE = 0,
        ONE_SLICE,
        THREE_SLICES
    } SliceMode;

protected :

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    VISUQVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUQVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap() throw(fwTools::Failed);

    void addAdaptor(std::string adaptor, int axis=-1);

    void setSliceMode(SliceMode sliceMode);
    SliceMode getSliceMode();
    ::boost::logic::tribool is3dModeEnabled();
    void set3dMode( bool enabled );

private:

    ::boost::logic::tribool m_3dModeEnabled;
    SliceMode m_sliceMode;
    SliceMode m_backupedSliceMode;
};

} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_NEGATOMPR_HPP_
