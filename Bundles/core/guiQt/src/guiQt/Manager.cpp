/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#include <cassert>
#include <vector>

#include <fwCore/base.hpp>
#include <fwServices/Factory.hpp>
#include <fwServices/helper.hpp>
#include <fwServices/bundle/runtime.hpp>

#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/helper.hpp>

#include <iostream>

#include "guiQt/Manager.hpp"

#include <QApplication>
#include <QPushButton>
#include <QMainWindow>
#include <QFrame>
#include <QMenuBar>
#include <QDialog>
#include <QWidget>
	

#define __FWCORE_TYPEDEF_SHARED_PTR_FACTORY_NAME NewSptr


namespace guiQt
{
/**
 * @brief Forward menu event received to the manager in order to update/execute the appropriate service
 * @author IRCAD (Research and Development Team).
 */

/**
 * @brief Application top window
 */
  //Creation de la classe MainWindow : public QMainWindow

//-----------------------------------------------------------------------------
Manager::Manager() throw()
{
	
}

//-----------------------------------------------------------------------------

Manager::~Manager() throw()
{
	
}


void Manager::initialize()
{
  int argc = 1;
  char* argv[1] = {"test"};
  
  QApplication app( argc,  argv);
  
  QWidget widget;
  widget.resize(950, 750);
  widget.show();
   
  app.setActiveWindow(&widget);

  //::guiQt::aspect::QtDefaultAspect aspect;
 // aspect.configure(); 
 
 //::guiQt::aspect::IQtMenu menu;
//  menu.start();
  
 //::guiQt::view::IQtView view;
 //view.configure;
  //view.start();
  
 //       ::fwServices::get< ::guiQt::aspect::IQtAspect >( getDefault()->m_unlessObject )->configure() ;
   //     ::fwServices::get< ::guiQt::aspect::IQtAspect >( getDefault()->m_unlessObject )->start();
   
    ::fwServices::OSR::initializeRootObject();
   
   app.exec();
}


/*
const ::guiQt::Manager::sptr Manager::getDefault() throw()
{
    if(m_instance.get() == 0)
    {
        m_instance = ::guiQt::Manager::NewSptr();
    }
    return m_instance;
}*/


} // namespace gui
/*
AMELIORER PERF
  - Augmenter la taille de la pipeline
  - diminier la taille des instructions
    --> Si branchement
      - predicateur n-bits
  - Architecture super-scalaire + ordonnencement
MEMORY
  Limit memory perf :
     - plusieurs niveaux de caches
     - demande de mots memoir
     - Bp memoire
   Amelioration latence de caches :
     - ratio hit , multi-thread, pre-loaded data
     - memory bound application
     - CPU frequency bound application
   Impact BP memoire : aumgente la taille des blocs.

COHERENCE CHACHE
  2 strateges :
   - invalidate toutes les autres copies
   - updates toutes les autres copies
  False shaing :
    - invalidate la ligne de cache
    - update la ligne de cache
  Consistance en copies:
    - conserver le nombre de copies
    - connaitre l'etat de chaque copie
  Implementation HardWare
    - Snoopy caches : all processus surveillent le bus et modifie leur ligne de cache en consequence => goulet => gestion shared pour les processeurs concernés => repertoire
    - Mecanisme repertoire : augmentation de la memoire d'un bitmap(bit de presence) memorisant les owner de blocs
    - combinaison des 2
ALGO PARALLELE
  - identifier tache simultanees
  - allouer ces taches sur des processeurs s'executant simultanement
  - distribuer donnees : entrees, sorties, intermediaires
  
  - taille de tache variable (1 tache = 1 calcul ou la moitie)
  - pas de dependances entre taches
  - sinon gaphe de dependance ou acyclique
  - exemple : matrice x vexteur, requete BDD
  
DEGRE DE CONCURRENCE
  -d° max = #max  de taches //
  -d° moyen = #tache// sur tout le temps d'execution = poids total/longueur chemin critique  d'apres grapheDep
  -probleme interaction : graphe interaction
  
MODELE POLYEDRIQUE
  - nid de boucle = representation geometrique de boucles imbriquees
  - iterations = points (avec coordonnees)
  - enveloppe de ces poiints = polyedre convexe
  
  */