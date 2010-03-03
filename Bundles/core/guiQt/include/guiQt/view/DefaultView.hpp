/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUIQT_VIEW_DEFAULT_VIEW_HPP_
#define _GUIQT_VIEW_DEFAULT_VIEW_HPP_

#include "guiQt/config.hpp"



#include <fwTools/Failed.hpp>
#include "guiQt/view/IView.hpp"



namespace guiQt
{
namespace view
{

/**
 * @brief   Defines the default panel for standard application.
 * @class   DefaultView.
 * @author  IRCAD (Research and Development Team)

 * @date    2009.
 */
class  DefaultView : public ::guiQt::view::IView
{

public :

    /// Constructor. Do nothing.
     DefaultView() throw() ;
    
    /// Destructor. Do nothing.
     virtual ~DefaultView() throw() ;
     
    void configuring() throw( ::fwTools::Failed );
    void starting() throw( ::fwTools::Failed );

};


} // namespace view
} // namespace gui
#endif
