# Tutorial 6 : tutoBundle #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  | See below    |
| fw4spl\_0.5  | [See Rev ebaf47ffa0](Tutorial6_fw4spl_05_Revision_ebaf47ffa0.md) |
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial6_fw4spl_06_Revision_e2e9e6239200.md) |
| fw4spl\_0.7.0 | [TutoBundle](Tutorial6.md) |
| experimental |              |

## Tutorial aim ##

The tutorial **tutoDataServiceBasic** explains how to create a **bundle** containing an image visualization **service** as a 3D negatoscope ([VTK documentation](http://www.vtk.org/VTK)).
This **bundle** has been used in [tutoDataService](Tutorial3.md).
In the context of this tutorial, we will not describe in details the method allowing to visualize a file representing a 3D medical image using the VTK library.

As shown in the [Global description of the Framework](Description.md), the creation of a new **bundle** requires the following files :

  * **build.options**
  * **plugin.xml**
  * **RendererService.hpp** and **RenderService.cpp**
  * **Plugin.hpp** and **Plugin.hpp**

**Important**: All that files must be save in the directory structure describes in the following picture.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial06Resources/bundleDirBundle.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial06Resources/bundleDirBundle.png)

This bundle does not need specific resources (such as icones, images, patient data...) thus in the **rc** folder, only the file **plugin.xml** exists (cf diagram above).

First of all, we will describe the files **RendererService.cpp** and **RendererService.hpp** that contain the visualization **service** code of our 3D negatoscope.
Then, we will describe the **service** contained in the **plugin.xml** file.
Finally, we will describe the **build.options** file allowing to inform the compilation system ([sconspiracy](Build.md)) in order to build the **bundle** which means the creation of the vtkSimpleNegato\_libVersion.dll file under Windows or the vtkSimpleNegato\_libVersion.so file under Linux or Mac.

## Writing of the different files ##

### RendererService.cpp and RendererService.hpp files ###

The RendererService.cpp and RendererService.hpp files contain the eponymous class corresponding to the visualization **service**.
This class must contain the five following methods :
  1. The **configuring** method configures the **service** parameters, here we recover the window identifier where the image will be displayed.
  1. The **starting** method launches the **service**, here we instantiate the components (containers) wxWidgets and VTK used in this class.
  1. The **stopping** method stops the **service**, here we build the negatoscopes, the wxWidgets and VTK components instantiated in the **starting** method.
  1. The **updating** method is called in order to make the service carry out its action on the associated **data**. Here, when a message arrives, which means a new image has been loaded, the VTK scene rendering is done.
  1. The **updating(message)** method is called when the service receives a message. Here, when a message arrives, informing us that a new image has been loaded, the scene is updated.

#### RendererService.hpp ####

We must protect our header against multiple inclusions. So, we use **#ifndef** and **#define** (supported by all compilation system).

We use the file name and the namespace hierarchy to build the protection tag, in order to avoid any conflict with the file having the same name but not present in the same namespace (`_NAMESPACE_CLASSE_HPP_`).

```
#ifndef _VTKSIMPLENEGATO_RENDERERSERVICE_HPP_
#define _VTKSIMPLENEGATO_RENDERERSERVICE_HPP_
```

These header inclusions allow to use the wxAuiManager components of the library [wxWidgets](http://www.wxwidgets.org/). This component allows to manage the wxWidgets element positions in the window. We try as much as possible to minimize the inclusions in a header file.

```
#include <wx/wxprec.h>
#include <wx/aui/aui.h>
```

The following header inclusions allow to use the components of the [VTK](http://www.vtk.org/) library.

```
#include  <vtkRenderer.h>
#include  <vtkImagePlaneWidget.h>
#include  <vtkOutlineFilter.h>
```

This header inclusion allows to use the wxVTKRenderWindowInteractor components of the [vtkINRIA3D](https://gforge.inria.fr/projects/vtkinria3d/) library. This component allows to use VTK in a wxWidgets windows.

```
#include <vtkinria3d/wxVTKRenderWindowInteractor.h>
```

This header must be include, in order to write our class deriving from IRender.

```
#include <fwRender/IRender.hpp>
```

By convention, we usualy organize our coding in a namespace having the bundle name. The namespaces can be seen as module delimiters.

```
namespace vtkSimpleNegato
{
```

We create the RendererService class deriving from ::fwRender::IRender. This class is the base of the visualization services.

The class name must be preceded by the macro **VTKSIMPLENEGATO\_CLASS\_API** that allows to manage the class visibility in the system. This macro is generated by the construction system. The following macro format must be respected : **NAMESPACE\_CLASS\_API**.

Some methods must be visible, therefore, it exists macro generated by the construction system using a **NAMESPACE\_API** form.

```
class VTKSIMPLENEGATO_CLASS_API RendererService : public ::fwRender::IRender
{
```

We use the **public** section first. This section is going to contain the class constructor and destructor, and also the macro.

The **fwCoreServiceClassDefinitionsMacro** macro allows to manage the common code to all service class types:
  * typedef list for the class (sptr, wptr, csptr et cwptr)
  * helpers allowing to make class conversions (cast)

```
public :
    fwCoreServiceClassDefinitionsMacro ( (RendererService)(::fwRender::IRender::Baseclass) );

    VTKSIMPLENEGATO_API RendererService() throw() ;
    VTKSIMPLENEGATO_API virtual ~RendererService() throw() ;
```

All services must implement five basic methods (_configuring, starting, stopping, updating and updating(message)_). They must be positionned in the **protected** section.

```
protected:
    VTKSIMPLENEGATO_API virtual void configuring() throw( ::fwTools::Failed);
    VTKSIMPLENEGATO_API virtual void starting() throw( ::fwTools::Failed);
    VTKSIMPLENEGATO_API virtual void stopping() throw( ::fwTools::Failed);
    VTKSIMPLENEGATO_API virtual void updating() throw( ::fwTools::Failed);
    VTKSIMPLENEGATO_API virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw( ::fwTools::Failed);
```

The **private** section is the last one. Methods and variables, which are situated in this section, are only used by our class (wxWidgets and VTK objects, initialisation and updating methods of the VTK scene).

```
private :
    void initVTKPipeline();
    void updateVTKPipeline();

    wxAuiManager* m_wxmanager;
    ::wxVTKRenderWindowInteractor* m_interactor;
    vtkOutlineFilter* m_outline;
    vtkImagePlaneWidget* m_negatoAxial;
    vtkImagePlaneWidget* m_negatoSagittal;
    vtkImagePlaneWidget* m_negatoFrontal;
    bool m_bPipelineIsInit;
    vtkRenderer * m_render ;
};

}
#endif
```

#### RendererService.cpp file ####

These header inclusions allow to use the panel and window components of wxWidgets library.

```
#include <wx/panel.h>
#include <wx/window.h>
```

This headers inclusion allows to use the VTK library components in order to display the medicale image.

```
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
```

This header inclusion allows to use the ::fwData::Image object.

```
#include <fwData/Image.hpp>
```

This header inclusion allows to check that the received message type is an ImageMsg.

```
#include <fwComEd/ImageMsg.hpp>
```

This header inclusion allows to use the helpers and a macro. The helpers simplify the service manipulations and the macro declares the association between the service and the object (REGISTER\_SERVICE).

```
#include <fwServices/Base.hpp>
```

This header inclusion allows to use the helpers. The helpers are able to convert the VTK objects into ::fwData::Object and the ::fwData::Object into VTK objects.

```
#include <vtkIO/vtk.hpp>
```

Header file of our class.

```
#include "vtkSimpleNegato/RendererService.hpp"
```

REGISTER\_SERVICE macro, corresponding to the association of a service with an object, is based on a string registration.

```
REGISTER_SERVICE( ::fwRender::IRender , ::vtkSimpleNegato::RendererService , ::fwData::Image) ;
```

```
namespace vtkSimpleNegato
{
```

Class constructor. In our case, there is no action.

```
RendererService::RendererService() throw()
    : m_render( 0 ), m_bPipelineIsInit(false)
{
}
```

Class destructor. In our case, there is no action.

```
RendererService::~RendererService() throw()
{
}
```

This method is called before the service starting.

```
void RendererService::configuring() throw( ::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->initGuiContainerId();
}
```

This method is called at the service starting. It will instantiate the wxWidget components (containers) used in this class. In particular, the m\_render component allows to control the VTK object rendering.

```
void RendererService::starting() throw(fwTools::Failed)
{
    this->initRender();
    m_bPipelineIsInit = false;

    wxWindow* const container = m_container ;
    assert( container ) ;
    m_wxmanager = new wxAuiManager( container );
    // Create a VTK-compliant window and insert it
    m_interactor = new ::wxVTKRenderWindowInteractor( container, -1 );
    m_wxmanager->AddPane( m_interactor, wxAuiPaneInfo().CentrePane() );
    // Repaint and resize window
    m_wxmanager->Update();

    // Renderer
    m_render = vtkRenderer::New();
    m_interactor->GetRenderWindow()->AddRenderer(m_render);
    // Repaint and resize window
    m_wxmanager->Update();
}
```

This method is called at the service stopping. It will destroy the negatoscopes, the wxWidgets containers and the renderer instantiated by the starting method. Concerning the negatoscopes, they are destroyed only if the pippeline has been instantiated. Indeed, negatoscopes are not necessarily instantiated.

```
void RendererService::stopping() throw(fwTools::Failed)
{
    if( m_render == 0 ) return;

    if( m_wxmanager == 0 ) return;

    if(m_bPipelineIsInit)
    {
        m_negatoSagittal->Delete();
        m_negatoFrontal->Delete();
        m_negatoAxial->Delete();
        m_outline->Delete();
    }

    assert( m_interactor );
    m_interactor->Delete();
    m_interactor = 0;

    assert( m_wxmanager );
    m_wxmanager->UnInit();
    delete m_wxmanager;
    m_wxmanager = 0;

    m_container->DestroyChildren() ;

    assert( m_render );
    m_render->Delete();
    m_render = 0;

        this->stopRender();
}
```

This method is called every time the system ask for a service updating.

```
void RendererService::updating() throw(fwTools::Failed)
{
    m_interactor->Render();
}
```

This method is called when the service receives a message.
If the message is an ImageMsg (warning that a new image is loaded), the method will create the VTK pipeline (if it does not exist, else it only updates the pipeline).
Finally the method will render the VTK scene (taking the planes positions into account).

```
void RendererService::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{
    // If message is a ImageMsg
    ::fwComEd::ImageMsg::csptr pImageMsg = ::fwComEd::ImageMsg::dynamicConstCast( _msg ) ;
    if (pImageMsg)
    {
        if( pImageMsg->hasEvent( ::fwComEd::ImageMsg::NEW_IMAGE ))
        {
            if(!m_bPipelineIsInit)
            {
                initVTKPipeline();
                m_bPipelineIsInit = true;
            }
            else
            {
                updateVTKPipeline();
            }

            ::fwData::Image::sptr img = this->getObject< ::fwData::Image >();
            unsigned int axialIndex = img->getSize()[2]/2;
            unsigned int frontalIndex = img->getSize()[1]/2;
            unsigned int sagittalIndex = img->getSize()[0]/2;

            m_negatoAxial->SetSliceIndex( axialIndex );
            m_negatoFrontal->SetSliceIndex( frontalIndex );
            m_negatoSagittal->SetSliceIndex( sagittalIndex );

            m_interactor->Render();
        }
    }
}
```

This method initializes the VTK pipeline and creates the VTK objects necessary for the tridimensional negatoscope render (sagittal, frontal and axial planes).
The camera is repositioned in order to center the objects into the scene.

```
void RendererService::initVTKPipeline()
{
    vtkImageData* vtk_img = ::vtkIO::toVTKImage( this->getObject< ::fwData::Image >());

    m_outline = vtkOutlineFilter::New();
    m_outline->SetInput(vtk_img);

    vtkPolyDataMapper* outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(m_outline->GetOutputPort());

    vtkActor* outlineActor =  vtkActor::New();
    outlineActor->SetMapper( outlineMapper);

    vtkCellPicker* picker = vtkCellPicker::New();
    picker->SetTolerance(0.005);

    //assign default props to the ipw's texture plane actor
    m_negatoSagittal = vtkImagePlaneWidget::New();
    m_negatoSagittal->SetInteractor( m_interactor);
    m_negatoSagittal->SetKeyPressActivationValue('x');
    m_negatoSagittal->SetPicker(picker);
    m_negatoSagittal->GetPlaneProperty()->SetColor(1,0,0);
    m_negatoSagittal->TextureInterpolateOn();
    m_negatoSagittal->SetInput(vtk_img);
    m_negatoSagittal->SetPlaneOrientationToXAxes();
    m_negatoSagittal->DisplayTextOn();
    m_negatoSagittal->On();
    m_negatoSagittal->InteractionOn();

    m_negatoFrontal = vtkImagePlaneWidget::New();
    m_negatoFrontal->SetInteractor( m_interactor);
    m_negatoFrontal->SetKeyPressActivationValue('y');
    m_negatoFrontal->SetPicker(picker);
    m_negatoFrontal->GetPlaneProperty()->SetColor(0,1,0);
    m_negatoFrontal->TextureInterpolateOn();
    m_negatoFrontal->SetInput(vtk_img);
    m_negatoFrontal->SetPlaneOrientationToYAxes();
    m_negatoFrontal->SetLookupTable( m_negatoSagittal->GetLookupTable());
    m_negatoFrontal->DisplayTextOn();
    m_negatoFrontal->UpdatePlacement();
    m_negatoFrontal->On();

    m_negatoAxial = vtkImagePlaneWidget::New();
    m_negatoAxial->SetInteractor( m_interactor);
    m_negatoAxial->SetKeyPressActivationValue('z');
    m_negatoAxial->SetPicker(picker);
    m_negatoAxial->GetPlaneProperty()->SetColor(0,0,1);
    m_negatoAxial->TextureInterpolateOn();
    m_negatoAxial->SetInput(vtk_img);
    m_negatoAxial->SetPlaneOrientationToZAxes();
    m_negatoAxial->SetLookupTable( m_negatoSagittal->GetLookupTable());
    m_negatoAxial->DisplayTextOn();
    m_negatoAxial->On();

    // Add the actors
    m_render->AddActor( outlineActor);

    // Repaint and resize window
    m_wxmanager->Update();
    m_render->ResetCamera();

    picker->Delete();
    outlineActor->Delete();
    outlineMapper->Delete();
}
```

This method updates the medical image visualized in the negatoscope.

```
void RendererService::updateVTKPipeline()
{
    assert(this->getObject< ::fwData::Image >());
    vtkImageData* vtk_img = ::vtkIO::toVTKImage( this->getObject< ::fwData::Image >());

    m_outline->SetInput(vtk_img);
    m_negatoSagittal->SetInput(vtk_img);
    m_negatoFrontal->SetInput(vtk_img);
    m_negatoAxial->SetInput(vtk_img);
}
}
```

### Plugin.hpp and Plugin.cpp files ###

The Plugin class contains a **start** method which guarantees the service registration. This method is called at the component loading. It contains also a **stop** method which is called at the bundle closing. In our example, the **stop** method does nothing.

#### Plugin.hpp file ####

```
#ifndef _VTKSIMPLENEGATO_PLUGIN_HPP_
#define _VTKSIMPLENEGATO_PLUGIN_HPP_

#include <fwRuntime/Plugin.hpp>

/*
 * @brief    Namespace vtkSimpleNegato.
 * @namespace    vtkSimpleNegato.
 * @author    IRCAD (Research and Development Team).
 * @date    2009.
 *
 *  The namespace vtkSimpleNegato contains a simple negato renderer using vtk.
 */
namespace vtkSimpleNegato
{

class Plugin : public ::fwRuntime::Plugin
{
public:
     /*
     * @brief    Destructor
     */
     ~Plugin() throw();

     /*
     * @brief Start method.
     *
     * @exception ::fwRuntime::RuntimeException.
     * This method is used by runtime to initialize the bundle.
     */
     void start() throw(::fwRuntime::RuntimeException);

     /*
     * @brief Stop method.
     *
     * This method is used by runtime to stop the bundle.
     */
     void stop() throw();
    
};

} // namespace vtkSimpleNegato

#endif //_VTKSIMPLENEGATO_PLUGIN_HPP_

```

#### Plugin.cpp file ####

```
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include "vtkSimpleNegato/Plugin.hpp"

namespace vtkSimpleNegato
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::vtkSimpleNegato::Plugin");

Plugin::~Plugin() throw()
{}

void Plugin::start() throw(::fwRuntime::RuntimeException)
{}

void Plugin::stop() throw()
{}

} // namespace vtkSimpleNegato
```


### Fichier plugin.xml ###

Remember that the **plugin.xml** file contains the descriptions of the **services** provided by the **bundle**. In this tutorial context, the **bundle** is composed of only one service called ::vtkSimpleNegato::RendererService. Therefore, our **plugin.xml** file contains an unique tag _point_ with id attribute corresponding to the **service** name.
Furthermore, into the _point_ tag, we must add between the _implements_ tag :

  * the base service name (::fwRender::IRender), because our service is an implementation of this basic service.

  * the **data** object type (::fwData::Image).

```
<plugin id="vtkSimpleNegato" class="::vtkSimpleNegato::Plugin" >
    <library name="vtkSimpleNegato" />

    <point id="::vtkSimpleNegato::RendererService" >
        <implements>::fwRender::IRender</implements>
        <implements>::fwData::Image</implements>
    </point>
</plugin>
```

### build.options file ###

Remember that the **build.options** file allows to inform the compilation system ([sconspiracy](Build.md)) by using the following fields :
  * the **TYPE** field allows to clarify the project type. Here, the project is a **bundle**.
  * the **USE** field allows to clarify the project dependences with the external library. Here, we need **wxWidgets** ('wx'), **VTK**, **boost** and **vtkINRIA3D** libraries.
  * the **LIB** field allows to clarify the project dependences with the compilation system project used by the bundle (**fwCore**, **fwRuntime**, **fwData** ...).
  * the **LOGLEVEL** field allows to give the debug information level. Here, we fix the level to 'trace', it means that all the debugging informations are shown (see [sconspiracy](Build.md)).
  * the **VERSION** field gives the component version number. Here, the bundle version is fixed to 0.1.

```
TYPE = 'bundle'
USE = ['wx','vtk','boost','vtkinria3d']
LIB = [
       'fwCore_0-1'     ,
       'fwRuntime_0-3'  ,
       'fwData_0-1'     ,
       'fwServices_0-1' ,
       'fwComEd_0-1'    ,
       'ioVTK_0-1'      ,
       'fwTools_0-1'    ,
       'fwWX_0-1'       ,
       'fwDataIO_0-2'   ,
       'fwRender_0-1'   ,
       ]
LOGLEVEL = 'trace'
VERSION = '0-1'
```

## Compiling project ##

So, to compile our bundle named **vtkSimpleNegato**, you just have to type:

```
racy vtkSimpleNegato
```