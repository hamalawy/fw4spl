/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUIQT_ASPECT_DEFAULTASPECT_HPP_
#define _GUIQT_ASPECT_DEFAULTASPECT_HPP_

#include "guiQt/config.hpp"


#include <vector>
#include <set>

#include <fwServices/IService.hpp>

#include "guiQt/aspect/IAspect.hpp"


namespace guiQt
{
namespace aspect
{


/**
 * @brief   Defines the default aspect for standard application
 * @class   DefaultAspect.
 * @author  IRCAD (Research and Development Team)

 * @date    2009.
 */
class  DefaultAspect : public ::guiQt::aspect::IAspect
{

public :
    
    /// Constructor. Do nothing.
    DefaultAspect() throw() ;

    /// Destructor. Do nothing.
    virtual ~DefaultAspect() throw() ;

protected:

    /**
     * @brief   This method initializes class member parameters from configuration elements.
     *
     */
    virtual void configuring() throw( ::fwTools::Failed ) ;

    /**
     * @brief This method first registers the aspect gui manager.
     * It secondly starts the menus contained in the IAspect::m_menus
     * and finally all IView services contained in the IAspect::m_views
     */
     virtual void starting() throw(::fwTools::Failed);

    /**
     * @brief This method first unregisters the aspect gui manager.
     * It secondly stops the menus contained in the IAspect::m_menus
     * and finally all IView services contained in the IAspect::m_views
     */
     virtual void stopping() throw(::fwTools::Failed);

    /// This method gives information about the class. Do nothing.
     virtual void info(std::ostream &_sstream ) ;

    /**
     * @brief   Normally, perform some computations according to object (this service is attached to) attribute values and its internal state.
     *          Do nothing here.
     */
     virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw( ::fwTools::Failed );

    /**
     * @brief   Normally, perform some computations according to object (this service is attached to) attribute values and its internal state.
     *          Do nothing here.
     */
     virtual void updating() throw( ::fwTools::Failed );
     
private:

    std::string  m_uid;
    bool         m_autostart;
    std::vector< std::string >       m_managedServices;
};


} // namespace aspect
} // namespace gui

#endif

