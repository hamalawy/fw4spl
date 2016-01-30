# Tools box #

## Contents ##

This document consists of :
  * Goal of the tools box
  * Tools
    * Profile.xml file
    * Configuration.xml file

## Goal of the tools box ##

The goal of the **tools box** is to give some samples of code to build correct **profile.xml** and **configuration.xml** files.

## Tools ##

### Profile.xml file ###

Wrting a **profile** for an application. It consists of a beginning tag and an ending tag. The new profil is written inside these tags.
The beginning tag has an attribut which is a name but it isn't used, in the following example the profile is called "ProfilName":

```
<profile name="ProfilName">
    ... Description of the application profil ...
</profile>
```

For example, if the profile name is "tutoBasic" :
```
<profile name="tutoBasic">
    ... Description of the application profil ...
</profile>
```

Loading of a **bundle** which has an identifier (id) called "BundleName" and a version number (version) called "BundleVersonNumber":
```
<activate id="BundleName" version="BundleVersonNumber" >
    ... Bundle parameters ...
</activate>
```

For example, if you want to load the dataReg bundle which has no parameters:
  * The identifier (id) will be "dataReg"
  * The version number (version) will be "0-1"

```
<activate id="dataReg" version="0-1" />
```

Now if you want to load a bundle with parameters like **gui bundle**, the following parameters "startingMode", "rootObject", "configFile" and "config" must be added. So:
  * The identifier (id) will be "gui"
  * The version number (version) will be "0-1".
  * The "startingMode" parameter could be "windows" or "console". For now, only "windows" mode works.
  * The "rootObject" parameter is used to define the root object of the application. There is a lot of kind of root object (Image, Triangular mesh, Composite, ...).
  * The "configFile" parameter define the path to the configuration file of the application.
  * The "config" parameter define the identifier of the **configuration** used to launch the application. In fact, it can exist several **configurations** in the same **configuration file**.

```
<activate id="gui" version="0-1" >
    <param id="startingMode" value="windows" />
    <param id="rootObject" value="::fwTools::Object" />
    <param id="configFile" value="./Bundles/tutoBasic_0-1/tutoBasicConfigFile.xml" />
    <param id="config" value="tutoBasicConfig" />
</activate >
```

> Launching a **bundle** named "BundleName" is written like this:

```
   <start id="BundleName" />
```

> For the **gui bundle**:

```
   <start id="gui" />
```


### Configuration.xml file ###

Writting of the **configuration** of the application is done between a beginning and ending tags. The beginning tag has an id which is the name of the **bundle**, for example "BundleName". For now, it isn't used.

```
<plugin  id="BundleName">
    ... Description of all the configurations ...
</plugin>
```

In this other example, the **bundle** name is "tutoBasic":

```
<plugin  id="tutoBasic">
    ... Description of all the configurations ...
</plugin>
```

Writting the beginning and ending tags by use of an identifier called "BundleIdentifier" and an object type called "DataType":

```
<extension id="BundleIdentifier" implements="DataType">
    ... Configuration of the application ...
</extension>
```

An other example where the configuration name is "tutoBasicConfig" and the data type is "::fwTools::Object":

```
<extension id="tutoBasicConfig" implements="::fwTools::Object">
    ... Configuration of the application ...
</extension>
```

Writting of the beginning and ending tags of the main object (rootObject). The description of the associated services is done between these two tags.

```
<object id="DataRootName" type="RootObjectType">
    ... Instanciation of the services associated with the rootObject ...
</object>
```

**Services** instanciation. An identifier is associated (manually or automatically) to the created service, it is used to identify the services during their using (starting, stopping). The service description is done between the beginning and ending tags of the root object.
For each services, we must give the "ServiceType", the "ServiceImplementation" and the "ServiceCommunicationState".

```
<service type="ServiceType" implementation="ServiceImplementation" autoComChannel="ServiceCommunicationState" >
    ... Service parameters ...
</service>
```

In the following **gui service** sample:
  * the service type is "::gui::aspect::IAspect",
  * the implementation service is "::gui::aspect::DefaultAspect",
  * the service communication state is "no".

```
<service type="::gui::aspect::IAspect" implementation="::gui::aspect::DefaultAspect" autoComChannel="no" >
    ... Service parameters ...
</service>
```

**Service** parameters instanciation. The **service** parameter value is given between tag nammed with parameter name:

```
<parameterName>parameterValue</parameterName>
```


In the = Tools box =

## Contents ##

This document consists of :
  * Goal of the tools box
  * Tools
    * Profile.xml file
    * Configuration.xml file

## Goal of the tools box ##

The goal of the **tools box** is to give some samples of code to build correct **profile.xml** and **cofiguration.xml** files.

## Tools ##

### Profile.xml file ###

Wrting a **profile** for an application. It consists of a beginning tag and an ending tag. The new profil is written inside these tags.
The beginning tag has an attribut which is a name but it isn't used, in the following example the profile is called "ProfilName":

```
<profile name="ProfilName">
    ... Description of the application profil ...
</profile>
```

For example, if the profile name is "tutoBasic" :
```
<profile name="tutoBasic">
    ... Description of the application profil ...
</profile>
```

Loading of a **bundle** which has an identifier (id) called "BundleName" and a version number (version) called "BundleVersonNumber":
```
<activate id="BundleName" version="BundleVersonNumber" >
    ... Bundle parameters ...
</activate>
```

For example, if you want to load the dataReg bundle which has no parameters:
  * The identifier (id) will be "dataReg"
  * The version number (version) will be "0-1"

```
<activate id="dataReg" version="0-1" />
```

Now if you want to load a bundle with parameters like **gui bundle**, the following parameters "startingMode", "rootObject", "configFile" and "config" must be added. So:
  * The identifier (id) will be "gui"
  * The version number (version) will be "0-1".
  * The "startingMode" parameter could be "windows" or "console". For now, only "windows" mode works.
  * The "rootObject" parameter is used to define the root object of the application. There is a lot of kind of root object (Image, Triangular mesh, Composite, ...).
  * The "configFile" parameter define the path to the configuration file of the application.
  * The "config" parameter define the identifier of the **configuration** used to launch the application. In fact, it can exist several **configuration** in the same **configuration file**.

```
<activate id="gui" version="0-1" >
    <param id="startingMode" value="windows" />
    <param id="rootObject" value="::fwTools::Object" />
    <param id="configFile" value="./Bundles/tutoBasic_0-1/tutoBasicConfigFile.xml" />
    <param id="config" value="tutoBasicConfig" />
</activate >
```

> Launching a **bundle** named "BundleName" is written like this:

```
<start id="BundleName" />
```

> For the **gui bundle**:

```
<start id="gui" />
```


### Configuration.xml file ###

Writting of the **configuration** of the application is done between a beginning and ending tags. The beginning tag has an id which is the name of the **bundle**, for example "BundleName". For now, it isn't used.

```
<plugin  id="BundleName">
    ... Description of all the configurations ...
</plugin>
```

In this other example, the **bundle** name is "tutoBasic":

```
<plugin  id="tutoBasic">
    ... Description of all the configurations ...
</plugin>
```

Writting the beginning and ending tags by use of an identifier called "BundleIdentifier" and an object type called "DataType":

```
<extension id="BundleIdentifier" implements="DataType">
    ... Configuration of the application ...
</extension>
```

An other example where the configuration name is "tutoBasicConfig" and the data type is "::fwTools::Object":

```
<extension id="tutoBasicConfig" implements="::fwTools::Object">
    ... Configuration of the application ...
</extension>
```

Writting of the beginning and ending tags of the main object (rootObject). The description of the associated services is done between these two tags.

```
<object id="DataRootName" type="RootObjectType">
    ... Instanciation of the services associated with the rootObject ...
</object>
```

**Services** instanciation. An identifier is associated (manually or automatically) to the created service, it is used to identify the services during their using (starting, stopping). The service description is done between the beginning and ending tags of the root object.
For each services, we must give  the "ServiceType", the "ServiceImplementation" and the "ServiceCommunicationState".

```
<service type="ServiceType" implementation="ServiceImplementation" autoComChannel="ServiceCommunicationState" >
    ... Service parameters ...
</service>
```

In the following **gui service** sample:
  * the service type is "::gui::aspect::IAspect",
  * the implementation service is "::gui::aspect::DefaultAspect",
  * the service communication state is "no".

```
<service type="::gui::aspect::IAspect" implementation="::gui::aspect::DefaultAspect" autoComChannel="no" >
    ... Service parameters ...
</service>
```

**Service** parameters instanciation. The **service** parameter value is given between tag nammed with parameter name:

```
<parameterName>parameterValue</parameterName>
```


In the following **service gui** example, there is two parameters:
  * the name of the main window named "name" and it takes the value "tutoBasicApplicationName",
  * the icon of the main window named "icon" and it takes the value "Bundles/tutoBasic\_0-1/tuto.ico".

```
<name>tutoBasicApplicationName</name>
<icon>Bundles/tutoBasic_0-1/tuto.ico</icon>
```

For the specific case of **data composite**, which is a **data** makes up with several sub-**data**, all the services, describe inside the beginning and ending tags, are not necessary associated to the **data composite**.
When a service is associated to a sub data, it will describe inside the beginning and ending tag of this sub-data as we can see in the following example. Three services are instanciated but only one, the first, is associated to the data composite.
The second is associetd to the "::fwData::Image" sub-data and the third to the "::fwData::TriangulareMesh" sub-data.

```
<object id="nomDuDataComposite" type="::fwData::Composite">
    <service type="ServiceType" implementation="ServiceImplémentation" autoComChannel="ServiceCommunicationState" >
        ... Service parameters ...
    </service>

    <object id="nomDeLapremièreDataContenueDansLaDataComposite" type="::fwData::Image">
        <service type="ServiceType" implementation="ServiceImplémentation" autoComChannel="ServiceCommunicationState" >
            ... Service parameters ...
        </service>
    </object>

    <object id="nomDeLaSecondeDataContenueDansLaDataComposite" type="::fwData::TriangulareMesh">
        <service type="ServiceType" implementation="ServiceImplémentation" autoComChannel="ServiceCommunicationState" >
            ... Service parameters ...
        </service>
    </object>

</object>
```

**Service** starting (The starting order is optional, but it must be necessary that a **service** starts before an other one)

```
<start type="ServiceType" />
```

For example, with the **gui service**, the **service**type is "::gui::aspect::IAspect":

```
<start type="::gui::aspect::IAspect" />
```

**Service** closing (the closing order is optional, but sometime, some services must be closed before an other one).

```
<stop type="typeDuService" />
```

For example, with the **gui service**, the **service**type is "::gui::aspect::IAspect":

```
<stop type="::gui::aspect::IAspect" />
```
**service gui** example, there is two parameters:
  * the name of the main window named "name" and it will take the value "tutoBasicApplicationName",
  * the icon of the main window named "icon" and it will take the value "Bundles/tutoBasic\_0-1/tuto.ico".

```
<name>tutoBasicApplicationName</name>
<icon>Bundles/tutoBasic_0-1/tuto.ico</icon>
```

For the specific case of **data composite**, which is a **data** makes up with several sub-**data**, all the services, describe inside the beginning and ending tags, are not necessary associated to the **data composite**.
When a service is associated to a sub data, it will describe inside the beginning and ending tag of this sub-data as we can see in the following example. Three services are instanciated but only one, the first is associated to the data composite.
The second is associetd to the "::fwData::Image" sub-data and the third to the "::fwData::TriangulareMesh" sub-data.

```
<object id="nomDuDataComposite" type="::fwData::Composite">
    <service type="ServiceType" implementation="ServiceImplémentation" autoComChannel="ServiceCommunicationState" >
        ... Service parameters ...
    </service>

    <object id="nomDeLapremièreDataContenueDansLaDataComposite" type="::fwData::Image">
        <service type="ServiceType" implementation="ServiceImplémentation" autoComChannel="ServiceCommunicationState" >
            ... Service parameters ...
        </service>
    </object>

    <object id="nomDeLaSecondeDataContenueDansLaDataComposite" type="::fwData::TriangulareMesh">
        <service type="ServiceType" implementation="ServiceImplémentation" autoComChannel="ServiceCommunicationState" >
            ... Service parameters ...
        </service>
    </object>

</object>
```

**Service** starting (The starting order is optional, but it must be necessary that a **service** starts before an other one)

```
<start type="ServiceType" />
```

For example, with the **gui service**, the **service**type is "::gui::aspect::IAspect":

```
<start type="::gui::aspect::IAspect" />
```

**Service** closing (the closing order is optional, but sometime, some services must be closed before an other one).

```
<stop type="typeDuService" />
```

For example, with the **gui service**, the **service**type is "::gui::aspect::IAspect":

```
<stop type="::gui::aspect::IAspect" />
```