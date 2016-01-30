# Tutorial 1: tutoBasic #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  |              |
| fw4spl\_0.5  |  See below   |
| fw4spl\_0.6  | [See Rev e2e9e6239200](Tutorial1_fw4spl_06_Revision_e2e9e6239200.md) |
| fw4spl\_0.7.0 | [tutoBasic](Tutorial1.md) |

## Introduction ##

This tutorial explains how to create a basic application with a simple window without any user's interactions (except to quit the application with the cross). Indeed, this application contains only a title and an icon (cf screenshot).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial01/sampleframework.jpg](http://wiki.fw4spl.googlecode.com/hg/Tutorial01/sampleframework.jpg)

In **FW4SPL** programming, writing an application means put together components (**Bundles**) by using two XML files to create the **profile** and the **configuration** of the application.
The framework supplies some basics components but it is also possible to create your own (see [tuto Bunble tutorial which explain how to create a bundle](http://code.google.com/p/fw4spl/wiki/Tutorial6) for an example). In this documment, we are going to describe these two files specifically.

**Important**: The parser of **FW4SPL** is case sensitive, so be careful with uppercase and lowercase.

As you have seen in [Global description of the framework](http://code.google.com/p/fw4spl/wiki/Description), **FW4SPL** is built on the data and service concept. So for this tutorial, we are going to use the **HMI** (Human machine interface) service to draw the main window and a generic data (root class of datas, ::fwTools::Object) due to the fact that we don't really need data.Of course with a more complex application, we'll use more specific datas (see the next tutorials).

![http://wiki.fw4spl.googlecode.com/hg/Tutorial01/serviceDataTuto1.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial01/serviceDataTuto1.png)

## tutoBasic profile ##

We'll call application profil **tutoBasicProfile.xml**. The **launcher** needs this file in order to know **bundles** name necessary to launch the program. These bundles are listed with their id in this file. This application requires only three basic bundles: **serviceReg**, **dataReg** and **gui**.
The **bundle gui**, corresponding to the HMI (user interface) application is not only activated (like other bundles), but also launched at the start of the application. Parameters of the **bundle gui**, are defined with an id and a value. These settings allow you to choose the configuration (related with activated components) of the application.

```
<!-- Start tag of the profile. The name of this application is tutoBasic. -->
<profile name="tutoBasic" version="0.1" >

   <!-- List of all bundles that must be loaded to start application (tag <activate>).
     If the launcher doesn't find one of these bundles, the program could not be launched and will display an error message.
     We will present briefly each bundles. -->

   <!-- The bundle dataReg (data registration) describes all types of data existing in FW4SPL. 
    The version attribute specifies the version of the bundle to use,
   here we want to load the bundle dataReg in its version 0.1 -->
   <activate id="dataReg" version="0-1" />

   <!-- The bundle servicesReg (registration service) describes all types of low-level services essential to FW4SPL. -->
   <activate id="servicesReg" version="0-1" />

   <!-- The bundle gui contains services that can install the user interface (elements of wxWidgets library).
    Parameters startingMode, rootObject, configFile and config and are used by the gui bundle to read and apply the configuration.
    So it isn't necessary to give these values in the console to launch the application. -->
   <activate id="gui" version="0-1" >
       <!-- Starts the application in a window -->
       <param id="startingMode" value="windows" />
       <!-- Used to define the main object (root) on which the application will work. -->
       <param id="rootObject" value="::fwTools::Object" />
       <!-- XML file containing the application configuration. -->
       <param id="configFile" value="./Bundles/tutoBasic_0-1/tutoBasicConfigFile.xml" />
       <!-- Id of the configuration to launch the application. -->
       <param id="config" value="tutoBasicConfig" />
   </activate>
   
   <start id="dataReg" />
   <!-- We start the application when we start gui. -->
   <start id="gui" />
 
<!-- End tag of the profile -->
</profile>
```


## tutoBasic configuration ##

The file tutoBasicConfigFile.xml contains the configuration of our application. Let's call it tutoBasicConfig (notice that the name of the configuration is passed to the gui bundle in [application profile](Tutorial1#tutoBasic_profile.md)).
It allows to instantiate the datas requested by the application, but also create and configure all the services associated with each datas.
However, as our application doesn't use data, we choose a generic data (superclass of datas, ::fwTools::Object). With a more complex application, we'll use more specific datas (see the next tutorials).

```
<!-- Definition of root node -->
<plugin  id="tutoBasic">

    <!-- This application does not require specific root data (rootObject previously defined in the profile), 
    since any real data is manipulated. But it must attach a data to the application,
    we choose a generic object ::fwTools::Object. In the next tutorials, we'll use more specific datas
    for more complex applications. -->
    <extension id="tutoBasicConfig" implements="::fwTools::Object">

        <object id="tutoBasicObject" type="::fwTools::Object">

            <!-- Definition of different services used by tutoBasic. 
            When a service is instantiated, an unique id is associated with it (manually or automatically), so you can
           easily differentiate each services and manage their start order.-->
 

            <!-- This service describes the user interface. 
            Service ::gui::aspect::DefaultAspect will automatically arrange each containers in the main program window. 

    
            For this service (like any other service) we must specify its type (::gui::aspect::IAspect),
            its implementation (::gui::aspect::DefaultAspect) and the field autoComChannel
            (to manage communications between the service and its object).
            This value is set to "no". We'll see later, when we can set to "yes".
            (we'll see in detail communications system in the tutorial tutoComChannel).

            Furthermore we define specific parameters to the selected service.
            Here we specify the window title with tag <name>,
            and the file name corresponding to the icon with the tag <icon>. -->
            <service type="::gui::aspect::IAspect" implementation="::gui::aspect::DefaultAspect" autoComChannel="no" >
               <name>tutoBasicApplicationName</name>
               <icon>Bundles/tutoBasic_0-1/tuto.ico</icon>
            </service>

            <!-- Setting the order of starting and stopping services.
             This order is used to start and exit properly the program.
             It's generally optional, but can be necessary if a service must be started before another.
             In the case, we use a only one service, therefore there is no order with other services.
             Here, service ::gui::aspect::IAspect is started when the application starts. -->
            <start type="::gui::aspect::IAspect" />
          
            <stop type="::gui::aspect::IAspect" />
        </object>
    </extension>
</plugin>
```

## Graphic summary ##

Our program has this structure:

![http://wiki.fw4spl.googlecode.com/hg/Tutorial01/Schemaframework-2.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial01/Schemaframework-2.png)

## Launching tutoBasic application ##

To start our program, go in the directory containing the **Bundles** and **bin** directories, and type the following command:

```
./bin/launcher.exe ./Bundles/tuto01Basic_0-1/tutoBasicProfile.xml 
```