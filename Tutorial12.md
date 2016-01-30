= Tutoriel 12 : tutoPicker =

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutoriel** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  |              |
| fw4spl\_0.5  |              |
| fw4spl\_0.6  |              |
| fw4spl\_0.7.0 | Voir ci dessous |

## Introduction ##

This tutorial shows how to get back mouse coordinates from the generic scene to the rest of the FW4SPL application.
To illustrate this, we are going to create an aplication which display the mouse coordinates on a specific service. These coordinnates come from the generic scene of the application.

So our application has:
  * a menu bar with a File menu to open data file.
  * a view to display the mouse coordinate.
  * a view to display a generic scene

At the end, Tuto12Picker has the following aspect:
> ![http://wiki.fw4spl.googlecode.com/hg/Tutorial12Resources/Tuto12Picker.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial12Resources/Tuto12Picker.png)

This tutorial can be tested with the sample file patient1.vtk, patient2.vtk and liverMask.fxz available in Sample.tar.bz2 in GoogleCode download section.

To be noted, without file opens, the coordinates of the mouse don't change.

## OSR Scheme ##

This tutorial is similar to _Tuto08GenericScene_ which introduce the generic scene. So we found the same OSR with two new services (show in green in the OSR):
  * an interactor ::visuVTKAdaptor::PickerInteractor  to get back the position of the mouse in the generic scene.
  * a service to display the point coordinate pointed by the mouse with the following look:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial12Resources/Tuto12_PointInformation.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial12Resources/Tuto12_PointInformation.png)

So, we obtain the following OSR:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial12Resources/serviceDataTuto12TechnicalLevel.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial12Resources/serviceDataTuto12TechnicalLevel.png)

## The profile file ##

The Tuto12Picker profile is the identical with the Tuto08GenericScene , so we don't do any comments (see [Tuto08GenericScene](Tutorial8.md)).

The profile.xml file using Qt:

```
 <profile name="Tuto12Picker" version="0.1" check-single-instance="true">

    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    <activate id="visu" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />

    <activate id="gui" version="0-1" />
    <activate id="guiQt" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="config" value="Tuto12Picker" />
        <param id="configFile" value="./Bundles/Tuto12Picker_0-1/configurations/configTutoPicker.xml" />
    </activate>

    <activate id="ctrlSelection" version="0-1" />
    <activate id="ioVTK" version="0-1" />
    <activate id="ioXML" version="0-1" />
    <activate id="visuVTK" version="0-1" />
    <activate id="visuVTKAdaptor" version="0-1" />
    <activate id="uiIO" version="0-1" />

    <activate id="opVTK" version="0-1" />
    <activate id="uiData" version="0-1" />
    <activate id="uiDataQt" version="0-1" />

    <activate id="uiVisuQt" version="0-1" />
    <activate id="uiImageQt" version="0-1" />

    <start id="visuVTK" />
    <start id="visuVTKAdaptor" />
    <start id="visuVTKQt" /> 
    <start id="guiQt" />

 </profile>
```

The profile.xml file using WxWidgets:
```
  <profile name="Tuto12Picker" version="0.1" check-single-instance="true">

    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    <activate id="visu" version="0-1" />
    <activate id="visuVTKWx" version="0-1" />

    <activate id="gui" version="0-1" />
    <activate id="guiWx" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="config" value="Tuto12Picker" />
        <param id="configFile" value="./Bundles/Tuto12Picker_0-1/configurations/configTutoPicker.xml" />
    </activate>

    <activate id="ctrlSelection" version="0-1" />
    <activate id="ioVTK" version="0-1" />
    <activate id="ioXML" version="0-1" />
    <activate id="visuVTK" version="0-1" />
    <activate id="visuVTKAdaptor" version="0-1" />
    <activate id="uiIO" version="0-1" />

    <activate id="opVTK" version="0-1" />
    <activate id="uiData" version="0-1" />
    <activate id="uiDataWx" version="0-1" />

    <activate id="uiVisuWx" version="0-1" />
    <activate id="uiImageWx" version="0-1" />

    <start id="visuVTK" />
    <start id="visuVTKAdaptor" />
    <start id="visuVTKWx" />
    <start id="guiWx" />

  </profile>
```

## The configuration file ##

In the same way, the config.xml file is closed to the Tuto08GenericScene's config.xml, so we are taking to account only the two new services interactor and visualization service.

# The interactor #

It is declared like this:

```
     <service uid="genericScene" implementation="::fwRenderVTK::VtkRenderService" type="::fwRender::IRender" autoComChannel="yes">
        <scene>
            <picker id="myPicker" vtkclass="fwVtkCellPicker" />

            <renderer id="default" background="0.0" />
            
            ...
            
            <adaptor id="pickerInteractor" class="::visuVTKAdaptor::PickerInteractor" objectId="self" >
                <config renderer="default" picker="myPicker" />
            </adaptor>
            
            ...
            
        </scene>
    </service>
```

If we have a look on the generic scene code, we will see that it is based on the **VTK** library. It implements the picker and render vtk functionnalities.
This "VTK world" must communicate with the others services of the application. This is why the adaptor has been created. They allow to transfer information between the two worlds (The "FW4SPL world" and the "VTK world").


Concerning the ::visuVTKAdaptor::PickerInteractor adaptor, it allows to get the mouse information coming from the generic scene (mouse buttons state, control touch state (CTRL, SHIFT) , mouse coordinates).
These information are provide to the "FW4SPL world" by message (see [channel Communication channel](Description#Communication.md)).


All the subscriber of this message will receive information as the ::uiVisu::PointEditor service do it .

**To be noted**:
  * id = "pickerInteractor" gives an identifier to the adaptor.
  * class="::visuVTKAdaptor::PickerInteractor" identifies the adaptor to use.
  * objectId="self" allows to indicate the data which is attach to the service. Here "self" means  the data which encapsulate the service in this case the composite  _myCompositeVisu_.
  * renderer="default" gives the render to use for this adpator (a generic scene can have several renders).
  * picker="myPicker" identifies the picker used (a generic scene can have several pickers).

The generic scene can have sevral adaptor which can be attached on differents data (for example it can be reconstruction, acquisition,..). All kind of data render in the generic scene.

# The visualization service for mouse coordinates #

It receives the message emitted by the PickerInteractor concerning the mouse move, decodes it and extract the mouse position to display it.
As the intercator is associated to the _myCompositeVisu_, the service is attached to it too (see [channel Communication channel](Description#Communication.md)).

```
        <item key="myCompositeVisu">
            <object uid="myCompositeVisu" type="::fwData::Composite">
...
                <service uid="pointView" implementation="::uiVisu::PointEditor" type="::gui::editor::IEditor" autoComChannel="yes"/>
...
            </object>
        </item>
```

This service allows to display the mouse coordinate in the following way:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial12Resources/Tuto12_PointInformation.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial12Resources/Tuto12_PointInformation.png)


# The complete config.xml file #

```
<plugin id="TutoPickerConfiguration">
<extension id="Tuto12Picker" implements="::fwServices::ServiceObjectConfig">

    <object type="::fwData::Composite">
        <service uid="MyIHM" implementation="::gui::frame::DefaultFrame" type="::fwGui::IFrameSrv" autoComChannel="no">
            <gui>
                <frame>
                    <name>Tuto12Picker</name>
                    <icon>Bundles/Tuto12Picker_0-1/tuto.ico</icon>
                    <minSize width="800" height="600"/>
                </frame>
                <menuBar />
            </gui>
            <registry>
                <menuBar sid="myMenuBar" start="yes" />
                <view sid="mainView" start="yes" />
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

        <service uid="action_openFile" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
            <start uid="myReaderPathFile" />
        </service>
        <service uid="action_quit" type="::fwGui::IActionSrv" implementation="::gui::action::QuitAction" autoComChannel="no" />

        <service uid="mainView" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no" >
            <gui>
                <layout type="::fwGui::LineLayoutManager" >
                    <orientation value="vertical" />
                    <view caption="Picker coordinate." />
                    <view caption="Generic scene" border="11" />
                </layout>
            </gui>
            <registry>
                <view sid="pointView" start="yes" />
                <view sid="multiViewScene" start="yes" />
            </registry>
        </service>

        <service uid="multiViewScene" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no" >
            <gui>
                <layout type="::fwGui::CardinalLayoutManager" >
                    <view align="center" />
                    <view align="bottom" minWidth="400" minHeight="30" resizable="no" />
                </layout>
            </gui>
            <registry>
                <view sid="genericScene" start="yes" />
                <view sid="multiViewScene_bottom" start="yes" />
            </registry>
        </service>


        <service uid="multiViewScene_bottom" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no">
            <gui>
                <layout type="::fwGui::LineLayoutManager" >
                    <orientation value="horizontal" />
                    <view proportion="0" minWidth="30" />
                    <view proportion="0" minWidth="50" />
                    <view proportion="1" />
                    <view proportion="0" minWidth="30" />
                </layout>
            </gui>
            <registry>
                <view sid="sliceListEditor" start="no" />
                <view sid="showScanEditor" start="no" />
                <view sid="sliderIndexEditor" start="no" />
                <view sid="snapshotScene1Editor" start="yes" />
            </registry>
        </service>

        <service uid="snapshotScene1Editor" type="::gui::editor::IEditor" implementation="::uiVisu::SnapshotEditor" autoComChannel="no">
            <snap>
                <scene uid="genericScene" />
            </snap>
        </service>

        <!-- myPatientDB description -->
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

                <service uid="myUpdaterComCannelImg" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no">
                    <target>myUpdaterImg</target>
                </service>

            </object>
        </item>

        <!-- CompositeVisu description -->
        <item key="myCompositeVisu">
            <object uid="myCompositeVisu" type="::fwData::Composite">

                <service uid="pointView" implementation="::uiVisu::PointEditor" type="::gui::editor::IEditor" autoComChannel="yes"/>
                <service uid="myUpdaterAcq" implementation="::ctrlSelection::updater::AcqFromPDBUpdaterSrv" type="::ctrlSelection::IUpdaterSrv" autoComChannel="no">
                    <update compositeKey="myAcquisition" onEvent="NEW_LOADED_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP" />
                    <update compositeKey="myAcquisition" onEvent="CLEAR_PATIENT" fromUID="myPatientDB" actionType="REMOVE" />
                </service>

                <service uid="myUpdaterImg" implementation="::ctrlSelection::updater::ImageFromPDBUpdaterSrv" type="::ctrlSelection::IUpdaterSrv" autoComChannel="no">
                    <update compositeKey="myImage" onEvent="NEW_LOADED_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP" />
                    <update compositeKey="myImage" onEvent="NEW_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP" />
                    <update compositeKey="myImage" onEvent="CLEAR_PATIENT" fromUID="myPatientDB" actionType="REMOVE" />
                </service>

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

                        <adaptor uid="MPRNegatoScene3D" id="MPRNegato3D" class="::visuVTKAdaptor::NegatoMPR" objectId="myImage">
                            <config renderer="default" picker="myPicker" mode="3D" slices="3" sliceIndex="axial" />
                        </adaptor>
                        
                        <adaptor id="snapshot1" class="::visuVTKAdaptor::Snapshot" objectId="self" >
                            <config renderer="default" />
                        </adaptor>

                        <adaptor id="pickerInteractor" class="::visuVTKAdaptor::PickerInteractor" objectId="self" >
                            <config renderer="default" picker="myPicker" />
                        </adaptor>
                    </scene>
                </service>
            </object>
        </item>

        <start uid="MyIHM" />
        <start uid="myUpdaterAcq" />
        <start uid="myUpdaterImg" />
        <start uid="myManager" />
        <start uid="myUpdaterComCannelAcq" />
        <start uid="myUpdaterComCannelImg" />
        <start uid="newOBject2newPatientDB"/>
        <stop uid="newOBject2newPatientDB"/>
        <stop uid="myUpdaterComCannelImg" />
        <stop uid="myUpdaterComCannelAcq" />
        <stop uid="myManager" />
        <stop uid="myUpdaterImg" />
        <stop uid="myUpdaterAcq" />
        <stop uid="MyIHM" />

    </object>

</extension>

</plugin>

```

## Hints and Tips ##

  * To establish a communication between the generic scene ("VTK world") and the rest of the application ("FW4SPL world"), it is necessary to create an adaptor. In this tutorial, the adaptor allows to get the mouse information (button state and mouse coordinate) and some keyboard information concerning CTRL and SHIFT key. But other adaptor can be used to extracted ohter information from data displayed in the generic scene.

  * The number of adaptor in generic scene isn't linmited.

> ## Application launching ##

To launch the application, we use the following command line, when located in the MyInstallDir directory :
  * For using WxWidgets library : ` ./bin/launcher.exe ./Bundles/Tuto12Picker_0-1/profile.xml `
  * For using Qt library : ` ./bin/launcher.exe ./Bundles/Tuto12Picker_0-1/profileQt.xml `


