/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include <set>

#include <fwServices/IService.hpp>
#include <QMenuBar>

namespace guiQt
{
namespace aspect
{

class IQtMenu : public ::fwServices::IService
{

public :

    fwCoreNonInstanciableClassDefinitionsMacro ( (IQtMenu)( ::fwServices::IService::Baseclass) ) ;

    /// Default constructor, do nothing.
    IQtMenu() throw() ;

    /// Default destructor, do nothing.
    virtual ~IQtMenu() throw() ;
  
protected :

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /// Retrieve IMenu::m_menuName in configuration element
    virtual void configuring() throw( ::fwTools::Failed ) ;

    virtual void starting() throw( ::fwTools::Failed ) ;

    /**
     * @brief Stop all actions and separator of this menu and remove menu to wxMenuBar
     * @todo ACH : uncomment assert => all actions must be started
     * @todo Menu not empty on MAC with specials Actions like help, Quit...
     */
    virtual void stopping() throw( ::fwTools::Failed ) ;

    /// Updating service, do nothing.
    virtual void updating() throw(::fwTools::Failed);

    /// Updating service on notification, do nothing.
    virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);
    ///@}
 
private:
    /// Service vector of ::gui::aspect::IMenu UUID.  List of the menus declared in the aspect config.
    std::vector< std::string >  m_menusUID ;

};


}
}
