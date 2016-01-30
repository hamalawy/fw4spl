# Tutorial 09: Mesher with Generic scene #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  |              |
| fw4spl\_0.5  |              |
| fw4spl\_0.6  | See below    |
| fw4spl\_0.7.0 | [Tuto09MesherWithGenericScene](Tutorial9.md) |
| experimental |              |

## Introduction ##

This tutorial gives an example of use of the mesher avaialable in **FW4SPL** with the generic scene .
It is based on [Tutorial 8: GenericScene ](Tutorial8.md). Some new fonctionnalities are introduced like a mesher and the use of editors on reconstruction.
In this conrtext, only the new points will be described.The mesher of this tutorial are based on VTK mesher on which an reduction factor is applied to reduce the number of triangles in a triangle mesh. The mesher "Mesh 50" has a reduction factor of 50% and "Mesh 80" has a reduction factor of 80%.

This tutorial can be tested with the sample file _liverMask.vtk_ available in _Sample.tar.bz2_ in GoogleCode downloads section.

The application has three states which are presented below. It is a good illustration of the manager functionnalities presented after.

The application after start

![http://wiki.fw4spl.googlecode.com/hg/Tutorial09Resources/Tuto09ApplicationAtStart.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial09Resources/Tuto09ApplicationAtStart.png)

Figure1

The application after the loading of the file _liverMask.vtk_

![http://wiki.fw4spl.googlecode.com/hg/Tutorial09Resources/Tuto09ApplicationAfterDataLoading.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial09Resources/Tuto09ApplicationAfterDataLoading.png)

Figure2

The application after the mesher execution (here Mesh 80)

![http://wiki.fw4spl.googlecode.com/hg/Tutorial09Resources/Tuto09ApplicationAfterMesherExecution.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial09Resources/Tuto09ApplicationAfterMesherExecution.png)

Figure3

We can notice the change in the interface with the creation of the tool bar and the adding of the several editors when the reconstruction is computed.

## The profile file ##

The profile file of the tutorial is very similar to the tutorial 08 profile. So only the new bundles introduced are presented.

  * The **uiReconstruction** bundle allows to use editors to define parameters of reconstructions (like texture, colors, etc).
  * The **uiAcquisition** bundle allows to use an editor for listing the reconstruction computed from the acquisition.
  * The **opVTKMesh** bundle allows to use mesher available in the framework **FW4SPL**.

```

    <activate id="uiReconstruction" version="0-1" />
    <activate id="uiAcquisition" version="0-1" />
    <activate id="opVTKMesh" version="0-1" />

```

## The configuration file ##

In comparison to [Tutorial 8: GenericScene ](Tutorial8.md), a new menu _menu\_Mesher_ is added to launch the two meshers defined ("Mesh 50" and "Mesh 80").
A save fonctionnality is introduced to save the result of the mesher in fwXML format (IRCAD format).

```
    <!-- Creation of the menu bar -->  
    <service uid="myMenuBar" type="::gui::aspect::IMenuBar" implementation="::gui::aspect::DefaultMenuBar" autoComChannel="no" >
        <menus>
            <menu uid="menu_File" />
            <menu uid="menu_Mesher" />
        </menus>
    </service>
    <!-- Creation of the first menu -->
    <service uid="menu_File" implementation="::gui::aspect::DefaultMenu" type="::gui::aspect::IMenu" name="File" autoComChannel="no" >
            <action uid="action_openFile"/>
            <action uid="action_saveFile"/>
    </service>
    <!-- Creation of the second menu -->
    <service uid="menu_Mesher" implementation="::gui::aspect::DefaultMenu" type="::gui::aspect::IMenu" name="Mesher" autoComChannel="no" >
            <action uid="action_createMesh50"/>
            <action uid="action_createMesh80"/>
    </service>
```

The "StarterActionService" service can launch an other service in two ways:
  * The first one uses the _start_ tag. In this case the service is configured and started if it isn't or simply updated.
  * The second uses _start\_if\_exists_ tag. In this case the service must be available (that means already confgured) to be started. But the mesher service _mesher50ServiceUID_ or _mesher80ServiceUID_ are available only if an acquisition is loaded (this functionnality is managed by the manager _myManager_)

```
    <service uid="action_saveFile" implementation="::gui::action::StarterActionService" name="Save File" type="::gui::action::IAction"  shortcut="Ctrl+S" autoComChannel="no" >
            <start uid="myWriterPathFile"/>
    </service>

    <service uid="action_createMesh50" implementation="::gui::action::StarterActionService" name="Create Mesh 50" type="::gui::action::IAction" autoComChannel="no" >
            <start_if_exists uid="mesher50ServiceUID"/>
    </service>

    <service uid="action_createMesh80" implementation="::gui::action::StarterActionService" name="Create Mesh 80" type="::gui::action::IAction" autoComChannel="no" >
            <start_if_exists uid="mesher80ServiceUID"/>
    </service>  
```

The _multiView\_scene1_ service defines the feature and the position of window in the application. The result of the following configuration is shown in this picture:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial09Resources/Tuto09ApplicationAfterMesherExecutionAnnoted.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial09Resources/Tuto09ApplicationAfterMesherExecutionAnnoted.png)

```
    <service uid="multiView_scene1" type="::gui::view::IView" implementation="::gui::view::MultiView" autoComChannel="no">
        <views>
            <view uid="genericScene" align="center" autoStart="yes" />
            <view uid="listOrganEditor" align="right" minWidth="200" minHeight="200" position="0" />
            <view uid="organMaterialEditor" align="right" minWidth="200" minHeight="80" position="1" />
            <view uid="representationEditor" align="right" minWidth="200" minHeight="200" position="2" />
            <view uid="multiView_scene1_bottom" align="bottom" minHeight="30" resizable="no" autoStart="yes" />
        </views>
    </service>
```

A new object Patient _myPatientDB_ is created. Along to the three services declared for the tutorial08, an other communication channel _myUpdaterComCannelReconst_ is added.
It creates a communication channel between the _myPatientDB_ and _myUpdaterComCannelReconst_. This channel will be used to update the generic scene with a new reconstruction or a refresh.
A writer service is also added to save the patient with reconstruction to the fwXML format.

**To be noted**: we exclude an old fwXML format from the available choice in the format selector as we did in the tutorial 08 for the reader.

```
    <object id="myPatientDB" uid="myPatientDB" type="::fwData::PatientDB">
        
        <! Creation of the writer service -->
        <service uid="myWriterPathFile" implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor"  autoComChannel="no" >
           <type mode="writer"/>
           <selection mode="exclude" />
           <addSelection service="::ioXML::FwXMLPatientDBWriterService" />
        </service>

        <!-- A communication channel is created between myPatientDB and myUpdaterAcq -->
        <service uid="myUpdaterComCannelAcq" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no" >
            <target>myUpdaterAcq</target>
        </service>
        <!-- A communication channel is created between myPatientDB and myUpdaterImg -->
        <service uid="myUpdaterComCannelImg" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no" >
            <target>myUpdaterImg</target>
        </service>
        <!-- A communication channel is created between myPatientDB and myUpdaterReconst -->
        <service uid="myUpdaterComCannelReconst" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no" >
            <target>myUpdaterReconst</target>
        </service>
    </object>
```

The manager _myManager_, defined below, manages the services for three types of data :
  * Acquisition
  * Image
  * Reconstruction

It has to launch the services associated to the data available in the application and stop its when the data object disappear.
  * If the acquisition object _myAcquisition_ is present so the services _myUpdaterComCannelReconst2_, _listOrganEditor_, _mesher50ServiceUID_ and _mesher80ServiceUID_ are started.
  * If the image object _myImage_ is present so the services _sliceListEditor_, _showScanEditor_ and _sliderIndexEditor_ are started.
  * If the reconstruction object _myReconstruction_ is present so the services _organMaterialEditor_ and _representationEditor_ are started.

The stop mode means that when one of the object managed is removed of the composite, the services associated are stopped by the manager.

**To be noted**, the use of a manager allows to add display of services user interface when data requested is available as seen in the [figure1](Figure1.md), [figure2](Figure2.md) and [figure3](Figure3.md) in the begin of this tutorial.
The loading of the liverMask.vtk sample data file leads to the creation of an image and an acquisition data in the _myPatientDB_ object so the services associated to this data have been created by th manager _mymanager_.
We can see that by the display of the interface of _listOrganEditor_ service ([see figure 2](Figure2.md)). In the same manner, when the reconstruction is computed, the display of _representationEditor_ and _representationEditor_ appears ([see figure 3](Figure3.md)).

```
        <service uid="myManager" implementation="::ctrlSelection::manager::SwapperSrv" type="::ctrlSelection::IManagerSrv" autoComChannel="yes">
            <mode type="stop" />
            <config>
                <object id="myAcquisition" type="::fwData::Acquisition" >
                     <service uid="myUpdaterComCannelReconst2" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no" >
                         <target>myUpdaterReconst</target>
                     </service>
                     <service uid="listOrganEditor" implementation="::uiAcquisition::OrganListEditor" type="::gui::editor::IEditor" autoComChannel="yes" />
                     <service uid="mesher50ServiceUID" implementation="::opVTKMesh::VTKMesherService" type="::opVTKMesh::IMesher" autoComChannel="no" >
                         <percentReduction value="50"/>
                     </service>
                     <service uid="mesher80ServiceUID" implementation="::opVTKMesh::VTKMesherService" type="::opVTKMesh::IMesher" autoComChannel="no" >
                         <percentReduction value="80"/>
                     </service>
                 </object>
                 <object id="myImage" type="::fwData::Image" >
                     <service uid="sliceListEditor" type="::gui::editor::IEditor" implementation="::uiImage::SliceListEditor" autoComChannel="yes">
                        <negatoAdaptor uid="MPRNegatoScene3D" slices="3"/>
                     </service>
                     <service uid="showScanEditor" type="::gui::editor::IEditor" implementation="::uiImage::ShowScanEditor" autoComChannel="no">
                         <negatoAdaptor uid="MPRNegatoScene3D" />
                     </service>
                     <service uid="sliderIndexEditor" type="::gui::editor::IEditor"  implementation="::uiImage::SliceIndexPositionEditor" autoComChannel="yes" sliceIndex="axial" />
                 </object>
                 <object id="myReconstruction" type="::fwData::Reconstruction" >
                     <service uid="organMaterialEditor" implementation="::uiReconstruction::OrganMaterialEditor" type="::gui::editor::IEditor" autoComChannel="yes" />
                     <service uid="representationEditor" implementation="::uiReconstruction::RepresentationEditor" type="::gui::editor::IEditor" autoComChannel="yes" />
                 </object>
            </config>
        </service>
```

To finish, the configuration file lists all the required services that must be started at the launch.

```
    <start uid="MyIHM"/>
    <start uid="myMenuBar" />
    <start uid="myUpdaterAcq"/>
    <start uid="myUpdaterImg"/>
    <start uid="myUpdaterReconst"/>
    <start uid="myMedicalImageManager"/>
    <start uid="myManager"/>
    <start uid="myUpdaterComCannelAcq"/>
    <start uid="myUpdaterComCannelImg"/>
    <start uid="myUpdaterComCannelReconst"/>
    <start uid="newOBject2newPatientDB"/>

```

The configuration file lists all the required services that must be stopped at the closing of the application.

```
    <stop uid="newOBject2newPatientDB"/>
    <stop uid="myUpdaterComCannelReconst"/>
    <stop uid="myUpdaterComCannelImg"/>
    <stop uid="myUpdaterComCannelAcq"/>
    <stop uid="myManager"/>
    <stop uid="myMedicalImageManager"/>
    <stop uid="myUpdaterReconst"/>
    <stop uid="myUpdaterImg"/>
    <stop uid="myUpdaterAcq"/>
    <stop uid="myMenuBar" />
    <stop uid="MyIHM"/>
```