# Tutorial 10: Tuto10MatrixTransformInGS #

The following table gives the access point to the tutorial for each branch.

| **Branches** | **Tutorial** |
|:-------------|:-------------|
| default      |              |
| fw4spl\_0.4   | N.A          |
| fw4spl\_0.5   | N.A          |
| fw4spl\_0.6   |              |
| fw4spl\_0.7  | See below Tags fw4spl\_0.7.9.6 |

## Introduction ##

This tutorial explains how to use the matrix transformation (rotation , translation, sclaing, ...) in the generic scene.
To do that, we use the FW4SPL object called ::fwData::TransformationMatrix3D. In this example, it will be apply to a mesh.
In this application, we put a cursor to show how to change the matrix coefficient and show the result in real-time in the generic scene.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial10Resources/Tuto10MatrixTransformInGS.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial10Resources/Tuto10MatrixTransformInGS.png)

## OSR Scheme ##

![http://wiki.fw4spl.googlecode.com/hg/Tutorial10Resources/ServiceDataTuto10TechnicalLevel.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial10Resources/ServiceDataTuto10TechnicalLevel.png)

## The profile file ##

In this profile, many of the component are used in the previous tutorials.

  * The basic component **dataReg**, **serviceReg** and **io**
  * The components for displaying **gui** and **guiQt**
  * The components for data reading **ioData** for trian files and **ioVTK** for vtk files.

  * The components for uisng the VTK render in a window **visuVTK** and **visuVTkQt**.
  * The Tuto10MatrixTransformInGS comonent which contains the configuration of the application.
  * The appXml component which specify the configuration file to use. This component must always be started.
  * The visuVTKAdaptor which allow the _transform_ adaptor. More details will be done later.

```

<profile name="tutoDataService" version="0.1">

    <!-- List of basic bundles. -->
    <activate id="dataReg" version="0-1" />
    <activate id="servicesReg" version="0-1" />
    <activate id="io" version="0-1" />
    <activate id="gui" version="0-1" />
    <activate id="visu" version="0-1" />
    
    <activate id="gui" version="0-1" />
    <activate id="guiQt" version="0-1" />

    <activate id="Tuto10MatrixTransformInGS" version="0-1" />
    <activate id="appXml" version="0-1">
        <param id="config" value="tuto" />
    </activate>
    
    <!-- List of specific bundles. -->
    <activate id="ioVTK" version="0-1" />
    <activate id="ioData" version="0-1" />
    <activate id="uiIO" version="0-1" />
    <activate id="uiVisuQt" version="0-1" />
    <activate id="visuVTK" version="0-1" />
    <activate id="visuVTKQt" version="0-1" />
    <activate id="visuVTKAdaptor" version="0-1" />
    
    <start id="dataReg" />
    <start id="visuVTK" />
    <start id="visuVTKQt" />
    <start id="visuVTKAdaptor" />
    <start id="guiQt" />
    <start id="appXml" />
    
</profile>

```

## The configuration file ##

The following picture shows the services use to build the graphical user interface.

![http://wiki.fw4spl.googlecode.com/hg/Tutorial10Resources/Tuto10MatrixTransformInGSAnnoted.png](http://wiki.fw4spl.googlecode.com/hg/Tutorial10Resources/Tuto10MatrixTransformInGSAnnoted.png)

```

<plugin id="Tuto10MatrixTransformInGS">

    <requirement id="servicesReg" />

    <extension implements="::fwServices::registry::AppConfig">
        <id>tuto</id>
        <type>standard</type>
        <config>
        

        <object type="::fwData::Composite" >
        
            <service uid="myFrame" type="::fwGui::IFrameSrv" implementation="::gui::frame::DefaultFrame" autoComChannel="no" >
                <gui>
                    <frame>
                        <name>Tuto10MatrixTransformInGS</name>
                        <icon>Bundles/Tuto10MatrixTransformInGS_0-1/tuto.ico</icon>
                        <minSize width="800" height="600"/>
                    </frame>
                    <menuBar />
                </gui>
                <registry>
                    <menuBar sid="myMenuBar" start="yes" />
                    <view sid="multiView" start="yes" />
                </registry>
            </service>

            <service uid="myMenuBar" type="::fwGui::IMenuBarSrv" implementation="::gui::aspect::DefaultMenuBarSrv" autoComChannel="no" >
                <gui>
                    <layout>
                        <menu name="File"/>
                    </layout>
                </gui>
                <registry>
                    <menu sid="menu_File" start="yes" />
                </registry>
            </service>

            <service uid="menu_File" type="::fwGui::IMenuSrv" implementation="::gui::aspect::DefaultMenuSrv" autoComChannel="no">
                <gui>
                    <layout>
                        <menuItem name="OpenFile" shortcut="Ctrl+O" />
                        <separator />
                        <menuItem name="Quit" shortcut="Ctrl+Q" specialAction="QUIT" />
                    </layout>
                </gui>
                <registry>
                    <menuItem sid="action_openFile" start="yes" />
                    <menuItem sid="action_quit" start="yes" />
                </registry>
            </service>

            <service uid="action_openFile" type="::fwGui::IActionSrv" implementation="::gui::action::StarterActionService" autoComChannel="no">
                <start uid="myReaderPathFile" />
            </service>

            <service uid="action_quit" type="::fwGui::IActionSrv" implementation="::gui::action::QuitAction" autoComChannel="no" />


            <service uid="multiView" type="::gui::view::IView" implementation="::gui::view::DefaultView" autoComChannel="no" >
                <gui>
                    <layout type="::fwGui::CardinalLayoutManager" >
                        <view align="center" />
                        <view align="bottom" minHeight="40" position="0" />
                    </layout>
                </gui>
                <registry>
                    <view sid="genericScene" start="yes" />
                    <view sid="matrixEditor" start="yes" />
                </registry>
            </service>

            <service uid="genericScene" implementation="::fwRenderVTK::VtkRenderService" type="::fwRender::IRender" autoComChannel="yes">
                <scene>

                    <renderer id="default" background="0.0" />

                    <picker id="myPicker" vtkclass="fwVtkCellPicker" />

                    <vtkObject id="mat1" class="vtkTransform" />
                    <vtkObject id="mat2" class="vtkTransform" />
                    <vtkObject id="mat3" class="vtkTransform" />
                    <vtkObject id="mat4" class="vtkTransform" />
                    <vtkObject id="mat5" class="vtkTransform" />
                    
                    <!-- mat5 = mat5 x mat1 x mat4 -->
                    <vtkObject id="mat6" class="vtkTransform" >
                        <vtkTransform>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat1</concatenate>
                            <concatenate>mat4</concatenate>
                        </vtkTransform>
                    </vtkObject>
                    
                    <!-- mat6 = mat5 x mat2 x mat5 x mat5 x mat5 x mat4 x mat4 -->
                    <vtkObject id="mat7" class="vtkTransform" >
                        <vtkTransform>
                            <concatenate inverse="yes">mat5</concatenate>
                            <concatenate>mat2</concatenate>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat4</concatenate>
                            <concatenate>mat4</concatenate>
                        </vtkTransform>
                    </vtkObject>
                    
                    <!-- mat7 = mat3 x mat5 x mat4 x mat4 x mat4 -->
                    <vtkObject id="mat8" class="vtkTransform" >
                        <vtkTransform>
                            <concatenate>mat3</concatenate>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat4</concatenate>
                            <concatenate>mat4</concatenate>
                            <concatenate>mat4</concatenate>
                        </vtkTransform>
                    </vtkObject>

                    <adaptor id="MatrixAdatorA" class="::visuVTKAdaptor::Transform" objectId="matrixA">
                        <config renderer="default" picker="" transform="mat1" />
                    </adaptor>

                    <adaptor id="MatrixAdatorB" class="::visuVTKAdaptor::Transform" objectId="matrixB">
                        <config renderer="default" picker="" transform="mat2" />
                    </adaptor>
                    
                    <adaptor id="MatrixAdatorC" class="::visuVTKAdaptor::Transform" objectId="matrixC">
                        <config renderer="default" picker="" transform="mat3" />
                    </adaptor>
                    
                    <adaptor id="MatrixAdatorD" class="::visuVTKAdaptor::Transform" objectId="matrixD">
                        <config renderer="default" picker="" transform="mat4" />
                    </adaptor>
                    
                    <adaptor id="MatrixAdatorE" class="::visuVTKAdaptor::Transform" objectId="matrixE">
                        <config renderer="default" picker="" transform="mat5" />
                    </adaptor>
                    
                    <adaptor id="Renderer" class="::visuVTKAdaptor::Render" objectId="self">
                        <config renderer="default" picker="" />
                    </adaptor>

                    <adaptor id="TMAdaptor1" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
                        <config renderer="default" picker="" />
                    </adaptor>

                    <adaptor id="TMAdaptor2" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
                        <config renderer="default" picker="" transform="mat6" />
                    </adaptor>
                    
                    <adaptor id="TMAdaptor3" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
                        <config renderer="default" picker="" transform="mat7" />
                    </adaptor>
                    
                    <adaptor id="TMAdaptor4" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
                        <config renderer="default" picker="" transform="mat8" />
                    </adaptor>

                </scene>
            </service>
            
            <item key="matrixA">
                <object uid="matrixA" type="::fwData::TransformationMatrix3D">
                    <matrix>
                        1 0 0 2
                        0 1 0 0
                        0 0 1 0
                        0 0 0 1 
                    </matrix>
                </object>
            </item>
            
            <item key="matrixB">
                <object uid="matrixB" type="::fwData::TransformationMatrix3D">
                    <matrix>
                        1 0 0 4
                        0 1 0 0
                        0 0 1 0
                        0 0 0 1 
                    </matrix>
                </object>
            </item>
            
            <item key="matrixC">
                <object uid="matrixC" type="::fwData::TransformationMatrix3D">
                    <matrix>
                        1 0 0 0
                        0 1 0 0
                        0 0 1 2
                        0 0 0 1 
                    </matrix>
                </object>
            </item>
            
            <item key="matrixD">
                <object uid="matrixD" type="::fwData::TransformationMatrix3D">
                    <matrix>
                        0.75 0 0 0
                        0 0.75 0 0
                        0 0 0.75 0
                        0 0 0 1 
                    </matrix>
                </object>
            </item>
            
            <item key="matrixE">
                <object uid="matrixE" type="::fwData::TransformationMatrix3D">
                    <service uid="matrixEditor" implementation="::uiVisu::TransformationMatrixEditor" type="::gui::editor::IEditor" autoComChannel="yes" />
                </object>
            </item>

            <item key="mesh">
                <object uid="mesh" type="::fwData::TriangularMesh">
                    <service uid="myReaderPathFile" implementation="::uiIO::editor::IOSelectorService" type="::gui::editor::IDialogEditor" autoComChannel="no" />
                </object>
            </item>
            
            <start uid="myFrame" />
        </object>
    
        </config>
    </extension>
</plugin>

```

This configuration is simple and similar to those already seen in previous tutorials. We will therefore focus primarily on two new points:

  * _The declaration of transformation matrix_:

FW4SPL provides a data structure called "TransformationMatrix3D" for storing a transform (rotation, translation, scaling ...).
It can be used in the same way as other types of data FW4SPL (image, mesh, ...). It can be initialized during its declaration.
By default it is initialized with the identity matrix.

In this tutorial, 5 transformations were defined, four have been initialized, the latter being initiated by the service associated with it.

```
            <!-- This matrix does a translation on x  -->
            <item key="matrixA">
                <object uid="matrixA" type="::fwData::TransformationMatrix3D">
                    <matrix>
                        1 0 0 2
                        0 1 0 0
                        0 0 1 0
                        0 0 0 1 
                    </matrix>
                </object>
            </item>
            
            <!-- This matrix does a translation on x  -->
            <item key="matrixB">
                <object uid="matrixB" type="::fwData::TransformationMatrix3D">
                    <matrix>
                        1 0 0 4
                        0 1 0 0
                        0 0 1 0
                        0 0 0 1 
                    </matrix>
                </object>
            </item>
            
            <!-- This matrix does a translation on z  -->
            <item key="matrixC">
                <object uid="matrixC" type="::fwData::TransformationMatrix3D">
                    <matrix>
                        1 0 0 0
                        0 1 0 0
                        0 0 1 2
                        0 0 0 1 
                    </matrix>
                </object>
            </item>

            <!-- This matrix does a rotation -->            
            <item key="matrixD">
                <object uid="matrixD" type="::fwData::TransformationMatrix3D">
                    <matrix>
                        0.75 0 0 0
                        0 0.75 0 0
                        0 0 0.75 0
                        0 0 0 1 
                    </matrix>
                </object>
            </item>
```

As with other types of data, it is also possible to associate services for manage it. For example

```
            <!-- This matrix is initialized with the identity matrix and modify by the editor --> 
            <item key="matrixE">
                <object uid="matrixE" type="::fwData::TransformationMatrix3D">
                    <service uid="matrixEditor" implementation="::uiVisu::TransformationMatrixEditor" type="::gui::editor::IEditor" autoComChannel="yes" />
                </object>
            </item>

```

The ::uiVisu::TransformationMatrixEditor service is a cursor widget to fix the value of an angle between [0�, 360�]. Then the transformation matrix is calculated by the service and stored in the matrixE.

  * _The configuration of the generic scene corresponding to the service whose uid "genericScene"_
It shows how to define the transformation matrix. We can identify the following steps:

  * Declaration of matrix for vtk.
  * Declaration of the adaptor to make the connection between the FW4SPL world and the VTK world (association of vtk matrix with FW4SPL matrix).
  * Declaration of the adaptor which use to update the display in he generic scene (The specifed trasformation matrix has been applied).

```
            <service uid="genericScene" implementation="::fwRenderVTK::VtkRenderService" type="::fwRender::IRender" autoComChannel="yes">
                <scene>

                    <renderer id="default" background="0.0" />

                    <picker id="myPicker" vtkclass="fwVtkCellPicker" />

                    <!-- Declaration -->
                    <vtkObject id="mat1" class="vtkTransform" />
                    <vtkObject id="mat2" class="vtkTransform" />
                    <vtkObject id="mat3" class="vtkTransform" />
                    <vtkObject id="mat4" class="vtkTransform" />
                    <vtkObject id="mat5" class="vtkTransform" />
                    
                    <!-- mat6 = mat5 x mat1 x mat4 -->
                    <vtkObject id="mat6" class="vtkTransform" >
                        <vtkTransform>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat1</concatenate>
                            <concatenate>mat4</concatenate>
                        </vtkTransform>
                    </vtkObject>
                    
                    <!-- mat7 = inv(mat5)  x mat2 x mat5 x mat5 x mat5 x mat4 x mat4 -->
                    <vtkObject id="mat7" class="vtkTransform" >
                        <vtkTransform>
                            <concatenate inverse="yes">mat5</concatenate>
                            <concatenate>mat2</concatenate>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat4</concatenate>
                            <concatenate>mat4</concatenate>
                        </vtkTransform>
                    </vtkObject>
                    
                    <!-- mat8 = mat3 x mat5 x mat4 x mat4 x mat4 -->
                    <vtkObject id="mat8" class="vtkTransform" >
                        <vtkTransform>
                            <concatenate>mat3</concatenate>
                            <concatenate>mat5</concatenate>
                            <concatenate>mat4</concatenate>
                            <concatenate>mat4</concatenate>
                            <concatenate>mat4</concatenate>
                        </vtkTransform>
                    </vtkObject>

                    <adaptor id="MatrixAdatorA" class="::visuVTKAdaptor::Transform" objectId="matrixA">
                        <config renderer="default" picker="" transform="mat1" />
                    </adaptor>

                    <adaptor id="MatrixAdatorB" class="::visuVTKAdaptor::Transform" objectId="matrixB">
                        <config renderer="default" picker="" transform="mat2" />
                    </adaptor>
                    
                    <adaptor id="MatrixAdatorC" class="::visuVTKAdaptor::Transform" objectId="matrixC">
                        <config renderer="default" picker="" transform="mat3" />
                    </adaptor>
                    
                    <adaptor id="MatrixAdatorD" class="::visuVTKAdaptor::Transform" objectId="matrixD">
                        <config renderer="default" picker="" transform="mat4" />
                    </adaptor>
                    
                    <adaptor id="MatrixAdatorE" class="::visuVTKAdaptor::Transform" objectId="matrixE">
                        <config renderer="default" picker="" transform="mat5" />
                    </adaptor>
                    
                    <adaptor id="Renderer" class="::visuVTKAdaptor::Render" objectId="self">
                        <config renderer="default" picker="" />
                    </adaptor>

                    <adaptor id="TMAdaptor1" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
                        <config renderer="default" picker="" />
                    </adaptor>

                    <adaptor id="TMAdaptor2" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
                        <config renderer="default" picker="" transform="mat6" />
                    </adaptor>
                    
                    <adaptor id="TMAdaptor3" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
                        <config renderer="default" picker="" transform="mat7" />
                    </adaptor>
                    
                    <adaptor id="TMAdaptor4" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
                        <config renderer="default" picker="" transform="mat8" />
                    </adaptor>

                </scene>
            </service>
```

The new things of this tutorial are the introduction of new adapters:

  * Those for data consistency between the vtk world of the generic scene (mat1, mat2, mat3, mat4, mat5) and FW4SPL world  (matrixA, matrixB, matrixC, matrixD, matrixE) as follows:

> | FW4SPL world | vtk world (scene g�n�rique) |
|:-------------|:----------------------------|
> | matrixA      |mat1                         |
> | matrixB      |mat2                         |
> | matrixC      |mat3                         |
> | matrixD      |mat4                         |
> | matrixE      |mat5                         |

  * Those which allow to display the mesh

  * TMAdaptor1 allows to display the original mesh whose id is "mesh" (without transformation)
```
       <adaptor id="TMAdaptor1" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
           <config renderer="default" picker="" />
       </adaptor>
```

  * TMAdaptor2 allows to display the mesh whose id is "mesh" after applying the transformation matrix mat6
```
        <adaptor id="TMAdaptor2" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
            <config renderer="default" picker="" transform="mat6" />
        </adaptor>    
```
> where mat6 = mat5 x mat1 x mat4

  * TMAdaptor3 allows to display the mesh whose id is "mesh" after applying the transformation matrix mat7
```
        <adaptor id="TMAdaptor3" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
            <config renderer="default" picker="" transform="mat7" />
        </adaptor>   
```
> where mat7 = inv(mat5) x mat2 x mat5 x mat5 x mat5 x mat4 x mat4

  * TMAdaptor4 allows to display the mesh whose id is "mesh" after applying the transformation matrix mat8
```
        <adaptor id="TMAdaptor4" class="::visuVTKAdaptor::TriangularMesh" objectId="mesh">
            <config renderer="default" picker="" transform="mat8" />
        </adaptor>  
```
> where mat8 = mat3 x mat5 x mat4 x mat4 x mat4

_**To be noted**_: The calculated matrix mat6, mat7 and mat8 depend on mat5 which are synchronized with the MatrixE in FW4SPL world. Given that the MatrixE value can be modified by the cursor widget _matrixEditor_,
the modification of the cursor value will produce the movement of the mesh which are tranformed by the matrix mat6, mat7 and mat8 (see TMAdaptor2, TMAdaptor3 and TMAdaptor4).

> ## Hints and Tips ##

  * The ::fwData::TransformationMatrix3D data allows to store transformation matrix in the FW4SPL.
  * This transformation can be apply to mesh and display in generic scene.