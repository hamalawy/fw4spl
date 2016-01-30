# Tutorial 5 : tutoMesher #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  | See below    |
| fw4spl\_0.5  | [See Rev ebaf47ffa0](Tutorial5_fw4spl_05_Revision_ebaf47ffa0.md) |
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial5_fw4spl_06_Revision_e2e9e6239200.md) |
| fw4spl\_0.7.0 | [tutoMesher](Tutorial5.md) |

## Tutorial aim ##

The tutorial explains how to use simultaneously two **data** types (Image and TriangularMesh) within an application using a **data Composite**. The application allows to create also a surfacic tridimensional from a mask corresponding to an organ segmentation. Remember that a mask is a binary representation of an image. Each mask point is set to a different value according to its membership or not to the **data** structure. Furthermore, the application allows to save both images and meshes.

The application is composed of two windows and contains a File and a Mesher menu.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen0.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen0.png)

The File menu allows to load a medical image represented as a tridimensional negatoscope and/or a tridimensional surfacic mesh. Furthermore, this menu allows to save these two **data** type in the .vtk format for the images and .trian and .vtk for the meshs.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen1.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen1.png)

In the File menu, we choose to open an organ mask in a .vtk format.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen2.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen2.png)

the organ mask is displayed as a tridimentional negatoscope (the used mask represents a humain skeleton segmentation).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen3.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen3.png)

In the Mesher menu, we use the **menuItem** named Compute Mesh (VTK).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen4.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen4.png)

On the right windows, we finaly obtain the mesh corresponding to the left image mask.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen5.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/Tuto5_screen5.png)

As in the previous tutorials, we are going to write a **profile** and a **configuration**.

In this tutorial, we use a new **data** type, the **Composite** type. This **data** type corresponds to the association of several **data**. These **data** can be of different type. Here, the **Composite data** corresponds to the association of an **Image** and a **TriangularMesh**. Each **data** contained in the **Composite data**, but the **Composite data** itself, have its own **services** :
  * The **Composite data** is associated to the menu, action and HMI **services**.
  * The **data Image** is associated to the visualisation, reading and writing **services** dedicated to images.
  * The **data TriangularMesh** is associated to the visualization, reading and writing **services** dedicated to meshes.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/serviceDataTuto5.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/serviceDataTuto5.png)

## tutoMesher profile ##

As in the previous tutorials, we need the same basic **bundles** (**serviceReg**, **dataReg**, **io**, **gui** and **visu**).

Furthermore, we add the specific **bundles** containing the reading, writing and visualization **services** of a medical image (**ioVTK** et **vtkSimpleNegato**) and of a mesh (**ioVTK**, **ioData** and **vtkSimpleMesh**). In order to create a mesh from a mask, we use the **opVTKMesh bundle** also.
Finally, we include the **uiIO bundle** in order to use the **services selector**.

The comments already present in the previous tutorials have been voluntarily deleted in order to clarify the presentation.

```
<profile name="Mesher" version="0.1" >

  <!-- List of basic bundles. -->
  <activate id="dataReg" version="0-1"/>
  <activate id="servicesReg" version="0-1"/>
  <activate id="io" version="0-1"/>
  <activate id="visu" version="0-1"/>
  
  <activate id="gui" version="0-1">
    <param id="startingMode" value="windows" />
    <param id="rootObject" value="::fwData::Composite" />
    <param id="configFile" value="./Bundles/Tuto05Mesher_0-1/configurations/config-05_Mesher.xml" />
    <param id="config" value="MesherConfig" />
  </activate>
   
  <!-- List of specific bundles. -->   
  <activate id="ioData" version="0-1"/>
  <activate id="vtkSimpleMesh" version="0-1"/>
  <activate id="vtkSimpleNegato" version="0-1"/>
  <activate id="ioVTK" version="0-1" />
  <activate id="uiIO" version="0-1" />
  <activate id="opVTKMesh" version="0-1" />
  
  <start id="dataReg" />
  <start id="gui" />
</profile>
```

## tutoMesher configuration ##

As in the previous tutorial, we are going to write a configuration file named **config-05\_Mesher.xml**. The **tutoMesher** configuration  is named **tutoMesherConfig** (cf [application profile](Tutorial5#tutoMesher_profile.md)).

In this configuration profile, the main object (root) is a **Composite data**. It contains two secondary objects : an **Image data** and a **TriangularMesh data**. All the **data** are created between a tag `<object>` and a tag `</object>`. The **services** created between two tags are associated at the object defined by these two tags. When a **Composite data** is used, its creation tags contain the creation tags of the secondary object.
Therefore, the **services** contained between the creation tags of a **Composite data** are not necessarily associated to that **data**. Indeed, if a **service** is contained between the creation tags of a secondary object, it will be associated to the secondary object and not to the **Composite data**.

```
<plugin id="MesherConfiguration">

    <extension id="MesherConfig" implements="::fwData::Composite">
  
    <!--  Composite data creation. -->
    <object id="MesherConfig" type="::fwData::Composite">
  
        <!-- Service associated to the Composite data :
        main window creation. -->
        <service type="::gui::aspect::IAspect" implementation="::gui::aspect::DefaultAspect" autoComChannel="no" >
            <name>Mesher</name>
            <icon>Bundles/Tuto05Mesher_0-1/tuto.ico</icon>
            <minSize width="720" height="600"/>
            <views>
                <view guiContainerId="900"/>
                <view guiContainerId="901" minWidth="500" minHeight="100"/>
            </views>
            <menus>
                <menu uid="menuFile"/>
                <menu uid="menuMesher"/>
            </menus>
        </service>


        <!-- Service associated to the Composite data :
        menu creation. -->
        <service name="File" uid="menuFile" type="::gui::aspect::IMenu" implementation="::gui::aspect::DefaultMenu" autoComChannel="no" >
            <action uid="actionOpenImageFile"/>
            <action uid="actionSaveImageFile"/>
            <separator/>
            <action uid="actionOpenMeshFile"/>
            <action uid="actionSaveMeshFile"/>
            <separator/> 
            <action uid="actionQuit"/>
        </service>         

        <service name="Mesher" uid="menuMesher" type="::gui::aspect::IMenu" implementation="::gui::aspect::DefaultMenu" autoComChannel="no" >
            <action uid="actionCreateVTKMesh"/>
        </service> 
    
        <service uid="actionQuit" specialAction="QUIT"  name="Quit" type="::gui::action::IAction" implementation="::gui::action::QuitAction" shortcut="Ctrl+Q" autoComChannel="no" />

        <!-- Service associated to the Composite data :
        menu services creation.
        Actions in relationship with images and meshes-->
        <service uid="actionOpenImageFile" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" name="Open image file" autoComChannel="no" >
            <start uid="readerPathImageFile"/>
        </service>
    
        <service uid="actionSaveImageFile" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" name="Save image file" autoComChannel="no" >
            <start uid="writerImageFile"/>
        </service> 
    
        <service uid="actionOpenMeshFile" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" name="Open mesh file" autoComChannel="no" >
            <start uid="readerPathMeshFile"/>
        </service>
    
        <service uid="actionSaveMeshFile" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" name="Save mesh file" autoComChannel="no" >
            <start uid="writerMeshFile"/>
        </service>       
    
        <service uid="actionCreateVTKMesh" type="::gui::action::IAction" implementation="::opVTKMesh::action::VTKMeshCreation" name="Compute Mesh (VTK)" autoComChannel="no" >
            <image uid="myImage"/>
            <mesh uid="myMesh"/>
        </service>
                          

        <!-- Image object creation 
        Services contained between the tags <object> and </object>
        are associated to the Image data. -->
        <object id="myImage" uid="myImage" type="::fwData::Image">

            <!-- Services associated to the Image data :
            Visualization, reading and writing service creation. -->
            <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="RenderingImage" autoComChannel="yes" >
                <win guiContainerId="900"/>
            </service>

            <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="readerPathImageFile" autoComChannel="no" >
                <win guiContainerId="900"/>
                <type mode="reader"/>
            </service>

            <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="writerImageFile" autoComChannel="no" >
                <win guiContainerId="900"/>
                <type mode="writer"/>
            </service>

        </object>       

        <!-- TriangularMesh objet creation. 
        Services contained between the tags <object> and </object> 
        are associated to the TriangularMesh data. -->
        <object id="myMesh" uid="myMesh" type="::fwData::TriangularMesh">
             
            <!-- Services associated to the TriangularMesh data :
            Visualization, reading and writing service creation. -->
            <service implementation="::vtkSimpleMesh::RendererService" type="::fwRender::IRender" uid="RenderingMesh" autoComChannel="yes" >
                <masterSlaveRelation>master</masterSlaveRelation>
                <win guiContainerId="901"/>
            </service>
                 
            <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="readerPathMeshFile" autoComChannel="no" >
                <win guiContainerId="901"/>
                <type mode="reader"/>
            </service>
      
            <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="writerMeshFile" autoComChannel="no" >
                <win guiContainerId="901"/>
                <type mode="writer"/>
            </service>
                 
        </object> 
    
    <!-- In order to start the different services,
    it does not need to follow the objects creation order.
    Note that the services associated to the Image and TriangularMesh data
    are not started between the tags corresponding to these two data definitions
    but, before the end of the Composite data definition. -->
    <start type="::gui::aspect::IAspect" />
    <start uid="RenderingImage" />
    <start uid="RenderingMesh" />
    
    <stop uid="RenderingImage" />
    <stop uid="RenderingMesh" />
    <stop type="::gui::aspect::IAspect" />
  </object>
  </extension>
</plugin>    

```

## tutoMesher application launching ##

To launch the application, we use the following command line, when located in the MyInstallDir directory :

```
./bin/launcher.exe ./Bundles/Tuto_0-1/profile-05_Mesher.xml
```