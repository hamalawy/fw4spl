= Liste et description des Bundles

Cette page a pour objectif de fournir une première documentation sur ce qui forme en très grande partie notre FrameWork, c'est-à-dire les Bundles.

## Bundles primaires ##

### serviceReg ###

Le bundle serviceReg (service registration) décrit tous les types de services de bas niveau essentiels à FW4SPL.

### dataReg ###

Le bundle dataReg (data registration) décrit tous les types de data existants dans FW4SPL.

### gui ###

Le bundle gui (generic user interface) contient les services permettant d’installer des composants de type Interface Homme-Machine (IHM).

### visu ###

Le bundle visu (visualisation) contient la base des services de visualisation.

### io ###

Le bundle io (input output) contient la base des services d’écriture et de lecture.

## Bundles secondaires ##

### ioData ###

Le bundle ioData propose des services de lecture et d'écriture sur des data sans utiliser de librairie externe (ce bundle nécessite le bundle io).

### ioVTK ###

Le bundle ioVTK propose des services de lecture et d'écriture sur des images et des maillages en utilisant la librairie VTK (ce bundle nécessite le bundle io).

### uiIO ###

Le bundle uiIO permet d'utiliser une IHM qui propose de choisir un service de lecture ou d'écriture parmi les services disponibles (ce bundle nécessite les bundles io et gui).

### vtkSimpleMesh ###

Le bundle vtkSimpleMesh propose un service de visualisation de maillages surfaciques tridimensionnels (ce bundle nécessite le bundle visu).

### vtkSimpleNegato ###

Le bundle vtkSimpleNegato propose un service de visualisation d'images sous forme d'un négatoscope tridimensionnel (ce bundle nécessite le bundle visu).

