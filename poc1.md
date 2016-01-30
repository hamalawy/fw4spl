# PoC1 : Integration principle Sofa in FW4SPL #

# Introduction #

## Framework version ##

This document has been created in compatibility with existing frameworks [FW4SPL](http://code.google.com/p/fw4spl/) and [SOFA](http://www.sofa-framework.org/) in the following versions :

| **FW4SPL**  | **SOFA** |
|:------------|:---------|
| fw4spl\_0.7.0 | SOFA 1.0 |

## Objectives of the document "Proof of Concept" ##

The aim of this document is to understand how it is possible to create an application allowing the visualization of a 3D mesh file .trian transformed by Sofa.
Here we won't describe in detail the methods that have been explained in the tutorials FW4SPL.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=XTyOHJy7EmA' target='_blank'><img src='http://img.youtube.com/vi/XTyOHJy7EmA/0.jpg' width='425' height=344 /></a>

![http://wiki.fw4spl.googlecode.com/hg/poc1pic1d.jpg](http://wiki.fw4spl.googlecode.com/hg/poc1pic1d.jpg)


The application consists of a window containing two menus : "Fichier" and "Sofa". The menu « Fichier » allows to load a ".trian" file (tridimensional surface mesh) :


![http://wiki.fw4spl.googlecode.com/hg/poc1pic1a.jpg](http://wiki.fw4spl.googlecode.com/hg/poc1pic1a.jpg)


We choose to open a “.trian” file (for example : bone.trian).


![http://wiki.fw4spl.googlecode.com/hg/poc1pic1b.jpg](http://wiki.fw4spl.googlecode.com/hg/poc1pic1b.jpg)


The picture appears on the screen. In order to show a bone deformation, we have just to click on the Sofa menu "Déformer".


![http://wiki.fw4spl.googlecode.com/hg/poc1pic1c.jpg](http://wiki.fw4spl.googlecode.com/hg/poc1pic1c.jpg)


By choosing the option "Déformer", a new dialog box opens asking the number of pictures per second desired in the animation.


![http://wiki.fw4spl.googlecode.com/hg/poc1pic1e.jpg](http://wiki.fw4spl.googlecode.com/hg/poc1pic1e.jpg)


To create our new bundle "opSofa", we must create the following files by respecting a specified tree :

  * **build.options**
  * **Plugin.xml**
  * **Plugin.cpp** and **Plugin.hpp**
  * **MeshF4S.cpp** and **MeshF4S.hpp**
  * **OglModelF4S.cpp** and **OglModelF4S.hpp**
  * **MVector.hpp**
  * **Config.hpp**
  * **SofaBusiness.cpp** and **SofaBusiness.hpp**
  * **SofaTrianSrv.cpp** and **SofaTrianSrv.hpp**
  * **SofaThread.cpp** and **SofaThread.hpp**

Finally to develop our application, we need to describe its profile and configuration in the files **profileQt.xml** and **config.xml**.

In the subfolder "Sofa", we find all the classes inherited from Sofa and adapted for the FW4SPL framework. The file tree is described in the following diagram :


![http://wiki.fw4spl.googlecode.com/hg/poc1Dessin2.gif](http://wiki.fw4spl.googlecode.com/hg/poc1Dessin2.gif)


## Operating principle ##
### Frameworks interaction ###


The following diagram shows the mechanisms introduced to integrate Sofa in FW4SPL. Our application uses several FW4SPL services including the new communication service with Sofa. L'interaction is used to load a file and to deformate the mesh.


![http://wiki.fw4spl.googlecode.com/hg/poc1Dessin3.gif](http://wiki.fw4spl.googlecode.com/hg/poc1Dessin3.gif)


### Data Sharing ###

Once the .trian file has been loaded in the FW4SPL memory, we create a copy of this file in Sofa, and then we deallocate the used memory in FW4SPL (shown in the diagram below).
Indeed, we use the storage structure of the mesh from Sofa. Sofa stores continuously the mesh in memory while FW4SPL fragments it. To finish, we point the FW4SPL’s storage structure on the Sofa’s data to have a same memory area.


![http://wiki.fw4spl.googlecode.com/hg/poc1Dessin1.gif](http://wiki.fw4spl.googlecode.com/hg/poc1Dessin1.gif)


We will describe firstly the files SofaTrianSrv.cpp and SofaTrianSrv.hpp which contain the code source allowing the transformation of a .trian file by Sofa. Similarly, we describe SofaBusiness.cpp et SofaBusiness.hpp which create the link between FW4SPL and Sofa.  Then we will detail MeshF4S.cpp, OglModelF4S.cpp, and MVector.hpp which are classes that overload Sofa and “std ::vector”. These files are respectively used to load directly a mesh in Sofa without the rereading of a .trian file and to have a common memory space. Finally, we'll describe SofaThread.cpp and SofaTread.hpp allowing to run a thread for Sofa calculations.

We'll also describe the files .xml used for the application development using our "opSofa" bundle, and the "build.options" files used for the compilation.

# Files writing for the bundle #
## Class SofaTrianSrv ##

The files SofaTrianSrv.cpp and SofaTrianSrv.hpp contain the eponym class corresponding to the interfacing service with the Sofa library. This class must contain six methods : **configuring()**, **starting()**, **stopping()**, **updating()**, **updating(message)** and **info()**. The role of these methods and their call conditions are specified in the Description section and in the tutorial n°6 of Google Code weebsite of FW4SPL.

### SofaTrianSrv.hpp ###

Firstly, we must secure our header against intrusions.

```
#ifndef _OPSOFA_ACTION_SOFATRIAN_HPP_
#define _OPSOFA_ACTION_SOFATRIANSRV_HPP_
```

The inclusion of this header allows the processing of macros preceding the name of the class and methods prototypes, **OPSOFA\_CLASS\_API** et **OPSOFA\_API**.

```
#include "config.hpp"
```

The inclusion of this header allows the use of the **SofaBusiness** class.

```
#include "opSofa/SofaBusiness.hpp"
```

The inclusion of this header is necessary to write our derived class from **IActionSrv**.

```
#include <fwGui/IActionSrv.hpp>
```

Conventionally, it is necessary to have an organized code in the namespaces which have the same name as the bundle.

```
namespace opSofa
{

```

We create the class **SofaTrianSrv** derived from **fwGui IActionSrv**. This class allows the link of the service with the window menu.

```
class SofaTrianSrv : public ::fwGui::IActionSrv
{
```

Now, let's move to the public section containing the conventional macro **fwCoreServiceClassDefinitionsMacro**, the constructor and the destructor.

```
public :
    fwCoreServiceClassDefinitionsMacro ( (SofaTrianSrv)(::fwGui::IActionSrv) );

    SofaTrianSrv() throw() ;
    virtual ~SofaTrianSrv() throw() ;
```

The part below is the protected section which contains the 6 basic methods.

```
protected:

    void configuring() throw ( ::fwTools::Failed );
    void starting() throw ( ::fwTools::Failed );
    void stopping() throw ( ::fwTools::Failed );
    void updating() throw ( ::fwTools::Failed );
    void updating( fwServices::ObjectMsg::csptr _pMsg ) throw ( ::fwTools::Failed );
```

Finally, we describe the private section containing only the class attributes, i.e. the object **SofaBusiness**.

```
private :
	std::string m_meshUID;
    SofaBusiness *sofa;
};

} // namespace opSofa

#endif // _OPSOFA_ACTION_SOFATRIANSRV_HPP_
```

### SofaTrianSrv.cpp ###

The inclusion of these headers allows the use of the basic tools of the FW4SPL framework.

```
#include <fwTools/UUID.hpp>
#include <fwData/TriangularMesh.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwComEd/TriangularMeshMsg.hpp>
#include <vtkIO/vtk.hpp>
```

The inclusion of this header allows the use of a Qt dialogue box [Qt](http://qt.nokia.com/products).

```
#include <QInputDialog>
```

Finally, the header file from our class.

```
#include "opSofa/SofaTrianSrv.hpp"
```

We add the macro allowing the association between the service and an object.

```
namespace opSofa
{

REGISTER_SERVICE( ::fwGui::IActionSrv , ::opSofa::action::SofaTrianSrv, ::fwData::Acquisition ) ;
```

Class constructor initializing only the login **m\_meshUID**.

```
SofaTrianSrv::SofaTrianSrv() throw()
{
}
```

Class destructor.

```
SofaTrianSrv::~SofaTrianSrv() throw()
{
}
```

This method is called before starting the service.

```
void SofaTrianSrv::configuring() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC() ;
	this->initialize();

    SLM_ASSERT( "Mesh UID andImage UID must be defined in the service configuration", m_configuration->findConfigurationElement("mesh") );
    m_meshUID = m_configuration->findConfigurationElement("mesh")->getExistingAttributeValue("uid");

    OSLM_INFO( "Mesh UID = " << m_meshUID);
}
```

This method is called when the service starts.

```
void SofaTrianSrv::starting() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
	this->actionServiceStarting();
}
```

This method is called when the service stops.

```
void SofaTrianSrv::stopping() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
	this->actionServiceStopping();
}
```


This method is called whenever the system requires an update of the service, that is, in our case, to each new click on the menu item associated with our service.

```
void SofaTrianSrv::updating() throw ( ::fwTools::Failed )
{
}
```

This method is called when a service reveices a message. At this time, our class is not able to handle any message.

```
void SofaTrianSrv::updating( fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed )
{
    int static etat = 1;

    if (etat == 1) {
        etat++;
        SLM_TRACE_FUNC();

        // Retreive object
        OSLM_ASSERT("Not found the mesh defined by uid : " << m_meshUID, ::fwTools::fwID::exist(m_meshUID)) ;
        ::fwData::TriangularMesh::sptr pMesh = ::fwData::TriangularMesh::dynamicCast(::fwTools::fwID::getObject( m_meshUID )) ;

        int ips = QInputDialog::getDouble(0, "Nombre d'image par seconde", "FPS :", 10, 1, 1000, 0);

        sofa = new SofaBusiness();
        sofa->loadMesh(pMesh, this->getSptr());

        if (ips!=0)
        {    sofa->setTimeStepAnimation(1000/ips); }
        else
        {    sofa->setTimeStepAnimation(1000/10); }

        sofa->startThread();

    } else if (etat == 2) {
        sofa->reset();
    }
}

}// namespace opSofa
```


## Classe SofaBusiness ##

These files describe the class **SofaBusiness** responsible for deformation control in Sofa. It allows the initialization and the data sharing between two frameworks, bu also the control of the tread dedicated to the deformation treatment described in the class **SofaTread**.

### SofaBusiness.hpp ###
Firstly, we protect our headers from multiple inclusions.

```
#ifndef OPSOFA_SOFABUSINESS_HPP_
#define OPSOFA_SOFABUSINESS_HPP_
```

These following inclusions allow to use the Sofa framework inside the class.

```
#include <fwData/PatientDB.hpp>
#include <sofa/component/typedef/Sofa_typedef.h>
#include <sofa/component/visualmodel/OglModel.h>
#include <sofa/component/forcefield/StiffSpringForceField.h>
```

The inclusion of these headers inclusion allows the use of the tools FW4SPL.

```
#include <fwServices/IEditionService.hpp>
#include <fwData/TriangularMesh.hpp>
```

The inclusion of this header allows the use of the classes created by us.

```
#include "opSofa/SofaThread.hpp"
```

The following instructions allow the use of Sofa framework classes without specifying in each space the full namespace.

```
using namespace sofa::simulation::tree;
using sofa::component::visualmodel::OglModel;
using namespace sofa::defaulttype;
```

This instruction is necessary for the use of the class **SofaThread** in **SofaBusiness**.

```
class SofaThread;
```

In the public section, we find firstly the class contructor and destructor.

```
class SofaBusiness
{

public:
    SofaBusiness();
    virtual ~SofaBusiness();
```

Then we declare the two methods allowing to load a scene or a mesh.

```
    void loadMesh(::fwData::TriangularMesh::sptr, ::fwServices::IService::sptr);
```

Then, we declare the accessor and the mutator of the deformation step.

```
    unsigned int getTimeStepAnimation();
    void setTimeStepAnimation(unsigned int timeStepAnimation);
```

And finally, we declare the methods allowing to calculate the mesh deformation and to start, to stop and to restart the deformation.

```
    void animate();
    void startThread();
    void stopThread();
    void reset();
	void shakeMesh(std::string, int);
    void moveMesh(std::string, int, int, int, float, float, float);
```

The private section contains a method serving to translate the FW4SPL mesh pointers to get a common sharing space in memory.

```
private:
    void fillOglModelVector(GNode*, std::vector<OglModel*>*);
    void fillSpringForceField(GNode*, std::map<std::string, StiffSpringForceField3*>*);
    void fillTriangularMeshVector(::fwData::Acquisition::sptr, std::vector<fwData::TriangularMesh::sptr>*);
    void translationPointer(OglModel*, ::fwData::TriangularMesh::sptr);
    void clearTranslationPointer();
```

Il also contains the class attributes. The class **GNode** decribes the physical environment of Sofa, **OglModelF4S** contains the mesh specifications et **timeStepAnimation** contains the deformation step value.

```
	GNode *groot;
    std::vector<fwData::TriangularMesh::sptr> *meshs;
    unsigned int timeStepAnimation;
    SofaThread *thread;
	std::map<std::string, StiffSpringForceField3*> *springs;
};

#endif // OPSOFA_SOFABUSINESS_HPP_
```


### SofaBusiness.cpp ###

Firstly, we declare the necessary inclusions.

```
#include "opSofa/SofaBusiness.hpp"
#include "opSofa/MVector.hpp"
#include "opSofa/SofaThread.hpp"
#include "opSofa/sofa/OglModelF4S.hpp"

#include <sofa/component/init.h>
#include <sofa/simulation/tree/xml/initXml.h>
#include <sofa/simulation/tree/TreeSimulation.h>
#include <sofa/helper/ArgumentParser.h>
#include <sofa/component/contextobject/Gravity.h>
#include <sofa/component/contextobject/CoordinateSystem.h>
#include <sofa/core/objectmodel/Context.h>
#include <sofa/component/odesolver/CGImplicitSolver.h>
#include <sofa/helper/system/FileRepository.h> 
#include <sofa/gui/SofaGUI.h>
#include <sofa/component/typedef/Sofa_typedef.h>
#include <sofa/helper/system/glut.h>
```

The constructor loads the FW4SPL mesh in SOFA and generates all the objects necessary for the deformation calculation.

```
SofaBusiness::SofaBusiness()
{
}

SofaBusiness::~SofaBusiness()
{
    thread->stop();
    clearTranslationPointer();
    delete thread;    
    delete meshs;
    delete springs;
    delete groot;
}
```

Here is the method which allows to load a Mesh.

```
void SofaBusiness::loadMesh(::fwData::TriangularMesh::sptr pMesh,  ::fwServices::IService::sptr service)
{
    // Default value : 100 millisecond
    timeStepAnimation = 100;
    
    // root node (scene)
    groot = new GNode;
    groot->setName( "root" );
    groot->setGravityInWorld( Coord3(0,-10,0) );    // setting gravity

    // create solver (calculates new particles position)
    CGImplicitSolver* solver = new CGImplicitSolver;
    groot->addObject(solver);

    // setting some characteristics of the mesh (coord, speed, etc.)
    MechanicalObject3* DOF = new MechanicalObject3;
    groot->addObject(DOF);
    DOF->resize(4);
    DOF->setName("DOF");
    VecCoord3& x = *DOF->getX(); // setting coord
    x[0] = Coord3(0,10,0);
    x[1] = Coord3(10,0,0);
    x[2] = Coord3(-10*0.5,0,10*0.866);
    x[3] = Coord3(-10*0.5,0,-10*0.866);

    // setting the mass
    UniformMass3* mass = new UniformMass3;
    groot->addObject(mass);
    mass->setMass(2);
    mass->setName("mass");
  
    // setting the mesh (composed of lines, triangles, etc.)
    MeshTopology* topology = new MeshTopology;
    topology->setName("mesh topology");
    groot->addObject( topology );
    topology->addTetra(0,1,2,3);

    // setting constraints
    FixedConstraint3* constraints = new FixedConstraint3;
    constraints->setName("constraints");
    groot->addObject(constraints);
    constraints->addConstraint(0);

    // setting forces
    TetrahedronFEMForceField3* fem = new  TetrahedronFEMForceField3;
    fem->setName("FEM");
    groot->addObject(fem);
    fem->setMethod("polar");
    fem->setUpdateStiffnessMatrix(true);
    fem->setYoungModulus(6);

    // child node (.trian view)
    GNode* skin = new GNode("skin",groot);

    // creating the view of the child node
    OglModelF4S *visual = new OglModelF4S();
    visual->setName( "visual" );
    visual->loadTriangularMesh(pMesh);
    visual->setColor("red");
    visual->applyScale(1);
    skin->addObject(visual);

    // mapping between the 2 objects
    BarycentricMapping3_to_Ext3* mapping = new BarycentricMapping3_to_Ext3(DOF, visual);
    mapping->setName( "mapping" );
    skin->addObject(mapping);

    // scene initialisation
    getSimulation()->init(groot);

    // Create Thread
    meshs = new std::vector<fwData::TriangularMesh::sptr>();
    meshs->push_back(pMesh);
    thread = new SofaThread(this, meshs, service);

    // Translate pointer between sofa and fw4spl
    translationPointer(visual, pMesh);
}
```

Then there are the getters and the setters of the deformation step.

```
unsigned int SofaBusiness::getTimeStepAnimation() 
{
    return timeStepAnimation;
}

void SofaBusiness::setTimeStepAnimation(unsigned int timeStepAnimation) 
{
    groot->setDt((float)timeStepAnimation/(float)1000); // Animation step define
    this->timeStepAnimation = timeStepAnimation;
}
```

Finally, we implement the control functions of the animation and the thread.

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

void SofaBusiness::shakeMesh(std::string idMesh, int value)
{
    if (springs->count(idMesh)) {
        StiffSpringForceField3 *spring = (*springs)[idMesh];

        spring->clear();
        spring->addSpring(1, 153, value, 5, 0);
    }
}

void SofaBusiness::moveMesh(std::string idMesh, int x, int y, int z, float rx, float ry, float rz)
{
    GNode *souris = groot;
    MechanicalObjectRigid3f *mechanical = (MechanicalObjectRigid3f*) (souris->getObject(sofa::core::objectmodel::TClassInfo<MechanicalObjectRigid3f>::get(), idMesh));
    std::string name = mechanical->getName();
    VecCoordRigid3f& coord = *mechanical->getX();
    coord[0][0] = x;
    coord[0][1] = y;
    coord[0][2] = z;
}
```

We set some methods to get objects in a FW4SPL format.

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

void SofaBusiness::fillSpringForceField(GNode *node, std::map<std::string, StiffSpringForceField3*> *springs)
{
   sofa::helper::vector<sofa::core::objectmodel::BaseNode*> gchild = node->getChildren();
   for (unsigned int i=0; i<gchild.size(); i++) {
        GNode *children = node->getChild(gchild[i]->getName());
        StiffSpringForceField3 *spring = (StiffSpringForceField3*) (children->getObject(sofa::core::objectmodel::TClassInfo<StiffSpringForceField3>::get(), ""));
        if (spring != NULL) {
            std::string name = spring->getName();
            (*springs)[name] = spring;
        }
        fillSpringForceField(children, springs);
    }
}

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

        // Get TriangularMesh
        ::fwData::TriangularMesh::sptr mesh = rec->getTriangularMesh();
        boost::filesystem::path path = rec->getPath();
        mesh->setName(organName);
        meshs->push_back(mesh);

        reconstruction++;
    }    
}


```


The last method allows the translation of pointers, through the use of the class **MVector** inherited from the vector.

```
void SofaBusiness::translationPointer(OglModel *visual, ::fwData::TriangularMesh::sptr pMesh)
{
    // Change pointer vertices
    float *verticesSofa = (float*) visual->getVertices()->getData()->data();
    std::vector<std::vector<float > > *verticesF4S = &(pMesh->points());
    int const nbVertices = pMesh->getNumPoints();
    for (int i=0; i<nbVertices; ++i) {
        std::vector<float> *vertex = &((*verticesF4S)[i]);
        MVector<float> *vector = (MVector<float>*) vertex;
        vector->setFront(verticesSofa, 3);
        verticesSofa += 3;
    }

    // Change pointer Triangles
    int *trianglesSofa = (int*) visual->getTriangles()->getData()->data();
    std::vector<std::vector<int > > *trianglesF4S = &(pMesh->cells());
    int const nbTriangles = pMesh->getNumCells();
    for (int i=0; i<nbTriangles; ++i) {
        std::vector<int> *triangleF4S = &((*trianglesF4S)[i]);
        MVector<int> *vector = (MVector<int>*) triangleF4S;
        vector->setFront(trianglesSofa, 3);
        trianglesSofa += 3;
    }
}
```

### Class MVector and MVector.hpp ###

**MVector** class is derived from **vector** class, used for mesh storing in FW4SPL. In order to use a memory space common to both frameworks, it is necessary to be able to point the vector class to the location of our choice. It is therefore necessary to derive the same class to make private pointers accessible via a new public method **setFront()**.

```
#ifndef MVECTOR_H
#define MVECTOR_H

#include <vector>

template<class T> class MVector : public std::vector<T> 
{

public :
```

Here, we define the new method which gives an access to the class pointers.

```
    void setFront(float *ptr, int nbElements) {
        
        this->_Myfirst = ptr;
        this->_Mylast = ptr+nbElements;
        this->_Myend = ptr+nbElements;
    }
};

#endif //MVECTOR_H
```

## Class OglModelF4S and MeshF4S ##

The files **OglModelF4S.cpp** et **OglModelF4S.hpp** contain the eponym class allowing to load the objet **fwData TriangularMesh** from FW4SPL to Sofa without going through the file .trian rereading. This class inherits from the class **OglModel** of Sofa. It contains a method **loadTriangularMesh()** and uses the class **MeshF4S**.

### File OglModelF4S.hpp ###

We protect our header against the multiple inclusions.

```
#ifndef OGLMODELF4S_H
#define OGLMODELF4S_H
```

The inclusion of these headers allows the heritage of **OglModel** from Sofa and the use of **TriangularMesh** of FW4SPL.

```
#include <sofa/component/visualmodel/OglModel.h>
#include <fwData/TriangularMesh.hpp>
```

We create **OglModelF4S** class inheriting from **OglModel**.

```
class OglModelF4S : public sofa::component::visualmodel::OglModel
{
```

We define a public method **loadTriangularMesh()** that will take as argument an object **TriangularMesh**.

```
    public:
    void loadTriangularMesh(::fwData::TriangularMesh::sptr pMesh);

};
#endif
```

### OglModelF4S.cpp ###

The inclusion of these headers allows to use **OglModelF4S** and **MeshF4S**.

```
#include "opSofa/sofa/OglModelF4S.hpp"
#include "opSofa/sofa/MeshF4S.hpp"
```

We define our method **loadTriangulaireMesh()**.

```
void OglModelF4S::loadTriangularMesh(::fwData::TriangularMesh::sptr pMesh)
{
```

We create an object **MeshF4S()**, then we call the **loadTriangularMesh()** method of this same object.

```
    MeshF4S *mesh = new MeshF4S();
    mesh->loadTriangularMesh(pMesh);
```

Finally we create from our object **MeshF4S** a **Mesh** object that we send by the method **setMesh ()**.

```
    sofa::helper::io::Mesh *objLoader = mesh;
    setMesh(*objLoader,false);
    applyUVTransformation();
}
```

### MeshF4S.hpp ###

We protect our header against the multiple inclusions.

```
#ifndef MESHF4S_H
#define MESHF4S_H
```

The inclusion of these headers allows the use of **Mesh** by Sofa and **TriangularMesh** by FW4SPL.

```
#include <sofa/helper/io/Mesh.h>
#include <fwData/TriangularMesh.hpp>
```

We create **MeshF4S** class inheriting from **Mesh** and implement **loadTriangularMesh()** method.

```
class MeshF4S : public sofa::helper::io::Mesh
{

public:
    void loadTriangularMesh(::fwData::TriangularMesh::sptr);

};
#endif
```

### MeshF4S.cpp ###

We include the header from **MeshF4S**.

```
#include "opSofa/sofa/MeshF4S.hpp"
```

We create **loadTriangulaireMesh()** method which loads FW4SPL points and links into Sofa.

```
void MeshF4S::loadTriangularMesh(::fwData::TriangularMesh::sptr pMesh) {
```

We copy the coordonates (x, y, z) of this each point from FW4SPL to SOFA.

```
    int const nbPoints = pMesh->getNumPoints();
    std::vector<std::vector<float>> &fromMesh = pMesh->points();
    vertices.resize(nbPoints);
    for (int p=0;p<nbPoints;p++)
    {
        vertices[p][0] = (SReal)fromMesh[p][0];
        vertices[p][1] = (SReal)fromMesh[p][1];
        vertices[p][2] = (SReal)fromMesh[p][2];
    }
```

We copy the links between the points from FW4SPL to SOFA.

```
    int const nbCells = pMesh->getNumCells();
    std::vector<std::vector<int>> &fromMesh2 = pMesh->cells();
    facets.resize(nbCells);
    for (int f=0;f<nbCells;f++)
    {
        facets[f].resize(3);
        facets[f][0].resize(3);
        facets[f][1].resize(3);
        facets[f][2].resize(3);
        facets[f][0][0] = fromMesh2[f][0];
        facets[f][0][1] = fromMesh2[f][1];
        facets[f][0][2] = fromMesh2[f][2];
    }
}
```

## SofaThread.cpp and SofaThread.hpp ##

The files SofaThread.cpp and SofaThread.hpp contain the eponym class allowing to start a thread dedicated to the calculations of the deformations in Sofa. This class inherits from the class **QThread** of Qt. It contains a constructor and the following methods **run()**, **stop()**, **refreshVtk()**.

### SofaThread.hpp ###

We protect our header against the multiple inclusions.

```
#ifndef OPSOFA_SOFATHREAD_H
#define OPSOFA_SOFATHREAD_H
```

The inclusion of these headers allows the use of **QThread**, **SofaBusiness**, **TriangulaireMeshMsg** et **IEditionService**.

```
#include <QThread>
#include "opSofa/SofaBusiness.hpp"
#include <fwComEd/TriangularMeshMsg.hpp>
#include <fwServices/IEditionService.hpp>
```

We create **SofaThread** class inheriting from **QThread**.

```
class SofaThread : public QThread
{
```

We use **Q\_OBJECT** macro in order to use the Qt slots.

```
    Q_OBJECT
```

We define the public constructor and the method **stop()**.

```
public:
    SofaThread(SofaBusiness*,::fwData::TriangularMesh::sptr,::fwServices::IService::sptr service);
    void stop();
```

We define a signal **refreshvtkRequestSignal()**. The notion of signal and slots is specific to the Qt framework. Indeed, Qt allows to generate signals and to connect them to the declared slot methods. This mechanism allows the communication between two different threads.

```
signals:
    void refreshVtkRequestSignal();
```

We define **refreshVtk()** slot (it will be called by our **refreshvtkRequestSignal()** signal previously created).

```
public slots:
    void refreshVtk();
```

We define the private method run() which corresponds to the executed loop of our thread.

```
private:
    void run();
```

We create our private attributes.

```
    bool stopRun; 
    SofaBusiness *sofa;
    ::fwComEd::TriangularMeshMsg::NewSptr msg;
    ::fwServices::IService::sptr service;
    ::fwData::TriangularMesh::sptr  pMesh;
	QMutex mutex;

};
#endif
```


### SofaThread.cpp ###

The inclusion of these headers allows to use **IEditionService**, **SofaThread** et **QTime**.

```
#include <fwServices/IEditionService.hpp>
#include "opSofa/SofaThread.hpp"
#include <QTime>
```

We create our constructor with following arguments : a pointer to the object **SofaBusiness**, a ponter to **TriangularMesh** and finally a pointer to **iServices**.

```
SofaThread::SofaThread(	SofaBusiness* sofa,	::fwData::TriangularMesh::sptr pMesh, ::fwServices::IService::sptr service)
{
```

We update our attributes.
```
    this->sofa = sofa;
    this->service = service;
    this->pMesh = pMesh;
```

We create a connexion between our signal **refreshVtkRequestSignal()** and **refreshVtk()**. Whenever the signal is emitted, the method **refreshVtk()** is called.

```
    connect(this, SIGNAL(refreshVtkRequestSignal()), this, SLOT(refreshVtk()));
```

We create a new message **TriangularMeshMsg** in order to report to VTK the display refresh.

```
    msg->addEvent( ::fwComEd::TriangularMeshMsg::NEW_MESH );
}
```

Then, we create **run()** method corresponding to the thread.

```
void SofaThread::run() {
```

We initialize **stopRun** to “false”, then we create **QTime** object to clock the thread.

```
    stopRun = false;
    QTime time;
```

We recover the animation step, then we launch the loop that calculates the deformation for the next step.

```
    unsigned int step = sofa->getTimeStepAnimation();
    while(!stopRun) {

        time.start();
        mutex.lock();
        sofa->animate();
```

We generate a signal asking for the refreshment of VTK.

```
        emit refreshVtkRequestSignal();
```

We put the thread in rest.

```
        condition.wait(&mutex);
        mutex.unlock();
		
        unsigned int sec = (unsigned int)time.elapsed();
        if (sec < step) {
        this->msleep((unsigned long)(step-sec));
        }
    }
}
```

Here, we define the Qt slot that sends a message asking VTK to refresh the picture. This method requires **service** and **msg** attributes.

```
void SofaThread::refreshVtk()
{
    mutex.lock();

    // Send message of each mesh at vtk to refresh screen
    int size = meshs->size();
    for (int i=0; i<size; ++i) {
        ::fwServices::IEditionService::notify(service, meshs->at(i), msg);
    }

    // wake thread sofa
    condition.wakeOne();

    mutex.unlock();
}
```

We define **stop()** method to stop the thread by **stopRun**.

```
void SofaThread::stop()
{
    stopRun = true;
}
```

## Files profileQt.xml, plugin.xml and build.options ##

These files are required for the tutorial compilation. The file **config.hpp** declares the macros for the visibility of the classes and the **opSofa** methods. The plug-in files declare the required methods for the proper functioning of the bundle. The implementation of these files follows the same model as those implemented in the tutorial N°6 of FW4SPL (**tutoBundle**).
The **build.options** file requires a few specificities :

```
TYPE    = 'bundle'
VERSION = '0-1'
USE     = ['glut', 'glext', 'qtCore', 'vtkWidgets', 'boost', 'boostFilesystem', 'sofa',
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
          'fwServices_0-1',
          'fwComEd_0-1',
          'fwVtkIO_0-1',
          'fwGui_0-1',
          ]
BUNDLES = ['gui_0-1']
```

This version allows to take into account the Sofa framework when compiling using the “USE” instruction.

## config.hpp ##

```
#ifndef _OPSOFA_CONFIG_HPP_
#define _OPSOFA_CONFIG_HPP_


#ifdef _WIN32

    #ifdef OPSOFA_EXPORTS
    #define OPSOFA_API __declspec(dllexport)
    #else
    #define OPSOFA_API __declspec(dllimport)
    #endif

    #define OPSOFA_CLASS_API

    #pragma warning(disable: 4290)

#elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

    #ifdef OPSOFA_EXPORTS
    #define OPSOFA_API __attribute__ ((visibility("default")))
    #define OPSOFA_CLASS_API __attribute__ ((visibility("default")))
    #else
    #define OPSOFA_API __attribute__ ((visibility("hidden")))
    #define OPSOFA_CLASS_API __attribute__ ((visibility("hidden")))
    #endif

#else

    #define OPSOFA_API
    #define OPSOFA_CLASS_API

#endif

#endif //_OPSOFA_CONFIG_HPP_
```


## plugin.hpp ##

```
#ifndef _OPSOFA_PLUGIN_HPP_
#define _OPSOFA_PLUGIN_HPP_

#include <fwRuntime/Plugin.hpp>


namespace opSofa
{

struct Plugin : public ::fwRuntime::Plugin
{
    ~Plugin() throw();
    void start() throw(::fwRuntime::RuntimeException);
    void stop() throw();
};


} // namespace opSofa


#endif //_OPSOFA_PLUGIN_HPP_
```

## plugin.cpp ##

```
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include "opSofa/Plugin.hpp"

namespace opSofa
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::opSofa::Plugin");


Plugin::~Plugin() throw()
{
}

void Plugin::start() throw( ::fwRuntime::RuntimeException )
{
}

void Plugin::stop() throw()
{
}

} // namespace opSofa
```


# Files writing for the application #

## File build.options ##

The file **build.options** from the application folder requires the addition of the line `'opSofa_0-1'` in the instruction **BUNDLES** in order to compile the application correctly.

```
TYPE    = 'bundle'
VERSION = '0-1'

BUNDLES = [ 
    'io_0-1'                ,
    'gui_0-1'               ,
    'guiQt_0-1'             ,
    'dataReg_0-1'           ,
    'servicesReg_0-1'       ,
    'visu_0-1'              ,
    'visuVTKQt_0-1'         ,
    'launcher_0-1'          ,
    'ioVTK_0-1'             ,
    'ioData_0-1'            ,
    'vtkSimpleMesh_0-1'     ,
    'uiIO_0-1'              ,
    'opVTKMesh_0-1'         ,
    'opSofa_0-1'     		,
 ]
```

## File profileQt.xml ##

We use the “Qt version” of the profiles file, introduced since the 0.7.0 version of the FW4SPL framework. This file contains same classic profile as in the applications developped in the last tutorial of FW4SPL and adds **opSofa** bundle activation. **appXml** is also added to specify the configuration to use.

```
<profile name="DemoComSofa" version="0.1">

    <!-- List of the basic bundles. -->
    <activate id="dataReg" version="0-1"/>
    <activate id="servicesReg" version="0-1"/>
    <activate id="io" version="0-1"/>

    <activate id="visu" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />
    <activate id="visuVTK" version="0-1" />
    <activate id="visuVTKAdaptor" version="0-1" />
   
    <activate id="gui" version="0-1"/>    
    <activate id="guiQt" version="0-1"/>
	
	<activate id="DemoComSofaConfiguration" />
    <activate id="appXml" version="0-1" >
        <param id="config" value="DemoComSofaConfigurationID" />
    </activate>
     
    <!-- List of the specific bundles. -->   
    <activate id="ioData" version="0-1"/>
    <activate id="uiIO" version="0-1" />
```

Declaration of **opSofa** bundle use.

```
    <activate id="opSofa" version="0-1" />
    
    <start id="dataReg" />
    <start id="visuVTKQt" />
    <start id="visuVTKAdaptor" />
    <start id="guiQt" /> 
	<start id="appXml" />
</profile>
```

## File plugin.xml ##

The file **plugin.xml** allows to instance the data and the services used in the application.

```
<plugin id="DemoComSofaConfiguration">
```

Let's start by creating the extension description.

```
    <requirement id="servicesReg" />

	
	<extension implements="::fwServices::registry::AppConfig">
        <id>DemoComSofaConfigurationID</id>
        <type>standard</type>
        <config>
```

We then create the composite visualisation object and the render service.

```
<object uid="CompositeVisu" type="::fwData::Composite">

<service uid="RenderingMesh" implementation="::fwRenderVTK::VtkRenderService" type="::fwRender::IRender" autoComChannel="yes">
                    <scene>
                        <renderer id="default" background="0.0" />
                        <adaptor id="myRenderAdaptor" class="::visuVTKAdaptor::Render" objectId="self">
                            <config renderer="default" picker="" />
                        </adaptor>

                        <adaptor id="MeshAdaptor" class="::visuVTKAdaptor::TriangularMesh" objectId="TrianMesh">
                            <config renderer="default" picker="" />
                        </adaptor>

                    </scene>
                </service>
```

In our case, we are dealing with a triangular mesh.

```
		<!--  Mesh data creation. -->
        <item key="TrianMesh">
        <object uid="myMesh" type="::fwData::TriangularMesh">
```

We configure the main window and an area for the mesh visualization.

```
			<!-- Main window -->
            <service uid="MyIHM" type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no">
                <gui>
                    <frame>
                        <name>Medic@ : Demo integration SOFA-FW4SPL</name>
                        <icon>Bundles/DemoComSofa_0-1/altran.ico</icon>
                        <minSize width="800" height="600" />
                    </frame>
                    <menuBar />
                </gui>
                <registry>
                    <menuBar sid="myMenuBar" start="yes" />
                    <view sid="myDefaultView" start="yes" />
                </registry>
            </service>

			<!-- Visualisation part -->
            <service uid="myDefaultView" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no">
                <gui>
                    <layout type="::fwGui::LineLayoutManager">
                        <orientation value="horizontal" />
                        <view caption="" />
                    </layout>
                </gui>
                <registry>        
                    <view sid="RenderingMesh" start="yes" />
                </registry>
            </service>

			<!-- ****************************************** -->
```

We also define the described menu in the tutorial introduction.

```
            <!-- Menu list -->
            <service uid="myMenuBar" type="::fwGui::IMenuBarSrv" implementation="::gui::aspect::DefaultMenuBarSrv" autoComChannel="no">
                <gui>
                    <layout>
                        <menu name="Fichier" />                     
                        <menu name="Sofa" />                        
                        <menu name="?" />
                    </layout>
                </gui>
                <registry>
                    <menu sid="menuFile" start="yes" />
                    <menu sid="menuSofa" start="yes" />
                    <menu sid="menuAbout" start="yes" />
                </registry>
            </service>

			<!-- ****************************************** -->

			<!-- File menu -->
            <service uid="menuFile" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no">
                <gui>
                    <layout>
                        <menuItem name="Ouvrir"/>
                        <separator />
                        <menuItem name="Quitter" specialAction="QUIT" />
                    </layout>
                </gui>
                <registry>
                    <menuItem sid="actionOpen" start="yes" />
                    <menuItem sid="actionQuit" start="yes" />
                </registry>
            </service>

			<!-- Sofa menu -->
            <service uid="menuSofa" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no">
                <gui>
                    <layout>
                        <menuItem name="Deformer" />
                    </layout>
                </gui>
                <registry>
					<menuItem sid="actionDeform" start="yes" />
                </registry>
            </service>

			<!-- About menu -->
            <service uid="menuAbout" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no">
                <gui>
                    <layout>
                        <menuItem name="A propos" />
                    </layout>
                </gui>
                <registry>
					<menuItem sid="actionAbout" start="yes" />
                </registry>
            </service>

			<!-- ****************************************** -->

			<!-- Open action -->
            <service uid="actionOpen" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
                <start uid="readerPathMeshFile" />
            </service>

			<!-- Quit action -->
            <service uid="actionQuit" type="::fwGui::IActionSrv" implementation="::gui::action::QuitAction" autoComChannel="no" />
```

We relate here the service actionDeform SofaTrianSrv service. The call for this service is therefore performed for each click on the menu "Déformer".

```
			<!-- Deform action which calls SofaTrianSrv -->
			<service uid="actionDeform" type="::fwGui::IActionSrv" implementation="::opSofa::action::SofaTrianSrv" autoComChannel="no">
				<mesh uid="myMesh" />
            </service>

			<!-- About action -->
            <service uid="actionAbout" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
            </service>

    		<!-- ****************************************** -->
```

And finally the reading service which allows the opening of a .trian file (containing the mesh).

```
			<!-- Reading file service -->
            <service implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IDialogEditor" uid="readerPathMeshFile" autoComChannel="no">
                <type mode="reader" />
            </service>

    		<!-- ****************************************** -->

			</object>
			</item>
			
			<!-- Starting the service with UID -->
            <start uid="MyIHM" />

        </object>
	  </config>
    </extension>
</plugin>    
```

## Launching of the application tutoSofa ##

To launch our program, we use this following handle line : :

`./bin/launcher.exe ./Bundles/DemoComSofa_0-1/profileQt.xml`

by placing in the "Install" directory.
