# Tutorial 2: tutoDataServiceBasic #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  | [See Rev ebaf47ffa0](Tutorial2_fw4spl_04_Revision_ebaf47ffa0.md) |
| fw4spl\_0.5  | [See Rev ebaf47ffa0](Tutorial2_fw4spl_05_Revision_ebaf47ffa0.md) |
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial2_fw4spl_06_Revision_e2e9e6239200.md) |
| fw4spl\_0.7.0 | See below    |

## Objective ##

The aim of the tutorial **tutoDataServiceBasic** is to show a simple use of **data** and **services** throw an application that display medical image under a three dimensional negatoscop form (cf screenshot). The image path is defined in the configuration file. Therefore, there is no user intervention to choose the file location.
For more details about the notion of **data** and **service** objects, please see the specifique documentation ([Data description in details](DataDescription.md) and [Service description in details](ServiceDescription.md)) or the [Global description of the Framework](Description.md).

For this tutorial, we only focus on the new concept introduce which are
  * Reading an image
  * Showing an image

Here, a screen shot of the application we want

![http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/tutoDataServiceBasic.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/tutoDataServiceBasic.png)

## OSR Scheme ##

![http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/serviceDataTuto2.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/serviceDataTuto2.png)

After looking for available data in FW4SPL ([Data description in details](DataDescription.md)) and also available service ([Service description in details](ServiceDescription.md)).
We have find the following élements:

| **Elements to find** | **Element in FW4SPL** |
|:---------------------|:----------------------|
| An image data type   | ::fwData::Image       |
| A service to create the main window | ::gui::frame::DefaultFrame |
| A service to read a vtk image | ::ioVTK::ImageReaderService |
| A service to display a vtk image | ::vtkSimpleNegato::RendererService |

So we can write a OSR more technical:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/serviceDataTuto2TechnicalLevel.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial02Resources/serviceDataTuto2TechnicalLevel.png)

Now we have to write the two configuration files (profile.xml and config.xml) to build the FW4SPL application.

_Notice_: the requested element(Component/service) to build the application can miss. In this case, it will be necessary to create a new component [([tutoBundle  ](Tutorial6.md))] or a new service in an existing component ([tutoBundle](Tutorial6.md)).

## TutoDataServiceBasic Profile ##

To build this application, we will use an image type as **data** and three **services** (a HMI, a reader and a viewer).
Comparing to the previous tutorial **tuto01Basic**, we have to activate two new bundles the **ioVTK bundle** and the **vtkSimpleNegato bundle**.

  * The **ioVTK bundle** allows to manage the medical image reader using the VTK library. This **bundle** derives of the **io bundle**. Therefore, the **io bundle** must be loaded in order to allow the use of **ioVTK bundle**.

  * The **vtkSimpleNegato bundle** allows to manage the medical image viewer using the VTK library. This **bundle** derives of the **visu bundle**. Therefore, the **visu bundle** must be loaded in order to allow the use of **vtkSimpleNegato bundle**.

To streamline these commented extracted code, we have deliberately deleted the comments already done in the previous tutorial.

The profile.xml for the application using WxWidgets

```
<profile name="tutoDataServiceBasic" version="0.1">
    <!-- List of the basic bundles having to be loaded to launch the application. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    
    <!-- The visu bundle (visualization) is needed to use the vtkSimpleNegato bundle. -->
    <activate id="visu" version="0-1" />
    <activate id="visuVTKWx" version="0-1" />  
    
    <activate id="gui" version="0-1" />
    <activate id="guiWx" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwData::Image" />
        <param id="config" value="tutoDataServiceBasicConfig" />
        <param id="configFile" value="./Bundles/Tuto_0-1/configurations/config.xml" />
    </activate>

    <!-- List of the specific bundles that need to be loaded in this application context. -->
    <!-- The ioVTK bundle (input output VTK) is a set of writing and reading services using the VTK library. -->
    <activate id="ioVTK" version="0-1" />
    <!-- The vtkSimpleNegato bundle contains the visualization service of medical images using the VTK library. -->
    <activate id="vtkSimpleNegato" version="0-1" />
    
    <start id="dataReg" />
       
    <start id="visuVTKWx" />
    <start id="guiWx" />
          
</profile>
```

The profile.xml for the application using Qt
```
<profile name="tutoDataServiceBasic" version="0.1">
    <!-- List of the basic bundles having to be loaded to launch the application. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    
    <!-- The visu bundle (visualization) is needed to use the vtkSimpleNegato bundle. -->
    <activate id="visu" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />
    
    <activate id="gui" version="0-1" />
    <activate id="guiQt" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwData::Image" />
        <param id="config" value="tutoDataServiceBasicConfig" />
        <param id="configFile" value="./Bundles/Tuto_0-1/configurations/config.xml" />
    </activate>

    <!-- List of the specific bundles that need to be loaded in this application context. -->
    <!-- The ioVTK bundle (input output VTK) is a set of writing and reading services using the VTK library. -->
    <activate id="ioVTK" version="0-1" />
    <!-- The vtkSimpleNegato bundle contains the visualization service of medical images using the VTK library. -->
    <activate id="vtkSimpleNegato" version="0-1" />
    
    <start id="dataReg" />
    
    <start id="visuVTKQt" />
    <start id="guiQt" /> 
              
</profile>
```

## TutoDataServiceBasic configuration ##

The application configuration file is named **config.xml**. The **tutoDataServiceBasic** configuration is named **tutoDataServiceBasicConfig** (see the application profile).
Remenber that the **configuration** file allows to instantiate the data needed by the application. In this tutorial, we are going to load and display medical image, thus, we naturally choose ::fwData::Image as  **data** type.
**TutoDataServiceBasic** is an application allowing to read an image in a vtk format and to display it as a tridimensional negatoscope. Therefore, the two following services are requested:
  * ::ioVTK::ImageReaderService: the reading **service** of a vtk format image is named.
  * ::vtkSimpleNegato::RendererService: the visualization **service** of a 3D negatoscope.

This application doesn't need interaction with the user, therefore, a similar HMI service to the one used in the previous tutorial is enough. In the next tutorial, we are going to see how to add a simple interaction with the user.

### Configuration file ###

```
<plugin id="TutoConfigurations">
<extension id="tutoDataServiceBasicConfig" implements="::fwData::Image">

    <!-- In tutoDataServiceBasic, the central data  object is a ::fwData::Image type.-->
    <object type="::fwData::Image">

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
        Furthermore, a guiContainerId attribut is also associated to the service.-->
        <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="myRenderingTuto" autoComChannel="yes" />
        
        <!-- Description service of the HMI.
        Here, the container will be used by the 3D visualization service previously described. 
        The ::gui::frame::DefaultFrame service automaticly positions the different containers in the application main window.
        In our tutorial, this service does not need to receive information about its linked data. Therefore the autoComChannel is set to no.-->
        <service type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no">
            <gui>
                <frame>
                    <name>tutoDataServiceBasic</name>
                    <icon>Bundles/Tuto02DataServiceBasic_0-1/tuto.ico</icon>
                    <minSize width="800" height="600" />
                </frame>
            </gui>
            <registry>
                <view sid="myRenderingTuto" start="yes" />
            </registry>
        </service>
        
        <!-- Definition of the starting and closing order of the different services.
        Here, the three services ::fwGui::IFrameSrv, and myReaderPathFile are started,
        nevertheless, only the myReaderPathFile service is updated after the launching.
        Indeed, as the image location is known (contained in the configuration file),
        the image can be loaded since the application starting. It corresponds to the update flag for myReaderPathFile.
        The HMI service must be started first, then the rendering service, and finaly the reading service.
        The stopping of the services is done in the reverse order compared to the starting one. -->
        
        <start type="::fwGui::IFrameSrv" />
        <start uid="myReaderPathFile"/>

        <!-- launch the reading of the image -->
        <update uid="myReaderPathFile"/>

        <stop uid="myReaderPathFile"/>
        <stop type="::fwGui::IFrameSrv" />

    </object>
</extension>

</plugin>
```

## Hints and Tips ##

  * We see here the benefit of the Framework FW4SPL. It is very easy to do a new application without have to write code. You just have to modify the config.xml and the profile.xml files.

  * This tutorial shows how :
    * to add a view on the main HMI to display a vtk 3D negatoscope.
    * to add a reader file service.

  * When a service must be inform of the change of his associated data, you can set the attribut **autoComChannel** to **yes**. This attribut establishes a communication channel so each time the data (here the image) changes,
the service (here myRenderingTuto) is automatically informed.

## Launching tutoDataServiceBasic application launching ##

To start the application, go to the MyInstallDir directory and type the following command line:

  * For using WxWidgets library : ` ./bin/launcher.exe ./Bundles/Tuto02DataServiceBasic_0-1/profile.xml `
  * For using Qt library : ` ./bin/launcher.exe ./Bundles/Tuto02DataServiceBasic_0-1/profileQt.xml `