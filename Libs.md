# Librairies de fw4spl #

## fwTools / fwCore ##

Desc :

Ce qui manque à C++ pour en faire un langage "pratique"

Spec :

Définition de type de base, Dispatcher (à partir d'une liste de type et d'une chaine de caractère : évite les if pour les instanciations de template), ClassFactory (association à la volée), DynamicType (représentation des types de bases)

## fwRuntime ##

Desc :

Brique de base pour gérer les composants

Spec :

Chargement des composants, de lister leurs contenus, de lire les fichiers XML, de démarrer telle ou telle fonction d'un composant.

### TODO ###

Résoudre le problème de recouvrement du Registrar avec celui de fwTools, interprétation des variables des fichiers XML (ceux du profile et ceux du dossier patient)

## fwData ##

Desc :

Définition des structures de données

Spec :

fwData permet d'avoir une organisation flexible des données

Portée :

Les fwData sont un exemple de données pouvant être utilisée avec fw4spl.
D'autres objets peuvent être créés en utilisant cette librairie comme exemple.

## fwServices ##

Desc :

Ajout de fonctions aux datas de manière dynamique.

Spec :

Services (classes de bases +  gestion des bindings : définit le comment +  gestion de la communication + monitoring), dataServices(services de base: editeurs (proxy : getter/setter), observations et signaux).
