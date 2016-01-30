# Global description of the Framework #

## Introduction ##

This **framework**, created by **Ircad**, is a modular workspace for rapid application development. It provides some software modules to make easier the creation of software application. It is mainly dedicated to the medical software. Therefore, many functionalities like digital image processing, simulation of medical interactions are allowed.
Afterwards, the framework will be named **FW4SPL** or **F4S** for **FrameWork for Software Production Line**.
This document will introduce the general concepts of **FW4SPL**,  the creation of new component, the creation of application (compilation and launch) and finally it describes some tutorials.

## General concepts ##

**FW4SPL** has been built around three ideas:
  * The concept of **data** and **services**
  * The component approach
  * The communication links

### The concept of **data** and **services** ###

### Data ###

A **data**  represents the information manipulate by the framework. It can be an **Image**, a **Video**, a **TraingularMesh** but also a container, called **composite**, which contains several data (a comopsite can obtain differents types of data or others comopsites). These types could be independent of the input data format. For example, an image could be as various as a Jpeg or a Dicom image.
For each application, it exists a specific data called **rootObject**.

![http://wiki.fw4spl.googlecode.com/hg/UML_Data.png](http://wiki.fw4spl.googlecode.com/hg/UML_Data.png)

For more details see [Data description in details](DataDescription.md).

### Services ###

A **service** represents a functionality which use or modify the data. A **service** is always associated with a **data**. For example, an image data can have a reader service, a writer service and a visualization service.

![http://wiki.fw4spl.googlecode.com/hg/conceptDataService.png](http://wiki.fw4spl.googlecode.com/hg/conceptDataService.png)

On the previous picture, the black arrows represent the link between the **data** and the **service**. We can say that the service2 is associted with data1 for example.
Each service is qualified by a generic type and an specific implementation. For example, the implementation ImageReader and MeshReader have the same type IReader.

For more details see [Service description in details](ServiceDescription.md).

### Component point of view ###

**Services** with similar functionnality are in the same **bundles**. But it is possible to find **Bundle** with only one service. This organization allows more clarity. Each application builds with this framework are done with a group of **bundles**. Futhermore, **bundle** and component are similar for the **FW4SPL**. In the rest of the document, both terms are used.
See below for a list of existing bundles associated to the directory where it is defined.

![http://wiki.fw4spl.googlecode.com/hg/Fw4splOrganisation.png](http://wiki.fw4spl.googlecode.com/hg/Fw4splOrganisation.png)


### Communication channel ###

The communication between the services are done by message throw a communication channel. An channel relies a **service** to an special service called **edition service**. Each data has its own **edition service**. First, an **edition service** receives a message from an **service** associated to its data. Second, it sends this message to all others **services** associated to its data. With this approach, no explicit connexion exists between **services**. The component approach is easier to implement.

Notice that it is possible to disconnect the channel between a **service** and its **edition service**. An example is done on the following illustration. First, service1 send a message to the edition service rely to its data. Second, this message is sent to the service3 and service4, but not to the service2 because it is disconnected.

![http://wiki.fw4spl.googlecode.com/hg/communication.png](http://wiki.fw4spl.googlecode.com/hg/communication.png)

## Creating an application with components ##

**FW4SPL** has a set of components to build simple application (see below, **data** are drawn in black, **service** in red and **bundle** in blue).

  * **dataReg** and **serviceReg** bundles are always required because they contain elementary basis of an application build with **FW4SPL**.
    * **dataReg** bundle (data registration) describes all the data types existing on the **FW4SPL**.
    * **ServiceReg** bundle (registration service) describes all essential basis services of **FW4SPL**

  * **gui**, **io** and **visu** bundles are also basis and general components which allow the use for specific bundles.
    * **gui** bundle (generic user interface) brings **services** together to install human-machine interface (IHM).
    * **io** bundle (input output) brings basis of writer, reader **services** together.
    * **visu** bundle(visualization) bring basis of visualization services together.

  * **uiIO**, **ioVTK**, **ioData**, **vtkSimpleNegato** and **vtkSimpleMesh** bring functionality to the application:
    * **uiIO** provides a HMI for selecting between the available read/write **services**.This bundle requires **io** and **gui**.
    * **ioVTK** brings read/write **services** and meshing **services** using VTK library. This bundle requires **io**.
    * **ioData** brings independent read/write **services**. This bundle requires **io**.
    * **vtkSimpleNegato**  brings a visualization image **service** by  3D negatoscope. **visu** is required.
    * **vtkSimpleMesh** brings a visualization **service** for 3D surfaces meshing. **visu** is required.

It is possible to develop new component or add **service** to existing component. This point will be explain later in this Wiki.

![http://wiki.fw4spl.googlecode.com/hg/composants.png](http://wiki.fw4spl.googlecode.com/hg/composants.png)

The creation of application with **FW4SPL** is very easy. You just have to write two XML files.
  * The first one describes the **profil** of the application. It lists the bundles used by the new application. Like we said before some bundles are always required (**serviceReg**, **dataReg** and **gui**).
  * The second one gives the **configuration**. It creates **data** needed and the associated **services**.

## Creating a new service ##

It requires the use of an existing bundle or the creation of a new one. For our study case, we choose to describe the creation of a new **bundle** from scratch and after we explain the implementation of a new **service**.

### Writing a new component ###

In this part, we will create an new **bundle** called **myBundle**. It will contain a **service** called **myService**.
In **FW4SPL** programming, the creation of a new bundle requires the writing of the following files :
  * **build.options** file describes the **services** list include in our new bundle. It provides information for the compiling system by using optional and mandatory fields.
    * Mandatory fields:
      * **TYPE** field gives the project type (bundle, lib or application).
      * **VERSION** field gives the component version number.
    * Optional fields:
      * **USE** field gives the list of external library (VTK, WxWidgets, boost ...).
      * **LIB** field gives the list of the compilation system project used by the bundle (fwData, fwRuntime, vtkIO ...).
      * **LOGLEVEL** field gives the debug information level.
      * **BUNDLE** gives the name of the new bundle.

  * **MyService.hpp** and **MyService.cpp** files describe the new **service** implementation.
  * **plugin.xml** file gives the description of the **services** provided by the **bundle**.
  * **Plugin.hpp** and **Plugin.cpp** files describe the start and stop methods which are used by the framework to manage our bundle. For example, the start method is called when the component is loaded.

**Important**: All that files must be save in the directory structure describes in the following picture. If your bundle needs extra resources like picture for icon for example, it must be save in the **rc** directory. Another point, a bundle without code doesn't have **src** and **include** directories.

Structure of a new **bundle** (in terms of directory):

![http://wiki.fw4spl.googlecode.com/hg/bundleDir.png](http://wiki.fw4spl.googlecode.com/hg/bundleDir.png)

### New service implementation ###

As we have seen before, to create a service we must fill MyService.hpp and MyService.cpp. For that, we are going to create the methods **configuring**, **starting**, **stopping**, **updating(message)** and **updating**. All these methods are connected with the **configure**, **start**, **stop**, **update(message)**, **update** explain above (see The concept of **data** and **services**). Each method represents a state in the service state machine.

![http://wiki.fw4spl.googlecode.com/hg/SimpleStates.png](http://wiki.fw4spl.googlecode.com/hg/SimpleStates.png)

## Creating a new application ##

In fw4spl programming, an application can be seen like a master component. It  supplies  build.options file to describe all the requested bundles.
Like for a bundle, the build.options file can  provide the followinf information
  * Mandatory fields:
  * **TYPE** field gives the project type (bundle, lib or application).
  * **VERSION** field gives the component version number.
  * Optional fields:
    * **USE** field gives the list of external library (VTK, WxWidgets, boost ...).
    * **LIB** field gives the list of the compilation system project used by the bundle (fwData, fwRuntime, vtkIO ...).
    * **LOGLEVEL** field gives the debug information level.
    * **BUNDLE** gives the name of the new bundle.

> Here an example of build.options file:
> > TYPE  = 'bundle'


> VERSION = '0-1'

> BUNDLES = [
> > 'io\_0-1'              ,
> > 'gui\_0-1'             ,
> > 'dataReg\_0-1'         ,
> > 'servicesReg\_0-1'     ,
> > 'visu\_0-1'            ,
> > 'launcher\_0-1'        ,
> > 'ioVTK\_0-1'           ,
> > 'ioData\_0-1'          ,
> > 'vtkSimpleNegato\_0-1' ,
> > 'vtkSimpleMesh\_0-1'   ,
> > 'uiIO\_0-1'            ,
> > 'opVTKMesh\_0-1'
> > ]

Here, the application loads all the available bundles of the framework but in reality, the list depends on the functionnality of the application.
For the provided tutorials, a "master" bundle has been written and joined to the fw4spl. Due to this fact, this aspect won't be explain on the tutorials.

Directory structure of an application:


> ![http://wiki.fw4spl.googlecode.com/hg/ApplicationDir.png](http://wiki.fw4spl.googlecode.com/hg/ApplicationDir.png)

As you can see on the previous drawing, the same "master" bundle can be use to generate several applications without programmation, they will be configurate by the profile and configuration file.
It can be very useful if you want to produce a light and a full version of your software for example.

## Compiling project ##

All the project files must be located in the directory that we call in our case **Dev**. It will be the **working directory**. It contains the following directories :
  * **Apps** directory  contains one directory by **application** (the name of the directory is the application name). Each has two sub-directories. The first called **bin** contains the build.options file which gives the application type, the application version and the list of required bundles. The second called **rc** contains the profile.xml and the configurtions.xml of the application.
  * **Bundles** directory brings all the bundles together (one bundle by directory. Its name is the bundle name).
  * **Launcher** directory  contains a specifc project used to build a **launcher**. Its aim is to run the application given in parameter by reading the profile.xml and the configuration.xml file.
  * **SrcLib** directory contains a list of library used by **FW4SPL**. Each one is a directory. The name of the directory is given by the name of the library.

To compile easier the new application on different OS (MacOS, Linux and Windows), we have developed a new compilation system [Sconspiracy](http://code.google.com/p/sconspiracy/), based on [Scons](http://www.scons.org/) and write with [Python](http://python.org/). For more information see [documentation Buid](build.md).

Before compiling,, you must create a directory, call it **MyinstallDir**, where the application will be created and configure [Sconspiracy](http://code.google.com/p/sconspiracy/) with this directory path.

So, to compile an application named **myApplication** and located in the **working directory**, you just have to type:

```
racy myApplication
```

**Important**: All the bundle and library required for the application are also compiled if necessary.

It is also possible to compile each bundle separately with the following command:

```
racy myBundle
```

After the compilation, the **MyInstallDir** directory will contain the following sub-directory:

  * **bin** directory contains the launcher **launcher.exe**.
  * **Bundles** contains all the compiled bundles (one directory by bundle). For information, a bundle is defined by the **plugin.xml** file, its **.dll** for windows and its resources. We can also find a sub-directory named like the application where a copy of **profile.xml** and **configuration.xml** of the application are copied from the **Dev\apps** directory.
  * **lib** directory contains all the static(`*`.lib) and dynamic (`*`.dll) used by **FW4SPL** but also the external library.
  * **share** directory contains all the resources of the internal library and **launcher**.

## Running the application ##

To do this, you must use the **launcher** compiled with your application. It receives the profile.xml file of the application as parameter.
First, move to the **MyInstallDir** directory and type:

```
bin/launcher.exe Bundles/myProject_version/myProfile.xml 
```

**Important**: Some environment variables must be defined (see installation of the development Environment).

Summarization diagram:

![http://wiki.fw4spl.googlecode.com/hg/Schemaframework.png](http://wiki.fw4spl.googlecode.com/hg/Schemaframework.png)

**gui** bundle allows to launch the **configuration** (call of the gui bundle **start** method at the end of the **profile.xml** file). To do this, you must configure the **gui** bundle in the  **profile.xml** with the following parameters:
  * **configFile** contains the name and location of the configuration file.
  * **rootObject** contains the type of the main **data**.
  * **startingMode** contains the type of application (bundle, xindows or console).
  * **config** contains the name of the configuration file.

```
    <activate id="gui" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwData::Image" />
        <param id="config" value="config01" />
        <param id="configFile" value="./Bundles/Tuto_0-1/configurations/config-01.xml" />
        </activate>
```

## Tutorials presentation ##

To practice **FW4SPL**, nine tutorials have been written. It allows to understand concept and uses of the framework through practical examples.

  * [tutorial 1](Tutorial1.md) explains how to build a basic application with only a main window. None interaction is allowed except for closing. In this example, the **data** is an image and the only **service** is the default HMI of **FW4SPL**.
  * [tutorial 2](Tutorial2.md) introduces the reading and displaying of an medical image. The localisation of the image is directly written in the **configuration.xml**. In this example, the **data** is an image and three **services** are used. The first is for the default HMI, the second for the reading of the image and the last for visualization.
  * [tutorial 3](Tutorial3.md) adds to the previous tutorial a menu. It allows to choose an  image by Open file entry menu.
  * [tutorial 4](Tutorial4.md) introduces the reading of a triangular mesh by several formats. In this case communication channel will be used to send messages.
  * [tutorial 5](Tutorial5.md) explains how to use two **data** types (image and mesh) in the same application. It will be possible to save the image and the mesh into a file.
  * [tutorial 6](Tutorial6.md) introduces the creation of a new service in a new **bundle**.
  * [tutorial 7](Tutorial7.md) explains how to import numerical data and save it in an xml file with the others application data like image.
  * [tutorial 8](Tutorial8.md) explains how to use the generic scene.
  * [tutorial 9](Tutorial9.md) explains how to use the generic scene associated with a mesher.
  * [tutorial 10](Tutorial10.md) explains how to use transformation matrix in generic scene.
  * [tutorial 11](Tutorial11.md) explains how to use multiple xml file configuration in an application.
  * [tutorial 12](Tutorial12.md) explains how to use a picker in generic scene.

All the code of the tutorials are provided with the **FW4SPL** source code.