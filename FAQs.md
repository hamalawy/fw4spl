**What is fw4spl?**

> This framework, created by Ircad, is a modular workspace for rapid application development. It provides some software modules to make easier the creation of software application. It is mainly dedicated to the medical software. Therefore, many functionalities like digital image processing, simulation of medical interactions are allowed.

**What does fw4spl mean?**

> FW4SPL means FrameWork for Software Production Line. It is also called F4S.

**What is the characteristics of fw4spl?**

> The framework is build around the notion of component (bundle).
> It is possible to build an application without writing any line of code (C++), you only write 2 xml files.
> Once a bundle is built, you can reuse it for many applications.
> It is very easy to extend or reduce (for light version) an application. You just have to modify the profile and configuration files.

**What platforms does fw4spl run on?**

> This framework can run  under Windows, Linux and MacOS.

**Where can I find applications done with fw4spl?**

> Some tutorials are provide with the framework.
> A free software VR-Render is available on  http://www.ircad.fr/softwares/vr-render/Software.php

**Which technology do you need to develop bundle?**

> You must have knowledge in C++. Concerning the configuration files, the syntx is closes to XML.

**What is BinPkgs?**

> The BinPkgs (binary packages) contain all the extern libraries and theirs interfaces requested for using and extending fw4spl. They depend on the OS system and the compiler. Some can be download on fw4spl repository (http://code.google.com/p/fw4spl/downloads/list).

**Is it difficult to compile an application with fw4spl?**

> No, it isn't.  Compile fw4spl means compile all the bundle of the framework.
> An example is provided with fw4spl, you just have to type �racy Tuto� after the installation.

**Why does fw4spl provide a launcher?**

> The launcher is used to create the applicationn. It parses the profile and configuration xml file to build the application.

**How can I write a bundle?**

> See Tutorial 6 tutoBundle, to have a sample.

**How can I debug my program ?**

> Firstly, I can change le log level of a sub-project when I compile the source code with racy. For that, for example, I can configurate each sub-projects by modifying the file "sconspiracy/Config/user.prj.options" (copy of file "sconspiracy/Config/user.prj.options-dist"). In this file I have :

> LOGLEVEL = {
'fwServices'    : 'debug',
'fwData'        : 'warning',
'fwRuntime' 	: 'debug',
'sub-project1' 	: 'info',
'sub-project2' 	: 'fatal',
}

> Where allowed values are : ['trace', 'debug', 'info', 'error', 'fatal', 'warning', 'disable']. the value 'trace' gives me the maximun of log, 'disable' disables log.
  * note a : Print many log messages ( by activating trace on all sub-projects for ex. ) can be very time consuming for the application.
  * note b : Under windows system, log messages are saved on filesystem in SLM.log file, in the working directory.

> Secondly, you can use gdb (Linux/Mac) or Visual Studio (Windows) to debug your application in running with a program compiling in debug (racy DEBUG=full myApp).
  * note a : you can use gdb like this "LD\_LIBRARY\_PATH=./lib gdb -arg bin/launcher Bundles/myApp/myProfile.xml", and press "r" for run the program
  * note b : you can use under gdb the command "catch throw" hence gdb will stop near the error
  * note c : Documentation to learn using gdb : http://www.cs.tau.ac.il/lin-club/lecture-notes/GDB_Linux_telux.pdf

> Thirdly, you can manage the program complexity by reducing the number of activated components (in profile.xml) and the number of created services (in config.xml) to better localize errors.

> Fourthly, verify that your profile.xml / config.xml and each bundle plugin.xml are well-formed, by using xmllint (command line tool provided by libxml2).



**I have an assertion/fatal message when I launch my program, any idea to correct the problem ?**

> First, I can read the message :) and try the problem.

> In many cases, there are two kind of problems. The program does not arrive :
  * to create the service given in configuration In this case, 4 reasons are possibles :
    * the name of service implementation in config.xml contains mistakes
    * the bundle that contains this service is not activated in the profile
    * the bundle plugin.xml, that contains this service, not declares the service or the declaration contains mistakes.
    * The service is not register in the Service Factory (forget of macro REGISTER\_SERVICE(...) in file .cpp)
  * to manage the configuration of service. In this case, the implementation code in .cpp file ( generally configuring() method of service ) does not correspond to description code in config.xml ( Missing arguments, or not well-formed, or mistakes string parameters ).


**If I use fw4spl, do I need wrap all my data ?**

> The first question is to know if the data is on center of application:
  * Need you to shared data between few bundles ?
  * Need you to attach services on this data ?

> If the answer is no, you not need wrapping your data. If the answer is yes. you need to have an object that inherits of ::fwData::Object.

> In this last case, do you need shared this object between different services which use different libraries, ex for Object Image : itk::Image vs vtkImage ?
  * If the answer is yes, you need create a new object like fwData::Image and a wrapping with fwData::Image<=>itk::Image and fwData::Image<=>vtkImage.
  * If the answer is no, you can just encapsulated an itk::Image in fwData::Image and create an accessor on it. ( however, this kind of choice implies that all applications that use fwData::Image need itk library for running. )

**Where can I find the documentation of the XML tags used in service configuration?**

> Today, we have not tag dictionnaries in the f4s. We think refactoring this part to normalize the sementic xml tag in the future and adding some comments in doxygen documentation of each configuring method, however is not one of our priority today. For the moment, to know exactly tags that we can use in a configuration service, you must see the method configuring() in the service implementation (in .cpp file).