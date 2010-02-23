/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUI_VIEW_IVIEW_HPP_
#define _GUI_VIEW_IVIEW_HPP_

#include <fwServices/IService.hpp>



namespace guiQt
{

/**
 * @namespace ::gui::view
 * @brief This namespace contains the classes which define container.
 */
namespace view
{

/**
 * @brief   Defines the service interface managing a view placed in main frame.
 * @class   IView.
 * @author  IRCAD (Research and Development Team).

 * @date    2009.
 *
 */
class  IQtView : public fwServices::IService
{

public :

    fwCoreNonInstanciableClassDefinitionsMacro ( (IQtView)( ::fwServices::IService::Baseclass) ) ;

    /**
    * @brief Constructor. Do nothing (Just initialize parameters).
    */
     IQtView() throw() ;

    /**
     * @brief Destructor. Do nothing.
     */
     virtual ~IQtView() throw() ;

    /**
     * @brief This method is used to get the id of a wxPanel that represents this view.
     *
     * @return WxMenuItem id.
     */
     int getGuiContainerId() ;

    /**
     * @brief This method is used to get wx container of the view.
     *
     * @return wx container.
     */
     void getWxContainer() const;

protected :

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief This method is used to configure the class parameters.
     */
     virtual void configuring() throw( ::fwTools::Failed ) ;

    /**
     * @brief Register a view with defined id.
     */
     virtual void starting() throw(::fwTools::Failed);

    /**
     * @brief This method remove the view in the frame.
     */
     virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
     virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed) ;

    /**
     * @brief This method is used to update services. Do nothing.
     */
     virtual void updating() throw(::fwTools::Failed) ;

    /**
     * @brief This method gives information about the class. Do nothing.
     */
     virtual void info(std::ostream &_sstream ) ;
    ///@}

    /// The identifier of the window.
    int m_guiContainerId ;

    // Panel minimum width.
    int m_minWidth;

    // Panel minimum height.
    int m_minHeight;

};



} // namespace view
} // namespace gui


#endif /*_GUI_VIEW_IVIEW_HPP_*/
