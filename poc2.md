# PoC2 : Principle of integration of Sofa in FW4SPL multi-organs #

# Introduction #

## Frameworks Version ##

This document has been created to be compatible with the following versions of [FW4SPL](http://code.google.com/p/fw4spl/) and [SOFA](http://www.sofa-framework.org/) frameworks:

| **FW4SPL**  | **SOFA** |
|:------------|:---------|
| fw4spl\_0.7.0 | SOFA 1.0 |

## Document intention "Proof of Concept" ##

The aim of the document is to understand how to create an application allowing the 3D view of a fxz file transformed by Sofa.
Remark: In this exercise we will not describe in detail the methods explained in FW4SPL tutorials.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=75eWdsT8Pn8' target='_blank'><img src='http://img.youtube.com/vi/75eWdsT8Pn8/0.jpg' width='425' height=344 /></a>

![http://wiki.fw4spl.googlecode.com/hg/poc2screen.png](http://wiki.fw4spl.googlecode.com/hg/poc2screen.png)


The application consists of a display window. It contains two menus called « File » and « Sofa ». The « File » menu allows to load a .fxz file choosing "Generic fwXML Reader (Ircad)"


![http://wiki.fw4spl.googlecode.com/hg/poc2screen1.png](http://wiki.fw4spl.googlecode.com/hg/poc2screen1.png)

We chose to open a Generic fwXML Reader (Ircad) file.


![http://wiki.fw4spl.googlecode.com/hg/poc2screen2.png](http://wiki.fw4spl.googlecode.com/hg/poc2screen2.png)

We chose to open a patient4liverroot.fxz file.

![http://wiki.fw4spl.googlecode.com/hg/poc2screen3.png](http://wiki.fw4spl.googlecode.com/hg/poc2screen3.png)

The model shows up on the screen. The organ list is displayed on the right side of the window. To visualize the organ deformation, click on the Sofa submenu « Create scene » (To create the new deformation scene) or « Load Scene » (to load a predefined scene). We choose to create a new scene (only selected organs will be integrated to the new scene).

![http://wiki.fw4spl.googlecode.com/hg/poc2screen4.png](http://wiki.fw4spl.googlecode.com/hg/poc2screen4.png)

We choose the destination folder for the new scene.

![http://wiki.fw4spl.googlecode.com/hg/poc2screen5.png](http://wiki.fw4spl.googlecode.com/hg/poc2screen5.png)

We are informed that the scene has been created properly. You need to click on "Yes" to launch the animation.

![http://wiki.fw4spl.googlecode.com/hg/poc2screen6.png](http://wiki.fw4spl.googlecode.com/hg/poc2screen6.png)


To create our opSofa bundle, we must write the following files respecting a defined tree :

  * **build.options**
  * **Plugin.xml**
  * **Plugin.cpp** et **Plugin.hpp**
  * **template.xml**
  * **nodeTemplate.xml**
  * **MVector.hpp**
  * **Config.hpp**
  * **SofaBusiness.cpp** et **SofaBusiness.hpp**
  * **SofaCoreSrv.cpp** et **SofaCoreSrv.hpp**
  * **SofaThread.cpp** et **SofaThread.hpp**
  * **SofaSceneWriterSrv.cpp** et **SofaSceneWriterSrv.hpp**
  * **SofaSceneReaderSrv.cpp** et **SofaSceneReaderSrv.hpp**

Finally, to create our application, we describe the profile and its configuration inside the following files **profileQt.xml** et **config.xml**.

The file arborescence is presented as the following model :

![http://wiki.fw4spl.googlecode.com/hg/poc2Dessin2.gif](http://wiki.fw4spl.googlecode.com/hg/poc2Dessin2.gif)


## Functioning principle ##
### Frameworks interaction ###


The following model presents mechanisms established for Sofa integration in FW4SPL. Our application needs several FW4SPL services including the new comunication service with Sofa. The interaction is used for the loading of a file and for the meshing deformation.


![http://wiki.fw4spl.googlecode.com/hg/poc2Dessin3.gif](http://wiki.fw4spl.googlecode.com/hg/poc2Dessin3.gif)



### Datas share ###

Once FW4SPL has loaded the .fxz file in memory, we make a copy of it in Sofa's memory, then we desallocate the memory used by FW4SPL as described in the following model.
We use the meshing storage structure of Sofa. Sofa stocks in memory the meshing continiously while FW4SPL fragment it in memory. Then we made the FW4SPL data structure point on Sofa datas in order to have a common memory space.


![http://wiki.fw4spl.googlecode.com/hg/poc2Dessin1.gif](http://wiki.fw4spl.googlecode.com/hg/poc2Dessin1.gif)


### Service presentation of the opSofa component ###
The following scheme presents the different services of the opSofa component and how they communicate together.
![http://wiki.fw4spl.googlecode.com/hg/poc2services.gif](http://wiki.fw4spl.googlecode.com/hg/poc2services.gif)


Firstly we decribe SofaCoreSrv.cpp and SofaCoreSrv.hpp files which contain .fxz file transformation service code with Sofa. In the same, we describe SofaBusiness.cpp and SofaBusiness.hpp files which establish the link between FW4SPL and Sofa. Then we detail MVector.hpp which is a class overloading Sofa and std::vector and allowing us to load a mesh in Sofa directly without going through the . fxz file replay and have a common storage space in memory. Finally we describe the files and SofaThread.hpp SofaThread.cpp allowing to run a dedicated thread Sofa calculations.

Then we expose .xml files for the writing application using the opSofa bundle, as well as build.options files necessary for the compilation.

# File writing for the bundle #
## Classe SofaCoreSrv ##

SofaCoreSrv.cpp and SofaCoreSrv.hpp files contain eponymous class corresponding to the interface service with the Sofa library. This class must contain the six methods **configuring()**, **starting()**, **stopping()**, **updating()**, **updating(message)** and **info()**. The role of these methods and the conditions of their respective call are detailed in the Description section and in the tutorial n°6 of the FW4SPL Google code website.

### SofaCoreSrv.hpp ###

First of all,the header must be protected from the multiple inclusions.

```
#ifndef _OPSOFA_SOFACORESRV_HPP_
#define _OPSOFA_SOFACORESRV_HPP_
```

The inclusion of this header allows the treatment of macros preceding the name of the class and the prototype methods, **OPSOFA\_CLASS\_API** and **OPSOFA\_API**.

```
#include "config.hpp"
```

The inclusion of this header allows the use of the class **SofaBusiness**.

```
#include "opSofa/SofaBusiness.hpp"
```

The inclusion of this header is necessary to write our class derivating from **IActionSrv**.

```
#include <fwGui/IActionSrv.hpp>
```

By convention, it is necessary to organize our code in a namespace bearing the name of the bundle.

```
namespace opSofa
{
```

We create the class **SofaCoreSrv** derivating from **fwGui IActionSrv**. This class allows the joining of the service to the window menu.

```
class SofaCoreSrv : public ::fwGui::IActionSrv
{
```

Now turn to the public section containing the traditionnal macro **fwCoreServiceClassDefinitionsMacro** and containing the constructor and the destructor.

```
public :
    fwCoreServiceClassDefinitionsMacro ( (SofaCoreSrv)(::fwGui::IActionSrv) );

    SofaCoreSrv() throw() ;
    virtual ~SofaCoreSrv() throw() ;
```

Ensues the protected section containing the 6 basic methods.

```
protected:
    void configuring() throw ( ::fwTools::Failed );
    void starting() throw ( ::fwTools::Failed );
    void stopping() throw ( ::fwTools::Failed );
    void updating() throw ( ::fwTools::Failed );
    void updating( fwServices::ObjectMsg::csptr _pMsg ) throw ( ::fwTools::Failed );
    void info ( std::ostream &_sstream ) ;
	void addMesh(std::string, std::string);
```

We finally reach the private section containing only an attribute of the class, ie the object **SofaBusiness**.

```
private :
    SofaBusiness *sofa;
};

} // namespace opSofa

#endif // _OPSOFA_SOFACORESRV_HPP_
```

### SofaCoreSrv.cpp ###

The inclusion of these header allows to use the basic tools of the FW4SPL framework.

```
#include <fwTools/UUID.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/IEditionService.hpp>
#include "opSofa/SofaCoreSrv.hpp"
#include <fwData/Acquisition.hpp>
#include <fwData/String.hpp>
#include <fwData/Integer.hpp>
#include <fwData/Float.hpp>
#include <fwData/Vector.hpp>
#include <fwDataIO/reader/TriangularMeshReader.hpp>
#include <QMessageBox>
```

We add the macro allowing the association of a service with an object.

```
namespace opSofa
{

REGISTER_SERVICE( ::fwGui::IActionSrv , ::opSofa::SofaCoreSrv, ::fwData::Acquisition ) ;
```

Constructor of the class just initializing the sofa attribute.

```
SofaCoreSrv::SofaCoreSrv() throw()
{
    sofa = NULL;
}
```

Destructor of the class.

```
SofaCoreSrv::~SofaCoreSrv() throw()
{
    delete sofa;
}
```

This method is called before the service starts.

```
void SofaCoreSrv::configuring() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();    
}
```

This method allows to define the parameters of services and analyze its configuration.
```
void SofaCoreSrv::configuring() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();    
}
```

This method is called when the service starts.

```
void SofaCoreSrv::starting() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}
```

This method is called when the service stops.

```
void SofaCoreSrv::stopping() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}
```

This method is called when the service receives a message.
Each time the message NEW\_SOFA\_SCENE is received, the method recovers the scene file path and the object aquisition to send to the class SofaBusiness. Then starts the thread to launch the animation.

```
void SofaCoreSrv::updating( fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed )
{
    if (msg->hasEvent("NEW_SOFA_SCENE")) {
        // Delete object sofa
        delete sofa;

        // Get Path Scn
        ::fwData::String::csptr pathScn = ::fwData::String::dynamicConstCast(msg->getDataInfo("NEW_SOFA_SCENE"));

        // Get acquisition
        ::fwData::Acquisition::sptr acq = this->getObject< ::fwData::Acquisition >();

        // Create object sofa
        sofa = new SofaBusiness(pathScn->value(), acq, this->getSptr());

        // Apply at sofa the number of image by second
        sofa->setTimeStepAnimation(1000/10);

        // Start thread sofa
        sofa->startThread();
    }
}
```

This method is called each time the system asks an update of the service, ie in our case at each new click on the element of the associted menu to our service.

```
void SofaCoreSrv::updating() throw ( ::fwTools::Failed )
{
}
```

This method gives the information concerning the service.
```
void SofaCoreSrv::info ( std::ostream &_sstream )
{
}
}// namespace opSofa
```

## Class SofaSceneWriterSrv ##

These files describe the service **SofaSceneWriterSrv** which generates a scene file (.scn) from a patient file (.fxz).

### SofaSceneWriterSrv.hpp ###
Firstly we protect our header from the multiple inclusions.

```
#ifndef _opSofa_SOFASCENEWRITERSRV_HPP_
#define _opSofa_SOFASCENEWRITERSRV_HPP_
```

The inclusion of these headers allows the use of tools FW4SPL.

```
#include <fwTools/Failed.hpp>
#include <fwServices/ObjectMsg.hpp>
```

We also include several standard header necessary in the class **SofaSceneWriterSrv**
```
#include <iostream>
#include <io/IWriter.hpp>
```

It's necessary to organize our code in a namespace with the same name than the bundle

```
namespace opSofa
{
```

We create a class SofaSceneWriterSrv which inherits IWriter because it's a writing service.
In the public part, we first describe the class constructor and the class destructor

```
class SofaSceneWriterSrv : public ::io::IWriter
{
    public :       
		fwCoreServiceClassDefinitionsMacro ( (SofaSceneWriterSrv)(::io::IWriter) ) ;
        SofaSceneWriterSrv() throw();
        virtual ~SofaSceneWriterSrv() throw();
```

Following methods are used to set,launch or stop the service , to call it to make an action on the data associated with it or when it receive a message from one ore more datas it listens.

```
        void configuring() throw ( ::fwTools::Failed );
        void starting() throw ( ::fwTools::Failed );
        void stopping() throw ( ::fwTools::Failed );
        void updating() throw ( ::fwTools::Failed );
        void updating( ::fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed );
        void info( std::ostream & ostr );
        void configureWithIHM();
		
	private :
        bool writeTrian;
        std::string m_sceneTemplate;
        bool m_useTempPath;
};
}
```

### SofaSceneWriterSrv.cpp ###

Firstly we note the necessary inclusions.

```
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <fwRuntime/ConfigurationElement.hpp>
#include <fwData/Acquisition.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwData/String.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>
#include <fwCore/spyLog.hpp>
#include <fwDataIO/writer/TriangularMeshWriter.hpp>
#include "opSofa/SofaSceneWriterSrv.hpp"
#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
```

We add the macro allowing the association of a service with an object.

```
namespace opSofa
{
REGISTER_SERVICE( ::io::IWriter, ::opSofa::SofaSceneWriterSrv, ::fwData::Acquisition );

```

Class constructor

```
SofaSceneWriterSrv::SofaSceneWriterSrv() throw()
{
}
```

Class destructor

```
SofaSceneWriterSrv::~SofaSceneWriterSrv() throw()
{
}
```

This method is called before the start of the service.

```
void SofaSceneWriterSrv::configuring() throw ( ::fwTools::Failed )
{
	if(m_configuration->findConfigurationElement("writeTrian"))
    {
        std::string write = m_configuration->findConfigurationElement("writeTrian")->getValue();
        if (write == "yes") {
            writeTrian = true;
        }
    }

    if(m_configuration->findConfigurationElement("sceneTemplate"))
    {
        m_sceneTemplate = m_configuration->findConfigurationElement("sceneTemplate")->getValue();
    }

    if(m_configuration->findConfigurationElement("useTempPath"))
    {
        if (m_configuration->findConfigurationElement("useTempPath")->getValue() == "yes") {
            m_useTempPath = true;
        }
    }
}
```

This method is called at the start of the service.

```
void SofaSceneWriterSrv::starting() throw ( ::fwTools::Failed )
{
}
```

This method is called at the stop of the service.

```
void SofaSceneWriterSrv::stopping() throw ( ::fwTools::Failed )
{
}
```

This method is called each time that the system asks an update of the service, ie in our case at each new click on the element of the  menu coupled with our service.

```
void SofaSceneWriterSrv::updating() throw ( ::fwTools::Failed )
{
```

We ask in which floder the scene file has to be writen.
```
    QString folder;
    if (m_useTempPath) {
        folder = QDir::tempPath() + QDir::separator().toAscii() + "opsofascene";
        QDir dir;
        dir.mkdir(folder);
    } else if (writeTrian) {
        folder = QFileDialog::getExistingDirectory(0, "Choose a folder to write file scene");
    } else {
        folder = QFileDialog::getSaveFileName(0, "Write file scn", QString(), "Scene (*.scn)");
    }
    if (folder == "") return;
```

We recover the object acquisition.
```
    ::fwData::Acquisition::sptr acq = this->getObject< ::fwData::Acquisition >();
```

We recover the templates used to create the scene file.
```
    QString templateFile;
    QString nodeTemplateFile;
    if (m_sceneTemplate != "") {
        QFile file("./Bundles/opSofa_0-1/" + QString(m_sceneTemplate.c_str()));
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        templateFile = file.readAll();
        QFile file2("./Bundles/opSofa_0-1/node" + QString(m_sceneTemplate.c_str()));
        file2.open(QIODevice::ReadOnly | QIODevice::Text);
        nodeTemplateFile = file2.readAll();
    } else {
        QFile file("./Bundles/opSofa_0-1/template.xml");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        templateFile = file.readAll();
        QFile file2("./Bundles/opSofa_0-1/nodeTemplate.xml");
        file2.open(QIODevice::ReadOnly | QIODevice::Text);
        nodeTemplateFile = file2.readAll();
    }
```

We create a string to store our scene
```
    QString nodesData;
```

We go through each reconstruction.
```
    std::pair< ::fwData::Acquisition::ReconstructionIterator, ::fwData::Acquisition::ReconstructionIterator > reconstructionIters = acq->getReconstructions();
    ::fwData::Acquisition::ReconstructionIterator reconstruction = reconstructionIters.first;
    for(; reconstruction != reconstructionIters.second;  ++reconstruction) {
        ::fwData::Reconstruction::sptr rec = (*reconstruction);
```

We recover informations of the object reconstruction.
```
        QString organName = QString(rec->getOrganName().c_str());
        bool organVisible = rec->getIsVisible();
        QString organUid = QString(rec->getID().c_str());
		::boost::filesystem::path filename = "";
```

If the object reconstruction gets the option visible enabled
```
        if (organVisible) {
```

We save the file trian in the folder.
```
            ::fwData::TriangularMesh::sptr mesh = rec->getTriangularMesh();
            std::stringstream meshPath;
            meshPath << folder.toStdString() << QDir::separator().toAscii() << organName.toStdString() << ".trian";
            ::boost::filesystem::path filename = ::boost::filesystem::path(meshPath.str()) ;        
            ::fwDataIO::writer::TriangularMeshWriter writer;
            writer.setObject(mesh);
            writer.setFile(filename);
            writer.write();
```

We add a node to the scene file.
```
            QString nodeFile = nodeTemplateFile;
            nodeFile.replace("ORGAN_NAME", organName);
            nodeFile.replace("ORGAN_ID", organUid);
            nodeFile.replace("TRIAN_LOCATION", QString(filename.string().c_str()));
            nodesData += nodeFile;
        }
    }  
```

We add the nodes to the scene file.
```
    templateFile.replace("AREMPLACER", nodesData);
```

We save the scene file.
```
    std::string scnFile;
    if (writeTrian) {
        scnFile =  folder.toStdString() + QDir::separator().toAscii()  + "sofa.scn";
    } else {
        scnFile =  folder.toStdString();
    }
    std::ofstream fileout (scnFile.c_str(), std::ios::out);
    fileout << templateFile.toStdString();
    fileout.close();
```

We open a dialog box which asks if we want to start the animation.
```
    int answer = QMessageBox::question(0, "Write successful !", "Do you want to launch animation ?", QMessageBox::Yes | QMessageBox::No);
```

If the answer is Yes, then we send a message to the service SofaCoreSrv with the path of the scene file, to make it launch the application.
```
    if (answer == QMessageBox::Yes) {
        ::fwServices::ObjectMsg::NewSptr msg;
        ::fwData::String::sptr data(new ::fwData::String(scnFile));
        msg->addEvent( "NEW_SOFA_SCENE", data );
        ::fwServices::IEditionService::notify(this->getSptr(), acq, msg);
    }
}
```

This method is called when the service receive a message.
```
void SofaSceneWriterSrv::updating( ::fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed )
{
}
```

This method gives the information concerning the service.
```
void SofaSceneWriterSrv::info( std::ostream & ostr )
{
}
```

This method allows to configure IHM.
```
void SofaSceneWriterSrv::configureWithIHM()
{
}

}// namespace opSofa
```

## Class SofaSceneReader ##

These files describe the class **SofaSceneReaderSrv** which allows to read the scen file (.scn)

### SofaSceneReaderSrv.hpp ###
We first protect our header from the multiple inclusions.

```
#ifndef _opSofa_SOFASCENEREADERSRV_HPP_
#define _opSofa_SOFASCENEREADERSRV_HPP_
```

The inclusion of these headers allows the use of the tools FW4SPL.

```
#include <fwTools/Failed.hpp>
#include <fwServices/ObjectMsg.hpp>
```

We also include several necessary standards headers in the class **SofaSceneReaderSrv**
```
#include <iostream>
#include <io/IReader.hpp>
```

It is necessary to organize our code in a namespace with the same name than the bundle

```
namespace opSofa
{
```

In the public part, we first describe the class constructor and class destructor as well as the macro.
Our class inherits of IReader because it is a reading service.

```
class SofaSceneReaderSrv : public ::io::IReader
{
    public :
        fwCoreServiceClassDefinitionsMacro ( (SofaSceneReaderSrv)(::io::IReader) ) ;

        SofaSceneReaderSrv() throw();
        virtual ~SofaSceneReaderSrv() throw();
```

The following methods are used to set, start or stop the service, to call the service to perform an action on the data associated with it or when it receives a message from one or more data it listens.

```
         void configuring() throw ( ::fwTools::Failed );
        void starting() throw ( ::fwTools::Failed );
        void stopping() throw ( ::fwTools::Failed );
        void updating() throw ( ::fwTools::Failed );
        void updating( ::fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed );
        void info( std::ostream & ostr );
        void configureWithIHM();

};
}
#endif // _opSofa_SOFASCENEREADERSRV_HPP_
```

### SofaSceneReaderSrv.cpp ###

First of all we signal the necessary inclusions.

```
#include <sstream>
#include <fwRuntime/ConfigurationElement.hpp>
#include <fwData/String.hpp>
#include <fwData/Acquisition.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>
#include <fwCore/spyLog.hpp>
#include "opSofa/SofaSceneReaderSrv.hpp"
#include <QInputDialog>
#include <QFileDialog>
```

We add the macro allowing the association of a service with an object.

```
namespace opSofa
{

REGISTER_SERVICE( ::io::IReader, ::opSofa::SofaSceneReaderSrv, ::fwData::Acquisition );
```

Class constructor

```
SofaSceneReaderSrv::SofaSceneReaderSrv() throw()
{
}
```

Class destructor.

```
SofaSceneReaderSrv::~SofaSceneReaderSrv() throw()
{
}
```

This method is called before the start of the service.

```
void SofaSceneReaderSrv::configuring() throw ( ::fwTools::Failed )
{
}
```

This method is called at the start of the service.

```
void SofaSceneReaderSrv::starting() throw ( ::fwTools::Failed )
{
}
```

This method is called at the stop of the service.

```
void SofaSceneReaderSrv::stopping() throw ( ::fwTools::Failed )
{
}
```

This method is called whenever the system requires an update of the service, ie in our case at each new click on the element of the menu coupled with our service.
```
void SofaSceneReaderSrv::updating() throw ( ::fwTools::Failed )
{
```

We recover the object acquisition.
```
    ::fwData::Acquisition::sptr acq = this->getObject< ::fwData::Acquisition >();
```

We open a dialog box which asks for the scene file to open.
```
    static QString defaultPath;
    std::string fileScn = QFileDialog::getOpenFileName(0, "Ouvrir un fichier scene", defaultPath, "Scene (*.scn)").toStdString();
    defaultPath = QString::fromStdString(fileScn);
```

If we have selected a scene file then we send it to the service SofaCoreSrv to launch the animation.
```
    if (fileScn != "") {
        ::fwServices::ObjectMsg::NewSptr msg;
        ::fwData::String::sptr data(new ::fwData::String(fileScn));
        msg->addEvent( "NEW_SOFA_SCENE", data );
        ::fwServices::IEditionService::notify(this->getSptr(), acq, msg);
    }
}
```

This method is called when the service receive a message.
```
void SofaSceneReaderSrv::updating( ::fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed )
{}
```

This method gives the information concerning the service.
```
void SofaSceneWriterSrv::info( std::ostream & ostr )
{}
```

This method configures the IHM.
```
void SofaSceneReaderSrv::configureWithIHM()
{
}

}// namespace opSofa
```


## Class SofaBusiness ##

These files describe the class **SofaBusiness** in charge of the control of the deformation by Sofa. It allows the initialization and the sharing of datas between the two frameworks, but also the control of the thread dedicated to the treatment of deformations and described in the class **SofaThread**.

### SofaBusiness.hpp ###
We first protect our header from the multiple inclusions.

```
#ifndef OPSOFA_SOFABUSINESS_HPP_
#define OPSOFA_SOFABUSINESS_HPP_
```

The following inclusions allows the use of the framework Sofa in the class.

```
#include <sofa/component/typedef/Sofa_typedef.h>
#include <sofa/component/visualmodel/OglModel.h>
```

The inclusion of this header allows the use of the tools FW4SPL.

```
#include <fwServices/IEditionService.hpp>
#include <fwData/TriangularMesh.hpp>
#include <fwData/PatientDB.hpp>
```

The inclusion of these headers allows the use of classes we have created.

```
#include "opSofa/SofaThread.hpp"
```

The following instructions allow the use of the classes of the framework Sofa witout specify each time spaces of complete names.

```
using namespace sofa::simulation::tree;
using sofa::component::visualmodel::OglModel;
```

This instruction is necessary to the utilisation of the class **SofaThread** in **SofaBusiness**.

```
class SofaThread;
```

In the public section, we first meet the class contructor and the class destructor.

```
class SofaBusiness
{

public:
    SofaBusiness(std::string, ::fwData::Acquisition::sptr, ::fwServices::IService::sptr);
    virtual ~SofaBusiness();
```

Then we indicate the accessors and mutators of the deformation step.

```
    unsigned int getTimeStepAnimation();
    void setTimeStepAnimation(unsigned int timeStepAnimation);
```

And finally the methods for calculating the deformation of the meshing and start, stop and restart the deformation.

```
    void animate();
    void startThread();
    void stopThread();
    void reset();
```

The private section contains several methods: **fillOglModelVector** for the use of OglModel of Sofa, **fillTriangularMeshVector** for the use of TringularMesh of FW4SPL and **translationPoint** used for the translation of pointers of the meshing FW4SPL to obtain a common memory storage space.

```
private:
    void fillOglModelVector(GNode*, std::vector<OglModel*>*);
    void fillTriangularMeshVector(::fwData::Acquisition::sptr, std::vector<fwData::TriangularMesh::sptr>*);
    void translationPointer(::fwData::TriangularMesh::sptr);
```

It also contains the attributes of the class. The class **GNode** describe the physical environment in Sofa, **meshs** is an array containing every objects TriangularMesh and **timeStepAnimation** contains the value of the deformation step

```
    GNode *groot;
    std::vector<fwData::TriangularMesh::sptr> *meshs;
    unsigned int timeStepAnimation;
    SofaThread *thread;
};

#endif // OPSOFA_SOFABUSINESS_HPP_
```


### SofaBusiness.cpp ###

First of all, we note the necessary inclusions.

```
#include "opSofa/SofaBusiness.hpp"
#include "opSofa/MVector.hpp"
#include <sofa/component/init.h>
#include <sofa/simulation/tree/xml/initXml.h>
#include <sofa/simulation/tree/TreeSimulation.h>
```

The method used loads the meshing FW4SPL in SOFA and generate all the neccesary object to the deformations calculations.

```
SofaBusiness::loadScn(std::string fileScn, ::fwData::Acquisition::sptr acquisition,  ::fwServices::IService::sptr service)
{
    this->timeStepAnimation = 100;
    meshs = new std::vector<fwData::TriangularMesh::sptr>();
```

Sofa initialisation.

```
    sofa::component::init();
    sofa::simulation::tree::xml::initXml();
```

The loading of a scn file in the scene.

```
    groot = dynamic_cast<sofa::simulation::tree::GNode*>( sofa::simulation::tree::getSimulation()->load(fileScn.c_str()));
```

Launching the scene initialization.

```
    getSimulation()->init(groot);
```

We create the vector OglModel.

```
    std::vector<OglModel*> visuals;
    fillOglModelVector(groot, &visuals);
```

We also create the vector TringularMesh

```
    std::vector<fwData::TriangularMesh::sptr> meshsF4s;
    fillTriangularMeshVector(acquisition, &meshsF4s);
```

We add correspondences between mesh sofa and mesh fw4spl.

```
    for (int i=0; i<visuals.size(); ++i) {
        std::string name = visuals[i]->getName();
        for (int j=0; j<meshsF4s.size(); ++j) {
            std::string name2 = meshsF4s[j]->getName();
            if (name == name2) {
                // Add mesh to vector refrech by vtk
                meshs->push_back(meshsF4s[j]);

                // Translate pointer between sofa and fw4spl
                translationPointer(visuals[i], meshsF4s[j]);
            }
        }
	}
```

Then, we create the object **SofaThread** which will host the thread dedicated to the calculation of deformations.

```
    thread = new SofaThread(this, pMesh, service);
```

We define the destructor

```
SofaBusiness::~SofaBusiness()
{
    thread->stop();
    thread->wait(3000);
    delete thread;
    delete groot;
    delete meshs;
}
```

Finally we implement the function control of the animation and of the thread as well as accessors and mutators of the deformation step.

```
void SofaBusiness::animate() 
{
    getSimulation()->animate(groot);
}

void SofaBusiness::startThread()
{
    thread->start();
}

void SofaBusiness::stopThread()
{
    thread->stop();
}

void SofaBusiness::reset()
{
    getSimulation()->reset(groot);
}

void SofaBusiness::setTimeStepAnimation(unsigned int timeStepAnimation) 
{
    groot->setDt((float)timeStepAnimation/(float)1000); // Animation step define
    this->timeStepAnimation = timeStepAnimation;
}

unsigned int SofaBusiness::getTimeStepAnimation() 
{
    return timeStepAnimation;
}
```

The method **fillOglModelVector** allows to get the objects OglModel of Sofa.

```
void SofaBusiness::fillOglModelVector(GNode *node, std::vector<OglModel*> *model) 
{
   sofa::helper::vector<sofa::core::objectmodel::BaseNode*> gchild = node->getChildren();
   for (unsigned int i=0; i<gchild.size(); i++) {
        GNode *children = node->getChild(gchild[i]->getName());
        OglModel *visu = (OglModel*) (children->getObject(sofa::core::objectmodel::TClassInfo<OglModel>::get(), ""));
        if (visu != NULL) {
            model->push_back(visu);
        }
        fillOglModelVector(children, model);
    }
}
```

The method **fillTriangularMeshVector** allows to get the objects TringularMesh and Fw4spl.

```
void SofaBusiness::fillTriangularMeshVector(::fwData::Acquisition::sptr acquisition, std::vector<fwData::TriangularMesh::sptr> *meshs)
{
     std::pair< ::fwData::Acquisition::ReconstructionIterator,
                ::fwData::Acquisition::ReconstructionIterator > 
                reconstructionIters = acquisition->getReconstructions();

    ::fwData::Acquisition::ReconstructionIterator reconstruction = reconstructionIters.first;
    while( reconstruction != reconstructionIters.second )
    {
        ::fwData::Reconstruction::sptr rec = (*reconstruction);

        // Info
        std::string organName = rec->getOrganName();
        bool isVisible = rec->getIsVisible();
        std::string uid = rec->getID();

        OSLM_DEBUG("reconstruction found => name ="<< organName <<" visible = "<< isVisible 
                    << " object id = " << uid );

        // Get TriangularMesh
        ::fwData::TriangularMesh::sptr mesh = rec->getTriangularMesh();
        boost::filesystem::path path = rec->getPath();
        mesh->setName(path.string());
        meshs->push_back(mesh);

        reconstruction++;
    }    
}
```

The last method allows the translation of pointers, through the use of the class **MVector** inherited from the class vector.

```
 void SofaBusiness::translationPointer(OglModel *visual, ::fwData::TriangularMesh::sptr pMesh)

```

It first performs the translation of the coordinates of the points.

```
    float *verticesSofa = (float*) visual->getVertices()->getData()->data();
    std::vector<std::vector<float>> *verticesF4S = &(pMesh->points());
    int const nbVertices = pMesh->getNumPoints();
    for (int i=0; i<nbVertices; ++i) {
        std::vector<float> *vertex = &((*verticesF4S)[i]);
        MVector<float> *vector = (MVector<float>*) vertex;
        vector->setFront(verticesSofa, 3);
        verticesSofa += 3;
    }
```

Then the translation of links between points.
```
    int *trianglesSofa = (int*) visual->getTriangles()->getData()->data();
    std::vector<std::vector<int>> *trianglesF4S = &(pMesh->cells());
    int const nbTriangles = pMesh->getNumCells();
    for (int i=0; i<nbTriangles; ++i) {
        std::vector<int> *triangleF4S = &((*trianglesF4S)[i]);
        MVector<int> *vector = (MVector<int>*) triangleF4S;
        vector->setFront(trianglesSofa, 3);
        trianglesSofa += 3;
    }
}
```

### Class MVector et fichier MVector.hpp ###


The class **MVector** is derived from the class **vector**, used for the storage of the meshing in FW4SPL. To use a common memory space to the two frameworks, It is necessary to be able to point the class vector to a location of our choice. It is therefore necessary to derive the same class to make private pointers accessible via a new public method, **setFront ()**.

```
#ifndef MVECTOR_H
#define MVECTOR_H

#include <vector>

template<class T> class MVector : public std::vector<T> 
{

public :
```

We define the method which gives access to the pointers of the class.

```
    void setFront(float *ptr, int nbElements) {  
        this->_Myfirst = ptr;
        this->_Mylast = ptr+nbElements;
        this->_Myend = ptr+nbElements;
    }
```

We also declare the method allowing the suppression of the link to the pointers.

```
    void clear() {
        this->_Myfirst = NULL;
        this->_Mylast = NULL;
    }
};
#endif //MVECTOR_H
```

## Files SofaThread.cpp and SofaThread.hpp ##

Files SofaThread.cpp and SofaThread.hpp contain the eponymous class which allows to launch a thread dedicated to the calculations of deformation in Sofa. This class inherits of the class **QThread** from Qt. It contains a constructor and methods **run()**, **stop()**, **refreshVtk()**.

### File SofaThread.hpp ###

We protect our header from the multiple inclusions.

```
#ifndef OPSOFA_SOFATHREAD_H
#define OPSOFA_SOFATHREAD_H
```

The inclusion of these headers allows the use of **QThread**, **QMutex**, **QWaitCondition**, **SofaBusiness**, **TriangulaireMeshMsg**, **IEditionService**.

```
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "opSofa/SofaBusiness.hpp"
#include <fwComEd/TriangularMeshMsg.hpp>
#include <fwServices/IEditionService.hpp>
```

We create the class **SofaThread** wich inherites from **QThread**.

```
class SofaThread : public QThread
{
```

We use the macro **Q\_OBJECT** to be able to use the slots of Qt.

```
    Q_OBJECT
```

we explain in public the constructor and the method **stop()**.

```
public:
    SofaThread(SofaBusiness*,::fwData::TriangularMesh::sptr,::fwServices::IService::sptr service);
    void stop();
```

We define the signal **refreshvtkRequestSignal()**. The concept of signal and slots is specific to Qt framework. Qt makes it possible to send signals and connect them to slots declared methods. This mechanism allows communication between two separate threads.

```
signals:
    void refreshVtkRequestSignal();
```

We determine a slot **refreshVtk()** (it will be called by our signal **refreshvtkRequestSignal()** previously created).

```
public slots:
    void refreshVtk();
```

We set in private the method **run()** which corresponds to the loop performed by our thread.

```
private:
    void run();
```

we create our private attributes.

```
    bool stopRun; 
    SofaBusiness *sofa;
    ::fwComEd::TriangularMeshMsg::NewSptr msg;
    ::fwServices::IService::sptr service;
    ::fwData::TriangularMesh::sptr  pMesh;
    QMutex mutex;
    QWaitCondition condition;
};
#endif
```


### File SofaThread.cpp ###

The inclusion of these headers allows the use **IEditionService**, **SofaThread** and **QTime**.

```
#include <fwServices/IEditionService.hpp>
#include <fmTools/UUID.hpp>
#include "opSofa/SofaThread.hpp"
#include <QTime>
```

We create our constructor with a pointer on the object **SofaBusiness** as argument, an other on **TriangularMesh** and finally one on **IService**.

```
SofaThread::SofaThread(SofaBusiness* sofa, std::vector<fwData::TriangularMesh::sptr> *meshs, ::fwServices::IService::sptr service)
{
```

We update our attributes.

```
    this->sofa = sofa;
    this->service = service;
    this->meshs = meshs;
```

We create a connexion between our signal **refreshVtkRequestSignal()** and **refreshVtk()**. Each time the signal is issued, the method **refreshVtk()** is called.

```
    connect(this, SIGNAL(refreshVtkRequestSignal()), this, SLOT(refreshVtk()));
```

We create a message **TriangularMeshMsg** to report to VTK to refresh the display.

```
    msg->addEvent( ::fwComEd::TriangularMeshMsg::NEW_MESH );
}
```

Then we create our method **run()** corresponding to the thread.

```
void SofaThread::run() {
```

We initialize **stopRun** at false then we create an object **QTime** to cloking the thread.

```
    stopRun = false;
    QTime time;
```

We recover the animation step, then we launch the loop calculating at every turn the deformation of the next step.

```
    unsigned int step = sofa->getTimeStepAnimation();
    while(!stopRun) {
        time.start();
        mutex.lock();
        sofa->animate();
```

We issue the signal to request the refreshment of the display of VTK.

```
        emit refreshVtkRequestSignal();
```

We break the thread and we unloack the mutex

```
        condition.wait(&mutex);
        mutex.unlock();
```

We recover the past time then we break the thread until the predefined time step is not elapsed.

```
        unsigned int sec = (unsigned int)time.elapsed();
        if (sec < step) {
            this->msleep((unsigned long)(step-sec));
        }  else {
            this->msleep(1);
        }
    }
}
```

We define the slot Qt which send a message to notify VTK to refresh the image. This method needs attributes **service** and **msg**.

```
void SofaThread::refreshVtk()
{
    int size = meshs->size();
    for (int i=0; i<size; ++i) {
        ::fwServices::IEditionService::notify(service, meshs->at(i), msg);
    }

    condition.wakeOne();
}
```

We define a method **stop()** to stop the thread via **stopRun**.

```
void SofaThread::stop()
{
    stopRun = true;
}
```

## Files config.hpp, Plugin.hpp, Plugin.cpp, plugin.xml and build.options ##

These files are necessary to the compilation of the program. The file **config.hpp** declares the macros for the visibility of classes and methods **opSofa**. The files **plugin** declare methods necessary to the proper functioning of the bundle. The implementation of these files follows the same pattern as those implemented in the tutorial n°6 on FW4SPL, **tutoBundle**.

### File Plugin.hpp ###

We define the necessary inclusions.

```
#ifndef _OPSOFA_PLUGIN_HPP_
#define _OPSOFA_PLUGIN_HPP_

#include <fwRuntime/Plugin.hpp>
```

It is necessary to organize our code in a namespace with the same name of thez bundle.

```
namespace opSofa
{
```

We define the implementation of the plugin for bundle opSofa.

```
struct Plugin : public ::fwRuntime::Plugin
{
    ~Plugin() throw();
    void start() throw(::fwRuntime::RuntimeException);
    void stop() throw();
};
} // namespace opSofa
#endif //_OPSOFA_PLUGIN_HPP_
```

### File Plugin.cpp ###

We define the necessary inclusions
```
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include "opSofa/Plugin.hpp"
```

We describe the constructor
```
namespace opSofa
{
static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::opSofa::Plugin");
```

then the destructor
```
Plugin::~Plugin() throw()
{
}
```

we define methods **start** and **stop**

```
void Plugin::start() throw( ::fwRuntime::RuntimeException )
{
}

void Plugin::stop() throw()
{
}
} // namespace opSofa
```

The file **build.options** needs some specificities :

```
TYPE    = 'bundle'
VERSION = '0-1'
USE     = [
          'boost',
          'boostFilesystem',
          'qtCore', 
          'sofasimulation',
          'sofacomponentodesolver',
          'sofacomponentvisualmodel',
          'sofatree',
          'sofacomponentconstraint',
          'sofacomponentbehaviormodel',
          'sofacomponent',
          'sofahelper'
          ]
LIB     = [
          'fwTools_0-1',
          'fwCore_0-1',
          'fwRuntime_0-3',
          'fwData_0-1',
          'fwDataIO_0-2',
          'fwServices_0-1',
          'fwVtkIO_0-1',
          'fwGui_0-1',
          ]
BUNDLES = ['io_0-1']

```

This version allows to take into account the framework Sofa when compiling, with the help of instruction « USE ».

# Writing files for the application #

## File build.options ##

The file **build.options** of the repertory of the application requires the addition of the line `'opSofa_0-1'` in the instruction **BUNDLES** to compile the application correctly.

```
YPE    = 'bundle'
VERSION = '0-1'

BUNDLES = [
    'launcher_0-1'           ,
    'dataReg_0-1'            ,
    'servicesReg_0-1'        ,
    'gui_0-1'                ,
    'guiWx_0-1'              ,
    'guiQt_0-1'              ,
    'io_0-1'                 ,
    'visu_0-1'               ,
    'ioData_0-1'             ,
    'ioVTK_0-1'              ,
    'ioXML_0-1'              ,
    'uiVisuWx_0-1'           ,
    'uiVisuQt_0-1'           ,
    'uiImageWx_0-1'          ,
    'uiImageQt_0-1'          ,
    'uiReconstructionWx_0-1' ,
    'uiReconstructionQt_0-1' ,
    'uiAcquisitionWx_0-1'    ,
    'uiAcquisitionQt_0-1'    ,
    'uiIO_0-1'               ,
    'visuVTK_0-1'            ,
    'visuVTKWx_0-1'          ,
    'visuVTKQt_0-1'          ,
    'visuVTKAdaptor_0-1'     ,
    'opVTKMesh_0-1'          ,
    'ctrlSelection_0-1'      ,
    'opSofa_0-1'             ,
]
```

## File profileQt.xml ##

We use the version « Qt » of the profils file, set since the version 0.7.0 of the framework FW4SPL. This file contains the classic profil of applications developped in tutorials FW4SPL and add the activation of bundle **opSofa**.

```
<profile name="DemoComSofaMultiOrgan" version="0.1" check-single-instance="true"> 

    <!-- List of the basic bundles. -->
    <activate id="dataReg" version="0-1"/>
    <activate id="servicesReg" version="0-1"/>

    <activate id="gui" version="0-1"/>    
    <activate id="guiQt" version="0-1"/>
	
	<activate id="DemoComSofa" />
	<activate id="appXml" version="0-1">
		<param id="config" value="DemoComSofaID" />
	</activate>
	
    <activate id="io" version="0-1" />
    <activate id="visu" version="0-1" />
    
    <activate id="ctrlSelection" version="0-1" />
    
    <activate id="ioVTK" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />
    <activate id="visuVTKAdaptor" version="0-1" />
      
    <activate id="uiIO" version="0-1" />
    <activate id="uiVisuQt" version="0-1" />
    <activate id="uiImageQt" version="0-1" />
    <activate id="uiReconstructionQt" version="0-1" />
    <activate id="uiAcquisitionQt" version="0-1" />
    
    <activate id="opVTKMesh" version="0-1" />

```

Declaration of the use of bundle **opSofa**.

```
    <activate id="opSofa" version="0-1" />
    
    <start id="visuVTK" />
    <start id="visuVTKQt" />
    <start id="visuVTKAdaptor" />
    <start id="guiQt" />
	<start id="appXml" />
</profile>
```

## File plugin.xml ##

The file **plugin.xml** allows to instanciate the datas and services used in the application.

```
<plugin id="DemoComSofa">
```

Let's begin with the extension description.

```
	<requirement id="servicesReg" />
	<extension implements="::fwServices::registry::AppConfig">
		<id>DemoComSofaID</id>
		<type>standard</type>
		<config>
```


In our case, the data handled is a compsite.

```
    <object type="::fwData::Composite">
```

We configure the main windows and the viewing space of the meshing.

```
    <service uid="MyIHM" implementation="::gui::frame::DefaultFrame" type="::fwGui::IFrameSrv" autoComChannel="no">
        <gui>
            <frame>
                <name>Altran EST - Medic@ : Demo integration SOFA-FW4SPL</name>
                <icon>Bundles/DemoComSofa_0-1/altran.ico</icon>
            </frame>
            <menuBar />
        </gui>
        <registry>
            <menuBar sid="myMenuBar" start="yes" />
            <view sid="multiView_scene1" start="yes" />
        </registry>
    </service>
```

We also determine the menu described in the document introduction.

```
    <service uid="myMenuBar" type="::fwGui::IMenuBarSrv" implementation="::gui::aspect::DefaultMenuBarSrv" autoComChannel="no" >
        <gui>
            <layout>
                <menu name="File" />
                <menu name="Sofa" />
            </layout>
        </gui>
        <registry>
            <menu sid="menu_File" start="yes" />
            <menu sid="menu_Sofa" start="yes" />
        </registry>
    </service>

    <service uid="multiView_scene1" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no">
        <gui>
            <layout type="::fwGui::CardinalLayoutManager" >
                <view align="center" minWidth="200"/>
                <view align="right" minWidth="200" minHeight="200" position="0" />
                <view align="right" minWidth="200" minHeight="80" position="1" />
                <view align="right" minWidth="200" minHeight="200" position="2" />
                <view align="bottom" minHeight="30" resizable="no" />
            </layout>
        </gui>
        <registry>
            <view sid="genericScene" start="yes" />
            <view sid="listOrganEditor" />
            <view sid="multiView_scene1_bottom" start="yes" />
        </registry>
    </service>
	
    <service uid="multiView_scene1_bottom" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no">
        <gui>
            <layout type="::fwGui::LineLayoutManager">
                <orientation value="horizontal" />
                <view proportion="0" minWidth="30" />
                <view proportion="0" minWidth="50" />
                <view proportion="1" />
                <view proportion="0" minWidth="30" />
            </layout>
        </gui>
    </service>
    
    <service uid="menu_File" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no" >
        <gui>
            <layout>
                <menuItem name="Open file" shortcut="Ctrl+O" />
                <menuItem name="Save File" shortcut="Ctrl+S" />
                <separator />
                <menuItem name="Quit" specialAction="QUIT" shortcut="Ctrl+Q" />
            </layout>
        </gui>
        <registry>
            <menuItem sid="action_openFile" start="yes" />
            <menuItem sid="action_saveFile" start="yes" />
            <menuItem sid="action_quit" start="yes" />
        </registry>
    </service>

    <service uid="menu_Sofa" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no" >
        <gui>
            <layout>
                <menuItem name="Save sofa scene" />
                <menuItem name="lancer animation" />
            </layout>
        </gui>
        <registry>
            <menuItem sid="mySofaWriterAction" start="no" />
            <menuItem sid="mySofaStartAction" start="no" />
        </registry>
    </service
	
	<service uid="action_openFile" implementation="::gui::action::StarterActionService" type="::fwGui::IActionSrv"  autoComChannel="no" >
        <start uid="myReaderPathFile"/>
    </service>
    <service uid="action_saveFile" implementation="::gui::action::StarterActionService" type="::fwGui::IActionSrv"  autoComChannel="no" >
        <start uid="myWriterPathFile"/>
    </service>
    <service uid="action_quit" type="::fwGui::IActionSrv" implementation="::gui::action::QuitAction" autoComChannel="no" />	
```

The next step consists in defining an object **myPatientDB** to manage the loaded patient.

```
   <item key="myPatientDB">
        <object uid="myPatientDB" type="::fwData::PatientDB">
            
            <service uid="myReaderPathFile" implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IDialogEditor"  autoComChannel="no" >
                <type mode="reader" />
                <selection mode="exclude" />
                <addSelection service="::ioXML::FwXMLPatientDBReaderService" />
            </service>
            <service uid="myWriterPathFile" implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IDialogEditor"  autoComChannel="no" >
               <type mode="writer"/>
               <selection mode="exclude" />
               <addSelection service="::ioXML::FwXMLPatientDBWriterService" />
            </service>
            <service implementation="::ctrlSelection::wrapper::PatientDBWrapperSrv" type="::ctrlSelection::IWrapperSrv" uid="newOBject2newPatientDB" autoComChannel="yes" />
    
            <service uid="myUpdaterComCannelAcq" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no" >
                <target>myUpdaterAcq</target>
            </service>
            <service uid="myUpdaterComCannelImg" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no" >
                <target>myUpdaterImg</target>
            </service>
            <service uid="myUpdaterComCannelReconst" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no" >
                <target>myUpdaterReconst</target>
            </service>
        </object>
    </item>
```

To use the generic scene, it is necessary to define a composite object **CompositeVisu** in which it is defined.

```
    <!-- CompositeVisu description -->
    <item key="myCompositeVisu">
        <object uid="myCompositeVisu" type="::fwData::Composite">
    
            <service uid="myMedicalImageManager" implementation="::ctrlSelection::manager::MedicalImageManagerSrv" type="::ctrlSelection::IManagerSrv" autoComChannel="yes" priority="0.6">
                <update imageCompositeKey="myImage" />
            </service>
            <service uid="myUpdaterAcq" implementation="::ctrlSelection::updater::AcqFromPDBUpdaterSrv" type="::ctrlSelection::IUpdaterSrv" autoComChannel="no">
                <update compositeKey="myAcquisition" onEvent="NEW_LOADED_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP"/>
                <update compositeKey="myAcquisition" onEvent="NEW_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP"/>
                <update compositeKey="myAcquisition" onEvent="CLEAR_PATIENT" fromUID="myPatientDB" actionType="REMOVE"/>
            </service>
            <service uid="myUpdaterImg" implementation="::ctrlSelection::updater::ImageFromPDBUpdaterSrv" type="::ctrlSelection::IUpdaterSrv" autoComChannel="no">
                <update compositeKey="myImage" onEvent="NEW_LOADED_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP"/>
                <update compositeKey="myImage" onEvent="NEW_PATIENT" fromUID="myPatientDB" actionType="ADD_OR_SWAP"/>
                <update compositeKey="myImage" onEvent="CLEAR_PATIENT" fromUID="myPatientDB" actionType="REMOVE"/>
            </service>
            <service uid="myUpdaterReconst" implementation="::ctrlSelection::updater::ReconstructionFromAcqUpdaterSrv" type="::ctrlSelection::IUpdaterSrv" autoComChannel="no">
                <update compositeKey="myReconstruction" onEvent="NEW_RECONSTRUCTION_SELECTED" fromUID="*" actionType="ADD_OR_SWAP"/>
                <update compositeKey="myReconstruction" onEvent="NEW_PATIENT" fromUID="myPatientDB" actionType="REMOVE_IF_PRESENT"/>
                <update compositeKey="myReconstruction" onEvent="NEW_LOADED_PATIENT" fromUID="myPatientDB" actionType="REMOVE_IF_PRESENT"/>
                <update compositeKey="myReconstruction" onEvent="CLEAR_PATIENT" fromUID="myPatientDB" actionType="REMOVE_IF_PRESENT"/>
            </service>
            
            <service uid="myManager" implementation="::ctrlSelection::manager::SwapperSrv" type="::ctrlSelection::IManagerSrv" autoComChannel="yes">
                <mode type="stop" />
                <config>
                    <object id="myAcquisition" type="::fwData::Acquisition" >
                            
                         <service uid="mySofaCore" implementation="::opSofa::SofaCoreSrv" type="::fwGui::IActionSrv" autoComChannel="yes" >
						 </service>   
							
                         <service uid="mySofaWriter"
                                    implementation="::opSofa::SofaSceneWriterSrv"
                                  type="::io::IWriter" autoComChannel="no" >
                            <writeTrian>no</writeTrian>
                         </service>

                         <service   uid="mySofaWriterAction"
                                    implementation="::gui::action::StarterActionService"
                                    type="::fwGui::IActionSrv" autoComChannel="no" >
                            <start uid="mySofaWriter"/>
                         </service>  
                         
                         <service uid="mySofaReader" implementation="::opSofa::SofaSceneReaderSrv" type="::io::IReader" autoComChannel="no" >
						 </service>

						 <service uid="mySofaReaderAction" implementation="::gui::action::StarterActionService" type="::fwGui::IActionSrv" autoComChannel="no" >
							<start uid="mySofaReader"/>
						 </service> 

                         <service uid="myUpdaterComCannelReconst2" implementation="::fwServices::ComChannelService" type="::fwServices::ICommunication" autoComChannel="no" >
                             <target>myUpdaterReconst</target>
                         </service>
                         <service uid="listOrganEditor" implementation="::uiAcquisition::OrganListEditor" type="::gui::editor::IEditor" autoComChannel="yes" />
                         <service uid="mesher50ServiceUID" implementation="::opVTKMesh::VTKMesherService" type="::opVTKMesh::IMesher" autoComChannel="no" >
                             <percentReduction value="50"/>
                         </service>
                         <service uid="mesher80ServiceUID" implementation="::opVTKMesh::VTKMesherService" type="::opVTKMesh::IMesher" autoComChannel="no" >
                             <percentReduction value="80"/>
                         </service>
                     </object>
                     <object id="myImage" type="::fwData::Image" >
                     </object>
                     <object id="myReconstruction" type="::fwData::Reconstruction" >
                     </object>
                </config>
            </service>
```

The service **genericScene** manage the generic scene. It defines an adaptator, a renderer and a picker.

```
            <service uid="genericScene" implementation="::fwRenderVTK::VtkRenderService" type="::fwRender::IRender" autoComChannel="yes">
                <scene>
                
                    <picker id="myPicker" vtkclass="fwVtkCellPicker" />
                    
                    <renderer id="default" background="0.0" />
                    <adaptor id="myRenderAdaptor" class="::visuVTKAdaptor::Render" objectId="self">
                        <config renderer="default" picker="" />
                    </adaptor>
                     
                    <adaptor id="myAcquisitionAdaptor" class="::visuVTKAdaptor::Acquisition" objectId="myAcquisition">
                        <config renderer="default" picker="" />
                    </adaptor>
                    
                </scene>
            </service>
        </object>
```

We define the order of starting services.

```
    <start uid="MyIHM"/>
    <start uid="myUpdaterAcq"/>
    <start uid="myUpdaterImg"/>
    <start uid="myUpdaterReconst"/>
    <start uid="myMedicalImageManager"/>
    <start uid="myManager"/>
    <start uid="myUpdaterComCannelAcq"/>
    <start uid="myUpdaterComCannelImg"/>
    <start uid="myUpdaterComCannelReconst"/>
    <start uid="newOBject2newPatientDB"/>
```

We end with the closing of marker still open.

```
    </item>
        </object>
		</config>
    </extension>
</plugin>    
```

## Launching of the application tutoSofa ##

To launch our program, we use the following command line:

`./bin/launcher.exe ./Bundles/DemoComSofa_0-1/profileQt.xml`

We place in the directory that we called « Install ».

# Remaining technological barriers #

## Vtk ##
The first technological barrier detected is the performance of VTK. We compared our program with PoC 1 and GLEW used by Sofa for different messhings. Here are the results:
![http://wiki.fw4spl.googlecode.com/hg/poc2performance.png](http://wiki.fw4spl.googlecode.com/hg/poc2performance.png)

These tests have been realized on Windows XP with the following configuration:
  * CPU : Intel Core 2 Duo E8200 @ 2.66GHz
  * Memory : 3Go RAM
  * graphics card : ATI Radeon HD 2400

## Sofa ##
The second technological barrier is the treatment duration realized by Sofa. It could be very important if the scene is complex (many collisions …).