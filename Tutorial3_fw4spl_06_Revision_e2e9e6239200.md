# Tutorial 3: tutoDataService #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  | [See Rev ebaf47ffa0](Tutorial3_fw4spl_04_Revision_ebaf47ffa0.md) |
| fw4spl\_0.5  | [See Rev ebaf47ffa0](Tutorial3_fw4spl_05_Revision_ebaf47ffa0.md) |
| fw4spl\_0.6  | See below    |
| fw4spl\_0.7.0 | [tutoDataService](Tutorial3.md) |

## Introduction ##

This tutorial explains how to add an user's interaction to the application done in the previous tutorial. This application will display the medical image (vtk file) selected by the user with a file dialog (in the previous tutorial image location was specified directly in the **configuration file**).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService1.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService1.png)


In this tutorial, we also explain how to create a menu with menuItem in the menubar and the specific actions associated with these menuItems:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/menuBar.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/menuBar.png)

The program has a menuBar with a menu "File", in this menu we find two menuItems "OpenFile" and "Quit" associated with a shortcut.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService2.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService2.png)

The menuItem "OpenFile" opens a file dialog to choose the image file (.vtk).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService3.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService3.png)

When the image path is specified and validated, the medical image is loaded and displayed, like in the previous tutorial.

Again, we have to write a **profile** and **configuration**.

In this tutorial, we use a data image, and several kind of services: HMI, image viewer, image reader, menus and actions.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/serviceDataTuto3.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/serviceDataTuto3.png)

## tutoDataService profil ##

As for **tutoDataServiceBasic**, we need the basic **bundles** (**serviceReg**, **dataReg**, **gui**, **io** and **visu**).
We also add the specific bundles containing services to read and display a medical image (**ioVTK** et **vtkSimpleNegato**).


To streamline these commented extracted code, we have deliberately deleted the comments already done in the previous tutorial.

Only configuration id's different from the previous tutorial.

```
<profile name="tutoDataService" version="0.1" >

    <!-- List of basic bundles. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    <activate id="visu" version="0-1" />

    <activate id="gui" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwData::Image" />
        <param id="config" value="tutoDataServiceConfig" />
        <param id="configFile" value="./Bundles/Tuto03DataService_0-1/configurations/config-03_DataService.xml" />
    </activate>

    <!-- List of specific bundles. -->
    <activate id="ioVTK" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="vtkSimpleNegato" version="0-1" />

    <start id="dataReg" />
    <start id="gui" />   
</profile>
```

## tutoDataService configuration ##

As for the previous tutorial, we write a configuration file called **config-03\_DataService.xml**. The **tutoDataService** configuration is called **tutoDataServiceConfig** (see [application profil](Tutorial3#tutoDataService_profil.md)).

menubar, menu and menuItem is managed by three kind of services:
  * ::gui::aspect::IAspect services for the **menuBar**.
  * ::gui::aspect::IMenu services for the **menus**.
  * ::gui::aspect::IAction services for the **menuItems**.


The ::gui::aspect::DefaultAspect service (::gui::aspect::IAspect type) creates the menuBar. In the configuration of this service, we specify the identifiers of menus services (type ::gui::aspect::IMenu) placed in the menubar.

The ::gui::aspect::DefaultMenu service (::gui::aspect::IMenu type) creates the menu. In the configuration of this service, we specify the identifiers of menuItems services (type ::gui::action::IAction) placed in the menu.

The two services ::gui::action::StarterActionService and ::gui::action::QuitAction (::gui::aspect::IAction type) create respectively menuItems "OpenFile" and "Quit".
A click on a menuItem executes the updating method in the associated service with the menuItem.

### Configuration file ###

```
<plugin id="TutoConfigurations">
<extension id="tutoDataServiceConfig" implements="::fwData::Image">

<!-- The root data object in tutoDataService is a ::fwData::Image. -->
<object id="tutoDataServiceImage" type="::fwData::Image">


    <!-- Graphical user interface manager. -->

    <service type="::gui::frame::IFrame" implementation="::gui::frame::MainFrame" autoComChannel="no">
        <name>tutoDataService</name>
        <icon>Bundles/Tuto03DataService_0-1/tuto.ico</icon>
        <view uid="myRenderingTuto" autoStart="yes"  />
    </service>

    <!-- In the tag <menus> we specify the identifiers of different services for the different menus.
    In this tutorial, only one service (::gui::aspect::IMenu type) is created,
    since the application's only one menu. The unique identifier (uid) of this service is "menu_file",
    it's an FW4SPL identifier. -->
    <service uid="myMenuBar" type="::gui::aspect::IMenuBar" implementation="::gui::aspect::DefaultMenuBar" autoComChannel="no" >
        <menus>
            <menu uid="menu_File" />
            </menus>
    </service>
    
    <!-- The ::gui::aspect::DefaultMenu service creates the menu "File". This menu contains two menuItems (with a separator).
    The actions associated with these two menuItem are identified with the uid "action_openFile" and "action_quit". -->
    
    <service name="File" uid="menu_File" type="::gui::aspect::IMenu" implementation="::gui::aspect::DefaultMenu" autoComChannel="no" >
        <action uid="action_openFile"/>
        <separator/>
        <action uid="action_quit"/>
    </service>

    <!-- The first service (::gui::action::StarterActionService) is a generic action. In the configuration of this service,
    we specify the id of the services to be started and updated (start and update methods) when user click on the menuItem "OpenFile".
    In this tutorial, ::gui::action::StarterActionService service starts and updates the service with the uid "myReaderPathFile". -->
    <service uid="action_openFile" name="OpenFile" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" shortcut="Ctrl+O" autoComChannel="no" >
        <start uid="myReaderPathFile"/>
    </service>
    <!-- The second service (::gui::action::QuitAction) is a generic action that will close the application
    when the user click on the menuItem "Quit".
    It's possible to associate with this kind of service (::gui::action::IAction) specific attributes to configure their style, shortcut... 
    (Note:
           In this tutorial, there is an attribute specialAction with the value "QUIT". On MS Windows, there's no impact.
           But on Mac OS, this value installs the menuItem in the system menu bar,
           and on Linux this value installs the default 'Quit' system icon in the menuItem.)    
           The attribute "shortcut" is set with the value "Ctrl+Q" to close the application with this specific shortcut. -->
    <service uid="action_quit" specialAction="QUIT"  name="Quit" type="::gui::action::IAction" implementation="::gui::action::QuitAction" shortcut="Ctrl+Q" autoComChannel="no" />


    <!-- The service ::gui::editor::IOSelectorService is started (start and update methods) by the service ::gui::action::StarterActionService. 
    Indeed, the uid of the service ::gui::editor::IOSelectorService is the same as the uid 
    defined in the service configuration ::gui::action::StarterActionService ( "myReaderPathFile").
    This service lists all services available to read a data image and
    it displays a dialog box to select the appropriate service.
    (Note: 
          if there is only one reading service, it's directly selected without dialog box.)
    When the service was chosen, it is started by the ::uiIO::editor::IOSelectorService service. -->
    <service uid="myReaderPathFile" implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor"  autoComChannel="no" />

    <!-- This service (::vtkSimpleNegato::RendererService) is a 3D visualization service of medical images. -->
    <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="myRenderingTuto" autoComChannel="yes" />

    <start type="::gui::frame::IFrame" />
    <start type="::gui::aspect::IMenuBar" />
    
    <stop type="::gui::aspect::IMenuBar" />
    <stop type="::gui::frame::IFrame" />

</object>

</extension>
</plugin>
```

## Launching tutoDataService application ##

To start our program, go in the directory containing the Bundles and bin directory (MyInstallDir directory)and then type the following command:

```
./bin/launcher.exe ./Bundles/Tuto03DataService_0-1/profile-03_DataService.xml
```