# Tutorial 08: Generic scene #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  |              |
| fw4spl\_0.5  |              |
| fw4spl\_0.6  | See below    |
| fw4spl\_0.7.0 | [Tuto08GenericScene](Tutorial8.md) |
| experimental |              |

## Introduction ##
> This tutorial gives an example of use of the generic scene bundle provide by the fw4spl framework.

> ![http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08Application.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08Application.png)

In terms of presentation, this application will have
  * a menu bar with a file menu.
  * a main view which is the generic scene.
  * a bottom bar which allows interaction with generic scene (like changing the number of displayed slicer, the active slider).

It will have the following functionnalities:
  * Take a snapshot of the main window which is the generic scene (yellow square in the picture below).
  * Choose the scan to display (Axial, Sagital, Frontal) (blue square in the picture below).
  * Display one or three scans (red square in the picture below).
  * Change the slice displayed with a slider bottom associated with the current displayed scan (blue square in the picture below).
  * Display or not the scan (green square in the picture below).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08ToolBarAnnoted.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08ToolBarAnnoted.png)

This tutorial can be tested with the data sample files patient1.vtk, patient2.vtk available in the file Sample.tar.bz2 in GoogleCode downloads section.

## Application Schema ##

In this part, we are going to modelize the application in terms of object, service (In the first tutorial, we have seen that fw4spl is built on object-service concept).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08SceneGenericSchemaOSR.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08SceneGenericSchemaOSR.png)

As describe in the **Global description of the framework**, we need to write a profile (profile\_TutoGenericScene.xml) and a configuration file (config\_TutoGenericScene.xml)

## Profile file ##

In this file you must activate the bundle that we need for your application.
As usual, we have two mandatories bundles that must be define in ALL application even the simplest

  * **dataReg** bundle (data registration) describes all the data types existing on the **FW4SPL**.
  * **ServiceReg** bundle (registration service) describes all essential basis services of **FW4SPL**

> For this specific application we will also need the following bundles:

  * **io** contains basis and general components which allow the use of specific reader/writer services.
  * **visu** contains basis and general components which allow the use of specific visu services.
  * **ctrlSelection** define the manager/ updater service
  * **ioVTK** allows to read the VTK format file (require io)
  * **ioXML** allows to read the fwXml format (IRCAD file format)
  * **visuVTK** allows to create the generic scene
  * **visuVTKAdaptor** defines all available adaptors for the generic scene
  * **uiIO** allows to manage the editor of reader/writer (requires io bundle) .
  * **uiVisu** allows to manage snapshot functionnalities!.
  * **uiImage** allows to manipulate three type of slice (sagital, axial and frontal).

> The final profile.xml file is:

```
<profile name="Tuto08GenericScene" version="0.1" check-single-instance="true">

     <!-- The two following component MUST be Activate for all application -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />

    <activate id="gui" version="0-1" >
        <param id="startingMode" value="windows" />
        <!-- Definition of the main object of the application -->
        <param id="rootObject" value="::fwData::Composite" />
        <!-- Give the id of the extension to use (it defines in configuration file specify after  -->
        <param id="config" value="Tuto08GenericScene" />
        <!-- Give the path to the configuration file (This path is relative to the Install directory where you MUST run the application).
        <param id="configFile" value="./Bundles/Tuto08GenericScene_0-1/configurations/config_TutoGenericScene.xml" />
    </activate>

     <!-- The two following components MUST be activate for all application -->
    <activate id="io" version="0-1" />
    <activate id="visu" version="0-1" />

    <!-- List of additionnal Bundles needed by the application.  -->
    <activate id="ctrlSelection" version="0-1" />
    <activate id="ioVTK" version="0-1" />
    <activate id="ioXML" version="0-1" />
    <activate id="visuVTK" version="0-1" />
    <activate id="visuVTKAdaptor" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="uiVisu" version="0-1" />
    <activate id="uiImage" version="0-1" />

    <!-- Start bundle -->
    <start id="visuVTK" />
    <start id="visuVTKAdaptor" />
    <start id="gui" />

</profile>
```

## Configuration file ##

The application is created with the name Tuto08GenericScene with the following code.
The main data (the root object) is a composite data because the application manages several objects. It contains a patient object (myPatientDB) and a comopsite object (myCompositeVisu).

```
<plugin id="TutoGenericSceneConfiguration">

    <extension id="Tuto08GenericScene" implements="::fwData::Composite">
        <object id="Tuto08GenericScene" type="::fwData::Composite">
```

Now the service MyIHM is defined. When it starts, it creates the main application window.
It defines the icon of the application and its name. Both elements appears in the title bar of the application.
A subwindow _multiView\_scene1_ is declared and started.

```
            <service uid="MyIHM" implementation="::gui::frame::MainFrame" type="::gui::frame::IFrame" autoComChannel="no">
                <name>Tuto08GenericScene</name>

                <icon>Bundles/Tuto08GenericScene_0-1/tuto.ico</icon>
                <view uid="multiView_scene1" autoStart="yes" />
            </service>
```

The following services creates a menu bar with one menu. This menu has only one entrance Open File.
For more details on menu creation see [Tutorial 3: tutoDataService ](Tutorial3.md)

When the user clicks on _Open File_ , a reader file service _myReaderPathFile_ is started to allow the loading of patient in Vtk of fwXML format.
```
            <service uid="myMenuBar" type="::gui::aspect::IMenuBar" implementation="::gui::aspect::DefaultMenuBar" autoComChannel="no" >
                <menus>
                    <menu uid="menu_File"/>
                </menus>
            </service>
            <service uid="menu_File" implementation="::gui::aspect::DefaultMenu" type="::gui::aspect::IMenu" name="File" autoComChannel="no">
                <action uid="action_openFile" />
            </service>
            <service uid="action_openFile" implementation="::gui::action::StarterActionService" name="Open File" type="::gui::action::IAction" shortcut="Ctrl+O" autoComChannel="no">
                <start uid="myReaderPathFile" />
            </service>
```

The next service creates two views, one for the generic scene and another one for the tool bar.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08Views.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08Views.png)

```
            <service uid="multiView_scene1" type="::gui::view::IView" implementation="::gui::view::MultiView" autoComChannel="no">
                <views>
                    <view uid="genericScene" align="center" autoStart="yes" />
                    <view uid="multiView_scene1_bottom" align="bottom" minHeight="30" resizable="no" autoStart="yes" />
                </views>
            </service>
```

This service displays the following Tool bar.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08ToolBar.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial08Resources/Tuto08ToolBar.png)

To be noted:
  * Only the _sliceIndexEditor_ can be extended (proportion = 1) in the direction define by the orientation field here horizontal.
  * _snapshotScene1Editor_ service is configured with the autoStart option setted to 'yes'. This configuration allows to draw the snaphsot button when the application is launched before any data has been loaded.
  * The services _sliceListEditor_, _showScanEditor_ and _sliderIndexEditor_, which don't have the autoStart option setted to 'yes', will be draw only after a data has been loaded in the application because its will start by the manager._myManager_

```
            <service uid="multiView_scene1_bottom" type="::gui::view::IView" implementation="::gui::view::MultiSizerView" autoComChannel="no">
                <views orientation="horizontal">
                    <view uid="sliceListEditor" proportion="0" minWidth="30" />
                    <view uid="showScanEditor" proportion="0" minWidth="50" />
                    <view uid="sliderIndexEditor" proportion="1" />
                    <view uid="snapshotScene1Editor" proportion="0" minWidth="30" autoStart="yes" />
                </views>
            </service>
```

This service defines the snapshot editor. When you click on the snapshot button this service send a message to the generic scene to have a snapshot of it by the way of the snapshot1 adaptor define in the generic scene.

```
            <service uid="snapshotScene1Editor" type="::gui::editor::IEditor" implementation="::uiVisu::SnapshotEditor" autoComChannel="no">
                <snap>
                    <scene uid="genericScene" />
                </snap>
            </service>
```

A new object Patient _myPatientDB_ is created, it will contain the data with 3 services
  * _myReaderPathFile_ service allows to read a file which contains a patient description.
  * _myUpdaterComCannelAcq_ creates a communication channel between the _myPatientDB_ and _myUpdaterAcq_. This channel will be used to update the generic scene with a new acquisition or a refresh.
  * _myUpdaterComCannelImg_ creates a communication channel between the _myPatientDB_ and _myUpdaterImg_. This channel will be used to update the generic scene with a new image or a refresh (see the definition of myManager below).

**To be noted**: we exclude an old fwXML format from the available choice in the format selector. We keep only the generic one. The _newOBject2newPatientDB_ service is used to translate the new message of generic reader to the old one.
With _newOBject2newPatientDB_ service, we don't have to modify all the bundle to change the old message to the new one.

```
            <!-- myPatientDB description -->
            <object id="myPatientDB" uid="myPatientDB" type="::fwData::PatientDB">

                <! Creation of the reader service -->
                <service uid="myReaderPathFile" implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor"  autoComChannel="no" >
                    <type mode="reader" />
                    <selection mode="exclude" />
                    <addSelection service="::ioXML::FwXMLPatientDBReaderService" />
                </service>
                
                <!-- Allows the use of Generic fwXML reader/writer -->
                 <service implementation="::ctrlSelection::wrapper::PatientDBWrapperSrv" type="::ctrlSelection::IWrapperSrv" uid="newOBject2newPatientDB" autoComChannel="yes" />

                <service uid="myUpdaterComCannelAcq" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no">
                    <target>myUpdaterAcq</target>
                </service>

                <service uid="myUpdaterComCannelImg" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no">
                    <target>myUpdaterImg</target>
                </service>

            </object>
```

This object is a composite used to store the objects that will be displayed in the generic scene.

**To be noted**: a generic scene is always inside a composite object.

```
            <!-- CompositeVisu description -->
            <object id="myCompositeVisu" uid="myCompositeVisu" type="::fwData::Composite">
```

We have seen before that a communication channel is established between _myPatientDB_ and the _myUpdaterAcq_ service. This service manages events on the data object _myAcquisition_ which is included in _myPatientDB_ object. The events take into account are _add patient_ and _remove patient_ events.

```
                <service uid="myUpdaterAcq" implementation="::ctrlSelection::updater::AcqFromPDBUpdaterSrv" type="::ctrlSelection::IUpdaterSrv" autoComChannel="no">
                    <update compositeKey="myAcquisition" onEvent="NEW_LOADED_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP" />
                    <update compositeKey="myAcquisition" onEvent="CLEAR_PATIENT" fromUID="myPatientDB" actionType="REMOVE" />
                </service>
```

We declare the same kind of service but this time on _myImage_. Here the events take into account are _add patient_ and _remove patient_ events.

```
                <service uid="myUpdaterImg" implementation="::ctrlSelection::updater::ImageFromPDBUpdaterSrv" type="::ctrlSelection::IUpdaterSrv" autoComChannel="no">
                    <update compositeKey="myImage" onEvent="NEW_LOADED_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP" />
                    <update compositeKey="myImage" onEvent="NEW_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP" />
                    <update compositeKey="myImage" onEvent="CLEAR_PATIENT" fromUID="myPatientDB" actionType="REMOVE" />
                </service>
```

This service manages the connection between the object, here _myImage_, and the four services attached to _myImage_. Each time an image appears (for example
after loaded of a new patient), the manager starts the following service: medicalImageConverter, sliceListEditor, showScanEditor and sliderIndexEditor and
attached its on the image object. The stop mode means that when the object _myImage_ is removed of the composite (for example when the patient is closed), the
four services associated are stopped by the manager.

The manager allows to start services only if the data attached with these services is available and stop it when the data disappear. This system allows to have a lighter
application in terms of memory.

```
                <service uid="myManager" implementation="::ctrlSelection::manager::SwapperSrv" type="::ctrlSelection::IManagerSrv" autoComChannel="yes" priority="0.6"  >
                    <mode type="stop" />
                    <config>
                        <object id="myImage" type="::fwData::Image">
                            <!-- This service convert a simple fwData::Image to medical Image -->
                            <service uid="medicalImageConverter" implementation="::ctrlSelection::MedicalImageSrv" type="::fwServices::IController"  autoComChannel="no" />
                            <service uid="sliceListEditor" type="::gui::editor::IEditor" implementation="::uiImage::SliceListEditor" autoComChannel="yes">
                                <negatoAdaptor uid="MPRNegatoScene3D" slices="3" />
                            </service>
                            <service uid="showScanEditor" type="::gui::editor::IEditor" implementation="::uiImage::ShowScanEditor" autoComChannel="no">
                                <negatoAdaptor uid="MPRNegatoScene3D" />
                            </service>
                            <service uid="sliderIndexEditor" type="::gui::editor::IEditor" implementation="::uiImage::SliceIndexPositionEditor" autoComChannel="yes" sliceIndex="axial" />
                        </object>
                    </config>
                </service>
```

The genericScene service manages the generic scene. It defines four adaptors, one renderer and one picker.

```
                <service uid="genericScene" implementation="::fwRenderVTK::VtkRenderService" type="::fwRender::IRender" autoComChannel="yes">
                    <scene>
                        <!-- Define the picker for the scene (the picker is the red cross that appears when you click on slice) -->
                        <picker id="myPicker" vtkclass="fwVtkCellPicker" />

                         <!-- Define the renderer for the scene -->
                        <renderer id="default" background="0.0" />

                        <!-- This adaptor display the composite with the default configuration given inside config tag  -->
                        <adaptor id="myRenderAdaptor" class="::visuVTKAdaptor::Render" objectId="myCompositeVisu">
                            <config renderer="default" picker="" />
                        </adaptor>

                        <!-- This adaptor display the acquisition with the default configuration given inside config tag -->
                        <adaptor id="myAcquisitionAdaptor" class="::visuVTKAdaptor::Acquisition" objectId="myAcquisition">
                            <config renderer="default" picker="" />
                        </adaptor>

                        <!-- This adaptor display the data with the default configuration given inside config tag -->
                        <adaptor uid="MPRNegatoScene3D" id="MPRNegato3D" class="::visuVTKAdaptor::NegatoMPR" objectId="myImage">
                            <config renderer="default" picker="myPicker" mode="3D" slices="3" sliceIndex="axial" />
                        </adaptor>

                        <!--This adaptor take the snap of the scene -->
                        <adaptor id="snapshot1" class="::visuVTKAdaptor::Snapshot" objectId="myCompositeVisu" >
                            <config renderer="default" />
                        </adaptor>
                    </scene>
                </service>

```

The declaration of myCompositeVisu object is closed.

```
            </object>
```

The next step is to start the services.

```
            <start uid="MyIHM" />
            <start uid="myMenuBar" />
            <start uid="myUpdaterAcq" />
            <start uid="myUpdaterImg" />
            <start uid="myManager" />
            <start uid="myUpdaterComCannelAcq" />
            <start uid="myUpdaterComCannelImg" />
            <start uid="newOBject2newPatientDB"/>

```

And final the order of closing services is defined.

```
            <stop uid="newOBject2newPatientDB"/>
            <stop uid="myUpdaterComCannelImg" />
            <stop uid="myUpdaterComCannelAcq" />
            <stop uid="myManager" />
            <stop uid="myUpdaterImg" />
            <stop uid="myUpdaterAcq" />
            <stop uid="myMenuBar" />
            <stop uid="MyIHM" />
```

The root object is closed.

```
        </object>
    </extension>
</plugin>
```