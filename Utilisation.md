# Use of framework #

## Introduction ##

In this page, we introduce howto:
  * Use Sconspiracy to create a FW4SPL project for the following IDE :
    * Microsoft Visual C++
    * QtCreator
    * Eclipse.
  * Compile and run an existing application .
  * Create a new service in an existing component.
  * Create a new component.
  * Create an application with components.

## Use Sconspiracy to create a FW4SPL project ##

**_To be noted_** a project is a set of source files.

Due to the component-oriented architecture of tha FW4SPL, the source code of an application is split in a set of component.
In order to have all the source code of an application, [Sconspiracy](http://code.google.com/p/sconspiracy/) provides an option to generate the right solution for several EDI.
It allows to create MS Visual C++ 2008, Qt Creator and Eclispe project very easily. Each EDI project will contain all the requested component for the application.

**_Memo_ To use this functionality, we must use the Sconspiracy branch 0.3 or later.**

Without this option in Sconspiracy, to create a EDI project you must open the build.options of your application and load each declared component in your EDI.

### Create a Microsoft Visual C++ solution (**.sln) for a FW4SPL application (**Windows user only**) ###**

Sconspiracy allows to create solution (**.sln) for a Microsoft Visual C++ Express 2008(MSVC2008). To do that, you must enter the following command in your console :**

```
racy <application name>/IDE=msvs BUILD=no
```

For example, to generate the MSVC2008 solution of the first tutorial Tuto01Basic, just type :

```
racy Tuto01Basic/IDE=msvs BUILD=no
```

The result is stored in the install directory of your development tree (See [Installation de FW4SPL](QuickStart#Installation_de_FW4SPL.md)) as you can see below:

![http://wiki.fw4spl.googlecode.com/hg/Tuto01BasicSolutionVisual.png](http://wiki.fw4spl.googlecode.com/hg/Tuto01BasicSolutionVisual.png)

Once the solution loaded in MSVC2008, you just have to build bundle\_tuto01Basic solution.
By default, the compilation of the selected component is done with all these dependencies ( equivalent ot **racy `<component name>`** ).

It is possible to compile a component without its dependencies (useful when you program) by changing its configurtaion in the configuration manager(menu Build/configuration manager). A sample is show below :

![http://wiki.fw4spl.googlecode.com/hg/MSVS_Cible.png](http://wiki.fw4spl.googlecode.com/hg/MSVS_Cible.png)

### Create a session QtCreator for a FW4SPL application ###

Sconspiracy allows to create a solution for Qt Creator. To do that, you must enter the following command in your console:

```
racy <Application name>/IDE=qtcreator BUILD=no
  
or for QtCreator 2.2
 
racy <Application name>/IDE=qtcreator2-2 BUILD=no 

or for QtCreator 2.3
 
racy <Application name>/IDE=qtcreator2-3 BUILD=no 
```

To generate the solution for the first tutorial Tuto01Basic, just type :

```
racy Tuto01Basic/IDE=qtcreator2-2 BUILD=no
```

The result is stored in the install directory of your development tree (See [Installation de FW4SPL](QuickStart#Installation_de_FW4SPL.md)) as you can see below:

![http://wiki.fw4spl.googlecode.com/hg/Tuto01BasicSolutionVisual.png](http://wiki.fw4spl.googlecode.com/hg/Tuto01BasicSolutionVisual.png)

To load the application, you just have to launch qtcreator 

&lt;ApplicationName&gt;

 or to open a new session
By default, the component compilation is done without dependencies (like it is with the commande line **racy `<ComponentName>` BUILDDEPS=no**). To modify this and compile all the application,
go to Project view of QtCreator and choose racy as show in the following picture.

![http://wiki.fw4spl.googlecode.com/hg/QtCreator_Cible.png](http://wiki.fw4spl.googlecode.com/hg/QtCreator_Cible.png)

_**To be noted**_ : If you update your IDE to a newest version, it is recommanded to delete all yours qws files which are in the generation folder (the install directory of your development tree).

### Create a Eclipse project for FW4SPL application ###

Comming sson

## Compile and run existing application ##

FW4SPL provides several tutorials to test the framework. In this part, we will see how to compile and launch these applications.

Les applications d�j� disponible dans FW4SPL se trouvent dans le r�pertoire Install de votre r�pertoire de d�veloppement (voir [Installation de FW4SPL](QuickStart#Installation_de_FW4SPL.md)) :

![http://wiki.fw4spl.googlecode.com/hg/UtilisationAppsFolderLocation.png](http://wiki.fw4spl.googlecode.com/hg/UtilisationAppsFolderLocation.png)

For example, to compile the Tuto01Basic turorial, two solutions is available :
  * use a IDE ( see [Utilisation#Utiliser\_SConspiracy\_pour\_cr�er\_un\_projet\_QtCreator QtCreator] or [Utilisation#Utilser\_Sconspiracy\_pour\_cr�er\_un\_projet\_Microsoft\_Visual\_C++ MS VC++])
  * use a terminal and type thecommand line as we will see

It is easy to compile in command line, you just have to type **racy** follow by the name of the application.

```
racy Tuto01Basic 
```

**_To be noted_** :  The result of the compilation is put in the Install directory of your development folder (see [Installation de FW4SPL](QuickStart#Installation_de_FW4SPL.md)). It has the following sub-directories :

  * The **bin** folder contains a launcher called **launcher\_0-1\_xxx\_yy\_zzzz** (avec xxx:win/lin/osx, yy:32/64 bits, zzzz:gcc4/vc9-0) which allow to run an FW4SPL application.
  * The **Bundles** contains all the compiled bundles ( one folder by bundle). Each bundle is composed by a plugin.xml**file, a dynamic library and a set of ressources.
> > En outre, le dossier**Bundles**contient �galement un sous dossier portant le nom de l'application dans lequel se trouve une copie des fichiers**profile.xml**et**configuration.xml**initialement pr�sents dans le dossier**Dev/Apps**.
  * The**bin**folder contains the dynamic libraries of FW4SPL and the externe dynamic libraries of the externe  ainsi que les**.dll**et**.lib**des biblioth�ques externes.
  * Le dossier**share**contient les**ressources**des biblioth�ques internes ainsi que les**ressources**du**launcher**.**

## Lancer un programme ##

Une fois la compilation termin�e, il ne reste plus qu'� lancer l'application de la mani�re suivante :

  * Mettez vous dans le dossier Install.
  * Utilisez cette ligne de commande (avec xxx:win/lin/osx, yy:32/64 bits, zzzz:gcc4/vc9-0):
    * **Pour une interface utilisant WxWidgets**: `./bin/launcher_0-1_xxx_yy_zzzz Bundles/<nom_Tutoriel>_0-1/profile.xml`
    * **Pour une interface utilisant Qt**: `./bin/launcher_0-1_xxx_yy_zzzz Bundles/<nom_Tutoriel>_0-1/profileQt.xml`

Par exemple avec Windows, on tapera :
  * **Pour une interface utilisant WxWidgets**: type `.\bin\launcher_0-1_win_32_cl9-0_D.exe .\Bundles\Tuto01Basic_0-1\profile.xml`
  * **Pour une interface utilisant Qt**: type `.\bin\launcher_0-1_win_32_cl9-0_D.exe .\Bundles\Tuto01Basic_0-1\profileQt.xml`

**A noter**: avec Linux, il est pr�f�rable de positionner la variable LD\_LIBRARY\_PATH au d�but de la commande de lancement :
  * **Pour une interface utilisant WxWidgets**: type `LD_LIBRARY_PATH=./lib .\bin\launcher_0-1_win_32_cl9-0_D .\Bundles\Tuto01Basic_0-1\profile.xml`
  * **Pour une interface utilisant Qt**: type `LD_LIBRARY_PATH=./lib .\bin\launcher_0-1_win_32_cl9-0_D .\Bundles\Tuto01Basic_0-1\profileQt.xml`

Pour lancer un programme, il faut utiliser l'ex�cutable ind�pendant appel� **launcher**. Le **launcher** charge le fichier **profile.xml** de l'application dont le nom est donn� en param�tre.

Attention, certaines variables d'environnement doivent �tre d�finies (voir [Installation de FW4SPL](QuickStart#Installation_de_FW4SPL.md))

Sch�ma r�capitulatif :

![http://wiki.fw4spl.googlecode.com/hg/Schemaframework.png](http://wiki.fw4spl.googlecode.com/hg/Schemaframework.png)

Le **bundle gui** permet de lancer la **configuration** (appel de la m�thode **start** du **bundle gui** � la fin du fichier **profile.xml**). Pour ce faire, il faut donner en param�tres au **bundle gui** :
  * le nom du fichier de **configuration** (configFile)
  * l'identifiant de la **configuration** (config)
  * le type d'application : windows ou console (startingMode)
  * le type de **data** principalement utilis� par l'application (rootObject).

```
<activate id="gui" version="0-1" >
    <param id="startingMode" value="windows" />
    <param id="rootObject" value="::fwData::Image" />
    <param id="config" value="config01" />
    <param id="configFile" value="./Bundles/Tuto_0-1/configurations/config-01.xml" />
</activate>
```

## Cr�er un nouveau service dans un composant existant ##

Ajouter un nouveau service dans un composant revient � ajouter une nouvelle **fonctionnalit�** � ce composant.

![http://wiki.fw4spl.googlecode.com/hg/NewService_bundleDir.png](http://wiki.fw4spl.googlecode.com/hg/NewService_bundleDir.png)

Un d�veloppeur �crit un **service** en impl�mentant plusieurs m�thodes dans les fichiers NewService.hpp et NewService.cpp. Il s'agit des m�thodes
**configuring**, **starting**, **stopping**, **updating(message)** et **updating**. Toutes ces m�thodes sont connect�es avec les m�thodes **configure**,
**start**, **stop**, **update(message)**, **update** pr�senter dans [Description d�taill�e des services](ServiceDescription.md). Chaque m�thode repr�sente un �tat dans la machine � �tat.

![http://wiki.fw4spl.googlecode.com/hg/SimpleStates.png](http://wiki.fw4spl.googlecode.com/hg/SimpleStates.png)

Un exemple d�taill�e de l'impl�mentation d'un service se trouve dans le [Tutoriel6](Tutorial6.md).

## Cr�er un nouveau composant ##

Voir [Tutorial6.wiki Tutoriel6]

## Cr�er une application � partir de composants ##

**FW4SPL** fournit un ensemble de composants qui permettent de construire des applications. Les bundles existants sont class�s en trois cat�gories :

  * I) Les services d'enregistrement

> Les **bundles dataReg** et **serviceReg** sont deux composants qui doivent �tre syst�matiquement utilis�s, parce qu'ils correspondent aux briques �l�mentaires d'une application :
    * Le bundle dataReg (data registration) d�crit tous les types de **data** existants dans **FW4SPL**.
    * Le bundle serviceReg (service registration) d�crit tous les types de **services** de bas niveau essentiels � **FW4SPL**.

  * II) Description des types de services, ou interface
> Les **bundles gui**, **io** et **visu** sont des composants basiques et g�n�raux qui permettent d'utiliser des **bundles** sp�cialis�s :
    * Le **bundle gui** (generic user interface) contient les **services** permettant d�installer des composants de type Interface Homme-Machine (IHM).
    * Le **bundle io** (input output) contient la base des **services** d��criture et de lecture.
    * Le **bundle visu** (visualisation) contient la base des **services** de visualisation.

  * III) Impl�mentation des services
> Enfin dans **FW4SPL**, les **bundles uiIO**, **ioVTK**, **ioData**, **vtkSimpleNegato** et **vtkSimpleMesh** sont les **bundles** sp�cialis�s qui apportent des fonctionnalit�s � l'application qui les utilise:
    * Le **bundle uiIO** permet d'utiliser une IHM qui propose de choisir un **service** de lecture ou d'�criture parmi les **services** disponibles (ce bundle n�cessite les bundles **io** et **gui**).
    * Le **bundle ioVTK** propose des **services** de lecture et d'�criture sur des images et des maillages en utilisant la biblioth�que VTK (ce bundle n�cessite le bundle **io**).
    * Le **bundle ioData** propose des **services** de lecture et d'�criture sur des **data** sans utiliser de biblioth�que externe (ce bundle n�cessite le bundle **io**).
    * Le **bundle vtkSimpleNegato** propose un **service** de visualisation d'images sous forme d'un n�gatoscope tridimensionnel (ce bundle n�cessite le bundle **visu**).
    * Le **bundle vtkSimpleMesh** propose un **service** de visualisation de maillages surfaciques tridimensionnels (ce bundle n�cessite le bundle **visu**).
    * Le **bundle opVTKMesh** propose un **service** qui est capable de g�n�rer un maillage � partir d'un masque binaire (ce bundle n�cessite les bundles **gui**).

Les applications sont construites � partir de composants. Pour ce faire, il faut �crire deux fichiers XML qui correspondent au **profil** et � la **configuration** de l'application.

  * Le premier fichier, correspondant au **profil** de l'application, comprend la liste des **bundles** que nous souhaitons utiliser dans notre application. Certain **bundle** doivent syst�matiquement �tre inclus dans le **profil**. Il s'agit des **bundles serviceReg**, **dataReg** et **gui**.

  * Le second fichier, qui contient la **configuration** de l'application, permet de d�clarer les **data** et les **services** dont l'application � besoin. En outre, il permet �galement de configurer ces diff�rents **services**.

Pour plus de d�tails concernant la r�daction de ces deux fichiers, voir les diff�rents [Description#Pr�sentation\_des\_tutoriaux tutoriaux].

