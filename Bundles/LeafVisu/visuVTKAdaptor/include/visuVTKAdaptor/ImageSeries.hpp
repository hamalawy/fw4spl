/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_IMAGESERIES_HPP__
#define __VISUVTKADAPTOR_IMAGESERIES_HPP__

#include <vector>

#include <fwComEd/helper/MedicalImageAdaptor.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/NegatoMPR.hpp"

namespace visuVTKAdaptor
{


/**
 * @brief This adaptor shows ImageSeries. Creates an adaptor for the image in the series.
 * @class ImageSeries
 */
class VISUVTKADAPTOR_CLASS_API ImageSeries : public ::fwComEd::helper::MedicalImageAdaptor,
                                             public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (ImageSeries)(::fwRenderVTK::IVtkAdaptorService) ) ;

    VISUVTKADAPTOR_API ImageSeries() throw();

    VISUVTKADAPTOR_API virtual ~ImageSeries() throw();

    void setAllowAlphaInTF(bool allow) {m_allowAlphaInTF = allow;};
    void setInterpolation(bool interpolation){m_interpolation = interpolation;};
    void setVtkImageSourceId(std::string id) {m_imageSourceId = id;};

protected:

    /// Calls doUpdate()
    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);

    /// Configure the adaptor.
    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);

    /// Calls doUpdate()
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);

    /// Creates and starts image adaptor. Redraw all (stop then restart sub services).
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);

    /// Stops and unregister image subservice.
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    /// Does nothing.
    VISUVTKADAPTOR_API void doReceive(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    /// Sets adaptor slice mode (NO_SLICE, ONE_SLICE, THREE_SLICES)
    void setSliceMode(NegatoMPR::SliceMode sliceMode);

    /// Gets adaptor slice mode (NO_SLICE, ONE_SLICE, THREE_SLICES)
    NegatoMPR::SliceMode getSliceMode();

    /// Returns true if 3d mode is enabled, false if it is not and indeterminate if it is not defined
    ::boost::logic::tribool is3dModeEnabled();

    /// Defines 3D mode
    void set3dMode( bool enabled );

private:
    bool m_allowAlphaInTF;
    bool m_interpolation;

    std::string m_imageSourceId;

    ::boost::logic::tribool m_3dModeEnabled;
    NegatoMPR::SliceMode m_sliceMode;

};




} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_IMAGESERIES_HPP__
