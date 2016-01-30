# Tutorial 5 : tutoMesher #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  | [See Rev ebaf47ffa0](Tutorial5_fw4spl_04_Revision_ebaf47ffa0.md) |
| fw4spl\_0.5  | [See Rev ebaf47ffa0](Tutorial5_fw4spl_05_Revision_ebaf47ffa0.md) |
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial5_fw4spl_06_Revision_e2e9e6239200.md) |
| fw4spl\_0.7.0 | See below    |

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

## OSR Scheme ##

In this tutorial, we use a new **data** type, the **Composite** type. This **data** type corresponds to the association of several **data**. These **data** can be of different type. Here, the **Composite data** corresponds to the association of an **Image** and a **TriangularMesh**. Each **data** contained in the **Composite data**, but the **Composite data** itself, have its own **services** :
  * The **Composite data** is associated to the menu, action and HMI **services**. It is the highest data level.
  * The **data Image** is associated to the visualization, reading and writing **services** dedicated to image.
  * The **data TriangularMesh** is associated to the visualization, reading and writing **services** dedicated to mesh.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/serviceDataTuto5.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/serviceDataTuto5.png)

After looking for available data in FW4SPL ([Data description in details](DataDescription.md)) and also available service ([Service description in details](ServiceDescription.md)).
We have find the following elements:

| **Elements to find** | **Element in FW4SPL** |
|:---------------------|:----------------------|
| A composite data type  | ::fwData::Composite   |
| An image data type   | ::fwData::Image       |
| A mesh data type     | ::fwData::TriangularMesh |
| A service to create the main window | ::gui::frame::DefaultFrame |
| A service to read/write a mesh file with different formats | ::uiIO::editor::IOSelectorService |
| A service to display an image | ::vtkSimpleNegato::RendererService |
| A service to display a mesh | ::vtkSimpleMesh::RendererService |
| A service to compute a mesh | ::opVTKMesh::action::VTKMeshCreation |
| A service to create a menu bar | ::gui::aspect::DefaultMenuBarSrv |
| A service to create a menu | ::gui::aspect::DefaultMenuSrv |
| A service to create a _Quit_ action | ::gui::action::QuitAction |
| A service to launch an other service | ::gui::action::StarterActionService |

So we can write a OSR more technical:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/serviceDataTuto4TechnicalLevel.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial05Resources/serviceDataTuto4TechnicalLevel.png)

Now we have to write the two configuration files (profile.xml and config.xml) to build the FW4SPL application.

## tutoMesher profile ##

As in the previous tutorials, we need the same basic **bundles** (**serviceReg**, **dataReg**, **io**, **gui** and **visu**).

Furthermore, we add the specific **bundles** containing the reading, writing and visualization **services** of a medical image (**ioVTK** et **vtkSimpleNegato**) and of a mesh (**ioVTK**, **ioData** and **vtkSimpleMesh**). In order to create a mesh from a mask, we use the **opVTKMesh bundle** also.
Finally, we include the **uiIO bundle** in order to use the **services selector**.

The comments already present in the previous tutorials have been voluntarily deleted in order to clarify the presentation.

The profile.xml for the application using WxWidgets
```
<profile name="Mesher" version="0.1">

    <!-- List of the basic bundles. -->
    <activate id="dataReg" version="0-1"/>
    <activate id="servicesReg" version="0-1"/>
    <activate id="io" version="0-1"/>
    
    <activate id="visu" version="0-1" />
    <activate id="visuVTKWx" version="0-1" />
    
    <activate id="gui" version="0-1"/>
    <activate id="guiWx" version="0-1">
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwData::Composite" />
        <param id="configFile" value="./Bundles/Tuto05Mesher_0-1/configurations/config.xml" />
        <param id="config" value="MesherConfig" />
    </activate>
     
    <!-- List of the specific bundles. -->   
    <activate id="ioData" version="0-1"/>
    <activate id="vtkSimpleMesh" version="0-1"/>
    <activate id="vtkSimpleNegato" version="0-1"/>
    <activate id="ioVTK" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="opVTKMesh" version="0-1" />
    
    <start id="dataReg" />
    <start id="visuVTKWx" />
    <start id="guiWx" /> 
</profile>
```

The profile.xml for the application using Qt
```
<profile name="Mesher" version="0.1">

    <!-- List of the basic bundles. -->
    <activate id="dataReg" version="0-1"/>
    <activate id="servicesReg" version="0-1"/>
    <activate id="io" version="0-1"/>
    
    <activate id="visu" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />
    
    <activate id="gui" version="0-1"/>
    <activate id="guiQt" version="0-1">
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwData::Composite" />
        <param id="configFile" value="./Bundles/Tuto05Mesher_0-1/configurations/config.xml" />
        <param id="config" value="MesherConfig" />
    </activate>
     
    <!-- List of the specific bundles. -->   
    <activate id="ioData" version="0-1"/>
    <activate id="vtkSimpleMesh" version="0-1"/>
    <activate id="vtkSimpleNegato" version="0-1"/>
    <activate id="ioVTK" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="opVTKMesh" version="0-1" />
    
    <start id="dataReg" />
    <start id="visuVTKQt" />
    <start id="guiQt" /> 
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
        <object type="::fwData::Composite">

            <!-- Frame & View -->

            <!--
                Service associated to the Composite data :
                main window creation.
            -->
            <service type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no">
                <gui>
                    <frame>
                        <name>Mesher</name>
                        <icon>Bundles/Tuto05Mesher_0-1/tuto.ico</icon>
                        <minSize width="800" height="600" />
                    </frame>
                    <menuBar />
                </gui>
                <registry>
                    <menuBar sid="myMenuBar" start="yes" />
                    <view sid="myDefaultView" start="yes" />
                </registry>
            </service>

            <service uid="myDefaultView" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no">
                <gui>
                    <layout type="::fwGui::LineLayoutManager">
                        <orientation value="horizontal" />
                        <view caption="Image view" />
                        <view caption="Mesh view" />
                    </layout>
                </gui>
                <registry>
                    <view sid="RenderingImage" start="yes" />
                    <view sid="RenderingMesh" start="yes" />
                </registry>
            </service>


            <!-- Menu Bar, Menus & Actions -->


            <service uid="myMenuBar" type="::fwGui::IMenuBarSrv" implementation="::gui::aspect::DefaultMenuBarSrv" autoComChannel="no">
                <gui>
                    <layout>
                        <menu name="File" />
                        <menu name="Mesher" />
                    </layout>
                </gui>
                <registry>
                    <menu sid="menuFile" start="yes" />
                    <menu sid="menuMesher" start="yes" />
                </registry>
            </service>


            <service uid="menuFile" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no">
                <gui>
                    <layout>
                        <menuItem name="Open image file" shortcut="Ctrl+O" />
                        <menuItem name="Save image" />
                        <separator />
                        <menuItem name="Open mesh file" shortcut="Ctrl+M" />
                        <menuItem name="Save mesh" />
                        <separator />
                        <menuItem name="Quit" specialAction="QUIT" shortcut="Ctrl+Q" />
                    </layout>
                </gui>
                <registry>
                    <menuItem sid="actionOpenImageFile" start="yes" />
                    <menuItem sid="actionSaveImageFile" start="yes" />
                    <menuItem sid="actionOpenMeshFile" start="yes" />
                    <menuItem sid="actionSaveMeshFile" start="yes" />
                    <menuItem sid="actionQuit" start="yes" />
                </registry>
            </service>


            <service uid="menuMesher" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no">
                <gui>
                    <layout>
                        <menuItem name="Compute Mesh (VTK)" />
                    </layout>
                </gui>
                <registry>
                    <menuItem sid="actionCreateVTKMesh" start="yes" />
                </registry>
            </service>

            <service uid="actionQuit" type="::fwGui::IActionSrv" implementation="::gui::action::QuitAction" autoComChannel="no" />



            <!--
                Service associated to the Composite data :
                menu services creation.
                Actions in relationship with images and meshe
            -->
            <service uid="actionOpenImageFile" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
                <start uid="readerPathImageFile" />
            </service>

            <service uid="actionSaveImageFile" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
                <start uid="writerImageFile" />
            </service>

            <service uid="actionOpenMeshFile" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
                <start uid="readerPathMeshFile" />
            </service>

            <service uid="actionSaveMeshFile" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
                <start uid="writerMeshFile" />
            </service>

            <service uid="actionCreateVTKMesh" type="::fwGui::IActionSrv" implementation="::opVTKMesh::action::VTKMeshCreation" autoComChannel="no">
                <image uid="myImage" />
                <mesh uid="myMesh" />
                <percentReduction value="0" />
            </service>


            <!--
                Image object creation Services contained between the tags <object> and </object>
                are associated to the Image data.
            -->
            <item key="myImage">
                <object uid="myImage" type="::fwData::Image">

                    <!--
                    Services associated to the Image data :
                    Visualization, reading and writing service creation.
                    -->
                    <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="RenderingImage" autoComChannel="yes" />

                    <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="readerPathImageFile" autoComChannel="no">
                        <type mode="reader" />
                    </service>

                    <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="writerImageFile" autoComChannel="no">
                        <type mode="writer" />
                    </service>

                </object>
            </item>

            <!--
                TriangularMesh object creation. Services contained between the tags <object> and </object> are
                associated to the TriangularMesh data.
             -->
            <item key="myMesh">
                <object uid="myMesh" type="::fwData::TriangularMesh">

                    <!--
                        Services associated to the TriangularMesh data :
                        Visualization, reading and writing service creation.
                    -->
                    <service implementation="::vtkSimpleMesh::RendererService" type="::fwRender::IRender" uid="RenderingMesh" autoComChannel="yes">
                        <masterSlaveRelation>master</masterSlaveRelation>
                    </service>

                    <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="readerPathMeshFile" autoComChannel="no">
                        <type mode="reader" />
                    </service>

                    <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="writerMeshFile" autoComChannel="no">
                        <type mode="writer" />
                    </service>

                </object>
            </item>

            <!--
                In order to start the different services, it does not need to follow the objects creation order. Note that the services associated to the Image and TriangularMesh data are not started
                between the tags corresponding to these two data definitions but, before the end of the Composite data definition.
            -->
            <start type="::fwGui::IFrameSrv" />

            <stop type="::fwGui::IFrameSrv" />

        </object>
    </extension>
</plugin>    

```

## Hints and Tips ##

  * FW4SPL provides the _::fwData::Composite_ data type to mix easily several kind of data in the same application.

## tutoMesher application launching ##

To launch the application, we use the following command line, when located in the MyInstallDir directory :

  * For using WxWidgets library : ` ./bin/launcher.exe ./Bundles/Tuto05Mesher_0-1/profile.xml `
  * For using Qt library : ` ./bin/launcher.exe ./Bundles/Tuto05Mesher_0-1/profileQt.xml `