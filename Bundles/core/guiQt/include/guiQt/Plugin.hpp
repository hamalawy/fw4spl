/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#include "guiQt/config.hpp"

#include <fwRuntime/Plugin.hpp>

#include <QApplication>
#include <QMainWindow>

/**
 * @brief	Namespace guiQt.
 * @namespace	guiQt
 * @author	IRCAD (Research and Development Team).

 *
 * @date	2009.
 *
 *  The namespace gui contains the basic services to build the application IHM (ie: wxContainer, wxMenu, wxLayer...).
 */
namespace guiQt
{

/**
 * @brief	This class is called when the gui bundle is loaded.
 * @class	Plugin.
 * @author	IRCAD (Research and Development Team).

 * @date	2009.
 */

class GUIQT_CLASS_API Plugin :  public  QObject, public ::fwRuntime::Plugin
{
  Q_OBJECT

public:

	 ///@brief	Destructor. Do nothing.
	 GUIQT_API ~Plugin() throw();

	/**
	 * @brief This method is used by runtime to initialize the bundle.
	 *
	 * @exception ::fwRuntime::RuntimeException.
	 *
	 * Register first basic services of type IAction, IEditor, IAspect and IView.
	 * Secondly, verifies if the command line and profile.xml are well formed and starts the wxWidgets IHM.
	 */
	 GUIQT_API void start() throw(::fwRuntime::RuntimeException);

	/**
	* @brief This method is used by runtime to close the bundle. Do nothing.
	*/
	  GUIQT_API void stop() throw();
	
public slots :
  void exit();
  void windowClosed();
  void printDestruction();

};

} // namespace guiQt

