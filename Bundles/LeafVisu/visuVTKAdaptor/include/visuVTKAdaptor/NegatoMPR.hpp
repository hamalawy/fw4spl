/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUVTKADAPTOR_NEGATOMPR_HPP_
#define _VISUVTKADAPTOR_NEGATOMPR_HPP_

#include <boost/logic/tribool.hpp>

#include <fwComEd/helper/MedicalImageAdaptor.hpp>
#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"

namespace visuVTKAdaptor
{


class VISUVTKADAPTOR_CLASS_API NegatoMPR: public ::fwComEd::helper::MedicalImageAdaptor,public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (NegatoMPR)(::fwRenderVTK::IVtkAdaptorService) ) ;

    VISUVTKADAPTOR_API NegatoMPR() throw();

    VISUVTKADAPTOR_API virtual ~NegatoMPR() throw();

    typedef enum
    {
        NO_SLICE = 0,
        ONE_SLICE,
        THREE_SLICES
    } SliceMode;

    void setAllowAlphaInTF(bool allow) {m_allowAlphaInTF = allow;};
    void setInterpolation(bool interpolation){m_interpolation = interpolation;};
    void setVtkImageSourceId(std::string id) {m_imageSourceId = id;};

    void setSliceMode(SliceMode sliceMode);
    SliceMode getSliceMode();
    ::boost::logic::tribool is3dModeEnabled();
    void set3dMode( bool enabled );

    /// Set actor opacity
    void setActorOpacity(double actorOpacity) {m_actorOpacity = actorOpacity;};

protected :

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doReceive(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    /**
     * @brief Configures the service
     *
     * @verbatim
       <adaptor id="negato" class="::visuVTKAdaptor::NegatoMPR" objectId="imageKey">
           <config renderer="default" picker="negatodefault" mode="2d" slices="1" sliceIndex="axial"
                   transform="trf" tfalpha="yes" interpolation="off" vtkimagesource="imgSource" actorOpacity="1.0"
                   selectedTFKey="tkKey" tfSelectionFwID="selectionID" />
       </adaptor>
       @endverbatim
     * - \b renderer (mandatory): defines the renderer to show the arrow. It must be different from the 3D objects renderer.
     * - \b picker (mandatory): identifier of the picker
     * - \b mode (optional, 2d or 3d): defines the scene mode. In 2d mode, the camera follow the negato in
     * axial/frontal/sagital orientation. In 3d mode, the camera is automatically reset when the image is modified. If
     * mode is not defined, the camera is free.
     * - \b slices (optional, default=3): number of slices shown in the adaptor
     * - \b sliceIndex (optional, axial/frontal/sagittal, default=axial): orientation of the negato
     * - \b transform (optional): the vtkTransform to associate to the adaptor
     * - \b tfalpha (optional, yes/no, default=no): if true, the opacity of the transfer function is used in the negato.
     * - \b interpolation (optional, yes/no, default=yes): if true, the image pixels are interpolated
     * - \b vtkimagesource (optional): source image, used for blend
     * - \b actorOpacity (optional, default=1.0): actor opacity (float)
     * - \b tfSelectionFwID (optional): fwID of the composite containing transfer functions
     * - \b selectedTFKey (optional): key of the transfer function to use in negato
     */
    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);

    void addAdaptor(std::string adaptor, int axis=-1);


private:

    bool m_allowAlphaInTF;
    bool m_interpolation;
    double m_actorOpacity;

    std::string m_imageSourceId;

    ::boost::logic::tribool m_3dModeEnabled;
    SliceMode m_sliceMode;
    SliceMode m_backupedSliceMode;
};




} //namespace visuVTKAdaptor

#endif // _VISUVTKADAPTOR_NEGATOMPR_HPP_
