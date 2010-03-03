/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */
#ifndef _GUIQT_MANAGER_HPP_
#define _GUIQT_MANAGER_HPP_

#include "guiQt/config.hpp"


#include <vector>
#include <fwCore/base.hpp>
#include <fwTools/Object.hpp>
#include <fwServices/IService.hpp>
#include <fwRuntime/ConfigurationElement.hpp>
#include "guiQt/aspect/IMenu.hpp"
#include "guiQt/view/DefaultView.hpp"
#include "guiQt/aspect/DefaultAspect.hpp"
#include "guiQt/action/IAction.hpp"

namespace guiQt
{
  

  
class  Manager : public ::fwCore::BaseObject
{

/**
 * @brief	Implements the controller
 * @note it is equivalent to a controller between application (basicDocument) and specific visualization bundle.
 * @class	Manager.
 * @author	IRCAD (Research and Development Team)

 * @date	2009.
 */
public:

	//fwCoreNonInstanciableClassDefinitionsMacro ( (Manager)( ::fwCore::BaseObject::Baseclass ) ) ;
	fwCoreClassDefinitionsWithFactoryMacro( (Manager)( ::fwCore::BaseObject::Baseclass ) , (()), new Manager);
	/**
	 * @brief Ctor
	 */

	 virtual ~Manager() throw();

	/**
	 * @brief set application top window and its menu handler, initialize general gui aspect and initialize application root object (and service from configuration)
	 */
	static void initialize() ;
	
	static void registerAction( ::guiQt::action::IAction::sptr _action ) ;
	
	static void processAction(int id ) ;

	static const ::guiQt::Manager::sptr getDefault() throw();
	

private:

    /**
     * @brief   a shared pointer to the kernel instance
     */
    static ::guiQt::Manager::sptr m_instance;
    
    typedef std::map< int , ::guiQt::action::IAction::wptr > ActionContainer;
    ActionContainer m_serviceCallBacks;

    /**
     * @brief Ctor
     */
    	 Manager() throw();
};
} // namespace guiQt

#endif