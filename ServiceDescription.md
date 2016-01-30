# Service description in details #

![http://wiki.fw4spl.googlecode.com/hg/FrameworkObject.png](http://wiki.fw4spl.googlecode.com/hg/FrameworkObject.png)

A **service** represents a functionality which use or modify the data. It is always associated with a **data**. Many services can be connected with the same **data**. For example, a service can be a reader or a writer of medical image, a visualization service, a digital image processing operator.

![http://wiki.fw4spl.googlecode.com/hg/conceptDataService.png](http://wiki.fw4spl.googlecode.com/hg/conceptDataService.png)

An user has several methods (**configure**, **start**, **stop**, **update(message)** and **update** ) to interact with a service.

  * **configure** method is used to define the **service** parameters and analyze its configuration. For example, the opening of an image file for a medical image reader service must be done in this method.

  * **start** method is used to launch the **service** (be careful, start and instantiate a service doesn't mean the same thing). For example, for a visualization service, the **start** method attachs container to the **service** to know the window which display the data. This method can be used more than once on the same service.

  * **stop** method is used to stop the **service**. For example, for an image reader service, this method destroys the container which is associate to the **service**. This method can be used more than once on the same service.

  * **update** method is called  to do an action on the data associated to the service. For example, for an image reader service, the service reads the image, converts and saves it to the associated **data**.

  * **update(message)** is called by a data to notify a service (in the fact, the message is sent by **the edition service** associated to the **data** ).  For example, after the reading of input data by an image reader service, the edition service sends a message to the others services associated to this data to inform of the availability of the new image.

## Kind of services ##

![http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FrameworkObject.png](http://wiki.fw4spl.googlecode.com/hg/ClassDiagram_FrameworkObject.png)

All the services are based on the base interface ::fwServices::IService. The FW4SPL framework provides some services. They are divided in many categories that we are presented here

### Input/Output category ###

The Input/Ouput category, stored in the directory **Bundles\LeafIO**, contains all the read/write services for different format files. They are split in several bundles:
  * The **ioData** bundle contains the read/write services for the .Spline file format (file for spline courbe definition), .trf (file for 3D Transformation Matrix data) and .trian (file for traingular mesh data).
  * The **ioTuto** bundle contains the read/write services for the .us file format (text file). It was developped specialy for the Tuto07LoaderForExternalData tutorial.
  * The **ioVTK** bundle contains the read/write services for the .vtk file format. It was used in many tutorials.
  * The **ioXML** bundle contains the read/write services for the .fwXML file format which is the IRCAD file format. It is used to save the ::fwData::PatientDB structure. This bundle is used in Tuto08GenericScene and  Tuto09MesherWithGenericScene.

### Operation category ###

The operator services, stored in directory **Bundles\LeafOP**, are split in two bundles:
  * the **opImageFilter** bundle has an action service which do a threshold on an image.
  * The **opVTKMesh** bundle has services which create a mesh from an acquisition. For that, the services use a contour filter, a smooth filter and a decimate filter define with a reduction parameter. This bundle is used in Tuto09MesherWithGenericScene.

### User interface category ###

It can be found in the directory **Bundles\LeafUI** and regroups all the service managing an interface. Now, Interface are written with wxWidget library.
This category is split by the type of Input/Output data to manage.

  * The bundle **uiImage** manages **image** data. It supplies
    * An editor (SliceListEditor) to manage the display of one or three slices.

![http://wiki.fw4spl.googlecode.com/hg/Service_SliceListEditor.png](http://wiki.fw4spl.googlecode.com/hg/Service_SliceListEditor.png)

  * An editor (SliceIndexPositionEditor) to manage the activate view (Axial, Sagital and Frontal) and the slice to display in this view.

![http://wiki.fw4spl.googlecode.com/hg/Service_SliceIndexPositionEditor.png](http://wiki.fw4spl.googlecode.com/hg/Service_SliceIndexPositionEditor.png)

  * An editor (ShowScanEditor) to display or not the scan.

![http://wiki.fw4spl.googlecode.com/hg/Service_Scan_NoScan.png](http://wiki.fw4spl.googlecode.com/hg/Service_Scan_NoScan.png)

  * The bundle **uiAcquisition** manages **acquisition** data and supplies an editor service to list the reconstruction built from acquisition.

  * The bundle **uiReconstruction** manages **reconstruction** data. It supplies two editors:

  * An editor (RepresentationEditor) to manage the way to draw the reconstruction (like type of shading, type of visualisation (Surface, point, Wireframe, edge)
> > and the element to show (like normals).

![http://wiki.fw4spl.googlecode.com/hg/Service_RepresentationEditor.png](http://wiki.fw4spl.googlecode.com/hg/Service_RepresentationEditor.png)

  * An editor (OrganMaterielEditor) to manage the color and the transparency of the reconstruction

![http://wiki.fw4spl.googlecode.com/hg/Service_OrganMaterielEditor.png](http://wiki.fw4spl.googlecode.com/hg/Service_OrganMaterielEditor.png)

  * The bundle **uiIO** supplies two services, an selector editor to manage the available Input/Output format in an application and a service to export acquisition from a patient (structure ::fwData::PatientDB).

  * The bundle **uiVisu** supplies a service to take a snapshot of current scene.

### Visualization category ###

It can be found in the directory **Bundles\LeafVisu** and regroups all the services allowing to draw data like triangular mesh. This part is mainly based on VTK library.

  * **visuVTKAdaptor** : This bundles contains the list of adaptors available for the generic scene.
  * **vtkCompositeMesh**: This bundle has a service which renders several meshes (several ::fwData::TrainagularMesh data are stored on a ::fwData::Composite).
  * **vtkSimpleMesh**:  This bundle has a service which renders a data which is ::fwData::TrainagularMesh
  * **vtkSimpleNegato**: This bunble has a visualization service of medical image (::fwData::Image).

### Manager/Updater services ###

It can be found in the directory **Bundles\LeafCtrl**.

  * n updater manages a selection composite. An updater is a specific service which listens specific events from specific data. When it receives an event (defined in configuration for example), it adds (or replace=swap) an item in the selection composite, and it notifies the listeners of this composite that few items are added, removed or swapped. IManagers receives these messages. 00Updater exists on the acquisition, reconstruction, resection, selected node and image data.
  * manager manages services on selected items if they are presents. When an IManager receives these messages, it starts, stops or swaps services defines in the manager configuration on concerned items.

### Communication services ###

It allows to establish communication channel.
On each data modification, a message can be send to inform the services that something happens. As the tab below shows, several messages are available for a datum. All the message definition is on ` <DevFolder>/Src/fw4spl/SrcLib/core/fwComEd `

| **Datum** | **Available message** |
|:----------|:----------------------|
| Acquisition | {ADD\_RECONSTRUCTION, VISIBILITY, NEW\_RECONSTRUCTION\_SELECTED} |
| Boolean   | {VALUE\_IS\_MODIFIED} |
| Camera    | {NEW\_CAMERA, CAMERA\_MOVING} |
| Color     | {VALUE\_IS\_MODIFIED} |
| Composite | {MODIFIED\_FIELDS, ADDED\_FIELDS, REMOVED\_FIELDS, SWAPPED\_FIELDS} |
| Float     | {VALUE\_IS\_MODIFIED} |
| Graph     | {NEW\_GRAPH, ADD\_NODE, REMOVE\_NODE, ADD\_EDGE, REMOVE\_EDGE, SELECTED\_NODE, UNSELECTED\_NODE, SELECTED\_OPERATOR, EXECUTE\_GRAPH, CHANGED\_NODE\_STATE} |
| Image     | {NEW\_IMAGE, BUFFER, MODIFIED, DIMENSION, SPACING, REGION, PIXELTYPE, TRANSFERTFUNCTION, WINDOWING, LANDMARK, DISTANCE, NEW\_DISTANCE, DELETE\_DISTANCE, SLICE\_INDEX, ACTIVATE\_SYNC\_CROSS, INACTIVATE\_SYNC\_CROSS, CHANGE\_SLICE\_TYPE, VALUE\_IS\_MODIFIED} |
| Integer   | {VALUE\_IS\_MODIFIED} |
| Interaction | {MOUSE\_LEFT\_UP, MOUSE\_RIGHT\_UP,  MOUSE\_MIDDLE\_UP, MOUSE\_WHEELFORWARD\_UP, MOUSE\_WHEELBACKWARD\_UP, MOUSE\_LEFT\_DOWN, MOUSE\_RIGHT\_DOWN, MOUSE\_MIDDLE\_DOWN, MOUSE\_WHEELFORWARD\_DOWN, MOUSE\_WHEELBACKWARD\_DOWN, MOUSE\_MOVE} |
| Location  | {LOCATION\_IS\_MODIFIED} |
| Material  | {MATERIAL\_IS\_MODIFIED} |
| Model     | {NEW\_MODEL}          |
| PatientDB | {NEW\_PATIENT, ADD\_PATIENT, CLEAR\_PATIENT, NEW\_IMAGE\_SELECTED, NEW\_LOADED\_PATIENT, NEW\_RESECTION\_SELECTED} |
| Patient   | {NEW\_PATIENT, NEW\_MATERIAL\_FOR\_RECONSTRUCTION} |
| PlaneList | {ADD\_PLANE, REMOVE\_PLANE, PLANELIST\_VISIBILITY, PLANELIST\_MODIFIED, DESELECT\_ALL\_PLANES} |
| Plane     | {PLANE\_MODIFIED, START\_PLANE\_INTERACTION, DESELECT\_PLANE, WAS\_SELECTED, WAS\_DESELECTED} |
| PointList | {ELEMENT\_MODIFIED, ELEMENT\_ADDED, ELEMENT\_REMOVED} |
| Point     | {POINT\_IS\_MODIFIED, START\_POINT\_INTERACTION} |
| Reconstruction | {MESH, VISIBILITY}    |
| ResectionDB | {NEW\_RESECTIONDB\_SELECTED, RESECTIONDB\_SELECTED, NEW\_RESECTION\_SELECTED, NEW\_SAFE\_PART\_SELECTED, ADD\_RESECTION, ADD\_SAFE\_PART, MODIFIED, RESECTIONDB\_INVALIDATED} |
| Resection | {ADD\_RECONSTRUCTION, VISIBILITY, NEW\_RECONSTRUCTION\_SELECTED, MODIFIED} |
| Spline    | {NEW\_SPLINE}         |
| String    | {VALUE\_IS\_MODIFIED} |
| Tag       | {TAG\_IS\_MODIFIED}   |
| TransformationMatrix3D | {MATRIX\_IS\_MODIFIED} |
| TriangularMesh | {NEW\_MESH}           |
| Video     | {VIDEO\_IS\_REFRESHED, VIDEO\_IS\_INITIALIZED} |