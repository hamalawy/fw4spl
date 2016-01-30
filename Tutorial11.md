# Tutorial 11: Tuto11LaunchBasicConfig #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  | N.A          |
| fw4spl\_0.5  | N.A          |
| fw4spl\_0.6  | N.A          |
| fw4spl\_0.7  | See below Tags fw4spl\_0.7.9.6 |

## Introduction ##

A software written with FW4SPL is based on a configuration which describes the services useful application at startup and operation.
But it is also possible to launch a secondary configuration to increase the modularity of the software. This allows us to create complex "widgets" describe by its configuration.
These can be inserted into the man window or become a secondary window. In this tutorial, a secondary configuration will be used to launch a second window from a menu entry.
The same image will be displayed in the two windows. In this way, we also show how to share data object between configurations.

To show how easy it is to extend an application with FW4SPL, we will take the Tuto03DataService configuration and modify it.
le service permettant de lancer la configuration secondaire et d'ajouter une entr�e de menu permettant de lancer cette configuration.

The main window and secondary window after clicking on the menu entry "Launch Config" :

![http://wiki.fw4spl.googlecode.com/hg/Tutorial11Resources/Tuto11LaunchBasicConfig.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial11Resources/Tuto11LaunchBasicConfig.png)

## OSR Scheme ##

### The OSR Scheme of the main window (main configuration) ###

> ![http://wiki.fw4spl.googlecode.com/hg/Tutorial11Resources/ServiceDataTuto11TechnicalLevel.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial11Resources/ServiceDataTuto11TechnicalLevel.png)

### The OSR Scheme of the secondary window (secondary configuration) ###

> ![http://wiki.fw4spl.googlecode.com/hg/Tutorial11Resources/ServiceDataTuto11TechnicalLevel_SecondaryConfig.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial11Resources/ServiceDataTuto11TechnicalLevel_SecondaryConfig.png)

We can notice two things :
  * The data Object here fwData::Image use a new attribut src="ref".
  * The uid services start with the "GENERIC\_UID" pattern.

All these points will be discuss later.

## The profile file ##

This file is similar to those already met and will not be discussed here. Note that the use of a second launch configuration by an action
does not require activation of additional components. In fact, the service used to launch the configuration (ConfigActionSrv) component is present in the gui.

```
<profile name="Tuto11LaunchBasicConfig" version="0.1">

    <!-- List of basic bundles. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    
    <activate id="visu" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />
    
    <activate id="gui" version="0-1" />
    <activate id="guiQt" version="0-1" />
    
    <activate id="Tuto11LaunchBasicConfig" version="0-1" />
    <activate id="appXml" version="0-1">
        <param id="config" value="TutoLaunchBasicConfig" />
    </activate>
    
    <!-- List of specific bundles. -->
    <activate id="ioVTK" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="vtkSimpleNegato" version="0-1" />
    
    <start id="dataReg" />
    <start id="visuVTKQt" />
    <start id="guiQt" />
    <start id="appXml" />

</profile>  
```


## The configuration files ##

This application has two configuration files :
  * The configuration file nammed config.xml is the configuration of the main window.
  * The configuration file nammed `BasicFrameConfig.xml` is the configuration of the secondary window.

### Main configuration ###

```
<plugin id="Tuto11LaunchBasicConfig">

    <requirement id="servicesReg" />

    <xi:include href="BasicFrameConfig.xml" xmlns:xi="http://www.w3.org/2003/XInclude" />
    
    <extension implements="::fwServices::registry::AppConfig">
        <id>TutoLaunchBasicConfig</id>
        <type>standard</type>
        <config>
            <object uid="myImage" type="::fwData::Image">

                <service uid="myFrame" type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no">
                    <gui>
                        <frame>
                            <name>Tuto 11 : Launch basic config</name>
                            <icon>Bundles/Tuto11LaunchBasicConfig_0-1/tuto.ico</icon>
                            <minSize width="800" height="600" />
                        </frame>
                        <menuBar />
                    </gui>
                    <registry>
                        <menuBar sid="mainMenuBar" start="yes" />
                        <view sid="mainView" start="yes" />
                    </registry>
                </service>

                <service uid="mainMenuBar" type="::fwGui::IMenuBarSrv" implementation="::gui::aspect::DefaultMenuBarSrv" autoComChannel="no">
                    <gui>
                        <layout>
                            <menu name="File" />
                        </layout>
                    </gui>
                    <registry>
                        <menu sid="menuFile" start="yes" />
                    </registry>
                </service>

                <service uid="menuFile" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no">
                    <gui>
                        <layout>
                            <menuItem name="Open file" shortcut="Ctrl+O" />
                            <separator />
                            <menuItem name="Launch Config" style="check" />
                            <separator />
                            <menuItem name="Quit" specialAction="QUIT" shortcut="Ctrl+Q" />
                        </layout>
                    </gui>
                    <registry>
                        <menuItem sid="actionOpenFile" start="yes" />
                        <menuItem sid="actionLaunchConfig" start="yes" />
                        <menuItem sid="actionQuit" start="yes" />
                    </registry>
                </service>

                <service uid="actionOpenFile" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
                    <start uid="myReaderPathFile" />
                </service>

                <service uid="actionQuit" type="::fwGui::IActionSrv" implementation="::gui::action::QuitAction" autoComChannel="no" />

                <service uid="myReaderPathFile" implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IDialogEditor" autoComChannel="no" />

                <service uid="actionLaunchConfig" type="::fwGui::IActionSrv" implementation="::gui::action::ConfigActionSrv" autoComChannel="no">
                    <config id="BasicFrameConfig" />
                    <replace val="myImage" pattern="SELECTED_UID_myImage" />
                </service>

                <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="mainView" autoComChannel="yes">
                </service>

                <start uid="myFrame" />

            </object>
        </config>
    </extension>
</plugin>
```

The path to the BasicFrameConfig xml file must be declared in the configuration where it is used by using `<xi:include .../>`.
The path in the href attribut is relative to the TutohBasicConfig configuration location.
Apart from this point, the configuration is similar to that already found in the tutorial Tuto03DataServiceBasic. Therefore we only introduce the new service
to launch a configuration from an action in the menu. This is the service "ConfigActionSrv."

_The service uses to launch configuration ::gui::action::ConfigActionSrv_

It is declared as follows :
```
<service uid="actionLaunchConfig" type="::fwGui::IActionSrv" implementation="::gui::action::ConfigActionSrv" autoComChannel="no">
    <config id="BasicFrameConfig" />
    <replace val="myImage" pattern="SELECTED_UID_myImage" />
</service>
```

The purpose of this service is to launch the configuration that is passed as a parameter in the config tag here "BasicFrameConfig."
This execution is done in four steps :
  * Reading the configuration "BasicFrameConfig".
  * Generating an unique ID that will be used to replace all strings GENERIC\_UID. This ensures that all services contained in the different configurations have unique identifiers (uid).

**To be noted**: The string GENERIC\_UID is predefined in FW4SPL and must be used in all sub-level configuration in the uid service.

  * If the service configuration contains one (or more) replace tag(s), each pattern attribut will be substituted by the value of the val attribut in the configuration "BasicFrameConfig". This allows to share data between configuration.
  * Completing the configuration.

## Secondary configuration ##

This is very simple since it opens a single window.

```
<extension implements="::fwServices::registry::AppConfig">
    <id>BasicFrameConfig</id>
    <type>standard</type>
    <config>

        <object uid="SELECTED_UID_myImage" src="ref" type="::fwData::Image">

            <service uid="GENERIC_UID_BasicViewerFrame" type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no">
                <window onclose="notify" />
                <gui>
                    <frame>
                        <name>Basic frame config</name>
                        <minSize width="900" height="600" />
                    </frame>
                </gui>
                <registry>
                    <view sid="GENERIC_UID_SecondView" start="no" />
                </registry>
            </service>

            <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="GENERIC_UID_SecondView" autoComChannel="yes">
            </service>

            <start uid="GENERIC_UID_BasicViewerFrame" />
            <start uid="GENERIC_UID_SecondView" />

            <update uid="GENERIC_UID_SecondView" />

        </object>

    </config>
</extension>    
```

> Assuming that the system generates the value 123456789 for GENERIC\_UID string, the configuration will become :

```
<extension implements="::fwServices::registry::AppConfig">
    <id>BasicFrameConfig</id>
    <type>standard</type>
    <config>

        <object uid="myImage" src="ref" type="::fwData::Image">

            <service uid="123456789_BasicViewerFrame" type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no">
                <window onclose="notify" />
                <gui>
                    <frame>
                        <name>Basic frame config</name>
                        <minSize width="900" height="600" />
                    </frame>
                </gui>
                <registry>
                    <view sid="123456789_SecondView" start="no" />
                </registry>
            </service>

            <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="123456789_SecondView" autoComChannel="yes">
            </service>

            <start uid="123456789_BasicViewerFrame" />
            <start uid="123456789_SecondView" />

            <update uid="123456789_SecondView" />

        </object>

    </config>
</extension>   
```

We can notice that the uid of the data object becomes myImage. This uid, which already exists in the main configuration, is associated with the attribut src="ref' means
that the secondary configuration and the main configuration will shared the image referenced by myImage.
It is why the same image is displayed twice.

It should be noted that this version of configuration is not visible. It is internal to the system.

## Hints and Tips ##

  * The ability to use multiple configurations allows a great reuse.
  * The number of configuration in an application is unlimited.
  * An object declared in a configuration can be reused in another.
  * No need to check if the service name already exists in a configuration. The useof GENERIC\_UID sring ensures that the service name is unique.
  * Each configuration is referenced through <xi:include .../> tag in the configuration which uses it
  * If secondary configuration launches a secondary window, it will be destroy when the applicatuion will be stopped
