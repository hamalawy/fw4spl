# FW4SPL and Ogre integration #

## Synopsis ##

> The goal of this project is to allow FW4SPL components to communicate with OGRE engine.

## Benefits to the FW4SPL Community ##

> OGRE is n efficient open-source real-time 3D graphics engine used for games, simulation, architectural and medical visualization. It supports Windows, Linux and OS X (as well as extensions to other platforms) and multiple rendering systems.
> FW4SPL is a component-oriented architecture which currently focuses on the problem of medical image processing and visualization. FW4SPL is based on a set of cross-platform C++ libraries. The rendering engine is currently based on VTK, but has been designed to allow for several rendering backend.
> The latest FW4SPL developments we have done has highlighted the need for simulation and vertex evolution-oriented 3D engine.

## Required skills ##

  * C++ programming
  * OpenGL or 3D skills
  * Qt

## References ##

  * http://www.ogre3d.org/
  * http://code.google.com/p/fw4spl/
  * http://qt.nokia.com/

## Deliverables ##

> Preferred license: Lesser General Public License (LGPL)

## Mentors ##

  * Main mentor: Nicolas Philipps (nicolasph at gmail dot com)
  * Secondary mentor: Vincent Agnus (vincent dot agnus at gmail dot com)


---


# DICOM/WADO component for FW4SPL #

## Synopsis ##

> The goal of this project is to add DICOM/WADO abilities to FW4SPL.

## Benefits to the FW4SPL Community ##

> Digital Imaging and Communications in Medicine (DICOM) is a standard for handling, storing, printing and transmitting information in medical imaging. Accessing DICOM information has never been simpler. With Web Access to DICOM Persistent Objects (WADO), a Web client can request DICOM persistent objects, such as images or medical imaging reports, from a remote repository through HTTP/HTTPS.
> FW4SPL is a component-oriented architecture which currently focuses on the problem of medical image processing and visualization. FW4SPL is based on a set of cross-platform C++ libraries. FW4SPL currently load Dicom images by using a reader based on Gdcm library.
> Software built with FW4SPL are intended to provide access to reports, images and waveforms by external clients(not within the hospital only). Integrating the WADO protocol in FW4SPL seems to be the best solution to tackle this issue.

## Required skills ##

  * C++
  * Python
  * Network skills

## References ##

  * http://medical.nema.org/dicom/2004.html
  * http://medical.nema.org/dicom/2004/04_18PU.PDF
  * http://www.dcm4che.org/confluence/display/ee2/WADO+Service
  * http://qt.nokia.com/
  * http://www.boost.org/
  * http://code.google.com/p/fw4spl/

## Deliverables ##

> Preferred license: Lesser General Public License (LGPL)

## Mentors ##

  * Main mentor: Pascal Monnier (plmonnier at gmail dot com)
  * Secondary mentor:


---


# FW4SPL and Bullet integration #

## Synopsis ##

> The goal of this project is to allow FW4SPL components to communicate with Bullet engine.

## Benefits to the FW4SPL Community ##

> Bullet is an open-source physics engine featuring 3D collision detection, soft body dynamics and rigid body dynamics. It is used in games and for visual effects in movies.
> FW4SPL is a component-oriented architecture which currently focuses on the problem of medical image processing and visualization. FW4SPL is based on a set of cross-platform C++ libraries.
> FW4SPL still does not contain any component (we have only some proof of concept with Sofa) which allows the management of soft body interaction, which is very important to develop surgical planning software. Indeed, such software request a realistic and efficient management of organ and instrument collision.
> Incorporating soft body physics from Bullet into FW4SPL would therefore bring useful new tools for the community among this project.

## Required skills ##

  * C++ programming
  * OpenGL or 3D skills
  * Physics skills

## References ##

  * http://qt.nokia.com/
  * http://www.boost.org/
  * http://code.google.com/p/fw4spl/
  * http://bulletphysics.org/

## Deliverables ##

> Preferred license: Lesser General Public License (LGPL)

## Mentors ##

  * Main mentor: Nicolas Philipps (nicolasph at gmail dot com)
  * Secondary mentor: Arnaud Charnoz (arnaud dot charnoz at gmail dot com)


---


# FW4SPL and Urbi integration #

## Synopsis ##

> The goal of this project is to allow FW4SPL components to communicate with Urbi engine.

## Benefits to the FW4SPL Community ##

> Urbi is an open-source software platform used to control robots or complex systems in general. It includes a C++ component library coming associated to a standard robot API to describe motors, sensors and algorithms. The goal of Urbi is to help making robots compatible and to simplify the process of writing software and behaviors for these robots.
> FW4SPL is a component-oriented architecture which currently focuses on the problem of medical image processing and visualization. FW4SPL is based on a set of cross-platform C++ libraries. FW4SPL can already take into account the organ deformation with Sofa.
> Linking the Urbi platform with FW4SPL will allow to develop high end medical software that includes robotic device management during medical intervention. Associated to other toolkits (like Sofa or Bullet for simulation and VTK or Ogre for 3D scene), this fully operational platform will provide a efficient environment for future activities in the operating room.

## Required skills ##

  * C++ programming
  * Robotics skills

## References ##

  * http://qt.nokia.com/
  * http://www.boost.org/
  * http://code.google.com/p/fw4spl/
  * http://www.urbiforge.org/

## Deliverables ##

> Preferred license: Lesser General Public License (LGPL)

## Mentors ##

  * Main mentor: Julien Waechter (julien dot waechter at gmail dot com)
  * Secondary mentor: Nicolas Philipps (nicolasph at gmail dot com)


---


# HL7 component for FW4SPL #

## Synopsis ##

> The goal of this project is to add HL7 abilities to FW4SPL, especially to connect this framework to the Mirth project.

## Benefits to the FW4SPL Community ##

> Hospitals and other healthcare organizations typically have many different computer systems used to manage very different tasks, from billing records to patient tracking.
> HL7 and its members provide a framework (and related standards) for the exchange, integration, sharing, and retrieval of electronic health information.
> Mirth is an open-source cross-platform HL7 interface engine that enables to send HL7 messages.
> FW4SPL is a component-oriented architecture which currently focuses on the problem of medical image processing and visualization. FW4SPL is based on a set of cross-platform C++ libraries.
> Software built with FW4SPL are intended to be used within (or not) the hospital. Today, HL7 recommendations are followed by a large number of hospitals and it seems strategic to add electronic health information management directly in software built with FW4SPL.


## Required skills ##

  * C++
  * Python
  * Network skills

## References ##

  * http://qt.nokia.com/
  * http://www.boost.org/
  * http://code.google.com/p/fw4spl/
  * http://www.mirthcorp.com/community/overview

## Deliverables ##

> Preferred license: Lesser General Public License (LGPL)

## Mentors ##

  * Main mentor: Johan Moreau (johan dot moreau at gmail dot com)
  * Secondary mentor: Pascal Monnier (plmonnier at gmail dot com)


---


# Google Chrome and NativeCL with FW4SPL #

## Synopsis ##

> The goal of this project is to build a proof of concept around Google NativeCL technologies and FW4SPL, especially for 3D rendering.

## Benefits to the FW4SPL Community ##

> Native Client is an open-source technology allowing you to build web applications that seamlessly execute native compiled code inside a browser. It is not a W3C standard, it is a Google draft supported in the Google Chrome Web navigator.
> FW4SPL is a component-oriented architecture which currently focuses on the problem of medical image processing and visualization. FW4SPL is based on a set of cross-platform C++ libraries.
> FW4SPL provides a large and complex set of 2D/3D functionalities for medical software that are difficult to develop in a web browser. In hospital setting, the number of workstations is important and deployment of applications is time consuming. A web development and deployment could be an extremely efficient alternative.
> For this reason, we believe that integration of NativeCL in FW4SPL will increase software set up in important structure like hospitals.

## Required skills ##

  * C++
  * Python
  * Web technologies

## References ##

  * http://code.google.com/intl/fr/chrome/nativeclient/
  * http://www.chromium.org/nativeclient
  * http://qt.nokia.com/
  * http://www.boost.org/
  * http://code.google.com/p/fw4spl/

## Deliverables ##

> Preferred license: Lesser General Public License (LGPL)

## Mentors ##

  * Main mentor: Julien Waechter (julien dot waechter at gmail dot com)
  * Secondary mentor: Johan Moreau (johan dot moreau at gmail dot com)