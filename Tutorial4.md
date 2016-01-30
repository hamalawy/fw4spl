# Tutorial 4 : tutoComChannel #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  | [See Rev ebaf47ffa0](Tutorial4_fw4spl_04_Revision_ebaf47ffa0.md) |
| fw4spl\_0.5  | [See Rev ebaf47ffa0](Tutorial4_fw4spl_05_Revision_ebaf47ffa0.md)|
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial4_fw4spl_06_Revision_e2e9e6239200.md) |
| fw4spl\_0.7.0 | See below    |

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

## OSR Scheme ##

![http://wiki.fw4spl.googlecode.com/hg/Tutorial04Resources/serviceDataTuto4.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial04Resources/serviceDataTuto4.png)

After looking for available data in FW4SPL ([Data description in details](DataDescription.md)) and also available service ([Service description in details](ServiceDescription.md)).
We have find the following �lements:

| **Elements to find** | **Element in FW4SPL** |
|:---------------------|:----------------------|
| A mesh data type     | ::fwData::TriangularMesh |
| A service to create the main window | ::gui::frame::DefaultFrame |
| A service to read a mesh file from different formats| ::uiIO::editor::IOSelectorService |
| A service to display a vtk mesh | ::vtkSimpleMesh::RendererService |
| A service to create a menu bar  | ::gui::aspect::DefaultMenuBarSrv |
| A service to create a menu | ::gui::aspect::DefaultMenuSrv |
| A service to create a _Quit_ action | ::gui::action::QuitAction |
| A service to execute an other service (here a service to read a file) | ::gui::action::StarterActionService |

So we can write a OSR more technical:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial04Resources/serviceDataTuto4TechnicalLevel.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial04Resources/serviceDataTuto4TechnicalLevel.png)

Now we have to write the two configuration files (profile.xml and config.xml) to build the FW4SPL application.

## TutoComChannel profile ##

In this tutorial, we use the **vtkSimpleMesh bundle** that provides a **service** allowing to display a surfacic mesh. Furthermore, we add the **ioData bundle** that contains the **service** allowing to read a mesh in the trian format (file .trian). the **service** allowing to read a mesh in the VTK format (fichier .vtk) is located in the  **ioVTK bundle**.

The profile.xml for the application using WxWidgets
```
<profile name="tutoComChannel" version="0.1">

    <!-- Basic bundles list. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    
    <activate id="visu" version="0-1" />
    <activate id="visuVTKWx" version="0-1" />
    
    <activate id="gui" version="0-1" />
    <activate id="guiWx" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="config" value="tutoComChannelConfig" />
        <param id="configFile" value="./Bundles/Tuto04ComChannel_0-1/configurations/config.xml" />
    </activate>

    <!-- Specific bundles list. -->
    <activate id="ioVTK" version="0-1" />
    <activate id="ioData" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="vtkSimpleMesh" version="0-1" />

    <start id="dataReg" />
    <start id="visuVTKWx" />
    <start id="guiWx" />   
</profile>
```

The profile.xml for the application using Qt
```
<profile name="tutoComChannel" version="0.1">

    <!-- Basic bundles list. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    
    <activate id="visu" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />
    
    <activate id="gui" version="0-1" />
    <activate id="guiQt" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="config" value="tutoComChannelConfig" />
        <param id="configFile" value="./Bundles/Tuto04ComChannel_0-1/configurations/config.xml" />
    </activate>

    <!-- Specific bundles list. -->
    <activate id="ioVTK" version="0-1" />
    <activate id="ioData" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="vtkSimpleMesh" version="0-1" />

    <start id="dataReg" />
    <start id="visuVTKQt" />
    <start id="guiQt" />   
</profile>
```

## tutoComChannel configuration ##

As the previous tutorials, we are going to write a **configuration** file that is named **config-04\_ComChannel.xml**. **TutoComChannel** configuration is named **tutoComChannelConfig** (cf [profile](Tutorial4#tutoComChannel_profile.md)).

### Configuration file ###

```
<plugin id="TutoConfigurations">
<extension id="tutoComChannelConfig" implements="::fwData::TriangularMesh">

    <!-- In tutoComChannel, the main data object type is ::fwData::TriangularMesh.-->
    <object type="::fwData::TriangularMesh">

        <service type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no" >
            <gui>
                <frame>
                    <name>tutoComChannel</name>
                    <icon>Bundles/Tuto04ComChannel_0-1/tuto.ico</icon>
                    <minSize width="720" height="600"/>
                </frame>
                <menuBar />
            </gui>
            <registry>
                <menuBar sid="myMenuBar" start="yes" />
                <view sid="myDefaultView" start="yes" />
            </registry>
        </service>

        <service uid="myMenuBar" type="::fwGui::IMenuBarSrv" implementation="::gui::aspect::DefaultMenuBarSrv" autoComChannel="no" >
             <gui>
                <layout>
                     <menu name="File" />
                </layout>
            </gui>
            <registry>
                <menu sid="menu_File" start="yes" />
            </registry>
        </service>
        
        <!-- The application contains three different views. The different views are characterized by their associated service uid.
                 Furthermore, a minimum window height and a width are given to the last two views.-->
         <service uid="myDefaultView" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no" >
            <gui>
                <layout type="::fwGui::CardinalLayoutManager" >
                    <view caption="Rendering 1" align="center" />
                    <view caption="Rendering 2" align="right" minWidth="400" minHeight="100" />
                    <view caption="Rendering 3" align="right" minWidth="400" minHeight="100" />
                </layout>
            </gui>
            <registry>
                <view sid="myRenderingTuto1" start="yes" />
                <view sid="myRenderingTuto2" start="yes" />
                <view sid="myRenderingTuto3" start="yes" />
            </registry>
        </service>
        
        <service uid="menu_File" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no" >
            <gui>
                <layout>
                    <menuItem name="Open file" shortcut="Ctrl+O" />
                    <separator />
                    <menuItem name="Quit" specialAction="QUIT" shortcut="Ctrl+Q" />
                </layout>
            </gui>
            <registry>
                <menuItem sid="action_openFile" start="yes" />
                <menuItem sid="action_quit" start="yes" />
            </registry>
        </service>


        <service uid="action_openFile" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no" >
            <start uid="myReaderPathFile"/>
        </service>

        <service uid="action_quit" type="::fwGui::IActionSrv" implementation="::gui::action::QuitAction" autoComChannel="no" />

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
        </service>

        
        <!-- Visualization service for the second window-->
        <service implementation="::vtkSimpleMesh::RendererService" type="::fwRender::IRender" uid="myRenderingTuto2" autoComChannel="yes" >
            <masterSlaveRelation>slave</masterSlaveRelation>
        </service>


        <!-- Visualization service for the third window-->
        <service implementation="::vtkSimpleMesh::RendererService" type="::fwRender::IRender" uid="myRenderingTuto3" autoComChannel="yes" >
            <masterSlaveRelation>master</masterSlaveRelation>
        </service>

        <start type="::fwGui::IFrameSrv" />

        <stop type="::fwGui::IFrameSrv" />

    </object>

</extension>
</plugin>
```

## Hints and Tips ##

  * Use of layout: a FW4SPL window can be divide in several sub-window by using a layout. It exists three kind of layouts (CardinalLayoutManager, TabLayoutManager, LineLayoutManager).
  * FW4SPL has a service (IOSelectorService) which allow to find all the available reader from the activated bundles in the profile.xml file.
  * FW4SPL has a mesh viewer service.
  * By using a master-slave system, it is possible to create a relation between two mesh visualization windows.
  * By using the _StarterActionService_ service, it is possible to launch a service (it is usually used for the user interaction).

## TutoComChannel application launching ##

To launch the application, we use the following command line, when located in the MyInstallDir directory :

  * For using WxWidgets library : ` ./bin/launcher.exe ./Bundles/Tuto04ComChannel_0-1/profile.xml `
  * For using Qt library : ` ./bin/launcher.exe ./Bundles/Tuto04ComChannel_0-1/profileQt.xml `