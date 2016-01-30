# Compilation with Sconspiracy #

## Introduction ##

SConspiracy, written in Python, is a set of scripts to configure Scons.
SCons (http://www.scons.org/) is an Open Source software construction tool written in Python. It proposes a set of functionalities (automatic dependency analysis built-in for C, C++ ). Think of SCons as an improved, cross-platform substitute for the classic Make utility with integrated functionality similar to autoconf/automake and compiler caches such as ccache. In short, SCons is an easier, more reliable and faster way to build software.

SConspiracy has by default:
  * a configuration for GNU C++, Visual Studio 2003 and 2008.
  * options for C++ traditional libraries like boost, wxWidgets ...
  * support for CPPUnit, Doxygen.
  * and several optimizations for your C++ projects.

For more details, you can visit  [SConspiracy quick start guide](http://code.google.com/p/sconspiracy/wiki/QuickStart).

In this documentation, we are going to explain how to use SConpiracy with fw4spl framework.

## Mandatory ##

  * A compiler known by Sconspiracy. Currently, it can be:
    * Visual C++ 2003
    * Visual C++ 2008 (included [express edition](http://www.microsoft.com/express/vc/))
    * g++
  * [Python 2.6](http://www.python.org/).
  * [SCons 1.2](http://www.scons.org/).
  * [Pywin32](http://pywin32.sourceforge.net/) (only for Windows users).

### Installation ###

  * Open a shell console, enter the following command:
```
hg clone https://sconspiracy.googlecode.com/hg/ sconspiracy -r sconspiracy_0.0
```
  * Add the sconspiracy bin folder path in your environment path.
  * Add RACY\_CONFIG\_DIR variable in your environment and set it to the path of sconspiracy Config folder.
  * Copy sconspiracy/Config/user.options-dist and rename it to user.options.
  * Set the right path in your user.options (normally you just have to change the dev\_dir variable).

For a complete fw4spl environment installation see [QuickStart](QuickStart.md).

## Use ##

### Project building ###

To Build a project (for example fwData), you just have to type the following command:
```
racy fwData
```

SConspiracy will automatically
  * Lookup  fwData  in the development folder (set in the user.options file) and its subfolders.
  * Lookup fwData dependency (in this case fwTools, fwCore).
  * Compile and install fwData and its dependencies in the Install folder.

To build a project (for example fwData) without its dependencies, you just have to type :
```
racy fwData BUILDDEPS=no
```

### Log level ###

SConspiracy allows to compile a project with a defined log level (loglevel).
The amount of data displayed depends on the log level you choose. Sconspiracy enables seven levels from the more precise to the less (without any log information):
  * trace
  * debug
  * info
  * warning
  * error
  * fatal
  * disable

The default level is **warning**.

To change the log level, you must give in your command:
```
racy fwData/LOGLEVEL:trace
```

This command will compile fwData with a log level 'trace' but all the dependencies of fwData (fwCore and fwTools) are going to be compile with the default log level.

If you want to change the log level of the project and the log level of one specific dependency (here fwCore), you can to something like this:
```
racy fwData/LOGLEVEL:trace fwCore/LOGLEVEL:debug
```
In this case fwData has a log level 'trace' and fwCore has a log level 'debug' and all others dependencies has the default log level.

If you want to change all log level for fwData and its dependencies, you can type the following command:
```
racy fwData LOGLEVEL=fatal
```
Here fwData and all dependencies of fwData (fwCore and fwTools) will be compile with the log level 'fatal'

It is also possible to mix the options. For example, the following command will build fwData with a log level 'trace' and all its dependencies with a log level  'warning':
```
racy fwData/LOGLEVEL:trace LOGLEVEL=warning
```

This command has two kind of options:
  * project option like xxx/LOGLEVEL:trace.
  * global option like LOGLEVEL=warning.

### Special command ###

#### Test units ####

SConspiracy manages the build and the execution of the test units with  [CppUnit](http://apps.sourceforge.net/mediawiki/cppunit/). To active this option proceed like this:
```
racy fwData/CPPUNIT:exec/CPPUNIT_RUN:yes
```

#### Documentation ####

SConspiracy allows to generate [Doxygen](http://www.doxygen.org/) documentation with the option DOX:
```
racy fwData/DOX:yes
```

#### Cleanning ####

SConspiracy can automatically clean your project (removal of obj files and binary files). For example with the project fwData, you will type:
```
racy fwData -c
```

#### Help ####

There is two kind of help.
  * The SCons help (clean, version, number of jobs) that you can have with:
```
racy -H
```

  * The help for the global options and the specific project options propose by SConspiracy (BUILDDEPS, DOX, LOGLEVEL, DEBUG...)
```
racy -h
```

### Options files ###

#### user.options ####

The file user.options are located in the Config folder of Sconspiracy where you can also find an user.option file example: "user.options-dist". This file, which is Python script, allows to configure the user global options.
Among the most important options, you can find:
  * **dev\_dir**: is the working directory of the user.
  * **MSVS\_VERSION**: only for the Windows users, allows to specify the compiler version(cl 7.1, 9.0...)
  * **RACY\_BUILD\_DIR**: gives the path to the compiled versions of projects.
  * **RACY\_INSTALL\_DIR**: gives the folder where you can find the application.
  * **RACY\_BINPKGS\_DIR**: gives the folder where you can find the binary packages.
  * **RACY\_CODE\_DIRS**: gives the path to the top level directory of your project source code.

#### build.options ####

The build.options file is located in the bin directory of each project. It configures the build of the project. This is a Python script where you can use all the Python module.

Among the most important options, we have:
  * **TYPE** field gives the project type (bundle, lib or application).
  * **USE** field gives the list of external library (VTK, boost...).
  * **LIB** field gives the list of the compilation system project used by the bundle (fwData, fwTools...).
  * **LOGLEVEL** field gives the debug information level(trace, debug, info, warning, error, fatal or disable)
  * **VERSION** field gives the component version number.