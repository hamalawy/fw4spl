# Tutorial 3: tutoDataService #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  | [See Rev ebaf47ffa0](Tutorial3_fw4spl_04_Revision_ebaf47ffa0.md) |
| fw4spl\_0.5  | [See Rev ebaf47ffa0](Tutorial3_fw4spl_05_Revision_ebaf47ffa0.md) |
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial3_fw4spl_06_Revision_e2e9e6239200.md) |
| fw4spl\_0.7.0 | See below    |

## Introduction ##

The tutorial explains how to add an user's interaction to the application done in the previous tutorial. This application will display the medical image (vtk file) selected by the user with a file dialog (in the previous tutorial image location was specified directly in the **configuration file**).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService1.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService1.png)


In this tutorial, we also explain how to create a menu with menuItem in the menubar and the specific actions associated with these menuItems:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/menuBar.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/menuBar.png)

The program has a menuBar with a menu "File", in this menu we find two menuItems "OpenFile" and "Quit" associated with a shortcut.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService2.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService2.png)

The menuItem "OpenFile" opens a file dialog to choose the image file (.vtk).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService3.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/tutoDataService3.png)

When the image path is specified and validated, the medical image is loaded and displayed, like in the previous tutorial.

## OSR Scheme ##

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/serviceDataTuto3.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/serviceDataTuto3.png)

After looking for available data in FW4SPL ([Data description in details](DataDescription.md)) and also available service ([Service description in details](ServiceDescription.md)).
We have find the following �lements:

| **Elements to find** | **Element in FW4SPL** |
|:---------------------|:----------------------|
| An image data type   | ::fwData::Image       |
| A service to create the main window | ::gui::frame::DefaultFrame |
| A service to read a vtk image | ::ioVTK::ImageReaderService |
| A service to display a vtk image | ::vtkSimpleNegato::RendererService |
| A service to create a menu bar  | ::gui::aspect::DefaultMenuBarSrv |
| A service to create a menu | ::gui::aspect::DefaultMenuSrv |
| A service to create a _Quit_ action | ::gui::action::QuitAction |
| A service to send an other service (here the service to read a file) | ::gui::action::StarterActionService |

So we can write a OSR more technical:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/serviceDataTuto3TechnicalLevel.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial03Resources/serviceDataTuto3TechnicalLevel.png)

Now we have to write the two configuration files (profile.xml and config.xml) to build the FW4SPL application.

_Notice_: the requested element(Component/service) to build the application can miss. In this case, it will be necessary to create a new component [([tutoBundle  ](Tutorial6.md))] or a new service in an existing component ([tutoBundle](Tutorial6.md)).

## tutoDataService profil ##

In this tutorial, we use a data image, and several kind of services: HMI, image viewer, image reader, menus and actions.
As for **tutoDataServiceBasic**, we need the basic **bundles** (**serviceReg**, **dataReg**, **gui**, **io** and **visu**).
We also add the specific bundles containing services to read and display a medical image (**ioVTK** et **vtkSimpleNegato**).


To streamline these commented extracted code, we have deliberately deleted the comments already done in the previous tutorial.

Only configuration id's different from the previous tutorial.

The profile.xml for the application using WxWidgets

```
<profile name="tutoDataService" version="0.1">

    <!-- List of basic bundles. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    
    <activate id="visu" version="0-1" />
    <activate id="visuVTKWx" version="0-1" />

    <activate id="gui" version="0-1" />
    <activate id="guiWx" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="config" value="tutoDataServiceConfig" />
        <param id="configFile" value="./Bundles/Tuto03DataService_0-1/configurations/config.xml" />
    </activate>

    <!-- List of specific bundles. -->
    <activate id="ioVTK" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="vtkSimpleNegato" version="0-1" />

    <start id="dataReg" />
    <start id="visuVTKWx" />
    <start id="guiWx" />   
</profile>
```

The profile.xml for the application using Qt

```
<profile name="tutoDataService" version="0.1">

    <!-- List of basic bundles. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    
    <activate id="visu" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />

    <activate id="gui" version="0-1" />
    <activate id="guiQt" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="config" value="tutoDataServiceConfig" />
        <param id="configFile" value="./Bundles/Tuto03DataService_0-1/configurations/config.xml" />
    </activate>

    <!-- List of specific bundles. -->
    <activate id="ioVTK" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="vtkSimpleNegato" version="0-1" />

    <start id="dataReg" />
    <start id="visuVTKQt" />
    <start id="guiQt" />   
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

    <service type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no">
      <gui>
            <frame>
                <name>tutoDataService</name>
                <icon>Bundles/Tuto03DataService_0-1/tuto.ico</icon>
                <minSize width="800" height="600" />
            </frame>
            <menuBar />
        </gui>
        <registry>
            <menuBar sid="myMenuBar" start="yes" />
            <view sid="myRenderingTuto" start="yes" />
        </registry>
    </service>

    <!-- In the tag <menus> we specify the identifiers of different services for the different menus.
    In this tutorial, only one service (::gui::aspect::IMenu type) is created,
    since the application's only one menu. The unique identifier (uid) of this service is "menu_file",
    it's an FW4SPL identifier. -->
    <service uid="myMenuBar" type="::fwGui::IMenuBarSrv" implementation="::gui::aspect::DefaultMenuBarSrv" autoComChannel="no" >
        <gui>
            <layout>
                <menu name="File" />
            </layout>
        </gui>
        <registry>
            <menu sid="myMenu" start="yes" />
        </registry>
    </service>
    
    <!-- The ::gui::aspect::DefaultMenu service creates the menu "File". This menu contains two menuItems (with a separator).
    The actions associated with these two menuItem are identified with the uid "action_openFile" and "action_quit". -->
    
    <service uid="myMenu" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no" >
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

    <start type="::fwGui::IFrameSrv" />

    <stop type="::fwGui::IFrameSrv" />

</object>

</extension>
</plugin>
```

## Hints and Tips ##

  * We have seen how to create a menu bar and its menus.
  * We have seen how to launch a service from a menu item.

## Launching tutoDataService application ##

To start our program, go in the directory containing the Bundles and bin directory (MyInstallDir directory)and then type the following command:

  * For using WxWidgets library : ` ./bin/launcher.exe ./Bundles/Tuto03DataService_0-1/profile.xml `
  * For using Qt library : ` ./bin/launcher.exe ./Bundles/Tuto03DataService_0-1/profileQt.xml `