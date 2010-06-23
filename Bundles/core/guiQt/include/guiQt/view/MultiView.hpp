/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUIQT_VIEW_MULTI_VIEW_HPP_
#define _GUIQT_VIEW_MULTI_VIEW_HPP_

#include <fwTools/Failed.hpp>

#include "guiQt/config.hpp"
#include "guiQt/view/IView.hpp"

#include <QMainWindow>
#include <QDockWidget>

namespace guiQt
{
namespace view
{

/**
 * @brief   Defines the default panel for standard application.
 * @class   MultiView.
 * @author  IRCAD (Research and Development Team)

 * @date    2009.
 */
class GUIQT_CLASS_API MultiView : public ::guiQt::view::IView
{
    class ViewInfo
    {
    public :

        ViewInfo() :
            m_align ("center"),
	    m_title (""),
            m_minSize (std::make_pair(-1,-1)),
            m_isResizable(true),
            m_visible (true),
            m_panel (0),
            m_position(0),
	    m_movable(1),
            m_layer(0),
            m_row(0),
            m_autostart(false),
            m_caption (std::make_pair(false,""))
        {}

        std::string                      m_align;
	std::string                      m_title;
        std::pair< int, int >            m_minSize;
        bool                             m_visible;
        QWidget*                         m_panel;
        bool                             m_isResizable;
        int                              m_position;
        int                              m_layer;
        int                              m_row;
        bool                             m_autostart;
        std::pair< bool, std::string >   m_caption;
	int				 m_movable;
    };

public :

    /// Constructor, does nothing.
    GUIQT_API MultiView() throw() ;

    /// Destructor, does nothing.
    GUIQT_API  virtual ~MultiView() throw() ;

    /// Analyses xml configuration to configure this service, espacially umber of panel and the layout of this view.
    GUIQT_API  void configuring() throw( ::fwTools::Failed );

    /// Reconfigures service, does nothing.
    GUIQT_API  void reconfiguring() throw( ::fwTools::Failed );

    /// Builds defined layout and creates wxPanel.
    GUIQT_API  void starting() throw( ::fwTools::Failed );

    /// Uninit layout and destroy panel.
    GUIQT_API  void stopping() throw( ::fwTools::Failed );

    /// Swap associated object, does nothing.
    GUIQT_API void swappping() throw( ::fwTools::Failed );

    /// Updates service, does nothing.
    GUIQT_API  void updating() throw( ::fwTools::Failed );

    /// Updates service on notification, does nothing.
    GUIQT_API void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _msg ) throw( ::fwTools::Failed );

    /// Prints service name
    GUIQT_API  void info(std::ostream &_sstream );

protected :

    /// Layout manager of this view
    QMainWindow* m_manager;

    /// New Container definition
    typedef std::map< std::string , ViewInfo > PanelContainer;

    /// Save layout configuration definition and created container
    PanelContainer m_panels;

};


} // namespace view
} // namespace gui


#endif /*_GUI_VIEW_MULTI_VIEW_HPP_*/
