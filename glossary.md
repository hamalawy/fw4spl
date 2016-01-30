# Glossary #


### Application ###
> An application is created from a set of **[componants](glossary#Componant.md)**, a **[profile](glossary#Profile.md)** file and a **[configuration](glossary#Configuration.md)** file. It contains an unique **[data](glossary#Data.md)** called **[rootObject](glossary#RootObject.md)**.


### Library ###
> A **library** is ...


### build.options ###
> A **[bundle](glossary#Bundle.md)** always contains a **build.options** file that descibres the **[bundle](glossary#Bundle.md)** list and the necessary lib to make the compilation possible.


### Bundle ###
> A **bundle** contains a set of **[services](glossary#Service.md)** (it can contain only one service also) having similar role. For example, the reading and writing **[services](glossary#Service.md)** using the VTK library are grouped together within the ioVTK **bundle**. A component is a composition unit, having specific and contractual interfaces, that has not explicit contextual dependances. In our case, it is a directory having a **plugin.xml** file defining the componant contract. The directory can contain a dynamic library and/or ressource files (xml, bmp, png,...). The development term used is **bundle**. This theoric concept is inspired by the OSGi componant system. The word **plugin** is used as synonymous of **bundle**

> In order to build basic applications, **[FW4SPL](glossary#FW4SPL.md)** provides a set of **bundles**. The **dataReg** and **serviceReg** bundles are two componant that must be use systematically, because they correspond to elementary brick of an application :

  * The **dataReg bundle** (data registration) describes the whole **[data](glossary#Data.md)** type existing in **[FW4SPL](glossary#FW4SPL.md)**.
  * The **serviceReg bundle** (service registration) describes the whole **[services](glossary#Service.md)** type of low level essential to **[FW4SPL](glossary#FW4SPL.md)**.

> The **gui**, **io** and **visu** bundle are basic and general componants allowing to use specialised **[bundles](glossary#Bundle.md)** :

  * The **gui bundle** (generic user interface) contains the **[services](glossary#Service.md)** allowing to install the componant of Interface Human-Machine (IHM) type.
  * The **io bundle** (input output) contains the base of the writing and reading services.
  * The **visu bundle** (visualisation) contains the base of the visualisation **[services](glossary#Service.md)**.

### Communication channel ###
> The **communication channels** are lines used by the **[edition service](glossary#Edition_serviceService.md)** in order to send messages to the other services associated to the **[data](glossary#Data.md)**.


### Componant ###
> See **[bundle](glossary#Bundle.md)**.

### Composite ###
> A **composite**, or **composite data** is a container of several **[data](glossary#Data.md). It can contain different type data or composites.**


### Configuration ###

> The **configuration.xml** file allows to precise the hierarchic relationship between the differents **[data](glossary#Data.md)** used and associated **[services](glossary#Service.md)**. The file localization is precised in the **[profile.xml](glossary#Profil.md)** file.


### Data ###
> A **data** can be a single data or a data container called **[composite](glossary#Composite.md)**. For instance, a **data** can be an image or a triangularMesh data type. A composite constains several data or an other composite. One data can represent information which has several origin formats. For example, an image data can represent an 2D Image in jpg or png format or an 3D Image in the DICOM or inr format. For each application, it exists a special **data** called **[rootObject](glossary#rootObject.md)**.


### Framework ###
> The framework, created by Ircad, is a modular workspace for rapid application development. It provides some software modules to make easier the creation of software application. It is mainly dedicated to the medical software. Therefore, many functionalities like digital image processing, simulation of medical interactions are allowed.


### FW4SPL ###
> The name of **[framework](glossary#Framework.md)** developped by **[Ircad](glossary#Ircad.md)** is **FW4SPL** or **[F4S](glossary#FW4SPL.md)**. It means **FrameWork for Software Production Line**.


### F4S ###
> See **[FW4SPL](glossary#FW4SPL.md)**.


### Ircad ###
  * rcad**is an acronym for**Institut de Recherche contre les Cancers de l'Appareil Digestif (http://www.ircad.fr/homepage.php link to the Ircad internet site). It is an original structure dedicates to the valorisation of research against the cancer. Ircad has been created on 1994 by the **Professor Jacques Marescaux**


### Launcher ###
> To launch the program, you must use a separate application called **launcher**. It loads the profile of the application **[profile.xml](glossary#Profil.md)** with the following way:
```
bin/launcher.exe Bundles/myProject_version/myProfile.xml
```


### State machine ###


### Configure method ###

> The **configure** method allows to configure the parameters of the **[service](glossary#Service.md)**. For example, in a reading image service, the localisation of the file to open must be done in this method. This method analyses the **[service](glossary#Service.md)** **[configuration](glossary#Configuration.md)** written in the **plugin.xml** file.


### Configuring method ###
> This method is written in each **[services](glossary#Service.md)**. It can only called by an other method inside the service. Usually, it is used by **[configure](glossary#Methode_configure.md)** method.


### Start/stop method ###
> Those methods allow to start and stop a **[service](glossary#Service.md)**. When a service is stopped, it isn't destroy. So it is possible to use the start and stop method several times with the same instance of the service.


### Starting method ###
The **starting** method is implemented in the **[services](glossary#Service.md)**. It can only called by an other method inside the service. Usually, it is used by **[start](glossary#Methode_start.md)** method. It is a transition of the service state machine.


### Stop method ###
> See **[start](glossary#Methode_start.md)**.


### Stopping method ###
> This method is implemented in the **[services](glossary#Service.md)**. It can only called by an other method inside the service. Usually, it is used by **[stop](glossary#Methode_stop.md)**. It is a transition of the service state machine.


### Update method ###
> The **update** method is called when the service needs to update its associated **[data](glossary#Data.md)**. It also exists a **[update(message)](glossary#Methode_update(message).md)** method.


### Updating method ###
> The **updating** method is implemented in the **[services](glossary#Service.md)**. It can only called by an other method inside the service. Usually, it is used by **[update](glossary#Methode_update.md)**. It is a transition of the service state machine.


### Update(message) method ###
> The **update(message)** method is called when the **[service](glossary#Service.md)** receives a **[message](glossary#Message.md)**.


### Updating(message) method ###
> The **updating(message)** method is implemented in the **[services](glossary#Service.md)**. It can only called by an other method inside the service. Usually, it is used by **[update(message)](glossary#Methode_update(message).md)**. It is a transition of the service state machine.


### Message ###
> A **message** is used to send information between **[services](glossary#Service.md)**. It is relayed by **[edition service](glossary#Edition_serviceService.md)**. It can contain a **[data](glossary#Data.md)**.


### Profil xml file ###
> The **profile.xml** file gives the list of **[bundles](glossary#Bundle.md)** which will be used by the application. To build an application, a **profile.xml** and a **[configuration.xml](glossary#Configuration.md)** are requested.


### rootObject ###
> A **data rootObject** is the unique **[data](glossary#Data.md)** associated to the main **[application](glossary#Application.md)**. It can be a simple type **[data](glossary#Data.md)** like Image, TriangularMesh or a **[composite data](glossary#Composite.md)**.


### Sconspiracy ###
> The compilation system **[Sconspiracy](http://code.google.com/p/sconspiracy/)**, developped with **[Python](http://www.python.org/)** and **[SCons](http://www.scons.org/)** allows to build application on different operating system (Windows, Linux and OS).


### Service ###
> A **service** is an object always associated with a specific **[data](glossary#Data.md)**. It allows to modify or use this data. Several **services** can be associated to the same **[data](glossary#Data.md)**. Each **service** has the same methods which are **[configure](glossary#Methode_configure.md)**, **[start](glossary#Methode_start.md)**, **[update](glossary#Methode_update.md)**, **[update(message)](glossary#Methode_update(message).md)** and **[stop](glossary#Methode_stop.md)**.


### Edition service ###
> The **edition service** is the **[service](glossary#Service.md)** used to send **messages** to the others **[services](glossary#Service.md)** associated to a **[data](glossary#Data.md)** by the **[communication channel](glossary#Communication_channel.md)**

> ### TriangularMesh ###
> A **triangular mesh** consists of a set of triangles which are connected by their common edges or corners.