# Tutorial 2: tutoDataServiceBasic #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  | [See Rev ebaf47ffa0](Tutorial2_fw4spl_04_Revision_ebaf47ffa0.md) |
| fw4spl\_0.5  |  See below   |
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial2_fw4spl_06_Revision_e2e9e6239200.md)|
| fw4spl\_0.7  | [tutoDataServiceBasic](Tutorial2.md) |

## Introduction ##

The aim of the tutorial **tutoDataServiceBasic** is to show a simple use of **data** and **services** throw an application that display medical image under a three dimensional negatoscop form (cf screenshot). The image path is defined in the configuration file. Therefore, there is no user intervention to choose the file location.

For more details about the notion of **data** and **service** objects, please see the specifique documentation ([Data description in details](DataDescription.md) and [Service description in details](ServiceDescription.md)) or the [Global description of the Framework](Description.md).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/tutoDataServiceBasic.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/tutoDataServiceBasic.png)

As for the previous tutorial, we will have to write a **profile** and a **configuration**.

To build this application, we will use an image type as **data** and three **services** (a HMI, a reader and a viewer).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/serviceDataTuto2.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/serviceDataTuto2.png)

## TutoDataServiceBasic Profile ##

As for **tutoBasic**, we need the basic bundles. These are **serviceReg**, **dataReg** and **gui** **bundles**. But we also need reader and viewer services of a medical image. Therefore, we use the **ioVTK bundle** and the **vtkSimpleNegato bundle** that contains this kind of **service**.

  * The **ioVTK bundle** allows to manage the medical image reader using the VTK library. This **bundle** derives of the **io bundle**. Therefore, the **io bundle** must be loaded in order to allow the use of **ioVTK bundle**.

  * The **vtkSimpleNegato bundle** allows to manage the medical image viewer using the VTK library. This **bundle** derives of the **visu bundle**. Therefore, the **visu bundle** must be loaded in order to allow the use of **vtkSimpleNegato bundle**.

To streamline these commented extracted code, we have deliberately deleted the comments already done in the previous tutorial.

```
<profile name="tutoDataServiceBasic" version="0.1" >
    <!-- List of the basic bundles having to be loaded to launch the application. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    
    <!-- The visu bundle (visualization) is needed to use the vtkSimpleNegato bundle. -->
    <activate id="visu" version="0-1" />

    <activate id="gui" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwData::Image" />
        <param id="config" value="tutoDataServiceBasicConfig" />
        <param id="configFile" value="./Bundles/Tuto_0-1/configurations/config-02_DataServiceBasic.xml" />
    </activate>

    <!-- List of the specific bundles that need to be loaded in this application context. -->
    <!-- The ioVTK bundle (input output VTK) is a set of writing and reading services using the VTK library. -->
    <activate id="ioVTK" version="0-1" />
    <!-- The vtkSimpleNegato bundle contains the visualization service of medical images using the VTK library. -->
    <activate id="vtkSimpleNegato" version="0-1" />
    
    <start id="dataReg" />
    <start id="gui" />   
</profile>
```

## TutoDataServiceBasic configuration ##

The application configuration file is named **config-02\_DataServiceBasic.xml**. The **tutoDataServiceBasic** configuration is named **tutoDataServiceBasicConfig** (see the application profile).
Remind us of the **configuration** file allows to instantiate the data needed by the application. In this tutorial, we are going to load and display medical image, thus, we naturally choose ::fwData::Image as  **data** type.
**TutoDataServiceBasic** is an application allowing to read an image in a vtk format and to display it as a tridimensional negatoscope. Therefore, the two following services are requested:
  * ::ioVTK::ImageReaderService: the reading **service** of a vtk format image is named.
  * ::vtkSimpleNegato::RendererService: the visualization **service** of a 3D negatoscope.

Finally, as for the previous tutorial, we are going to use HMI **service** named **gui::aspect::DefaultAspect**.

This application doesn't need interaction with the user, therefore, a similar HMI service to the one used in the previous tutorial is enough. In the next tutorial, we are going to see how to add a simple interaction with the user.

### Configuration file ###

```
<plugin id="TutoConfigurations">
<extension id="tutoDataServiceBasicConfig" implements="::fwData::Image">

<!-- In tutoDataServiceBasic, the central data  object is a ::fwData::Image type.-->
<object id="tutoDataServiceBasicImage" type="::fwData::Image">

    <!-- Reading service. Here, the service unique identifier (uid) is set to the myReaderPathFile value.
    In this tutorial, this service does not need to receive information about its linked data. Therefore the autoComChannel is set to no.
    We are going to see in detail the use of communications in the tutoComChannel tutorial.
    Furthermore, in the filename tag, an attribut (id) allows also to clarify the location of the 3D medical image file. 
    In our case, the image must be stored in the MyInstallDir directory, in a sub-directory named TutoData. --> 
    <service implementation="::ioVTK::ImageReaderService" type="::io::IReader" uid="myReaderPathFile" autoComChannel="no" >
        <filename id="./TutoData/patient1.vtk"/>
    </service>
 
    <!-- Visualization service of a 3D medical image.
    Here, the value of the service unique identifier (uid) is myRenderingTuto.
    In our tutorial, this service needs to receive information about its linked data,
    indeed, it has to receive a message when the image is effectively loaded. Therefore, the autoComChannel value is fixed to yes.
    Furthermore, a guiContainerId attribut is also associated to the service.
    It is the container identifier (arbitrarily set to "900") where the visualization service will be installed. 
    A container, which identifier is "900" must be created. -->
    <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="myRenderingTuto" autoComChannel="yes" > 
        <win guiContainerId="900"/>
    </service>

    <!-- Description service of the HMI.
    Creation of a container, which id's value is 900.
    Here, the container will be used by the 3D visualization service previously described. 
    The ::gui::aspect::DefaultAspect service automaticly positions the different containers in the application main window.
    In our tutorial, this service does not need to receive information about its linked data. Therefore the autoComChannel is set to no.
    Remember we will see in details the use of communications in the tutoComChannel tutorial.-->
    <service type="::gui::aspect::IAspect" implementation="::gui::aspect::DefaultAspect" autoComChannel="no" >
        <name>tutoDataServiceBasic</name>
        <icon>Bundles/Tuto_0-1/tuto.ico</icon>
        <views>
            <view guiContainerId="900" />
        </views>
    </service>

    <!-- Définition of the starting and closing order of the different services.
    Here, the three services ::gui::aspect::IAspect, myRenderingTuto, and myReaderPathFile are started,
    nevertheless, only the myReaderPathFile service is updated after the launching.
    Indeed, as the image location is known (contained in the configuration file),
    the image can be loaded since the application starting. It corresponds to the update flag for myReaderPathFile.
    The HMI service must be started first, then the rendering service, and finaly the reading service.
    The stopping of the services is done in the reverse order compared to the starting one. -->
    
    <start type="::gui::aspect::IAspect" />
    <start uid="myRenderingTuto"/>
    <start uid="myReaderPathFile"/>

    <update uid="myReaderPathFile"/>

    <stop uid="myReaderPathFile"/>
    <stop uid="myRenderingTuto"/>
    <stop type="::gui::aspect::IAspect" />

</object>
</extension>
</plugin>
```

## tutoDataServiceBasic application launching ##

To start the application, go to the MyInstallDir directory and type the following command line:

```
./bin/launcher.exe ./Bundles/Tuto02DataServiceBasic_0-1/profile-02_DataServiceBasic.xml
```