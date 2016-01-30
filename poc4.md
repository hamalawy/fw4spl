# PoC04 : Principle of real time interactive simulation with Sofa #

# Introduction #

## Frameworks Version ##

This document has been created to be compatible with following versions of [FW4SPL](http://code.google.com/p/fw4spl/) and [SOFA](http://www.sofa-framework.org/) frameworks :

| **FW4SPL**  | **SOFA** |
|:------------|:---------|
| fw4spl\_0.7.3 | SOFA 1.0 |

## Objectives of the document "Proof of Concept" ##

The goal of this document is to understand how to develop an application allowing direct interaction on organs with the mouse, by using Sofa deformation engine.

Note: in this exercise we won't describe into details the methods already explained in FW4SPL tutorials.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=nYdmboDWIZ4' target='_blank'><img src='http://img.youtube.com/vi/nYdmboDWIZ4/0.jpg' width='425' height=344 /></a>

![http://wiki.fw4spl.googlecode.com/hg/poc4screen1.png](http://wiki.fw4spl.googlecode.com/hg/poc4screen1.png)

The application consists of a window containing two menus: "File" and "Sofa", as well as two tools which can be moved by using the mouse.
![http://wiki.fw4spl.googlecode.com/hg/poc4screen2.png](http://wiki.fw4spl.googlecode.com/hg/poc4screen2.png)

"File" menu enables user to load a .fxz file by choosing "Load patient" option.
![http://wiki.fw4spl.googlecode.com/hg/poc4screen3.png](http://wiki.fw4spl.googlecode.com/hg/poc4screen3.png)

We then choose to open a 'Generic fwXML Reader (Ircad)' file.
![http://wiki.fw4spl.googlecode.com/hg/poc4screen4.png](http://wiki.fw4spl.googlecode.com/hg/poc4screen4.png)

We select the wanted .fxz file.
![http://wiki.fw4spl.googlecode.com/hg/poc4screen5.png](http://wiki.fw4spl.googlecode.com/hg/poc4screen5.png)

The model shows up on the screen. The list of organs is displayed on the right part of the screen.
![http://wiki.fw4spl.googlecode.com/hg/poc4screen6.png](http://wiki.fw4spl.googlecode.com/hg/poc4screen6.png)

We choose the organs that have to be treated by Sofa, and then click on 'Start' menu to launch it.
![http://wiki.fw4spl.googlecode.com/hg/poc4screen7.png](http://wiki.fw4spl.googlecode.com/hg/poc4screen7.png)

We can now use the tools to have interactions with the organs.
![http://wiki.fw4spl.googlecode.com/hg/poc4screen8.png](http://wiki.fw4spl.googlecode.com/hg/poc4screen8.png)

## Prerequisites ##

This PoC adds a feature to the opSofa bundle. To be able to follow this lesson you must have previously read PoC2 document.
We suppose that you already have the opSofa bundle corresponding to PoC2 document.


## Principal of functioning ##

To be able to move a mesh object with the mouse, we use a Vtk object (a cube in this particular case), assigned to our mesh object.  As the Vtk object can be moved with the mouse, we get back its x y z coordinates to send them to our mesh object to be moved.

![http://wiki.fw4spl.googlecode.com/hg/poc4fonctionnement_en.gif](http://wiki.fw4spl.googlecode.com/hg/poc4fonctionnement_en.gif)



# Writing of bundle's files #

We should add two files to the opSofa bundle :
  * **BoxWidget.cpp**
  * **BoxWidget.hpp**

We should also modify 5 files :
  * **build.options**
  * **Plugin.xml**
  * **SofaBusiness.cpp** et **SofaBusiness.hpp**
  * **SofaSceneWriterSrv.cpp** et **SofaSceneWriterSrv.hpp**
  * **SofaCoreSrv.cpp**

The file tree diagram is displayed in the model below :

![http://wiki.fw4spl.googlecode.com/hg/poc4tree.gif](http://wiki.fw4spl.googlecode.com/hg/poc4tree.gif)


## BoxWidget Class ##

The files BoxWidget.cpp and BoxWidget.hpp contain the eponymous class corresponding to the service allowing the creation of the Vtk cube. This cube will be displayed on the screen and can be moved with the mouse.


### BoxWidget.hpp ###

First of all we have to protect the header against multiple inclusions :
```
#ifndef _OPSOFA_BOXWIDGET_HPP_
#define _OPSOFA_BOXWIDGET_HPP_
```

We indicate the necessary inclusions :
```
#include <fwRenderVTK/IVtkAdaptorService.hpp>
#include "opSofa/config.hpp"
```

These instructions are necessary to use vtkTransform and vtkBoxWidget2 classes :
```
class vtkTransform;
class vtkBoxWidget2;
class Transform;
```

It is necessary to organize the source code inside a namespace having the bundle's name (opSofa in that case).
```
namespace opSofa
{
```

In the 'public' part, we first describe the class builder and destroyer as well as the macro. As it's a Vtk object, our class inherits from IVtkAdaptorService.

```
class  BoxWidget : public ::fwRenderVTK::IVtkAdaptorService
{
public:
	fwCoreServiceClassDefinitionsMacro( (BoxWidget)(::fwRenderVTK::IVtkAdaptorService) );
	BoxWidget() throw();
	virtual ~BoxWidget() throw();
	void updateFromVtk();
```

We write the methods' prototypes which will be explained into details in the corresponding cpp file :

```
protected:
	 virtual void configuring() throw( ::fwTools::Failed );
	 virtual void doStart()     throw( ::fwTools::Failed );
	 virtual void doStop()      throw( ::fwTools::Failed );
	 virtual void doSwap()      throw( ::fwTools::Failed );
	 virtual void doUpdate()    throw( ::fwTools::Failed );
	 virtual void doUpdate( ::fwServices::ObjectMsg::csptr msg ) throw( ::fwTools::Failed );
```

We write the class attributes :
```
    vtkTransform* m_transform;
    WPTR(::opSofa::Transform) m_transformService;

private:
	::vtkBoxWidget2* m_vtkBoxWidget;
    vtkCommand* m_boxWidgetCommand;
    std::string m_idMesh;
	
	double m_scaleFactor;
    bool m_enableScaling;
    std::string m_idMesh;
};
} // namespace opSofa

#endif
```

### BoxWidget.cpp ###

The inclusion of these headers enables us to use the basic tools of vtk :
```
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkBoxRepresentation.h>
#include <vtkBoxWidget2.h>
```

The inclusion of these headers enables us to use the basic tools of FW4SPL framework :
```
#include <fwComEd/TransformationMatrix3DMsg.hpp>
#include <fwData/TransformationMatrix3D.hpp>
#include <fwServices/Base.hpp>
#include "fwData/Integer.hpp"
#include "fwData/Vector.hpp"
#include "fwData/Float.hpp"
#include "fwData/String.hpp"
#include <fwServices/registry/ObjectService.hpp> 
#include <fwServices/IEditionService.hpp> 
#include <visuVTKAdaptor/Transform.hpp>
#include <fwRenderVTK/vtk/fwVtkBoxRepresentation.hpp>
```

We include the .hpp file :
```
#include "opSofa/BoxWidget.hpp"
```

And add the namespace :
```
namespace opSofa
{
```


We add the BoxClallback class. It enables us to call the updateFromVtk method every time an action is executed on the vtk object (for example if we move the object with the mouse) :

```
class BoxClallback : public ::vtkCommand
{
public:
    static BoxClallback* New(::opSofa::BoxWidget* adaptor) { 
        BoxClallback *cb = new BoxClallback; 
        cb->m_adaptor = adaptor;
        return cb;
    }

     BoxClallback() {}
    ~BoxClallback() {}

    virtual void Execute( ::vtkObject* pCaller, unsigned long, void* )
    {
        m_adaptor->updateFromVtk();
    }

    ::opSofa::BoxWidget *m_adaptor;
};
```

We add the macro allowing the association of a service with an object :
```
REGISTER_SERVICE( ::fwRenderVTK::IVtkAdaptorService, BoxWidget, ::fwData::TransformationMatrix3D );
```

The class constructor is initializing m\_boxWidgetCommand attribute.
```
BoxWidget::BoxWidget() throw() : ::fwRenderVTK::IVtkAdaptorService(), m_vtkBoxWidget(0), m_scaleFactor(1.0), m_enableScaling(true)
{
    m_boxWidgetCommand = BoxClallback::New(this);
}
```

Class destroyer is displayed below :
```
BoxWidget::~BoxWidget() throw()
{
}
```

This method is called before service start-up. We get back "renderer" and "transform" elements of the xml file there to inject them into our class. We also get back idMesh which will allow the identification of every BoxWidget instance.
```
void BoxWidget::configuring() throw( ::fwTools::Failed )
{
    setRenderId( m_configuration->getAttributeValue( "renderer" ) );
    this->setTransformId( m_configuration->getAttributeValue("transform") );

    if (m_configuration->hasAttribute("scaleFactor"))
    {
        m_scaleFactor = ::boost::lexical_cast<double>(m_configuration->getAttributeValue("scaleFactor"));
    }

    if (m_configuration->hasAttribute("enableScaling"))
    {
        SLM_ASSERT("Wrong value for 'enableScaling', must be 'true' or 'false'",
                m_configuration->getAttributeValue("enableScaling") == "yes" ||
                m_configuration->getAttributeValue("enableScaling") == "no");
        m_enableScaling = (m_configuration->getAttributeValue("enableScaling") == "yes");
    }

    m_idMesh = m_configuration->getAttributeValue("idMesh");
}
```

This method is called at initialization. Here we create our Vtk cube to be displayed :
```
void BoxWidget::doStart() throw( ::fwTools::Failed )
{
```

We get back 'transform' object of our Vtk cube, then assign x y z coordinates to it.
```
    m_transform = getTransform();
    SLM_ASSERT("BoxWidget need a vtkTransform", m_transform);
```

We then create a representation of the Vtk object in order to add the "transform" object.
```
    vtkBoxRepresentation *boxRep = vtkBoxRepresentation::New();
	boxRep->SetPlaceFactor(m_scaleFactor);

    double bounds[] = {-1,1,-1,1,-1,1};
    boxRep->PlaceWidget(bounds);
	
    boxRep->SetTransform(m_transform);
```

We create our Vtk cube to add here our representation :
```
    m_vtkBoxWidget = ::vtkBoxWidget2::New();
    m_vtkBoxWidget->SetRepresentation(boxRep);
    m_vtkBoxWidget->SetInteractor( this->getInteractor() );
	if (!m_enableScaling)
    {
        boxRep->ScalingEnabledOff();
    }
    m_vtkBoxWidget->On();
    m_vtkBoxWidget->AddObserver( ::vtkCommand::InteractionEvent, m_boxWidgetCommand );
}
```

The method below is called when the Vtk object is deleted :
```
void BoxWidget::doStop() throw( ::fwTools::Failed )
{
    unregisterServices();

    m_transform->Delete();
    m_transform = 0;
    m_vtkBoxWidget->RemoveObserver( m_boxWidgetCommand );
    m_vtkBoxWidget->Delete();
    m_vtkBoxWidget = 0;
}
```

Every time our cube is modified (for example moved with the mouse) this method is called :
```
void BoxWidget::updateFromVtk()
{
```


```
    m_vtkBoxWidget->RemoveObserver( m_boxWidgetCommand );

    vtkBoxRepresentation *repr = vtkBoxRepresentation::SafeDownCast( m_vtkBoxWidget->GetRepresentation() );
    if( repr )
    {
        repr->GetTransform(m_transform);
        m_transform->Modified();
    }
```

We get back the x y z cube coordinates to send them as a message to SofaCore :
```
    double tab[3];
    m_transform->GetPosition(tab);
	
    ::fwData::Vector::NewSptr data;
    ::fwData::String::NewSptr v1(m_idMesh);
    ::fwData::Integer::NewSptr v2(tab[0]);
    ::fwData::Integer::NewSptr v3(tab[1]);
    ::fwData::Integer::NewSptr v4(tab[2]);
    data->push_back(v1);
    data->push_back(v2);
    data->push_back(v3);
    data->push_back(v4);


    // Notification
    ::fwData::TransformationMatrix3D::sptr trf = this->getObject< ::fwData::TransformationMatrix3D >();
    ::fwServices::ObjectMsg::NewSptr msg2;
    msg2->addEvent("MOVE_MESH_SOFA", data);
    ::fwServices::IEditionService::notify(this->getSptr(), trf, msg2);


    m_vtkBoxWidget->AddObserver( ::vtkCommand::InteractionEvent, m_boxWidgetCommand );
}
```

The doUpdate methods are useless for our case :
```
void BoxWidget::doUpdate() throw( ::fwTools::Failed )
{
}

void BoxWidget::doUpdate( ::fwServices::ObjectMsg::csptr msg ) throw( ::fwTools::Failed )
{
}
}
```



## SofaCoreSrv Class ##

We will modify the SofaCoreSrv class in order to manage the messages sent by our Vtk cube (BoxWidget).

### SofaCoreSrv.cpp ###

We must add several inclusions :
```
#include <fwData/Integer.hpp>
#include <fwData/Vector.hpp>
```

We must add a few lines at the end of "updating(fwServices::ObjectMsg::csptr)" method.
If we get "MOVE\_MESH\_SOFA" message, then we get back the message data to send them to Sofa using "moveMesh" method.
```
    else if (msg->hasEvent("MOVE_MESH_SOFA")) {
        if (sofa) {
            ::fwData::Vector::csptr data = ::fwData::Vector::dynamicConstCast(msg->getDataInfo("MOVE_MESH_SOFA"));
            ::fwData::String::csptr idMesh = ::fwData::String::dynamicConstCast(data->at(0));
            ::fwData::Integer::csptr x = ::fwData::Integer::dynamicConstCast(data->at(1));
            ::fwData::Integer::csptr y = ::fwData::Integer::dynamicConstCast(data->at(2));
            ::fwData::Integer::csptr z = ::fwData::Integer::dynamicConstCast(data->at(3));
            sofa->moveMesh(idMesh->value(), x->value(), y->value(), z->value());
        }
    }
```



## SofaBusiness Class ##

In this class we are going to add the "moveMesh" method which will allow to send to Sofa the new coordinates of the object.

### SofaBusiness.cpp ###

We create the moveMesh method taking as argument the new x y z coordinates :
```
void SofaBusiness::moveMesh(std::string idMesh, int x, int y, int z)
{
```

We get back "MechanicalObjectRigid3f" object which represents the object to modify into Sofa :
```
    MechanicalObjectRigid3f *mechanical = (MechanicalObjectRigid3f*) (groot->getObject(sofa::core::objectmodel::TClassInfo<MechanicalObjectRigid3f>::get(), idMesh));
```

We assign new coordinates x y z to this object :
```
    VecCoordRigid3f& coord = *mechanical->getX();
    coord[0][0] = x;
    coord[0][1] = y;
    coord[0][2] = z;
}
```


## SofaSceneWriterSrv Class ##
We are going to modify this file so that it can create some scene files able to manage external interactions.

### SofaSceneWriterSrv.hpp ###
We need to add into this file the m\_useTempPath attribute :
```
bool m_useTempPath;
```

### SofaSceneWriterSrv.cpp ###
In the builder we need to add one line to initialize the attribute :
```
m_useTempPath = false;
```

In this method called "configuring()" we add the following line to get back the value of useTempPath present in config.xml file :
```
    if(m_configuration->findConfigurationElement("useTempPath"))
    {
        if (m_configuration->findConfigurationElement("useTempPath")->getValue() == "yes") {
            m_useTempPath = true;
        }
    }
```

In the "updating()" method we need to add code to specify if we write or not in a temporary folder :
```
// Ask folder destination
    QString folder;
    if (m_useTempPath) {
        folder = QDir::tempPath() + QDir::separator().toAscii() + "opsofascene";
        QDir dir;
        dir.mkdir(folder);
    } else if (writeTrian) {
        folder = QFileDialog::getExistingDirectory(0, "Choose a folder to write file scene");
    } else {
        folder = QFileDialog::getSaveFileName(0, "Write file scn", QString(), "Scene (*.scn)");
    }
    if (folder == "") return;

    // Get acquisition
    ::fwData::Acquisition::sptr acq = this->getObject< ::fwData::Acquisition >();
    SLM_ASSERT("Associated object is not an acquisition", acq);

    // Get templates
    QString templateFile;
    QString nodeTemplateFile;
    if (m_sceneTemplate != "") {
        QFile file("./Bundles/opSofa_0-1/" + QString(m_sceneTemplate.c_str()));
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        templateFile = file.readAll();
        QFile file2("./Bundles/opSofa_0-1/node" + QString(m_sceneTemplate.c_str()));
        file2.open(QIODevice::ReadOnly | QIODevice::Text);
        nodeTemplateFile = file2.readAll();
    } else {
        QFile file("./Bundles/opSofa_0-1/template.xml");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        templateFile = file.readAll();
        QFile file2("./Bundles/opSofa_0-1/nodeTemplate.xml");
        file2.open(QIODevice::ReadOnly | QIODevice::Text);
        nodeTemplateFile = file2.readAll();
    }

    // copy tools
    if (writeTrian && m_sceneTemplate != "") {
        QFile file("./Bundles/opSofa_0-1/cam.trian");
        file.copy(folder + "/cam.trian");
        QFile file2("./Bundles/opSofa_0-1/mors2.trian");
        file2.copy(folder + "/mors2.trian");
    }

    // Create nodes
    QString nodesData;

    // Travel each reconstructions
    std::pair< ::fwData::Acquisition::ReconstructionIterator, ::fwData::Acquisition::ReconstructionIterator > reconstructionIters = acq->getReconstructions();
    ::fwData::Acquisition::ReconstructionIterator reconstruction = reconstructionIters.first;
    for(; reconstruction != reconstructionIters.second;  ++reconstruction) {
        ::fwData::Reconstruction::sptr rec = (*reconstruction);

        // Get info organ
        QString organName = QString(rec->getOrganName().c_str());
        bool organVisible = rec->getIsVisible();
        QString organUid = QString(rec->getID().c_str());
        ::boost::filesystem::path filename = "";

        if (organVisible) {
            // Save mesh in filesystem
            if (writeTrian) {
                ::fwData::TriangularMesh::sptr mesh = rec->getTriangularMesh();
                std::stringstream meshPath;
                meshPath << folder.toStdString() << QDir::separator().toAscii() << organName.toStdString() << ".trian";
                filename = ::boost::filesystem::path(meshPath.str());
                ::fwDataIO::writer::TriangularMeshWriter writer;
                writer.setObject(mesh);
                writer.setFile(filename);
                writer.write();
            }

            // Parse nodeTemplate
            QString nodeFile = nodeTemplateFile;
            nodeFile.replace("ORGAN_NAME", organName);
            nodeFile.replace("ORGAN_ID", organUid);
            nodeFile.replace("TRIAN_LOCATION", QString(filename.string().c_str()));
            
            // Add node
            nodesData += nodeFile;
        }
    }    
    
    // Parse template
    templateFile.replace("AREMPLACER", nodesData);

    // Save file scn
    std::string scnFile;
    if (writeTrian) {
        scnFile =  folder.toStdString() + QDir::separator().toAscii()  + "sofa.scn";
    } else {
        scnFile =  folder.toStdString();
    }
    std::ofstream fileout (scnFile.c_str(), std::ios::out);
    fileout << templateFile.toStdString();
    fileout.close();

    // Ask launch animation
    int answer;
    if (m_useTempPath) {
        answer = QMessageBox::Yes;
    } else {
        answer = QMessageBox::question(0, "Write successful !", "Do you want to launch animation ?", QMessageBox::Yes | QMessageBox::No);
    }

    // If answer is yes
    if (answer == QMessageBox::Yes) {
        // Notification
        ::fwServices::ObjectMsg::NewSptr msg;
        ::fwData::String::sptr data(new ::fwData::String(scnFile));
        msg->addEvent( "NEW_SOFA_SCENE", data );
        ::fwServices::IEditionService::notify(this->getSptr(), acq, msg);
    }
```


## Build.options file ##

We should add library "fwRenderVTK0-1" and bundle "visuVTKAdaptor0-1" :
```
TYPE    = 'bundle'
VERSION = '0-1'
USE     = [
          'boost',
          'boostFilesystem',
          'qtCore',
          'sofasimulation',
          'sofacomponentodesolver',
          'sofacomponentvisualmodel',
          'sofatree',
          'sofacomponentconstraint',
          'sofacomponentbehaviormodel',
          'sofacomponent',
          'sofahelper'
          ]
LIB     = [
          'fwTools_0-1',
          'fwCore_0-1',
          'fwRuntime_0-3',
          'fwData_0-1',
          'fwDataIO_0-2',
          'fwServices_0-1',
          'fwVtkIO_0-1',
          'fwGui_0-1',
          'fwGuiQt_0-1',
          'fwRenderVTK_0-1',
          ]
BUNDLES = ['io_0-1', 'gui_0-1', 'visuVTKAdaptor_0-1']
```


# Files modification needed for the application #

We need to create the application profile and configuration in **profileQt.xml** and **config.xml** files.


## Build.options file ##
In this file we indicate the application version and the bundles we use :
```
TYPE    = 'bundle'
VERSION = '0-1'
BUNDLES = [
    'launcher_0-1'           ,
    'dataReg_0-1'            ,
    'servicesReg_0-1'        ,
    'gui_0-1'                ,
    'guiQt_0-1'              ,
    'io_0-1'                 ,
    'visu_0-1'               ,
    'ioData_0-1'             ,
    'ioVTK_0-1'              ,
    'ioXML_0-1'              ,
    'uiVisuQt_0-1'           ,
    'uiImageQt_0-1'          ,
    'uiReconstructionQt_0-1' ,
    'uiAcquisitionQt_0-1'    ,
    'uiIO_0-1'               ,
    'visuVTK_0-1'            ,
    'visuVTKQt_0-1'          ,
    'visuVTKAdaptor_0-1'     ,
    'opVTKMesh_0-1'          ,
    'ctrlSelection_0-1'      ,
    'opSofa_0-1'             ,
]
```

## ProfileQt.xml file ##
We use "Qt" version of the profiles file, established since version 0.7.0 of FW4SPL framework. This file resumes the classic profile of the applications developed by IRCAD in the FW4SPL tutorials, and adds the activation of opSofa bundle.

```
<profile name="PoC04SofaInteractive" version="0.1" check-single-instance="true"> 

    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" /> 

    <activate id="gui" version="0-1" />
    <activate id="guiQt" version="0-1" />

	<activate id="PoC04SofaInteractiveConf" />
	<activate id="appXml" version="0-1">
		<param id="config" value="PoC04SofaInteractiveID" />
	</activate>
	
    <activate id="io" version="0-1" />
    <activate id="visu" version="0-1" />

    <activate id="ctrlSelection" version="0-1" />

    <activate id="ioVTK" version="0-1" />
    <activate id="ioXML" version="0-1" />
    <activate id="ioData" version="0-1" />

    <activate id="visuVTK" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />
    <activate id="visuVTKAdaptor" version="0-1" />

    <activate id="uiIO" version="0-1" />
    <activate id="uiVisuQt" version="0-1" />
    <activate id="uiImageQt" version="0-1" />
    <activate id="uiReconstructionQt" version="0-1" />
    <activate id="uiAcquisitionQt" version="0-1" />

    <activate id="opSofa" version="0-1" />

    <activate id="opVTKMesh" version="0-1" />
```

We start the following components on opening of the application :
```
    <start id="visuVTK" />
    <start id="visuVTKQt" />
    <start id="visuVTKAdaptor" />
    <start id="guiQt" />
	<start id="appXml" />
	
</profile>
```


## plugin.xml file ##
plugin.xml file allows instanciation of data and services used in the application.

```
<plugin id="PoC04SofaInteractiveConf">

	<requirement id="servicesReg" />
	
	<extension implements="::fwServices::registry::AppConfig">
		<id>PoC04SofaInteractiveID</id>
		<type>standard</type>
		<config>
```

We work with a composite object :
```
    <object type="::fwData::Composite">
      <serviceList>
```

We define the GUI :
```
        <service uid="MyIHM" implementation="::gui::frame::DefaultFrame" type="::fwGui::IFrameSrv" autoComChannel="no">
          <gui>
            <frame>
              <name>Altran Est - Medic@ : PoC 4 - Principe de simulation interactive temps reel avec Sofa</name>
              <icon>Bundles/PoC04SofaInteractive_0-1/tuto.ico</icon>
              <minSize width="800" height="600"/>
            </frame>
            <menuBar />
          </gui>
          <registry>
            <menuBar sid="myMenuBar" start="yes" />
            <view sid="multiView_scene1" start="yes" />
          </registry>
        </service>
```

We define the menu :
```
        <service uid="myMenuBar" type="::fwGui::IMenuBarSrv" implementation="::gui::aspect::DefaultMenuBarSrv" autoComChannel="no" >
          <gui>
            <layout>
              <menu name="File" />
              <menu name="Sofa" />
            </layout>
          </gui>
          <registry>
            <menu sid="menu_File" start="yes" />
            <menu sid="menu_Sofa" start="yes" />
          </registry>
        </service>
```

We define "File" menu :
```
        <service uid="menu_File" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no" >
          <gui>
            <layout>
              <menuItem name="Load patient" shortcut="Ctrl+O" />
              <separator />
              <menuItem name="Quit" specialAction="QUIT" shortcut="Ctrl+Q" />
            </layout>
          </gui>
          <registry>
            <menuItem sid="action_openFile" start="yes" />
            <menuItem sid="action_quit" start="yes" />
          </registry>
        </service>
        
        <service uid="action_openFile" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
          <start uid="myReaderPathFile" />
        </service>

        <service uid="action_quit" type="::fwGui::IActionSrv" implementation="::gui::action::QuitAction" autoComChannel="no" />
```

We define "Sofa" menu :
```
        <service uid="menu_Sofa" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no" >
          <gui>
            <layout>
              <menuItem name="Start" shortcut="Ctrl+S" />
            </layout>
          </gui>
          <registry>
            <menuItem sid="mySofaWriterAction" start="no" />
          </registry>
        </service>
```

We divide screen board in two parts: one for visualization and the other one to display the list of organs :
```
        <service uid="multiView_scene1" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no" >
          <gui>
            <layout type="::fwGui::CardinalLayoutManager">
              <view align="center" minWidth="200"/>
              <view align="right" minWidth="200" minHeight="200" position="0" />
              <view align="right" minWidth="200" minHeight="80" position="1" />
            </layout>
          </gui>
          <registry>
            <view sid="genericScene" start="yes" />
            <view sid="listOrganEditor" />
          </registry>
        </service>
        
      </serviceList>
```

To be able to read patient files (.fxz), following code has been developped :
```
      <item key="myPatientDB">
        <object uid="myPatientDB" type="::fwData::PatientDB">
          <service uid="myReaderPathFile" implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IDialogEditor" autoComChannel="no">
            <type mode="reader" />
            <selection mode="exclude" />
            <addSelection service="::ioXML::FwXMLPatientDBReaderService" />
          </service>
          <service implementation="::ctrlSelection::wrapper::PatientDBWrapperSrv" type="::ctrlSelection::IWrapperSrv" uid="newOBject2newPatientDB" autoComChannel="yes" />
          <service uid="myUpdaterComCannelAcq" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no">
            <target>myUpdaterAcq</target>
          </service>
        </object>
      </item>
```

We set up the "composite visualization" part :
```
      <item key="myCompositeVisu">
        <object uid="myCompositeVisu" type="::fwData::Composite">

          <service uid="myUpdaterAcq" implementation="::ctrlSelection::updater::AcqFromPDBUpdaterSrv" type="::ctrlSelection::IUpdaterSrv" autoComChannel="no">
            <update compositeKey="myAcquisition" onEvent="NEW_LOADED_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP" />
            <update compositeKey="myAcquisition" onEvent="CLEAR_PATIENT" fromUID="myPatientDB" actionType="REMOVE" />
          </service>
```

We use a swapper to be able to dynamically load/unload the services :
```
          <service uid="myManager" implementation="::ctrlSelection::manager::SwapperSrv" type="::ctrlSelection::IManagerSrv" autoComChannel="yes" priority="0.6"  >
            <mode type="stop" />
            <config>
```

We set up the services which work on the acquisition object :
```
              <object id="myAcquisition" type="::fwData::Acquisition" >
```

Service that allows the display of the organs list :
```
                <service uid="listOrganEditor" implementation="::uiAcquisition::OrganListEditor" type="::gui::editor::IEditor" autoComChannel="yes" />
```

Service that allows the use of our opSofa bundle :
```
                <service uid="mySofaCore" implementation="::opSofa::SofaCoreSrv" type="::fwGui::IActionSrv" autoComChannel="yes" >
                </service>
```

Service that allows the creation of a Sofa scene from the acquisition object :
```
                <service uid="mySofaWriter"
                           implementation="::opSofa::SofaSceneWriterSrv"
                         type="::io::IWriter" autoComChannel="no" >
                  <writeTrian>yes</writeTrian>
                  <sceneTemplate>templatetools.xml</sceneTemplate>
                  <useTempPath>yes</useTempPath>
                </service>

                <service uid="mySofaWriterAction"
                           implementation="::gui::action::StarterActionService"
                           type="::fwGui::IActionSrv" autoComChannel="no" >
                  <start uid="mySofaWriter"/>
                  <start uid="myUpdaterComCannelSofa"/>
                </service>
```

Service that allows the animation display :
```
                <service uid="mySofaStart"
                              implementation="::opSofa::SofaStartSrv"
                              type="::fwGui::IActionSrv" autoComChannel="no" >
                </service>

              </object>
            </config>
          </service>
```

We define our generic scene :
```
          <service uid="genericScene" implementation="::fwRenderVTK::VtkRenderService" type="::fwRender::IRender" autoComChannel="yes">
            <scene>
              <picker id="myPicker" vtkclass="fwVtkCellPicker" />
              <renderer id="default" background="0.0" />
              <adaptor id="myRenderAdaptor" class="::visuVTKAdaptor::Render" objectId="self">
                <config renderer="default" picker="" />
              </adaptor>
              <adaptor id="myAcquisitionAdaptor" class="::visuVTKAdaptor::Acquisition" objectId="myAcquisition">
                <config renderer="default" picker="" />
              </adaptor>
```

We create our two Vtk objects which will include our tools :
```
              <vtkObject id="tool1MatrixTransform" class="vtkTransform" />
              <vtkObject id="tool2MatrixTransform" class="vtkTransform" />
```

We associate both meshes with their Vtk objects :
```
              <adaptor id="tool1MeshAdaptor" class="::visuVTKAdaptor::TriangularMesh" objectId="tool1Mesh">
                <config renderer="default" picker="myPicker" transform="tool1MatrixTransform" />
              </adaptor>

              <adaptor id="tool2MeshAdaptor" class="::visuVTKAdaptor::TriangularMesh" objectId="tool2Mesh">
                <config renderer="default" picker="myPicker" transform="tool2MatrixTransform" />
              </adaptor>
```

We associate a Vtk cube with every mesh :
```
              <adaptor id="tool1Widget" class="::opSofa::BoxWidget" objectId="view1">
                <config renderer="default" transform="tool1MatrixTransform" scaleFactor="2.0" enableScaling="no" idMesh="souris_mors2" />
              </adaptor>

              <adaptor id="tool2Widget" class="::opSofa::BoxWidget" objectId="view1">
                <config renderer="default" transform="tool2MatrixTransform" scaleFactor="2.0" enableScaling="no" idMesh="souris_cam" />
              </adaptor>

            </scene>
          </service>
```

We create an autoComChannel between Vtk cubes and opSofa bundle :
```
          <item key="view1">
            <object uid="view1" type="::fwData::TransformationMatrix3D">
              <service uid="myUpdaterComCannelSofa" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no">
                <target>mySofaCore</target>
              </service>
            </object>
          </item>
```

We then load the two meshes which will represent the tools :
```
          <item key="tool1Mesh">
            <object uid="tool1Mesh" type="::fwData::TriangularMesh">
              <service uid="tool1ReaderFile" implementation="::ioData::TriangularMeshReaderService" type="::io::IReader" autoComChannel="no" >
                <filename>Bundles/opSofa_0-1/cam.trian</filename>
              </service>
            </object>
          </item>

          <item key="tool2Mesh">
            <object uid="tool2Mesh" type="::fwData::TriangularMesh">
              <service uid="tool2ReaderFile" implementation="::ioData::TriangularMeshReaderService" type="::io::IReader" autoComChannel="no" >
                <filename>Bundles/opSofa_0-1/mors2.trian</filename>
              </service>
            </object>
          </item>
          
        </object>
      </item>
```

We define the services to start when application starts :
```
      <start uid="MyIHM" />
      <start uid="myUpdaterAcq" />
      <start uid="myManager" />
      <start uid="myUpdaterComCannelAcq" />
      <start uid="newOBject2newPatientDB"/>
      <start uid="tool1ReaderFile"/>
      <start uid="tool2ReaderFile"/>
      <update uid="tool1ReaderFile"/>
      <update uid="tool2ReaderFile"/>
```

We close the opened tags :
```

    </object>
	</config>
  </extension>
</plugin>
```



# Application launching #

To start our application, we use the following command line from "Install" folder :

./bin/launcher.exe ./Bundles/PoC04SofaInteractive\_0-1/profileQt.xml


# Remaining technological barriers #

The first detected technological barrier is to correctly configurate the Sofa scene so that interactions between the different organs get closer to reality. At the moment, each organ has the same "elastic" move when touched by a tool. Moreover, you can see it on the video.
To succeed in that task, a very good knowledge of scene editing feature into Sofa and on anatomy of each organ is required.


The second detected technological barrier is the calculation time. Collision management needs a lot of calculation power/time. Time needed to calculate collisions can become very quickly a bottleneck or blocking issue.
Here is a performance chart showing FrameRate per second as a function of points number.

![http://wiki.fw4spl.googlecode.com/hg/poc4lock2.png](http://wiki.fw4spl.googlecode.com/hg/poc4lock2.png)

These tests have been done on a Windows XP :
  * CPU : Intel Core 2 Duo E8200 @ 2.66GHz
  * Memory : 3GB RAM
  * GPU : ATI Radeon HD 2400

Third detected technological barrier is linked to management of object deformation, induced by the tool moved with the mouse, or even by an organ.
During collision, this tool or organ is not deformable and only the percuted organ gets distorded.
