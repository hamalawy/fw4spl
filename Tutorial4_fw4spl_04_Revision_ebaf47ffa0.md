# Tutorial 4 : tutoComChannel #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  |  See below   |
| fw4spl\_0.5  | [See Rev ebaf47ffa0](Tutorial4_fw4spl_05_Revision_ebaf47ffa0.md) |
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial4_fw4spl_06_Revision_e2e9e6239200.md) |
| fw4spl\_0.7.0 |[tutoComChannel](Tutorial4.md) |

| experimental | |
|:-------------|:|

## Introduction ##

This tutorial presents the event basic managment in **FW4SPL**. Especially, it shows us how **services** communicate between themselves. Here, we create another basic application which renders the same mesh in three different windows. Theses windows represent a 3D scene which contains the mesh and whih is observed by a camera. Theses points of view can be modified by the user from an IHM interaction on the concerned window. This interection corresponds to the camera modification (focale, position and orientation) in the 3D scene.

Furthermore, it is possible to synchronise camera (point of view) between visualization services by using comminucation. For that, two camera types are characterized : master camera and slave camera. When the user interacts on a master camera, the visualization service notifies the camera modifications at all the other services. Then, a visualization service, which manages a slave camera, receives this notifications, it analyses messages and synchronizes its camera.


## Application definition ##

We want to create an application with three visualization services. All services visualize the same mesh and can communicate together.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=pit00j-XyiM' target='_blank'><img src='http://img.youtube.com/vi/pit00j-XyiM/0.jpg' width='425' height=344 /></a>

Like the precedent tutorial, the user can load a surfacic mesh from the "Open File" menu item.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial04Resources/tutoComChannel1.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial04Resources/tutoComChannel1.png)

However, in this application we propose two different mesh reader services : a service which reads vtk file format (.vtk), and another which reads trian file format (.trian). Thus, when the user interacts on the "Open File" menu item,  a dialog box proposes to select a reader service.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial04Resources/tutoComChannel2.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial04Resources/tutoComChannel2.png)

We are going to write a **profile** and a **configuration** again.

## TutoComChannel profile ##

In this tutorial, we use the **vtkSimpleMesh bundle** that provides a **service** allowing to display a surfacic mesh. Furthermore, we add the **ioData bundle** that contains the **service** allowing to read a mesh in the trian format (file .trian). the **service** allowing to read a mesh in the VTK format (fichier .vtk) is located in the  **ioVTK bundle**.

```
<profile name="tutoComChannel" version="0.1" >

    <!-- Basic bundles list. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    <activate id="visu" version="0-1" />

    <activate id="gui" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwData::TriangularMesh" />
        <param id="config" value="tutoComChannelConfig" />
        <param id="configFile" value="./Bundles/Tuto04ComChannel_0-1/configurations/config-04_ComChannel.xml" />
    </activate>

    <!-- Specific bundles list. -->
    <activate id="ioVTK" version="0-1" />
    <activate id="ioData" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="vtkSimpleMesh" version="0-1" />

    <start id="dataReg" />
    <start id="gui" />   
</profile>
```


## tutoComChannel configuration ##

As the previous tutorials, we are going to write a **configuration** file that is named **config-04\_ComChannel.xml**. **TutoComChannel** configuration is named **tutoComChannelConfig** (cf [profile](Tutorial4#tutoComChannel_profile.md)).

### Configuration file ###

```
<plugin id="TutoConfigurations">
<extension id="tutoComChannelConfig" implements="::fwData::TriangularMesh">

<!-- In tutoComChannel, the main data object type is ::fwData::TriangularMesh.-->
<object id="tutoComChannelMesh" type="::fwData::TriangularMesh">

    <service type="::gui::aspect::IAspect" implementation="::gui::aspect::DefaultAspect" autoComChannel="no" >
        <name>tutoComChannel</name>
        <icon>Bundles/Tuto04ComChannel_0-1/tuto.ico</icon>
        <minSize width="720" height="600"/>

        <!-- The application contains three different views. the different views are characterized by their identifiers, here 900, 901 et 902.
             Furthermore, a minimum window height and a width are given to the last two views.-->
        <views>
            <view guiContainerId="900"/>
            <view guiContainerId="901" minWidth="400" minHeight="100"/>
            <view guiContainerId="902" minWidth="400" minHeight="100"/>
        </views>

        <menus>
            <menu uid="menu_File"/>
        </menus>
    </service>


    <service name="File" uid="menu_File" type="::gui::aspect::IMenu" implementation="::gui::aspect::DefaultMenu" autoComChannel="no" >
        <action uid="action_openFile"/>
        <separator/>
        <action uid="action_quit"/>
    </service>


    <service uid="action_openFile" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" name="OpenFile" shortcut="Ctrl+O" autoComChannel="no" >
        <start uid="myReaderPathFile"/>
    </service>

    <service uid="action_quit" specialAction="QUIT"  name="Quit" type="::gui::action::IAction" implementation="::gui::action::QuitAction" shortcut="Ctrl+Q" autoComChannel="no" />



    <service uid="myReaderPathFile" implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" autoComChannel="no" >
        <type mode="reader"/>
    </service>

    <!-- Each visualization service tag can be set to the value "master" or "slave". 
    1) If the value is master, the service sends a message containing informations relative to the camera (position, rotation, focus), called CameraMsg, 
    to all the others services associated to the object ::fwData::TriangularMesh. Furthermore, the CameraMsg received from the others services will not be treated.    
    2) Otherwise (slave value), the service does not send CameraMsg. On the other hand, the service will treat the CameraMsg sent by the others services.
    3) The three services listen to the messages concerning meshs, as if they do not treat them. That is the reason why autoComChannel="yes".  
    Visualization service for the first window.-->
    <service implementation="::vtkSimpleMesh::RendererService" type="::fwRender::IRender" uid="myRenderingTuto1" autoComChannel="yes" >
        <masterSlaveRelation>master</masterSlaveRelation>
        <win guiContainerId="900"/>
    </service>

    
    <!-- Visualization service for the second window-->
    <service implementation="::vtkSimpleMesh::RendererService" type="::fwRender::IRender" uid="myRenderingTuto2" autoComChannel="yes" >
        <masterSlaveRelation>slave</masterSlaveRelation>
        <win guiContainerId="901"/>
    </service>


    <!-- Visualization service for the third window-->
    <service implementation="::vtkSimpleMesh::RendererService" type="::fwRender::IRender" uid="myRenderingTuto3" autoComChannel="yes" >
        <masterSlaveRelation>master</masterSlaveRelation>
        <win guiContainerId="902"/>
    </service>

    <start type="::gui::aspect::IAspect" />
    
    <start uid="myRenderingTuto1"/> 
    <start uid="myRenderingTuto2"/> 
    <start uid="myRenderingTuto3"/> 
    
    <stop uid="myRenderingTuto3"/>
    <stop uid="myRenderingTuto2"/>
    <stop uid="myRenderingTuto1"/>
    
    <stop type="::gui::aspect::IAspect" />

</object>

</extension>
</plugin>
```

## TutoComChannel application launching ##

To launch the application, we use the following command line, when located in the MyInstallDir directory :

```
./bin/launcher.exe ./Bundles/Tuto_0-1/profile-04_ComChannel.xml
```