# Contributors #

From 2004 to 2006, an advanced modular software for patient modeling (see publication page) has been designed and implemented by Guillaume Brocker, Johan Moreau, Jean-Baptiste Fasquel, Vincent Agnus and Nicolas Papier. This represented the basis of the component management system of FW4SPL, essentially conceived by Guillaume Brocker and Johan Moreau. This framework version (v0.1) was used to create 3 software tools in visualization and medical image processing in the Eureka project Odysseus (3DVPM, 3DDVP and MARNS software).

Throughout 2007, Vincent Agnus and Jean-Baptiste Fasquel conceived and implemented the main core mechanisms of this new version of FW4SPL. Jean-Baptiste Fasquel focused on the notion of roles coupled with the component management system, the inter-role communication system, as well as an appropriate XML formalism for the description of both roles embedded into components and description of software. Many basic software tools have been built to validate the architecture (see publication page). Vincent Agnus also focused on role design, and more specifically on data structures, a generic serialization mechanism and a powerful template dispatching technique. During his internship in 2007, Benjamin Gaillard has improved the communication system in FW4SPL. In parallel with the work on the pure FW4SPL system, Johan Moreau got involved in the construction/compilation system and, together with Arnaud Charnoz, in the management of external dependencies and some specific medical data structures. Their work also led to advanced visualization of medical images (free download). Early 2008, the framework was available in version 0.2.

During the period from mid-2008 to mid-2009, some advanced data structures and functionalities have been developed on the basis of the architecture to further evaluate it and make it more robust. A larger development team has been involved, including Emilie Harquel, Julien Waechter and Nicolas Philipps additionally to Vincent Agnus, Jean-Baptiste Fasquel, Johan Moreau and Arnaud Charnoz. Additional efforts have been made by Johan Moreau and Arnaud Charnoz on the management of external dependencies. Nicolas Philipps, Julien Waechter and Johan Moreau also improved the construction environment Sconspiracy, initially opened as an opensource project YAMS++ in 2007. The version 0.3 of the framework had been achieved by early summer 2009.

From mid-2009 to mid-2010, the main work on FW4SPL included: performing generic scenes for visualization (mainly developed by Nicolas Philipps, Julien Waechter and Vincent Agnus), a new communication system (mainly developed by Nicolas Philipps and Arnaud Charnoz), new UI components (mainly developed by Emilie Harquel and Julien Waechter), better log and assert system (by Arnaud Charnoz), new documentation (mainly done by Pascal Monnier, Alexandre Hostettler).

FW4SPL (version 0.4) has been opened late 2009 and was used to create several software in the European project Passport (VR-Render, VR-Render WLE, AR-Surg, VR-Planning and VR-Probe software). In December, we had switched to version 0.5 (with generic scene). The latest stable version is 0.6 (new communication system) and the current branch development is the 0.6.1 branch.

Version 0.7 adds a limited Qt support during summer 2010 (Hocine Chekatt's internship) and limited support for Python, OpenNI and SOFA (these two last parts had been developed by Altran). During 2011, FW4SPL 0.8 adds a Qt based 2D scene (Ivan MATHIEU's internship), new buffer for meshes and images, new memory dump mechanisms, a new set of applications (Apps/Examples), a new Dicom reader (Jordi ROMERA's internship), new registration functionalities (Marc Schweitzer's internship) an improved image origin management, etc. A new scenegraph design has been developed but not yet integrated (Loïc Velut's internship).

Multithreading (fwThread), signal/slot (fwCom), dump management and data introspection (fwAtoms) mechanisms have been added during 2012 in version 0.9 (co-working between IRCAD and IHU). A new design to manage data and store data (Julien Weinzorn's internship) has been prototyped.

This version supports msvc2010 and has also been used to evaluate the transition to Android and iOS (Adrien Bensaibi's internship). Altran has added a connector towards the management tool of the MIDAS content developed by Kitware. Finally, a version management mechanism has been developed (fwAtomsPatch) (Clément Troesch's internship) and new data has been created (fwMedData). This version has been used by the Visible Patient company within the framework of their ISO 13485 certification. A new repository has also been created (fw4spl-ext) with the aim of welcoming not yet stabilized functionalities or to host PoC. The CMake construction system is also supported.

Version 0.10.0 provides the notion of timeline to manage temporal data (IHU). The SConspiracy construction system has been removed.

<br><br><br><br>

<table>
<tr>
<blockquote><td align='center'><a href='http://www.ircad.fr'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/ircad_france_couleur_petit.png' /></a></td>
<td>Core, visualization, image processing, applications and tutorials (Team : Pascal Monnier, Valentin Martinet, Arnaud Charnoz, Johan Moreau)</td>
</tr>
<tr>
<td align='center'><a href='http://www.ihu-strasbourg.eu'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/logoIHU.jpg' /></a></td>
<td>Core, visualization, image processing, applications and tutorials (Team : Nicolas Philipps, Julien Waechter, Emilie Harquel)</td>
</tr>
<tr>
<td align='center'><a href='http://www.altran.fr'><img src='http://www.altran.com/fileadmin/templates/main/media/logo/logo_altran.png' /></a></td>
<td>
Proof of concept on Kinect and Sofa integration :<br>
</blockquote><ul><li><a href='http://www.altran.com/fileadmin/medias/1.altran.com/files/Altitude_FR/Altran_200609_MAG10_FR.pdf'>French document</a> p12<br>
</li><li><a href='http://www.altran.fr/fileadmin/medias/1.altran.com/files/Altitude_FR/Altitude_17_20100407_FR.pdf'>French document</a> p26<br>
Proof of concept on MIDAS integration<br>
<blockquote></td>
</tr>
<tr>
<td align='center'><a href='http://ec.europa.eu/information_society/activities/health/index_en.htm'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/ehealth.gif' /></a></td>
<td>
This project has partly funded by the European Commission via PASSPORT project :<br>
</blockquote></li><li><a href='http://www.passport-liver.eu/'>http://www.passport-liver.eu/</a>
</li><li><a href='http://www.vph-noe.eu/vph-repository/doc_download/154-passportppt'>http://www.vph-noe.eu/vph-repository/doc_download/154-passportppt</a>
</li><li><a href='http://www.vph-noe.eu/vph-repository/.../188-vph-noe-4th-newsletter-july-2010'>http://www.vph-noe.eu/vph-repository/.../188-vph-noe-4th-newsletter-july-2010</a></li></ul>

<blockquote></td>
</tr>
</table></blockquote>

<br><br><br><br>

<h1>FLOSS projects using FW4SPL</h1>

<table>
<tr>
<blockquote><td align='center'><a href='http://www.programmez.com/actualites.php?id_actu=10873'>skuld-project</a> :<br>
Skuld project work on mobile port (iphone, android, meego/maemo, ...) of FW4SPL<br>
</td>
</tr>
</table></blockquote>

<br><br><br><br>

<h1>Libraries</h1>

<table>
<tr>
<blockquote><td align='center'><a href='http://www.dcmtk.org/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/DCMTK.png' /></a></td>
<td align='center'><a href='http://www.boost.org/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/boost.png' /></a></td>
<td align='center'><a href='http://www.gnu.org/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/free_software_foundation.png' /></a></td>
<td align='center'><a href='http://heasarc.gsfc.nasa.gov/docs/software/fitsio/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/cfitsio.png' /></a></td>
</tr>
<tr>
<td align='center'><a href='http://www.dcmtk.org/'>DCMTK</a></td>
<td align='center'><a href='http://www.boost.org/'>boost</a></td>
<td align='center'><a href='http://www.gnu.org/'>Free software foundation</a></td>
<td align='center'><a href='http://heasarc.gsfc.nasa.gov/docs/software/fitsio/'>cfitsio</a></td>
</tr>
<tr>
<td align='center'><a href='http://www.itk.org/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/itk.png' /></a></td>
<td align='center'><a href='http://www.vtk.org/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/vtk.png' /></a></td>
<td align='center'><a href='http://www-sop.inria.fr/asclepios/software/vtkINRIA3D/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/vtkINRIA3D.png' /></a></td>
<td align='center'><a href='http://www.wxwidgets.org/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/wxWidgets.png' /></a></td>
</tr>
<tr>
<td align='center'><a href='http://www.itk.org/'>Itk</a></td>
<td align='center'><a href='http://www.vtk.org/'>Vtk</a></td>
<td align='center'><a href='http://www-sop.inria.fr/asclepios/software/vtkINRIA3D/'>vtkINRIA3D</a></td>
<td align='center'><a href='http://www.wxwidgets.org/'>wxWidgets</a></td>
</tr>
<tr>
<td align='center'><a href='http://www.python.org/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/Python.png' /></a></td>
<td align='center'><a href='http://www.scons.org/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/SCons.png' /></a></td>
<td align='center'><a href='http://wix.sourceforge.net/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/wix.png' /></a></td>
<td align='center'><a href='http://www.zlib.net/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/zlib.png' /></a></td>
</tr>
<tr>
<td align='center'><a href='http://www.python.org/'>Python</a></td>
<td align='center'><a href='http://www.scons.org/'>SCons</a></td>
<td align='center'><a href='http://wix.sourceforge.net/'>wix</a></td>
<td align='center'><a href='http://www.zlib.net/'>zlib</a></td>
</tr>
<tr>
<td align='center'><a href='http://am.iic.harvard.edu/FITS-reader'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/fits2itk.png' /></a></td>
<td align='center'><a href='http://xmlsoft.org/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/Libxml2.png' /></a></td>
<td align='center'><a href='http://logging.apache.org/log4cxx/index.html'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/log4cxx.png' /></a></td>
<td align='center'></td>
</tr>
<tr>
<td align='center'><a href='http://am.iic.harvard.edu/FITS-reader'>fits2itk</a></td>
<td align='center'><a href='http://xmlsoft.org/'>Libxml2</a></td>
<td align='center'><a href='http://logging.apache.org/log4cxx/index.html'>log4cxx</a></td>
<td align='center'><a href='http://code.google.com/p/sconspiracy/'>Sconspiracy</a></td>
</tr>
<tr>
<td align='center'><a href='http://qt.nokia.com/products/'><img src='http://wiki.fw4spl.googlecode.com/hg/Partners/qt-logo.png' /></a></td>
<td align='center'></td>
<td align='center'></td>
<td align='center'></td>
</tr>
<tr>
<td align='center'><a href='http://qt.nokia.com/products/'>Qt</a></td>
<td align='center'></td>
<td align='center'></td>
<td align='center'></td>
</tr>
</table>