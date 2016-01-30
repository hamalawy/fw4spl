Tutorial 7 : Tuto07LoaderExternalData

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4  |              |
| fw4spl\_0.5  |              |
| fw4spl\_0.6  | See below    |
| fw4spl\_0.7.0 | [Tuto07LoaderExternalData](Tutorial7.md) |

## Introduction ##

In the previous tutorials, we have seen how to write application to load image and meshes.
The aim of this tutorial is to use image and numerical data (like mesure points) on an application.

For that, we are going to write an application to load a vtk image and to read a mumerical data file (file .us).
It will also provide two save functionalities (save all data (image and numerical data) or save numerical data only. The file format will be xml.
We can notice that the framework FW4SPL provides a component to save xml file called ioXml.

### Step 1: Creating an application to load a vtk image ###

This part matches to the tutorial n°3 TutoDataService so we just show the files **config.xml** and **profile.xml** updated
These files will be the base for our application.

For more details on this file show Tutorial n°3 TutoDataService

**Profile file**

```
<profile name="tutoLoaderExternalData" version="0.1">

    <!-- List of basic bundles. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />

    <activate id="gui" version="0-1" >
        <param id="startingMode" value="windows" />
        <param id="rootObject" value="::fwData::Composite" />
        <param id="config" value="tutoLoaderExternalDataConfig" />
        <param id="configFile" value="./Bundles/Tuto07LoaderForExternalData_0-1/configurations/config.xml" />
    </activate>

    <!-- List of specific bundles. -->
    <activate id="ioXML" version="0-1" />
    <activate id="ioVTK" version="0-1" />
    <activate id="ioData" version="0-1"/>
    <activate id="ioTuto" version="0-1"/>    
    <activate id="uiIO" version="0-1" />

    <activate id="visu" version="0-1" />
    <activate id="vtkSimpleNegato" version="0-1" />
    
    <!-- List of the specific bundles that need to be loaded in this application context. -->
    <start id="dataReg" />
    <start id="ioXML" />
    <start id="ioData"/>
    <start id="ioTuto"/>
    <start id="gui" />
</profile>
```

**Configuration file**

```
<plugin id="tutoLoaderExternalDataConfig">
<extension id="tutoLoaderExternalDataConfig" implements="::fwData::Composite">

<!-- The root data object in TutoConfigurations is a ::fwData::Composite. because it has to manage several objects -->
<object id="root" type="::fwData::Composite">
    <!--  a ::fwData::Image. -->
     <object id="tutoLoaderExternalDataImage" type="::fwData::Image">
        <!-- The first service (::gui::action::StarterActionService) is a generic action. In the configuration of this service,
        we specify the id of the services to be started and updated (start and update methods) when user click on the menuItem "Open image".
        In this tutorial, ::gui::action::StarterActionService service starts and updates the service with the uid "myReaderPathFile". -->
        <service uid="action_openFile" name="Open image" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" shortcut="Ctrl+O" autoComChannel="no" >
            <start uid="myReaderPathFile"/>
        </service>
        <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="myReaderPathFile" autoComChannel="no">
            <type mode="reader" />
        </service>

        <!-- The other service (::gui::action::StarterActionService) is a generic action. In the configuration of this service,
        we specify the id of the services to be started and updated (start and update methods) when user click on the menuItem "save image".
        In this tutorial, ::gui::action::StarterActionService service starts and updates the service with the uid "myWriterPathFile". -->
        <service uid="action_SaveFile" name="Save image" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" shortcut="Ctrl+S" autoComChannel="no" >
            <start uid="myWriterPathFile"/>
        </service>
        <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IEditor" uid="myWriterPathFile" autoComChannel="no">
            <type mode="writer" />
        </service>

        <!-- This service (::vtkSimpleNegato::RendererService) is a 3D visualization service of medical images. -->
        <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="myRenderingTuto" autoComChannel="yes" />
        
    </object>
    
     <!-- Graphical user interface manager.
     In the tag <menus> we specify the identifiers of different services for the different menus.
     In this tutorial, only one service (::gui::aspect::IMenu type) is created. 
     This application have 2 menus one to open/save image and one to load/save extra data.
     There are FW4SPL identifiers. -->

    <service type="::gui::frame::IFrame" implementation="::gui::frame::MainFrame" autoComChannel="no">
        <name>Tuto07LoaderForExternalData</name>
        <icon>Bundles/Tuto07LoaderForExternalData_0-1/tuto.ico</icon>
        <view uid="myRenderingTuto" autoStart="yes" />
    </service>
     
    <service uid="myMenuBar" type="::gui::aspect::IMenuBar" implementation="::gui::aspect::DefaultMenuBar" autoComChannel="no" >
        <menus>
            <menu uid="menu_File" />
        </menus>
     </service>
     
     <!-- This service define the file menu --> 
     <service name="File" uid="menu_File" type="::gui::aspect::IMenu" implementation="::gui::aspect::DefaultMenu" autoComChannel="no">
       <action uid="action_openFile" />
       <action uid="action_SaveFile" />
       <separator />    
       <action uid="action_quit" />
     </service>
     

     <!-- This service (::gui::action::QuitAction) is a generic action that will close the application when the user click on the menuItem "Quit".
     It's possible to associate with this kind of service (::gui::action::IAction) specific attributes to configure their style, shortcut... 
     (Note: In this tutorial, there is an attribute specialAction with the value "QUIT". On MS Windows, there's no impact.
            But on Mac OS, this value installs the menuItem in the system menu bar,
            and on Linux this value installs the default 'Quit' system icon in the menuItem.)
     The attribute "shortcut" is set with the value "Ctrl+Q" to close the application with this specific shortcut. -->
     <service uid="action_quit" specialAction="QUIT" name="Quit" type="::gui::action::IAction" implementation="::gui::action::QuitAction" shortcut="Ctrl+Q" autoComChannel="no" />

    <!-- Definition of the starting and closing order of the different services. -->
    <start type="::gui::frame::IFrame" />
    <start type="::gui::aspect::IMenuBar" />
    
    <stop type="::gui::aspect::IMenuBar" />
    <stop type="::gui::frame::IFrame" />    
</object>
</extension>
</plugin>
```

This application can be used to load a vtk or a fwXML image and to save it on fwXml format.
Only the xml format is available because we use it to show the result of the tutorial (show that the data are well saved).

**To be noted**: To save in fwXml format, it is better to use an empty folder, due to the several files saved (xml file , xsd files and vtk image).

### Loading image validation ###

To test the application at the end of this step, do the followed test:

  * Launch application
  * Load a vtk image (menu File/Open Image).
  * Save the file (menu File/Save Image).
  * Edit the file and check that the image are well saved or load the xml file.

If you have some trouble, an intermediate version of file config.xml and profile.xml (profileImageOnly.xml and configImageOnly.xml) is stored in the rc folder of this tutorial.

### Step 2: Add numerical extern data read and write ###

#### Imput data ####

The data that we are going to add are transformation matrix (4x4). The imput file of this tutorial has 4 matrix (one per line).
The name of the image associated to the matrix is given on the first line of the file.
The data to read:
```
      image1      
      1.1 2.1 3.1 4.1 5.1 6.1 7.1 8.1 9.1 10.1 11.1 12.1 13.1 14.1 15.1 16.1 
      11.1 12.1 13.1 14.1 15.1 16.1 17.1 18.1 19.1 20.1 21.1 22.1 23.1 24.1 25.1 26.1
      1.01 2.01 3.01 4.01 5.01 6.01 7.01 8.01 9.01 10.01 11.01 12.01 13.01 14.01 15.01 16.01 
      11.001 12.001 13.001 14.001 15.001 16.001 17.001 18.001 19.001 20.001 21.001 22.001 23.001 24.001 25.001 26.001
```

Two transformations TF1 and TF2 will be created with the data. The first one (TF1) will used the first and the third transformation matrix and the second one (TF2) will used the second and the fourth transformation matrix.

The file ExternaldData.us can be found on the rc folder of the tutorial Tuto07LoaderForExternalData.

#### Add extern data to the internal application data structure ####

The internal structure of our application presented in step 1 is:

```
<plugin id="tutoLoaderExternalDataConfig">
<extension id="tutoLoaderExternalDataConfig" implements="::fwData::Composite">
<object id="root" type="::fwData::Composite">
    <!--  a ::fwData::Image. -->
    <object id="tutoLoaderExternalDataImage" type="::fwData::Image">
    </object>     
</object>    
</extension>
</plugin> 
```

We have as expected an object to manage the vtk image. Its type is "::fwData::Image". To stocked the new data we are going to create a second object, but with "::fwData::Composite" type. So we obtain the following structure:

```
<plugin id="tutoLoaderExternalDataConfig">
<extension id="tutoLoaderExternalDataConfig" implements="::fwData::Composite">
<object id="root" type="::fwData::Composite">
    <!--  a ::fwData::Image. -->
    <object id="tutoLoaderExternalDataImage" type="::fwData::Image">
    </object>
     
    <!--  a ::fwData::Composite uses to store extra information. -->
    <object id="ExternalData" type="::fwData::Composite">
    </object>
</object>    
</extension>
</plugin> 
```

The new object "::fwData::composite" allows to manage different kind of data, a "composition" of different types, as we need (This kind of object has been used in tutorial 5: TutoMesherso you can have a look on this Tutorial if needed).In our case, we save a string (for the name of the image) and floating number for matrix data. Futhermore, the FW4SPL framework already has a composant (ioXml) to save this kind of data "::fwData::Composite".
Technically, data are saved with FIELD structure, the element can be add in the dynamic way.

**To be noted**: To add a new object in the application, the top level object type must be "::fwData::Composite", here called "root".

#### Realisation ####

In the design point of view, we put all the extern data functionnalities in the menu called "External data". It has three entries:
  * (External Data/Import data file) allows to load the data in application from the imput file (.us).
  * (External data/Save data) allows to save the data in a xml file with the ioXml compoment of the FW4SPL.
  * (External data/load data) allows to load the data from the xml file by using the ioXml compoment.

In the implementation point of view, a new bundle **ioTuto** will be created.

#### Implementation of bundle **ioTuto** ####

To see how to cretae a bundle, see "Tutorial n°6 tutoBundle"
In this tutorial, three services are created, one by functionnality. So we create three classes:
  * ExternalDataRedaerService class, which inherite of IRedaer interface of **FW4SPL**, implements the reading of the data file.
  * CompositeReaderService class, which inherite of IRedaer interface of **FW4SPL**, implements the reading of the xml file.
  * CompositeWriterService class, which inherite of IWriter interface of  **FW4SPL**, implements the writing of the xml file.

At the end, the structure of the new bundle **ioTuto** will be the following:

> ![http://wiki.fw4spl.googlecode.com/hg/Tutorial07Resources/ioTutoBundle.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial07Resources/ioTutoBundle.png)

For this tutorial, we only need to implement the updating() method of each class.

##### Updating method of ExternalDataRedaerService #####

```
    void ExternalDataReaderService::updating() throw(::fwTools::Failed)
    {
        SLM_INFO("[ExternalDataReaderService::update]");
      // Ask the file to read by an IHM. The result will be saved in m_fsExternalDataPath
      // see below for function definition. 
        configureWithIHM();
    
        std::string imageName;
        // Get the data composite to add data on it
        ::fwData::Composite::sptr dataComposite = this->getObject<::fwData::Composite>();
        assert( dataComposite ) ;
        try {
         // 
            if (::boost::filesystem::exists(m_fsExternalDataPath))
            {
                std::fstream file;
          // Open the file
                file.open(m_fsExternalDataPath.native_file_string().c_str(), std::fstream::in);
                if (!file.is_open())
                {
                    OSLM_ERROR( "External data file loading error for " << m_fsExternalDataPath);
                    std::string str = "Unable to open ";
                    str+= m_fsExternalDataPath.native_file_string();
                    throw std::ios_base::failure(str);
                }
                // Read the image name.
                file >> imageName;
                // Read the image name in the Composite map using key = "filename".
                dataComposite->getRefMap()["filename"] = ::fwData::String::NewSptr(imageName);
                 // Clean all the field             
                dataComposite->removeField("TF1");
                dataComposite->removeField("TF2");
                int readedValue = 0;
                double value;
    
                while(!file.eof())
                {
                 // Clean alle the structure
                    readedValue = 0;
                    ::fwData::TransformationMatrix3D::NewSptr transformation1;
                    transformation1->getRefCoefficients().clear();
                    ::fwData::TransformationMatrix3D::NewSptr transformation2;
                    transformation2->getRefCoefficients().clear();
                  while ( !file.eof() && readedValue<32 )
                  {
                      file >> value;
                      if  (readedValue<16)
                      {
                       // Add value in transformation1
                          transformation1->getRefCoefficients().push_back( value );
                      }
                      else
                      {
                       // Add value in transformation2
                          transformation2->getRefCoefficients().push_back( value );
                      }
                      readedValue++;
                  }
                  // TF1 contains the first and third tranformations                    
                  dataComposite->addFieldElement("TF1",transformation1);
                  // TF2 contains the second and fourth transformations
                  dataComposite->addFieldElement("TF2",transformation2);
                }
                assert( readedValue==32 );
                file.close();
            }
        }
        catch(std::ios_base::failure exception)
        {
            OSLM_ERROR( "External data file loading error for " << exception.what());
        }
    }

    void ExternalDataReaderService::configureWithIHM()
    {
        static wxString _sDefaultPath = _("");
        wxString title = _("Choose an external data file");
        wxString folder = wxFileSelector(
                title,
                _sDefaultPath,
                wxT(""),
                wxT(""),
                wxT("External data (*.us)|*.us"),
                wxFD_FILE_MUST_EXIST,
                wxTheApp->GetTopWindow() );
    
        if( folder.IsEmpty() == false)
        {
         // m_fsExternalDataPath is an attribute of ExternalDataReaderService which keeps the file selected with its path 
            m_fsExternalDataPath = ::boost::filesystem::path( wxConvertWX2MB(folder), ::boost::filesystem::native );
            _sDefaultPath = wxConvertMB2WX( m_fsExternalDataPath.branch_path().string().c_str() );
        }
    }
```

##### Updating method of CompositeReaderService #####
```
    void CompositeReaderService::updating( ) throw(::fwTools::Failed)
    {
        configureWithIHM();
        ::fwData::Composite::sptr pComposite = this->getObject< ::fwData::Composite >();
        assert( pComposite );
    
    
        ::fwXML::reader::FwXMLObjectReader myReader;
    
        // Initialiaze the reader with the xml file enter 
        myReader.setFile(m_fsExternalDataPath);
        if (m_fsExternalDataPath != "")
        {
            try
            {
             // Read the file
                myReader.read();
             // Get the data from the reader and store it on composite.
                ::fwData::Composite::sptr newCompo;
                newCompo = ::fwData::Composite::dynamicCast( myReader.getObject() );                
                *pComposite = *newCompo;
                // Copy all the field of composite.                
                std::copy( newCompo->children().begin(),  newCompo->children().end(), std::back_inserter( pComposite->children() ) );
            }
            catch (const std::exception & e)
            {
                std::stringstream ss;
                ss << "Warning during loading : " << e.what();
                wxString wxStmp( ss.str().c_str(), wxConvLocal );
                wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );
            }
            catch( ... )
            {
                std::stringstream ss;
                ss << "Warning during loading : ";
                wxString wxStmp( ss.str().c_str(), wxConvLocal );
                wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );
            }
        }
    }

    void CompositeReaderService::configureWithIHM()
    {
        static wxString _sDefaultPath = _("");
        wxString title = _("Xml file to read");
        wxString folder = wxFileSelector(
                title,
                _sDefaultPath,
                wxT(""),
                wxT(""),
                wxT("xml file (*.xml)|*.xml"),
                 wxFD_FILE_MUST_EXIST,
                wxTheApp->GetTopWindow() );
    
        if( folder.IsEmpty() == false)
        {
         // m_fsExternalDataPath is an attribute of ExternalDataReaderService which keeps the file selected with its path 
            m_fsExternalDataPath = ::boost::filesystem::path( wxConvertWX2MB(folder), ::boost::filesystem::native );
            _sDefaultPath = wxConvertMB2WX( m_fsExternalDataPath.branch_path().string().c_str() );
        }
    }
```

##### Updating method of CompositeWriterService #####

```
    void CompositeWriterService::updating( ) throw(::fwTools::Failed)
    {
        configureWithIHM();
        ::fwData::Composite::sptr pComposite = this->getObject< ::fwData::Composite >();
        assert( pComposite );
    
        // Initialize a fwXml writer. 
        ::fwXML::writer::FwXMLObjectWriter myWriter;
    
        if (m_fsExternalDataPath != "")
        {
          // Initiliaze the writer with the composite to save.
            myWriter.setObject(pComposite);
            // Initialize the writer with the destination file.
            myWriter.setFile(m_fsExternalDataPath);
    
            try
            {
             // Write the data to file.
                myWriter.write();
            }
            catch (const std::exception & e)
            {
                std::stringstream ss;
                ss << "Warning during loading : " << e.what();
                wxString wxStmp( ss.str().c_str(), wxConvLocal );
                wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );
            }
            catch( ... )
            {
                std::stringstream ss;
                ss << "Warning during loading : ";
                wxString wxStmp( ss.str().c_str(), wxConvLocal );
                wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );
            }
        }
    }

    void CompositeWriterService::configureWithIHM()
    {
        static wxString _sDefaultPath = _("");
        wxString title = _("Xml file to save");
        wxString folder = wxFileSelector(
                title,
                _sDefaultPath,
                wxT(""),
                wxT(""),
                wxT("us xml file (*.xml)|*.xml"),
                wxFD_SAVE,
                wxTheApp->GetTopWindow() );
    
        if( folder.IsEmpty() == false)
        {
         // m_fsExternalDataPath is an attribute of ExternalDataReaderService which keeps the file selected with its path         
            m_fsExternalDataPath = ::boost::filesystem::path( wxConvertWX2MB(folder), ::boost::filesystem::native );
            _sDefaultPath = wxConvertMB2WX( m_fsExternalDataPath.branch_path().string().c_str() );
        }
    }
```

#### Configuration file modification ####

At this point the new bundle is ready, so we can add the new functionnalities to our application by modifying the configuration file. The following draw shows the new structure of the application:

> ![http://wiki.fw4spl.googlecode.com/hg/Tutorial07Resources/SchemaOSRTutoLoaderExternalDataImage.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial07Resources/SchemaOSRTutoLoaderExternalDataImage.png)

To do that we use the service that allows to create a generic action. This action will launch (start + update) the service defined in start xml tag.
These services are put in composite object named "ExternalData" because they work on the numeric data.

# File (.us) import action
```
        <!-- The service (::gui::action::StarterActionService) is a generic action. In the configuration of this service,
        we specify the id of the services to be started and updated (start and update methods) when user click on the menuItem "Import External Data ".
        In this tutorial, ::gui::action::StarterActionService service starts and updates the service with the uid "loadExternalData". -->

        <service uid="ActionLoadExternalData" name="Import External data (*.us file)" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" autoComChannel="no">
            <start uid="loadExternalData" />
        </service>
        
        <service implementation="::ioTuto::ExternalDataReaderService" type="::io::IReader" uid="loadExternalData" autoComChannel="no" >
        </service>
```

# Saving to xml file action

```

        <!-- The other service (::gui::action::StarterActionService) is a generic action. In the configuration of this service,
        we specify the id of the services to be started and updated (start and update methods) when user click on the menuItem "save External data".
        In this tutorial, ::gui::action::StarterActionService service starts and updates the service with the uid "saveExternalData". -->

        <service uid="ActionSaveExternalData" name="save External data (*.xml file)" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" autoComChannel="no">
            <start uid="saveExternalData" />
        </service>

        <service implementation="::ioTuto::CompositeWriterService" type="::io::IWriter" uid="saveExternalData" autoComChannel="no" >
        </service>
        
```

# Loading of the xml file action
```

        <!-- The other service (::gui::action::StarterActionService) is a generic action. In the configuration of this service,
        we specify the id of the services to be started and updated (start and update methods) when user click on the menuItem "Load External data".
        In this tutorial, ::gui::action::StarterActionService service starts and updates the service with the uid "loadExternalDataNative". -->

        <service uid="ActionLoadExternalDataNative" name="Load External data (*.xml file)" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" autoComChannel="no">
            <start uid="loadExternalDataNative" />
        </service>

        <service implementation="::ioTuto::CompositeReaderService" type="::io::IReader" uid="loadExternalDataNative" autoComChannel="no" >
        </service>
        
```

To finish, the "External data" menu is created

```
     <!-- This service define the External menu --> 
     <service name="External data" uid="menu_ExternalData" type="::gui::aspect::IMenu" implementation="::gui::aspect::DefaultMenu" autoComChannel="no">
       <action uid="ActionLoadExternalData" />
       <action uid="ActionLoadExternalDataNative"/>
       <action uid="ActionSaveExternalData" />
     </service>

```

#### profile file modification ####

The new bundle **ioTuto** must be activated and start.

```
    <activate id="ioTuto" version="0-1"/>
    <start id="ioTuto"/>  
```

### Validation of the numeric data loading ###

To validate the application, do the followed test:

  * Launch application
  * Load the data file (ExternalData.us) given in the rc directory of tutorial n°7 (menu External data/ Import External Data).
  * Save the file (menu External data/ Save External Data).
  * Edit the file and check that the numerical data are well saved.

### Step 3: Add saving functionnality for all datas (External data + Image) ###

As the previous action, a new entry "Save all data (External data + image)" in the File menu is added.

```
     <!-- This service define the file menu --> 
     <service name="File" uid="menu_File" type="::gui::aspect::IMenu" implementation="::gui::aspect::DefaultMenu" autoComChannel="no">
       <action uid="action_openFile" />
       <action uid="action_SaveFile" />
       <separator />
       <action uid="action_SaveAll" />     <== Add new fonctionnality
       <separator />    
       <action uid="action_quit" />
     </service>   
```

So, we just have to declare a new service as following:

```
        <!-- The other service (::gui::action::StarterActionService) is a generic action. In the configuration of this service,
        we specify the id of the services to be started and updated (start and update methods) when user click on the menuItem "save image".
        In this tutorial, ::gui::action::StarterActionService service starts and updates the service with the uid "saveAllData". -->

        <service uid="action_SaveAll" name="Save all data (External data + image)" type="::gui::action::IAction" implementation="::gui::action::StarterActionService" autoComChannel="no">
      <start uid="saveAllData" />
     </service>

     <service implementation="::ioTuto::CompositeWriterService" type="::io::IWriter" uid="saveAllData" autoComChannel="no">
     </service>

```

The application is done.

## Tuto07LoaderForExternalData application launching ##

To create the application launch:
```
     racy Tuto07LoaderForExternalData
```

To start the program, go in your "MyInstallDir" directory defined at the installation and use the following line:

```
./bin/launcher.exe ./Bundles/Tuto07LoaderForExternalData_0-1/profile-07_LoaderForExternalData.xml
```

## Application validation ##

To validate the application, do the followed test:

  * Launch application
  * Load a vtk image (menu File/Open Image).
  * Load the data file given in the rc directory of tutorial n°7 (menu External data/ Import External Data).
  * Save the file (menu File/Save All data).
  * Edit the file and check that the numerical data and the image are well saved.

  * Data description in the xml file

```
    <Composite id="a6c5ae3e-1e72-47e0-b3a4-d6f29482b3a7" class="::fwData::Composite">
      <element>
        <key>ExternalData</key>
        <value>
          <Composite id="f5b7e311-6218-4f5d-8c57-292197df32c5" class="::fwData::Composite">
            <element>
              <key>filename</key>
              <value>
                <String id="d238300b-58ae-4fb6-8b39-bdfdef9ac888" class="::fwData::String">image1</String>
              </value>
            </element>
            <Field id="33c3ee49-b775-40e5-ae1a-ab0dd17e8c65" class="::fwTools::Field" label="TF1">
              <TransformationMatrix3D id="771c8a41-13ea-4f37-902b-eabdf8232a8a" class="::fwData::TransformationMatrix3D">
                <Coefficients>
                  <count>16</count>
                  <item_version>0</item_version>
                  <item>1.1</item>
                  <item>2.1</item>
                  <item>3.1</item>
                  <item>4.1</item>
                  <item>5.1</item>
                  <item>6.1</item>
                  <item>7.1</item>
                  <item>8.1</item>
                  <item>9.1</item>
                  <item>10.1</item>
                  <item>11.1</item>
                  <item>12.1</item>
                  <item>13.1</item>
                  <item>14.1</item>
                  <item>15.1</item>
                  <item>16.1</item>
                </Coefficients>
              </TransformationMatrix3D>
              <TransformationMatrix3D id="4ebc50f6-85b2-4371-a552-c7f12aaf9318" class="::fwData::TransformationMatrix3D">
                <Coefficients>
                  <count>16</count>
                  <item_version>0</item_version>
                  <item>1.01</item>
                  <item>2.01</item>
                  <item>3.01</item>
                  <item>4.01</item>
                  <item>5.01</item>
                  <item>6.01</item>
                  <item>7.01</item>
                  <item>8.01</item>
                  <item>9.01</item>
                  <item>10.01</item>
                  <item>11.01</item>
                  <item>12.01</item>
                  <item>13.01</item>
                  <item>14.01</item>
                  <item>15.01</item>
                  <item>16.01</item>
                </Coefficients>
              </TransformationMatrix3D>
            </Field>
            <Field id="8c61c826-45b0-4ddf-b7da-d1d47378a0b0" class="::fwTools::Field" label="TF2">
              <TransformationMatrix3D id="cd7974eb-0ebc-4f84-8c8c-e49c72a93321" class="::fwData::TransformationMatrix3D">
                <Coefficients>
                  <count>16</count>
                  <item_version>0</item_version>
                  <item>11.1</item>
                  <item>12.1</item>
                  <item>13.1</item>
                  <item>14.1</item>
                  <item>15.1</item>
                  <item>16.1</item>
                  <item>17.1</item>
                  <item>18.1</item>
                  <item>19.1</item>
                  <item>20.1</item>
                  <item>21.1</item>
                  <item>22.1</item>
                  <item>23.1</item>
                  <item>24.1</item>
                  <item>25.1</item>
                  <item>26.1</item>
                </Coefficients>
              </TransformationMatrix3D>
              <TransformationMatrix3D id="9681b08c-719e-4e03-a35b-4cc7e5aad245" class="::fwData::TransformationMatrix3D">
                <Coefficients>
                  <count>16</count>
                  <item_version>0</item_version>
                  <item>11.001</item>
                  <item>12.001</item>
                  <item>13.001</item>
                  <item>14.001</item>
                  <item>15.001</item>
                  <item>16.001</item>
                  <item>17.001</item>
                  <item>18.001</item>
                  <item>19.001</item>
                  <item>20.001</item>
                  <item>21.001</item>
                  <item>22.001</item>
                  <item>23.001</item>
                  <item>24.001</item>
                  <item>25.001</item>
                  <item>26.001</item>
                </Coefficients>
              </TransformationMatrix3D>
            </Field>
          </Composite>
```

  * Image description in the xml file

```
           <Image id="ceec40df-add1-47e0-b9cf-b7a34b4211da" class="::fwData::Image">
            <Dimension>3</Dimension>
            <PixelType>
              <DynamicType>signed short</DynamicType>
            </PixelType>
            <ManagesBuff>1</ManagesBuff>
            <Spacing>
              <count>3</count>
              <item_version>0</item_version>
              <item>0.57</item>
              <item>0.57</item>
              <item>1.6</item>
            </Spacing>
            <Origin>
              <count>3</count>
              <item_version>0</item_version>
              <item>0</item>
              <item>0</item>
              <item>0</item>
            </Origin>
            <Size>
              <count>3</count>
              <item_version>0</item_version>
              <item>512</item>
              <item>512</item>
              <item>129</item>
            </Size>
            <Filename/>
            <WindowCenter>0</WindowCenter>
            <WindowWidth>0</WindowWidth>
            <RescaleIntercept>0</RescaleIntercept>
            <Buffer filename="Image_ceec40df-add1-47e0-b9cf-b7a34b4211da.vtk" protocol="::vtkIO::ImageWriter"/>
          </Image>           
```

