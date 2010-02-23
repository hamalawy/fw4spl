/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwTools/Failed.hpp>
#include "guiQt/view/IQtView.hpp"



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
class  QtDefaultView : public ::guiQt::view::IQtView
{

public :

    /// Constructor. Do nothing.
     QtDefaultView() throw() ;
    
    /// Destructor. Do nothing.
     virtual ~QtDefaultView() throw() ;
     
    void configuring() throw( ::fwTools::Failed );
    void starting() throw( ::fwTools::Failed );

};


} // namespace view
} // namespace gui

