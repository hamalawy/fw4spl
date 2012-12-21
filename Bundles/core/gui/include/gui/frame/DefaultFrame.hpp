/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUI_FRAME_DEFAULT_FRAME_HPP_
#define _GUI_FRAME_DEFAULT_FRAME_HPP_

#include <fwTools/Failed.hpp>
#include <fwGui/IFrameSrv.hpp>

#include "gui/export.hpp"

namespace gui
{
namespace frame
{

/**
 * @brief   Defines the default frame for standard application.
 * @class   DefaultFrame.
 * @author  IRCAD (Research and Development Team)

 * @date    2009.
 */
class GUI_CLASS_API DefaultFrame : public ::fwGui::IFrameSrv
{

public :

    fwCoreServiceClassDefinitionsMacro ( (DefaultFrame)(::fwGui::IFrameSrv) ) ;

    /// Constructor. Do nothing.
    GUI_API DefaultFrame() throw() ;

    /// Destructor. Do nothing.
    GUI_API virtual ~DefaultFrame() throw() ;

protected :

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief This method is used to configure the class parameters.
     */
    GUI_API virtual void configuring() throw( ::fwTools::Failed );

    /**
     * @brief Register a view with defined id.
     */
    GUI_API virtual void starting() throw(::fwTools::Failed);

    /**
     * @brief This method remove the view in the frame.
     */
    GUI_API virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
    GUI_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed) ;

    /**
     * @brief This method is used to update services. Do nothing.
     */
    GUI_API virtual void updating() throw(::fwTools::Failed) ;

    /**
     * @brief This method gives information about the class. Do nothing.
     */
    GUI_API virtual void info(std::ostream &_sstream ) ;
    ///@}

};


} // namespace frame
} // namespace gui


#endif /*_GUI_FRAME_DEFAULT_FRAME_HPP_*/
