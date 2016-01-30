# Beginner training #







## Version ##

Training realized on :

  * fw4spl branch : fw4spl\_0.8.2

  * sconspiracy branch : sconspiracy\_0.4 ( with mako plugin )


Solutions are available on fw4spl in :
  * Apps/Training
  * Bundles/Training


## Knowledge requirements ##

  * POO

  * C++ dev

  * typedef

  * Namespace

  * ifdef on hpp

  * API export for dynamic library

  * assert

  * Qt (beginner level)

  * Factory design pattern

  * Smart pointer

  * Singleton design pattern



## FW4SPL requirements ##

  * Sconspiracy has been installed

  * FW4SPL framework has been installed

  * BinPkgs has been installed



## Tuto 01 : Creation my first service ##

Develop an application that create a reader service on a data string that will be initialize by reading a text file. The path of this file will be a parameter setted in the configuration file.



### Purpose ###



  * Create my first Application

  * Create my first Bundle

  * Create my first Service

  * Use log functions

  * Use Service API : start, stop, update, configure, getObject



### Step 01 : Create new App ###





Create new training repository dedicated to your work



```

mkdir Dev/Src/training

```



Create new Apps section



```

mkdir Dev/Src/training/Apps

```



Create new application called BeginnerTraining01



```

cd Dev/Src/training/Apps

racy BeginnerTraining01/CREATE_PRJ:app

```



**Not mandatory** : Generate your project for your favorite IDE (here Eclipse indigo) and then import it



```

racy BUILD=no BeginnerTraining01/IDE=eclipse-indigo

```



Study the new generated files created in BeginnerTraining01 and discuss together



```

bin/build.options // compilation information

rc/profile.xml // use by launcher to activate and start bundle

rc/plugin.xml // contain an application configuration 

```



Compile your application



```

racy BeginnerTraining01

```



Execute your application



```

cd Dev/Install

bin\launcher_0-1_win_64_cl9-0_D.exe Bundles\BeginnerTraining01_0-1\profile.xml

```



### Step 02 : Create new Bundle ###





Create a new bundle section



```

mkdir Dev/Src/training/Bundles

```



Create a new bundle beginnerTraining



```

cd Dev/Src/training/Bundles

racy beginnerTraining/CREATE_PRJ:bundle

```



**Not mandatory** : Generate your project for your favorite IDE and then import it



```

racy BUILD=no beginnerTraining/IDE=eclipse-indigo

```



Study new files created in beginnerTraining and discuss together



```

bin/build.options // compilation information

rc/plugin.xml // xml description of the bundle : name, library, bundle requirements, extension implementation

include/beginnerTraining/config.hpp // file to manage export with dynamic library

include/beginnerTraining/namespace.hpp // documentation file to generate the namespace documentation : beginnerTraining

include/beginnerTraining/Plugin.hpp // header file for Plugin class

src/beginnerTraining/Plugin.hpp // source file for Plugin class

```



**Note** : In many cases, Plugin class is not used by developper. This class is created and started just after the library loading to perform some work (ex: Init context, Init singleton, load and stock ressource, ... ). A stop method is also called when the library is free when the program is closed. Few bundles really use this class (servicesReg, guiQt, guiWx, ...)



Add your bundle dependency beginnerTraining to your application BeginnerTraining01



```

BeginnerTraining01/bin/build.options // Add bundle dependency

BeginnerTraining01/rc/profile.xml // Activate your bundle and start it

```



Recompile your application ( normally racy should also compile beginnerTraining ), then relaunch your application to verify if all goes fine.



To better understand the bundle starting, we are going to add trace to follow the execution. In the build.options file of your bundle, add fwCore library dependency. Then in Plugin.cpp, add the header file :



```

#include <fwCore/spyLog.hpp>

```



Then add the call of SLM\_TRACE\_FUNC() function in Plugin::start() and Plugin::stop() method.



Recompile only your bundle and enable trace message.



```

racy BUILDDEPS=no beginnerTraining/LOGLEVEL=trace

```



Relaunch your application and analyze log file SLM.log in your Dev/Install directory. To try, you can not start your bundle in your application profile to see the differences.



Log messages are very usefull during development and we don't use directly std::cout to print information.

To test the different level of log messages, add the following code in the start function of your bundle :



```

std::string str (" with stream");

bool condition = true;

SLM_TRACE("Trace message");

OSLM_TRACE("Trace message" << str );

OSLM_TRACE_IF("Trace message" << str << " when condition = " << condition, condition );



SLM_DEBUG("Debug message");

SLM_INFO("Info message");

SLM_WARN("Warning message");

SLM_ERROR("Error message");

SLM_FATAL("Fatal message");



SLM_ASSERT("Fatal message", condition); // only compile in debug

```



And see the trace in your SLM.log file with different bundle LOGLEVEL



```

racy BUILDDEPS=no beginnerTraining/LOGLEVEL=trace

LOGLEVEL allowed values : [ 'trace', 'debug', 'info', 'warning', 'error', 'fatal', 'disable' ]

```



### Step 03 : Create new reader service ###





The purpose here is to create in your bundle a new reader service to read a text file and set its content in a fwData::String.



Create a new service called SStringReader in sub namespace tuto01



```

cd Dev/Src/training/Bundles/beginnerTraining

racy CREATE_SRV=/tuto01/SStringReader

```



Define the service type and its associated data in the different files, here Type=::io::IReader and Data=::fwData::String



```

build.options // Add new lib dependencies : fwServices, fwData, fwTools and new bundle dependeny : io

SStringReader.hpp // update file to define good service type

SStringReader.cpp // update file to define good service type / data

```



Service of type ::io::IReader needs to implement another method configureWithIHM(), declare it in your service. This method does nothing for the moment.



```

BEGINNERTRAINING_API void configureWithIHM()

```



Add trace messages for all functions.



Add bundle requirements and declare service in the plugin.xml of your bundle.



```

<requirement id="dataReg"/>

<requirement id="servicesReg"/>

<requirement id="io"/>



<extension implements="::fwServices::registry::ServiceFactory">

    <type>::io::IReader</type>

    <service>::beginnerTraining::tuto01::SStringReader</service>

    <object>::fwData::String</object>

</extension>

```



Your bundle contains new service working on a fwData::String. Recompile only your bundle to verify your code (activate trace message).



Now, it is time to use your new service in your application by doing the following step :

  * Add bundle io in your application build.options

  * Activate this bundle in your application profile.xml ( because this bundle is required by your component now )

  * Modify your application to have as main object a ::fwData::String. Then add your new service on this data



```

<config>



    <object type="::fwData::String">



        <service uid="myFrame" ... />



        <service uid="myReader" type="::io::IReader" implementation="::beginnerTraining::tuto01::SStringReader" autoComChannel="no" />

        

        <start uid="myFrame" />

        <start uid="myReader" />

        

        <update uid="myReader" />

        

    </object>

    

</config>

```



In this config, you start your service and update it.



To take into account your modification, recompile your application. Then execute it and analyze log.



Perform few tests without start/update in config (DON'T FORGET TO RECOMPILE).



Now, it is time to read a file with your service. Create a text file in your bundle resource directory.



```

beginnerTraining/rc/data.txt // data.txt contains simple text

```



After bundle compilation this resource file will be install here



```

Dev/Install/Bundles/beginnerTraining_0-1/data.txt

```



Add some code in SStringReader::update() to read this file, for example :



```

// Create path "Bundles/beginnerTraining_0-1/data.txt"

std::stringstream myPath;

myPath << "Bundles/" << PRJ_NAME << "_" << BEGINNERTRAINING_VER << "/data.txt";

::boost::filesystem::path myBoostPath ( myPath.str() );



// Read data.txt

std::string line;

std::string data ("");

std::ifstream myfile ( myBoostPath.string().c_str() );

if ( myfile.is_open() )

{

	while ( myfile.good() )

	{

		getline( myfile, line );

		OSLM_DEBUG("Read line : " << line );

		data += line;

	}

	myfile.close();

}

else

{

	OSLM_ERROR("Unable to open file : " << myBoostPath.string() );

}

OSLM_DEBUG("Loaded data : " << data );

```



**Note** : The Boost.Filesystem library provides portable facilities to query and manipulate paths, files, and directories. We use here the extern library boost. Don't forget to add library dependency in your bundle build.options



```

USE=['boost']

```



Now you must set this new string value to your associated object

```

::fwData::String::sptr myAssociatedData = this->getObject< ::fwData::String >();

myAssociatedData->setValue( data );

```



### Step 04 : Configure reader service in xml ###





The purpose here, is to configure in xml the path of the file.txt used by the reader.



Modify the service declaration in your application config (plugin.xml) to set in parameter the service data file (data.txt), replace



```

<service uid="myReader" type="::io::IReader" implementation="::beginnerTraining::tuto01::SStringReader" autoComChannel="no" />

```



by



```

<service uid="myReader" type="::io::IReader" implementation="::beginnerTraining::tuto01::SStringReader" autoComChannel="no">

 <file>Bundles/beginnerTraining_0-1/data.txt</file>

</service>

```



Normally, you must modify your SStringReader::configuring() to analyze the new service declaration. But for ::io::IReader service type, superclass already provide a default implementation of configuring method. **Go analyze it ;-)**



To use it you must remove your configuring method and override ::io::IReader::getIOPathType method like this :



```

::io::IOPathType SStringReader::getIOPathType() const

{

    return ::io::FILE;

}

```



After that you can use inherited method hasLocationDefined() and getFile() in your SStringReader::update() method



```

if ( this->hasLocationDefined() )

{

    ...

    std::ifstream myfile ( this->getFile().string().c_str() );

    ...

}

```


## Tuto 02 : Create a Qt editor service ##

### Step 01 : Create new editor service ###
In beginnerTraining bundle, create service ::beginnerTraining::tuto02::SStringEditor of type ::gui::editor::IEditor and working on a ::fwData::String. Update created service, add gui bundle dependency ( build.options and plugin.xml ) and update plugin.xml to declare this service.

IEditor service need to call inherited function to parse xml configuration and create Qt or Wx panel, see above
```
void SStringEditor::configuring() throw ( ::fwTools::Failed )
{
    this->initialize(); // start with this inherited function
}

void SStringEditor::starting() throw ( ::fwTools::Failed )
{
    this->create(); // start with this inherited function
}

void SStringEditor::stopping() throw ( ::fwTools::Failed )
{
    this->destroy(); // finish with this inherited function
}
```
Note : see IGuiContainerSrv documentation (fwGui/IGuiContainerSrv.hpp) to have information on these functions. To generate the documentation of your project launch this command :
```
racy BUILD=no BeginnerTraining01/DOX:yes
```

After create() method you can retreive QWidget dedicated to your service with the following lines :
```
void SStringEditor::starting() throw ( ::fwTools::Failed )
{
    this->create(); // start with this inherited function

    // Retreive Qt container
    ::fwGuiQt::container::QtContainer::sptr qtContainer =  ::fwGuiQt::container::QtContainer::dynamicCast( this->getContainer() );
    QWidget* container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);
}
```
Note : Add qtCore extenr librairy and fwGuiQt in your build.options dependency

Change background color of your panel :
```
// Change background color in red
QPalette p ( container->palette() );
p.setColor( QPalette::Background, Qt::red );
container->setPalette( p );
container->setAutoFillBackground( true );
```


### Step 02 : Create new app ###

Create a new app called BeginnerTraining02 which is a clone of BeginnerTraining01 ( you can copy/paste BeginnerTraining01 and modify it after ). Add your editor service in the app configuration and configure your editor to use widget created by frame.

```
<service uid="myFrame" type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no">
    <gui>
        ...
    </gui>
    <registry>
        <view sid="myEditor" start="yes" />
    </registry>
</service>

<service uid="myEditor" type="::gui::editor::IEditor" implementation="::beginnerTraining::tuto02::SStringEditor" autoComChannel="no" />
```

### Step 03 : Show in your editor your data ###

In your starting method add an QTextEdit in your container
```
// Create textEditor
QVBoxLayout* layout = new QVBoxLayout( container );
m_textEditor = new QTextEdit( container );
m_textEditor->setPlainText( "Edit text !" );
layout->addWidget( m_textEditor );
container->setLayout( layout );
```

Modify your updating() method to get your data and set its content in the text editor
```
void SStringEditor::updating() throw ( ::fwTools::Failed )
{
    // Update your textEditor from your associated object
    ::fwData::String::sptr myAssociatedData = this->getObject< ::fwData::String >();
    m_textEditor->setPlainText( myAssociatedData->getValue().c_str() );
}
```

Then, modify your app config to update your editor after your reader update.

### Step 04 : Play with View layout ###

Modify your app configuration to have to editor service on your data by slippting the frame in 2 parts :
```
<service uid="myFrame" ... >
    ...
    <registry>
        <view sid="view" start="yes" />
    </registry>
</service>

<service uid="view" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no">
    <gui>
        <layout type="::fwGui::LineLayoutManager">
            <orientation value="horizontal" />
            <view />
            <view />
        </layout>
    </gui>
    <registry>
        <view sid="myEditorA" start="yes" />
        <view sid="myEditorB" start="yes" />
    </registry>
</service>

<service uid="myEditorA" ... />
<service uid="myEditorB" ... />
```

## Tuto 03 : Add communication between services ##

### Step 01 : Create new services / app ###

  * Create service ::beginnerTraining::tuto03::SStringReader ( clone of your previous reader )
  * Create service ::beginnerTraining::tuto03::SStringEditor ( clone of your previous editor )
  * Create app BeginnerTraining03 ( clone of your previous app BeginnerTraining02 )

### Step 02 : Reader send new event ###

Modify your reader to notify value modification
```
void SStringReader::notifyMessage()
{
    SLM_TRACE_FUNC();
    ::fwData::String::sptr associatedObj = this->getObject< ::fwData::String >();

    // Creation of an object message to say that data is modified
    ::fwServices::ObjectMsg::NewSptr msg;
    msg->addEvent( ::fwServices::ObjectMsg::UPDATED_OBJECT ) ;

    // Notifies message to all service listeners
    ::fwServices::IEditionService::notify( this->getSptr(), associatedObj, msg );
```

### Step 03 : Activate info messages for fwServices ###

you can recompile fwServices to log communication messages ( you can modify in sconspiracy user.prj.options )
```
LOGLEVEL = {
     'fwServices':'info',
     'beginnerTraining':'trace',
}
```

### Step 04 : Editor receive new event and manage it ###

First, activate comchannel on your app config and remove update on editor
```
<service uid="myEditorA" ... autoComChannel="yes" />
<service uid="myEditorB" ... autoComChannel="yes" />
```

Then, modify your editor to handle specific event :
```
SStringEditor::SStringEditor()
{
    addNewHandledEvent( ::fwServices::ObjectMsg::UPDATED_OBJECT );
}
```

And modify your update(msg) method to manage event
```
void SStringEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed )
{
    // If event is UPDATED_OBJECT
    if ( _msg->hasEvent( ::fwServices::ObjectMsg::UPDATED_OBJECT  ) )
    {
        this->updating();
    }
}
```

### Step 05 : Modify your editor to edit text and send event on modification ###

see the solution on fw4sl

## Tuto 04 : Add an action which creates a reader service ##
### Step 01 : Create a new action ###
### Step 02 : Create reader service in action ###