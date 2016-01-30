# Tutorial 1: tutoBasic #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  |              |
| fw4spl\_0.5  | [See Rev ebaf47ffa0](Tutorial1_fw4spl_05_Revision_ebaf47ffa0.md) |
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial1_fw4spl_06_Revision_e2e9e6239200.md) |
| fw4spl\_0.7.0 | See below    |

## Objective ##

This tutorial explains how to create a basic application with a simple window without any user's interactions (except to quit the application with the cross). Indeed, this application contains only a title and an icon (cf screenshot).
The better way to pratice is to draw the OSR scheme first ( see [OSR scheme](OSR_Scheme.md) for more information on how to create an OSR scheme) and only after that writting the profile.xml and config.xml.

## OSR Scheme ##

Here, a screen shot of the application we want

![http://wiki.fw4spl.googlecode.com/hg/Tutorial01Resources/sampleframework.jpg](http://wiki.fw4spl.googlecode.com/hg/Tutorial01Resources/sampleframework.jpg)

As you have seen in [Global description of the framework](http://code.google.com/p/fw4spl/wiki/Description), **FW4SPL** is built on the data and service concept. So for this tutorial, we need the **HMI** (Human machine interface) service to draw the main window and a generic data (root class of datas) due to the fact that we don't really need data.
So the OSR scheme is :

![http://wiki.fw4spl.googlecode.com/hg/Tutorial01Resources/serviceDataTuto1.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial01Resources/serviceDataTuto1.png)


As our application is empty, we don't need specific data object.So we are going to use the most simple data object (::fwTools::Object).
The framework has a specific service for define the main window of an application, it is called ::gui::frame::DefaultFrame.
Now, we are ready to build the technical version of the OSR Scheme .

![http://wiki.fw4spl.googlecode.com/hg/Tutorial01Resources/serviceDataTuto1TechnicalLevel.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial01Resources/serviceDataTuto1TechnicalLevel.png)

As we have found all objects and services requested for our application, we just have to write the two configuration files (profile.xml and config.xml) to build the FW4SPL application.

## TutoBasic profile ##

The profile xml file is the first file read by the **launcher** in order to know **bundles** requested by the program (activate node). These bundles are listed with their id in this file.
It also gives the name of the configuration file to use, the starting mode and the version of the application.
An application built whith the FW4SPL framework, can run with the Qt library or WxWidgets library. This choice is done in the profile by using the right bundles. For example,
the gui bundles is called guiQt for the Qt library and guiWx for the WxWidgets library.

An other role of this file s to start some bundles.
This application requires three basic bundles: **serviceReg**, **dataReg** and **gui** and a bundle (guiWx or guiQt) corresponding of the UI library used.
Let's call application profile **tutoBasicProfile.xml**.

The **bundle guiWx** or **bundle guiQt**, corresponding to the HMI (user interface) application is not only activated (like other bundles), but also launched at the start of the application. Parameters of the **bundle guiWx (guiQt)**, are defined with an id and a value. These settings allow you to choose the configuration (related with activated components) of the application.

The profile.xml for the application using Qt

```
<!-- Start tag of the profile. The name of this application is tutoBasic. -->
<profile name="tutoBasicName" version="0.1">

   <!-- List of all bundles that must be loaded to start application (tag <activate>).
     If the launcher doesn't find one of these bundles, the program could not be launched and will display an error message.
     We will present briefly each bundles. -->

   <!-- The bundle dataReg (data registration) describes all types of data existing in FW4SPL. 
    The version attribute specifies the version of the bundle to use,
   here we want to load the bundle dataReg in its version 0.1 -->
    <activate id="dataReg" version="0-1" />
 
   <!-- The bundle servicesReg (registration service) describes all types of low-level services essential to FW4SPL. -->
    <activate id="servicesReg" version="0-1" />
    
   <!-- The bundle gui contains services that can install the user interface (elements of Qt library).
    Parameters startingMode, rootObject, configFile and config and are used by the gui bundle to read and apply the configuration.
    So it isn't necessary to give these values in the console to launch the application. -->
   <activate id="gui" version="0-1" />
    
    <activate id="guiQt" version="0-1" >
       <!-- Starts the application in a window -->
        <param id="startingMode" value="windows" />
       <!-- Used to define the main object (root) on which the application will work. -->
        <param id="rootObject" value="::fwTools::Object" />
       <!-- Id of the configuration to launch the application. -->
        <param id="config" value="tutoBasicConfig" />
       <!-- XML file containing the application configuration. -->
        <param id="configFile" value="./Bundles/Tuto01Basic_0-1/configurations/config.xml" />
    </activate>

   <!-- We start the application when we start gui. -->
    <start id="guiQt" />  
<!-- End tag of the profile -->
</profile>

```

The profile.xml for the application using WxWidgets

```
<profile name="tutoBasicName" version="0.1">

    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="gui" version="0-1" />
    
    <activate id="guiWx" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwTools::Object" />
        <param id="config" value="tutoBasicConfig" />
        <param id="configFile" value="./Bundles/Tuto01Basic_0-1/configurations/config.xml" />
    </activate>

    <start id="guiWx" />  
</profile>
```

## TutoBasic configuration ##

The file tutoBasicConfigFile.xml contains the configuration of our application. Let's call it tutoBasicConfig (notice that the name of the configuration is passed to the gui bundle in [application profile](Tutorial1#TutoBasic_profile.md)).
It allows to instantiate the datas requested by the application, but also create and configure all the services associated with each datas.
However, as our application doesn't use data, we choose a generic data (superclass of datas, ::fwTools::Object).

```
<!-- Definition of root node -->
<plugin id="tutoBasicConfig">

    <extension id="tutoBasicConfig" implements="::fwTools::Object">
        <!-- Definition of the root object request by the object-service philosophy -->
        <object type="::fwTools::Object">

           <!-- Furthermore we define specific parameters to the selected service.
            Here we specify the window title with tag <name>,
            the file name corresponding to the icon with the tag <icon> and
            the nmimal size of the window. -->
            <service type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no" >
                <gui>
                    <frame>
                        <name>tutoBasicApplicationName</name>
                        <icon>Bundles/Tuto01Basic_0-1/tuto.ico</icon>
                        <minSize width="800" height="600"/>
                    </frame>
                </gui>
            </service>

            <!-- Setting the order of starting and stopping services.             
             Here, service ::fwGui::IFrameSrv is started to start the application. It is the first service. -->
            <start type="::fwGui::IFrameSrv" />
            
            <stop type="::fwGui::IFrameSrv" />

        </object>
    </extension>

</plugin>    
```

## Hints and Tips ##

  * This tutorial is the base of all applications with HMI built with the FW4SPL framework. We can notice that we have created an applicarion whitout writting any C++ code line.

  * The choice between using wxWidgets library and Qt library is done **ONLY** in the profile.xml. The config.xml is the same for the two applications.
    * If you want to do an application with wxWidgets, use WxWidgets bundle (here guiWx HMI bundle) in profile file.
    * If you want to do an application with Qt, use Qt bundle (here guiQt HMI bundle) in profile file.

  * The guiWx or guiQt bundle MUST be started at the end of the profile.xml.

## Graphic summary ##

Our program has this structure:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial01Resources/Schemaframework-2.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial01Resources/Schemaframework-2.png)

## Launching tutoBasic application ##

To start our program, go in the directory containing the **Bundles** and **bin** directories, and type the following command:

  * For using WxWidgets library : ` ./bin/launcher.exe ./Bundles/tuto01Basic_0-1/profile.xml `
  * For using Qt library : ` ./bin/launcher.exe ./Bundles/tuto01Basic_0-1/profileQt.xml `
