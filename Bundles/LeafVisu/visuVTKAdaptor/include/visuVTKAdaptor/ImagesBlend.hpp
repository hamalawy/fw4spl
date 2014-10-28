/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUVTKADAPTOR_IMAGESBLEND_HPP_
#define _VISUVTKADAPTOR_IMAGESBLEND_HPP_

#include <fwServices/helper/SigSlotConnection.hpp>

#include <fwData/Image.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>
#include <fwComEd/helper/MedicalImageAdaptor.hpp>

#include "visuVTKAdaptor/config.hpp"

class vtkImageActor;
class vtkLookupTable;
class vtkImageMapToColors;
class vtkImageData;
class vtkPolyDataMapper;
class vtkPolyData;
class vtkActor;


namespace visuVTKAdaptor
{

/**
 * @brief   Manage blend for image given in configuration.
 * @class   ImagesProbeCursor
 * 
 * @date    2010.
 */
class VISUVTKADAPTOR_CLASS_API ImagesBlend: public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (ImagesBlend)(::fwRenderVTK::IVtkAdaptorService) ) ;

    VISUVTKADAPTOR_API ImagesBlend() throw();

    VISUVTKADAPTOR_API virtual ~ImagesBlend() throw();

    void setVtkImageRegisterId(std::string id) {m_imageRegisterId = id;};

protected :

    class ImageInfo
    {

    public:
        ImageInfo():
            m_useTFAlfa(false),
            m_imageOpacity(1.0)
        {}

        bool   m_useTFAlfa;
        double m_imageOpacity;
        std::string m_tfSelectionFwID;
        std::string m_selectedTFKey;

        ::fwServices::helper::SigSlotConnection::sptr m_connections;
    };

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doReceive(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);
    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);

    void addImageAdaptors();
    void removeImageAdaptors();

    /// Return true if images to blend have the same spacing, origin and size. Else show a message dialog and return false
    bool checkImageInformations();

    std::vector<std::string> m_imageIds;
    typedef std::map< std::string, SPTR(ImageInfo) > ImageInfoMap;
    ImageInfoMap m_imagesInfo;

    /// map <fwId, adaptor>
    typedef std::map< std::string, ::fwRenderVTK::IVtkAdaptorService::sptr > RegisteredImageMap;
    RegisteredImageMap m_registeredImages;


    vtkImageBlend *m_imageBlend;
    std::string m_imageRegisterId;

};




} //namespace visuVTKAdaptor

#endif // _VISUVTKADAPTOR_IMAGESBLEND_HPP_
