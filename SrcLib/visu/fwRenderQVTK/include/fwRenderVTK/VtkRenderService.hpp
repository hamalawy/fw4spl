/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWRENDERVTK_VTKRENDERSERVICE_HPP_
#define _FWRENDERVTK_VTKRENDERSERVICE_HPP_


#include <map>

#include <fwRuntime/ConfigurationElement.hpp>

#include <fwQtRender/IRender.hpp>

#include <fwData/Composite.hpp>

#include "fwRenderVTK/config.hpp"
#include  <vtkRenderer.h>
#include <QVTKWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

namespace fwData
{

class TransformationMatrix3D;

}


namespace fwRenderVTK
{

class IVtkAdaptorService;

class  VtkRenderService : public ::fwQtRender::IRender
{
public :
    fwCoreServiceClassDefinitionsMacro ( (VtkRenderService)(::fwQtRender::IRender::Baseclass) ) ;

    typedef std::string RendererIdType;
    typedef std::string PickerIdType;
    typedef std::string ObjectIdType;
    typedef std::string AdaptorIdType;
    typedef std::string VtkObjectIdType;

    VtkRenderService() throw() ;

    virtual ~VtkRenderService() throw() ;

    vtkRenderer * getRenderer(RendererIdType rendererId);

    void render();
    bool isShownOnScreen();

    vtkAbstractPropPicker * getPicker(PickerIdType pickerId);

    vtkObject * getVtkObject(VtkObjectIdType objectId);

    // For temporary NegatoAdaptor use
    // wxAuiManager* getWxManager(){return m_wxmanager;};

     bool getPendingRenderRequest(){return m_pendingRenderRequest;}
     void setPendingRenderRequest(bool b){m_pendingRenderRequest=b;}

protected:

    /// Install observations : should be implemented in IService
     virtual void starting() throw( ::fwTools::Failed);
    /// Uninstall observations : should be implemented in IService
     virtual void stopping() throw( ::fwTools::Failed);

     virtual void configuring() throw( ::fwTools::Failed) ;

     void updating( ::fwServices::ObjectMsg::csptr message ) throw( ::fwTools::Failed);

     void updating() throw( ::fwTools::Failed);

private :

    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;
    ConfigurationType m_sceneConfiguration;
    class SceneAdaptor {

        public:

        SceneAdaptor()
        { }

        ~SceneAdaptor()
        { }

        SPTR (IVtkAdaptorService) getService()
        { return m_service.lock(); }

        ConfigurationType m_config;
        WPTR(IVtkAdaptorService) m_service;

    };



    /// @brief required to facilitate resize of an empty vtk rendering window : why ?
   // QWidget * m_manager;
    QVTKWidget *m_widget;
    vtkRenderer * m_render ;

    /// @brief VTK Interactor window
//     vtkRenderWindowInteractor *m_interactor;
//     vtkRenderWindow *m_renderWindow;


    /// @brief scene's renderers
    typedef std::map< RendererIdType , vtkRenderer* > RenderersMapType ;

    /// @brief scene's pickers
    typedef std::map< PickerIdType , vtkAbstractPropPicker * > PickersMapType ;

    /// @brief scene's transforms
    typedef std::map< VtkObjectIdType , vtkObject  * > VtkObjectMapType ;

    /// @brief Actives adaptors in scene
    typedef std::map< AdaptorIdType, SceneAdaptor > SceneAdaptorsMapType ;

    RenderersMapType     m_renderers;
    PickersMapType       m_pickers;
    SceneAdaptorsMapType m_sceneAdaptors;
    VtkObjectMapType     m_vtkObjects;

    bool m_pendingRenderRequest;

    void startContext();
    void stopContext();

    void configurePicker   ( ConfigurationType conf );
    void configureRenderer ( ConfigurationType conf );
    void configureObject   ( ConfigurationType conf );
    void configureVtkObject( ConfigurationType conf );
};

}

#endif //_FWRENDERVTK_VTKRENDERSERVICE_HPP_
