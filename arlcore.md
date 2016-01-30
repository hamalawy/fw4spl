ARLcore is a library that offers tools in order to develop simple software for augmented reality. It proposes some extensions to VXL library like matrix and vector of rigid transformation, basic data structures like « Camera » that includes intrinsic and extrinsic parameters, « Point » associated to a covariance matrix...

The ARLcore facilitates manipulation of euclidean frames/planes and proposes many calibration tool for camera, 3D reconstruction, reprojection, registration.

# Dependencies #
ARLcore use extensively VXL library for matrix calculus and optimisation tool (v3p\_netlib, vcl, vgl, vnl,
vnl\_algo). A compilation option activate the possibility to use ANN library for some process on point cloud. Without it some algorithm cannot be used (ICP registration).

# Virtual Classes #
There is 3 virtual class, mother from most of high level class in ARLcore.

## Object ##
« Object » is a general class that contains basic information and that allows to manage time and mutex. It contains the information of from its own mother class (m\_class): a string for the name (m\_name), a positive integer that indicate the build number of the object (m\_no) and another integer that indicate the total number of this class object (m\_counter). “Object” also allows to manage mutex for concurrent access to object in a multithread environment. It contains a index (m\_ok) that indicate whether the object is valid, and 2 long integer for the date and time (m\_date and m\_time) of the last object update. “Object” has also methods to display log.

## Particle ##
« Particle » is a class for object that belongs to a PlaneSystem. It has a pointer on the PlaneSystem (m\_universe) and a number of the plane to which it is linked to (m\_plane). For instance, the Tag class inherits from Object and Particle: it is an object located in a work space.


## Parameters ##
« Parameters » classe provides to his sons a parameter list of boolean or double. Save and access to these parameters can be done using class “ Parameters”. Each parameter is identified with a string and has some complementary information like default value, description, bounds... Some methods allows to give a value to each object parameter.

# Matrix computation #
Classes about transformation matrix are introduced as a contribution to VXL.

## vnl\_rigid\_matrix ##
Rigid transformation matrix that  inherits from vnl\_matrix\_fixed < double,4,4 >
It is represented by a n homogeneous 4x4 matrix, composed of a  3x3 rotation matrix and a 3d translation vector :
R  R  R T
R  R  R T
R  R  R T
0  0  0 1
The class is optimized, taking advantage of rotation matrix properties (notably for inverse computation). The definition of the rigid transformation allows to compose different plane changes. Tb<-a being a transformation from plane a to plane b, then T c <- b x T b <- a = T c <- a .

vnl\_rigid\_matrix also inherits from “Object” and has protection against concurrent access to its timestamp. PlaneSystem class defines the transformations between each plane using  vnl\_rigid\_matrix.

## vnl\_rigid\_vector ##
Transformation rigid vector inherits of vnl\_vector\_fixed < double,6 > . It is composed of:
> • Lines 0 à 2 = rotation vector
> • Lines 3 à 5 = translation vector

The vector representation of rigid transformation is particularly adapted when you need to perform a criterion optimization in which a transformation is used.

## vnl\_rotation3d\_matrix ##
Rotation Matrix  inherits of vnl\_matrix\_fixed < double,3,3 > .

## vnl\_rotation3d\_vector ##
Rotation vector inherits of vnl\_vector\_fixed < double,3 >. It is defined :
> • rotation axis is determined by the vector direction
> • rotation angle is determined by the vector norm modulo 2Pi in the counter clockwise direction

Rotation vector is part of rigid vector.
vnl\_covariance\_matrix : public vnl\_matrix

&lt;double&gt;


Covariance matrix in ARLcore is associated to points. It is a square matrix and its dimension is d x d for a point of dimension d. For 2x2 matrix, it has a computation method for uncertainty ellipse.

# PlaneSystem #
PlaneSystem class is an ensemble of Cartesian plane, linked by a rigid transformation. It is a Galilean universe, it allows to take instant “snapshot” of the universe. Temporal dimension is independent.

Class PlaneSystem

## The planes ##
Planes can be linked to object / tag or can be used as abstract intermediary plane. They have description features. The status of the object system is essentially described by the transformation in the PlaneSystem.

## Transformations ##
Each transformation has a time stamp. The method that allows to access a specific transformation between 2 planes (in case it is not directly available), will parse the transformation tree to compute its current value. A transformation can have a particular persistence. Usually, the transformation issued from callibration procedure are constant. However for a tag which is tracked in real-time, the transformation is updated in real-time.

## To do... ##
It is feasible to save or deduce dynamic information (speed, acceleration) allowing to predict position or to  anticipate them in occultation case.

# Points #
## Formats ##
> • File format for Point (see ar\_ARLFormats\_ENG.odt)
> • File format for PointList (see ar\_ARLFormats\_ENG.odt)

## PointList ##
...

## SmartPointList ##
Principle

This container is very important in ARL. It is used to save 2D points detected in video images. Its particularity is to have the ability to sort the points with respect to their source:  Camera, Tag, chessboard, index of structured light fringe. Its data structure allows to get point list :
> • by camera to display them
> • by point on a specific camera image in order to get their 3D reconstruction
> • by camera and by stripe fringe to get structured light surface

This data structure has been realized so that it optimize the access along all possible combination, avoiding duplication. It use an indexed map with a hash key on the camera number, the tag number, the fringe number and on 2 list lists of point pointer.

### Basic functions ###

> • Build : SmartPointsList spl;
> • point add: point are copied in smartPointsList and tagged with respect to following parameters
> > ° push\_back( const arl::Point& pt, unsigned int cam, void**tag=0, int fringe=0 );**

Camera number is mandatory. Each point can be asociated to a Tag and to a fringe.Using push\_pack, points are added one after one.


> • addPoint( const arl::Point&, unsigned int cam, void**tag, unsigned int tagNo );
AddPoint add a point associated to a particular tap position, on a specified camera ( For exemple : the  4th  point of ARTAG 238)
> •  retrieving a list : they are given in vector of Point pointer that are reference of saved point in the class. These references are active as long as the object is not empty. If it does not exist any point that corresponds to the specified request (for example if there is not point detected on the camera 2 image), a empty vector is given back. In case there is discontinuities in the list, a 0 pointer is inserted.**

> ° unsigned int getPointByCam( std::vector< const arl::Point >&, unsigned int camSize, void tag, unsigned int no, std::vector< bool >&whichCams) const;

This method gives the point list of Tag number 'no' in all camera for 3D reconstruction for instance. The boolean vector Le vecteur de booléens 'whichCams' will have the dimension of camSize (thei means the total number of camera) and indicates for each camera if the point is visible or not.

> • unsigned int getPoints( std::vector< const arl::Point >&, unsigned int cam, void t=0 ) const;
Gives back the list of points for a specific camera if t==0. If t != 0 the function gives back the point for a specific camera AND a specific Tag.

> • unsigned int getInvisiblePoints( std::vector< arl::Point >&, unsigned int cam, void t=0 ) const;
Like the previous function but for invisible points only. In Video, the estimation using reprojection of some point positions which were not detected  is added in the smartPointsList as invisible point. They will be redetected during a second process (cf. Video). This function allows to retrieve invisible point list for a given camera and for a given tag.

> • bool getPoint( const arl::Point , unsigned int cam, void tag, unsigned int tagNo ) const;
It returns the point Point from the tag with the number tagNo in the camera cam (if this point is not visible in camera cam, it returns false.
> •  clear() allow to empty the smartPointList.

# Tag & Tags #
A Tag is a point list defining its rigid geometry. Generally it is a surface (not necessarily a planar surface). The Point coordinates are defined in a plane related to the Tag. Tag's nature (electromagnetic, optical...) is specialized in high level library. Detection method are implemented in specialized tracker which update continuously the point measurement.

There is 3 Tag types:
> • ARLCORE\_TAG\_REGISTRATION\_3D3D : match between measured points and points defined in the Tag geometry are known. Then the 2 point set are registered using a 3D/3D point registration (at least 3 non colinear points are required) : the method optimize the criterion sum | TxM\_i – N\_i ||^2 where M\_i are the points defined in the Tag geometry and N\_i are the measured points.|
|:--------------|
> • ARLCORE\_TAG\_REGISTRATION\_3DWO : in this case, the match between the measured point and the point of the Tag geometry is not known. The registration method firstly match them using a prediction/verification algorithm, a 3D/3D registration being performed afterward.
> • ARLCORE\_TAG\_REGISTRATION\_ICP : this method register using ICP method 2 dense point cloud defining 2 surfaces. In that case, it is generally not possible to match exactly the point (number of point are usually different).

“Tags”  class is a list of Tag object. In our high level function, specialized trackers are composed ot a Tag list that has to be detected.

# Cameras #
Camera class models a physical stenope camera that includes radial and tangential distortion. See parameterization of Camera calibration Toolbox for more details (http://www.vision.caltech.edu/bouguetj/calib_doc/).

## planes/frames ##
> • pixel planes (pixelFrame) : pixel coordinate in image
> • plane of optical center in the focal plane (focalFrame), assuming a unitary focal plane (unitFocalPlane) z=1
> • camera plane or world plane or extrinsic plane (extrinsicFrame) : reference world plane. This plane is common to several cameras in polyscopic system.

## Parameters ##
Camera parameters can be loaded using the load function (see format to see how to load the camera parameters from a file). The ARL allows also to set a calibration file directory that contain all the calibration file of your camera with respect to the date of the calibration. Indeed, for some applications it is necessary to play recorded movie. Instead of loading manually the camera parameters, the directory containing the movie contains also the date of the movie record, then ARL automatically load the calibration file that corresponds   to the date of the movie record.

If you launch a software that is acquiring the current image of your camera the loadParameters function will load the camera calibration that corresponds to the current date. If you are the 05/01/2010, ARL will search the latest calibration file before 05/01/2010.

### Intrinsic parameters ###
The description of these parameters can be found in the camera calibration toolbox website. They are also described in Zhengyou Zhang famous paper “A flexible method for camera calibration”. They allow to compute the projection of a 3D point (described in camera plane) on the camera sensor (in pixel unit).
Intrinsic matrix:
[0 cx](fx.md)
[fy cy> 0](0.md)
[0 1](0.md)
**(fx,fy) <=> (AlphaU,AlphaV) : pix/mm unit, allows to compute the position of a point on the sensor plane.
> • (cx,cy) <=> (u0,v0) : center position of the optical axis on the sensor
distortion :
> • Radial :Modelled by k1, k2 ( k3 is currently not used)
> • Tangential: Modelled by p1 et p2 (also called t1 et t2)
Let Point Pi=transpose([Xi,Yi,1]) and [r2](https://code.google.com/p/fw4spl/source/detail?r=2)=Xi2 + Yi2, Pi becomes Pd=transpose([Xd,Yd,1]) after distortion
Xd = (1 + k1.[r2](https://code.google.com/p/fw4spl/source/detail?r=2) + k2.[r2](https://code.google.com/p/fw4spl/source/detail?r=2).[r2](https://code.google.com/p/fw4spl/source/detail?r=2))Xi + 2 . p1 . Xi . Yi + p2.([r2](https://code.google.com/p/fw4spl/source/detail?r=2) + 2.Xi2)
{ radial distortion} { tangential distortion}
Yd = (1 + k1.[r2](https://code.google.com/p/fw4spl/source/detail?r=2) + k2.[r2](https://code.google.com/p/fw4spl/source/detail?r=2).[r2](https://code.google.com/p/fw4spl/source/detail?r=2))Yi + 2 . p2 . Xi . Yi + p1.([r2](https://code.google.com/p/fw4spl/source/detail?r=2) + 2.Yi2)**

### Extrinsic parameters ###
Extrinsic parameters corresponds to the rigid transformation between the calibration object plane and the plane of the optical center (with Z axis toward the camera viewing direcntion).

## Transformations ##
> • From pixel frame (on the sensor) to the focal plane (real coordinate system):
cf. arl::Camera::pixelPlaneToUnitFocalPlane and arl::Camera::unitFocalPlaneToPixelPlane
> • 3D/2D registration and homography allow to compute object position in world frame from at least 3 points (4 for homography) in one or more cameras.
> • reprojection functions, inversely, compute pixel position of world 3D points :
> cf .  arl::Camera::project3DPoint

# 3D Reconstruction #
3D reconstruction function is available in  Reconst3D.h .:
reconst3D ( const std::vector< const arl::Point>&, const std::vector< const arl::Camera>&, arl::Point& Pt3D,
ARL\_RECONSTRUCTION3D method, unsigned int &plane, std::vector&log, const bool pixelFrame=true);

arl::reconst3D compute pt3D, from a 2D point visible in n cameras in the plane &plane. double log vector contains values describing the computed results , for instance reconstruction error. Some methods use only the 2 first cameras of the list, and you need sometime to specify pixelFrame value, depending on the 2D points source:
> • true,  default for pixel coordinate
> • false, for coordinates in focal plane (Z=1)
```
enum ARL_RECONSTRUCTION3D { ARL_R3D_UNKNOWN, ARL_R3D_HORAUD_APPROX, 
ARL_R3D_HORAUD_PERFECT, ARL_R3D_TWO_LINES_APPROX, ARL_R3D_TWO_LINES_PERFECT, 
ARL_R3D_MULTI_LINES_APPROX, ARL_R3D_MULTI_LINES_PERFECT, 
ARL_R3D_REPROJECTION_OPTIMIZATION, ARL_R3D_REPROJECTION_OPTIMIZATION_UNCERTAINTY, 
ARL_R3D_NBTYPE }; 
```
= Recalage
## Recalage 3D/3D ##
...

## Recalage 3D/2D ##
> • ARL\_PR\_PLANAR\_HOMOGRAPHY
Attention : Le calcul du planar homography utilise l'évaluation approchée de la distorsion. Le calcul diverge si les points
utilisés sont très en dehors des dimensions de l'image ou si la distorsion est très forte.
> • ARL\_PR\_EPPC
> • ARL\_PR\_ISPPC
> • ARL\_PR\_OSPPC

## Recalage ICP ##
...

# Synthetic scenes #
## Principe ##
In order to build monitored and test space, to simulate particular situations useful for algorithm evaluation (reconstruction, registration projection...), it is possible to build a “synthetic scene”, composed of point clouds and virtual cameras.
To build such a scene, it es necessary to create a new class that inherits from the Scene class. You have then to overload 5 methods of Scene to create a son class which works properly.

We explain below the case of Scene1 (available in  Scenes.h et Scenes.cpp).

World set up:
example class Scene1 : public Scene
This synthetic scene is composed of 500 points which are randomly built on the surface of a square centered at the origin. It is observed by 6 synthetic cameras randomly placed around the scene at a distance comprised between 1000 and 1500, and they are oriented toward the square center. Detected points on the camera images (in green) are noised (Gaussian noise with 0.1 standard deviation). White circle represents the reprojection of reconstructed points.

Declaration
```
\#ifndef _ARL_SCENES_H 
\#define _ARL_SCENES_H 
\#include 
"ARL/Common.h" 
\#include 
"ARL/Scene.h" 
namespace arl 
{ 
            class Scene1 : 
public Scene 
            { 
public: 
                      ARL_API Scene1( PlaneSystem& ); 
                      ARL_API ~Scene1( void ); 
private: 
                      void worldCreation( void ); 
                      void camerasCreation( void ); 
                      bool dynamicBehavior( unsigned 
int step ); 
                      unsigned 
int detectionBehavior( unsigned 
int cam, arl::SmartPointsList &, std::vector< 
const Tag* > &tags, std::map< 
const Tag*, unsigned 
int >& allTags ); 
            }; 
} // namespace ARL 
\#endif // _ARL_SCENES_H 

Initialisation 
Class initialisation

\#include 
"ARL/Scenes.h" 
\#include 
"ARL/Camera.h" 
\#include 
"ARL/Tags.h" 
\#include 
"ARL/SmartPointsList.h" 
arl::Scene1::Scene1( PlaneSystem &universe ): 
Scene(universe) 
{ 
          worldCreation(); 
          camerasCreation(); 
} 
arl::Scene1::~Scene1() 
{} 


World set up:
    • Add of Tags in the protected member  "Tags m_tags" 
    • they are added in the plane system of the scene
    • their virtual detection is associated to their origin tag (cf. Scene::detectionBehavior) 

void arl::Scene1::worldCreation() 
{ 
          Point centre; 
          centre.fill(0.0); 
          arl::PointsList m_pointsMonde; 
          m_pointsMonde.shape3DRandom(500, arl::ARL_SHAPE_EDGESQUARE, centre, 300); // ARL_SHAPE_CUBE 
          m_tags->addTag( "V:/AR/Data/Calibration/Tags/Reality.tag", m_pointsMonde); 
} 
```
In the above example, the new Tag is built from a Tag model (Reality.tag) and is
ns l'exemple ci-dessus, le nouveau Tag est crée à partir d'un modèle de Tag (Reality.tag) and is instantiated with a point cloud randmoly generated.

Camera set up
> • Set up of 6 cameras
> • Oriented toward the origin
> • distance to origin comprised between 1000 and 1500
> • Intrinsic parameters centered on int\_param with a noise of magnitude int\_range
```
void arl::Scene1::camerasCreation() 
{ 
          Point centre; 
          centre.fill(0.0); 
          std::vector< 
double> int_param, int_range; 
          int_param.push_back(1000); 
          int_param.push_back(1000); 
          int_param.push_back(400); 
          int_param.push_back(300); 
          int_param.push_back(2.0); 
          int_param.push_back(2.5); 
          unsigned 
int i; 
for( i=0 ; i<4 ; ++i ) int_param.push_back(0.0); 
for( i=0 ; i<10 ; ++i ) int_range.push_back(0.0); 
Scene::synthCamSetSamePoV(6, centre, 0.0, 500.0, 1000, int_param, int_range); 
} 
```
Dynamic behavious:
> > • Tags movement
> > • Noise on the 3D position

It is possible to implement dynamic behaviour of the scene using 2 methods:

> • Iterative time : the new behaviour depends on the previous iteration. Its type is of numerical serie
> • Real time : the behaviour is based on the internal computer clock. Its type is of f(t)

In the following example, it is a “Iterative time” behaviour. Frequency (clock tick) is related to request of the method Video::nextImages. It works in the same way when we use a recorded video.

```
// Collapsing and expansion of a point cloud
bool arl::Scene1::dynamicBehavior( unsigned int step) 
{ 
static double g=5.0; 
Point gravity, boundingBox1, boundingBox2; 
double minRadius, maxRadius; 
unsigned int i; 
for( i=0 ; i < m_tags->size() ; ++i )
           { 
                     PointsList &pl=m_tags->getTag(i)->getGeometry(); 
                     pl.properties( gravity, boundingBox1, boundingBox2, minRadius, maxRadius ); 
double dist=distance(boundingBox1, boundingBox2); 
                     std::cout<<dist<<"\n"; 
if(dist<50) g=-5.0; 
if(dist>300) g=5.0; 
                     pl.collapse(( double)step*g); 
           } 
           m_time++; 
return 
true; 
} 
```

Detection by cameras
> •  Compute the pixel position of world points
> •  Possibility to add 2D noise on the 2D pixel coordinates
> •  Occultation
> •  Generating false positive


```

// Detection of all points with an addition of Gaussian noise on the 2D coordinates of 0,1 pixel std.unsigned int arl::Scene1::detectionBehavior( unsigned int cam, arl::SmartPointsList &spl, std::vector< const Tag* > &tagsDetected, std::map< const Tag*, unsigned int >& allTagsDetected ) 

{ 
if(m_tags->size()<1) 
return 0; 
const 
double GaussianNoise=0.1; 
         unsigned 
int i, j, n; 
long 
int date=0, time=0; 
arl::Point pt(2,arl::Point::ARL_POINT_TYPE_ARTK,date,time); 
pt.setStatus(arl::Point::ARL_POINT_STATUS_DETECTED); 
const double ColorMin=50.0; 
const double ColorStep=(255.0-ColorMin)/( double)(m_tags->size()+2); 

for( i=0 ; i < m_tags->size() ; ++i ) 
         { 
                   PointsList &tagPoints=m_tags->getTag(i)->getGeometry(); 
for( j=0 ; j<tagPoints.size() ; ++j) 
                   { 
                            m_cameras[cam-1]->project3DPoint( *tagPoints[j], pt); 
const Colour color(0,ColorMin+ColorStep*(i+1),0); 
                            pt.setColour(color); 
                            pt.addGaussianNoise(GaussianNoise); 
                            //spl.addPoint(pt, cam, (void*)m_tags->m_tags[i], j); 
                            spl.push_back(pt, cam, (void*)m_tags->getTag(i)); 
                   } 
                   tagsDetected.push_back(m_tags->getTag(i)); 
                   n=1; 
                   std::map< const Tag*, unsigned int >::const_iterator found;                   found = allTagsDetected.find(m_tags->getTag(i)); 
if( found != allTagsDetected.end() ) 
                            n=(found->second)+1; 
                   allTagsDetected[m_tags->getTag(i)]=n; 
         } 
return m_tags->size(); 
} 
```

Using the class
Principle
> • Creating a plane system (PlaneSystem)
> • Creating a scene associated to this system (Scene1)
> • Creating a video (ensemble of n cameras) associated to the scene (Video)
> • the video object is automatically associated to:
> > ° the plane system of the scene
> > ° the cameras built by the scene

> • the function Video::nextImages() perform usually successively the following actions :
> > ° 1) update the video image
> > ° 2) Tag detection
> > ° 3) 3D Reconstruction of points
> > ° 4) Detection refinement
> > ° 5) Model registration
> > ° 6) Reconstructed points reprojection
> > ° 7) Update of visualization windows

Associated to a scene, the Video class behaves at each step in the following way :

> • 1) Scene::dynamicBehavior
> • 2) Scene::detectionBehavior
> • Keep the original steps 3, 5, 6 et 7
> • Step 4 is not performed since Scene::detectionBehavior should realize a final detection

Example of use of Scene1

```
// external use of classs Scene 
PlaneSystem univers( "Reality Emulator"); 
arl::Scene1 SC(univers); 
arl::Video *v0 = new arl::Video(SC); 
if(v0!=0)v0->createWindows(); 
while( true) 
if(v0!=0) v0->nextImages(); 
```
Linking to the Video class.
The Video classe can be instantiated like the following example:
> • a prerecorded video
> • a “live” video
> • a video linked to a virtual scene

In the latter case, the image to display are generated by the Scene class. Currently, Scene class creates a black image but avoids the detection of interest points (usually performed by Image class) and returns the 2D positions for each camera. Thie 'reprojection' can be perfect or noisy. The Video class does the other processing tasks :
> • display the points which are detected
> • reconstruction
> • registration
> • reprojection of reconstructed points

The processing core is identical whatever work mode you choose for the Video class, the data being real or live or totally virtual. The method nextImages of Video class launches the dynamic behaviour of the scene and then the world point detection. Afterward it processes these results like if they were coming from real video.

Comparison between physical detection and using synthetic scenes.

> • Physical capture : optical sensor action and image transmission
=> now : nothing. No synthetic image are built.
> • Detection : image processing leading to interest point detection  sorted by identified  (ARTAG, fringe points when structured light,...)
=> points of synthetic scene are reprojected on camera image. They can be perfect or noised or artificially hidden (true negative). We can also simulate false positive.

# Unitary Tests #

UnitTestArlCore contains a list of test that validate algorithms of ARLcore. For each test, we have to specify the iteration number and the tolerance on the evaluated criterion :
> • bool test( long int nbIterations, double tolerance );
If this tolerance is validated for all the random iteration, the function returns “true”. Most of the tests use a scene defined in SceneUnitTest :
> • 2 to 10 cameras with random parameters located in a cube of...
> • A point cloud is randomly placed in a cube of ….