= FW4SPL data in details =

for reminder, the framework is mainly composed by two objects the **service** and the **data**  which are in the center of the object-service concept of the FW4SPL.

![http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FrameworkObject.png](http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FrameworkObject.png)

All objects inherit of fwTools::Object as we can see on the diagram above.

The data used in the FW4SPL are based on fwData::Object class. It is a specialization of fwTools::Object class for the data and adds the dynamic attribut notion.
All the data declaration are stored in   ` <DevFolder>/Src/fw4spl/SrcLib/core/fwData ` .

# The differents kind of data for FW4SPL #

Several kind of data already exist on the framework. Its can be spilt in 4 groups.

## The **basic** data type ##

![http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_BaseData.png](http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_BaseData.png)

| **Data** | **Description** | **File** |
|:---------|:----------------|:---------|
| Boolean  | It is the bool type of C++ associated with FW4SPL fonctionalities. | Boolean.hpp/.cpp |
| Color    | Allows to store a color (RGB + alpha). | Color.hpp/.cpp |
| Composite | Allows to create a complex data type. | Composite.hpp/.cpp |
| DebugObject | This types has a static method which display debug information for object and field. | DebugObject.hpp/.cpp |
| Float    | It is the float type of C++ associated with FW4SPL fonctionalities. | Float.hpp/.cpp |
| GenericField | Allows to add a dynamic field in the composite data type (fwData::Composite). | GenericField.hpp/.cpp |
| Integer  | It is the int type of C++ associated with FW4SPL fonctionalities. | Integer.hpp/.cpp |
| List     | It is the std::list type of STL associated with FW4SPL fonctionalities. | List.hpp/.cpp |
| None     | This type indicates a data which is not present. | None.hpp/.cpp |
| String   | It is the std::string  type of STL associated with FW4SPL fonctionalities. |  String.hpp/.cpp |
| Vector   | It is the std::vector type of STL associated with FW4SPL fonctionalities. | Vector.hpp/.cpp |

> ## The data oriented **location** ##

> ![http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_LocationData.png](http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_LocationData.png)

| **Data** | **Description** | **File** |
|:---------|:----------------|:---------|
| Folder   | Defines a folder location (a path). | Folder.hpp/.cpp |
| ILocation | Defines interface class for classes which deal with location. | ILocation.hpp/.cpp |
| MultiFiles | Define a multi files location (list of files with their path). | MultiFiles.hpp/.cpp |
| SingleFile | Define a file location (file with its path). | SingleFile.hpp/.cpp |

> ## The data oriented **modélisation** ##

> ![http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_ModelisationData.png](http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_ModelisationData.png)

| **Data** | **Description** | **File** |
|:---------|:----------------|:---------|
| Material | Defines a material with an ambient and diffuse coefficient. | Material.hpp/.cpp |
| Spline   | Defines a spline object (Smoothed Polyline). | Spline.hpp/.cpp |
| TriangularMesh | Represents a mesh by a points list and a facets list (each facet is defined by 3 points). | Triangularmesh.hpp/.cpp |

## The data oriented **Geometry** ##

> ![http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_GeometricData.png](http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_GeometricData.png)

| **Data** | **Description**  | **File** |
|:---------|:-----------------|:---------|
| Line     | Creates a line from two points (::fwData::Point). | Line.hpp/.cpp |
| Point    | Defines an 3D point. | Point.hpp/.cpp |
| PointList | Defines a ::fwData::point list. | PointList.hpp/.cpp |
| Plane    | Defines a plane with three points (fwData::Point). | Plane.hpp/.cpp |
| PlaneList | Defines a fwData::Plane list. | PointList.hpp/.cpp |

> ## The data oriented **medicine** ##

![http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_MedicalData.png](http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_MedicalData.png)

| **Data** |  **Description** | **File** |
|:---------|:-----------------|:---------|
| Acquisition | Contains the information of medical examination. | Acquisition.hpp/.cpp |
| Dictionary | Defines a dictionary of ::fwData::DictionaryOrgan data. | Dictionary.hpp/.cpp |
| DictionaryOrgan | Aggregates organ information. | DictionaryOrgan.hpp/.cpp |
| Image    | Defines an image and its parameters (size, spacing, pixel type, ...). | Image.hpp/.cpp |
| Model    | List of couple (::fwData::TraiangularMesh, fwData::Material). | Model.hpp/.cpp |
| Patient  | Defines a medical patient records (name, surname, ID Dicom, sex, size, date of birth, ... ). | Patient.hpp/.cpp |
| PatientDB | Regroups a list of patients. | PatientDB.hpp/.cpp |
| Reconstruction | Regroups organ reconstruction information. | Reconstruction.hpp/.cpp |
| Resection | Defines a resection (Plans de coupe, resection name , ... ). | Resection.hpp/cpp |
| ResectionDB | Stocks a resection list. | ResectionDB.hpp/.cpp |
| Study    | Defines a study. A study holds acquisitions. | Study.hpp/.cpp |
| Tag      | Defines a set of points. | Tag.hpp/.cpp |

## The data oriented **graph** ##

![http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_GraphData.png](http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_GraphData.png)

| **Data** |  **Description** | **File** |
|:---------|:-----------------|:---------|
| Edge     | Defines a graph edge. | Edge.hpp/.cpp |
| Graph    | Defines a graph. | Graph.hpp/.cpp |
| Node     | Defines a graph node. | Node.hpp/.cpp |
| Pipeline | Defines a pipeleine. | Pipeline.hpp/.cpp |
| Port     | Defines a connexion port between a node and an edge. | Port.hpp/.cpp |

## The others types ##

![http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_OtherData.png](http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FwData_OtherData.png)

| **Data** |  **Description**  | **File** |
|:---------|:------------------|:---------|
| Camera   | Defines the camera parameters. | Camera.hpp/.cpp |
| Video    | Defines a video.  | Video.hpp/.cpp |

# Create a new type of data #

To create a new data type in the framwork you must:
  * Define a new class which it inheritates of fwData::Object.
  * Add the new data type to the factory with the REGISTER\_BINDING\_BY\_CLASSNAME() macro in the cpp file of the new data.
