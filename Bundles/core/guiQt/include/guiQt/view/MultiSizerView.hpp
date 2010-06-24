/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _GUI_VIEW_MULTI_SIZER_VIEW_HPP_
#define _GUI_VIEW_MULTI_SIZER_VIEW_HPP_



#include <fwTools/Failed.hpp>

#include "guiQt/config.hpp"
#include "guiQt/view/IView.hpp"
#include <QWidget>
#include <QMainWindow>


namespace gui
{
namespace view
{

/**
 * @brief   Defines the default panel for standard application.
 * @class   MultiSizerView.
 * @author  IRCAD (Research and Development Team)

 * @date    2009.
 */
class GUIQT_CLASS_API MultiSizerView : public ::guiQt::view::IView
{
    class ViewInfo
    {
    public :

        ViewInfo() :
            m_proportion (1),
            m_border(0),
            m_panel (0),
            m_uid(""),
	    // Negative sizes are not possible
            m_minSize (std::make_pair(0,0)),
            m_autostart(false),
            m_caption (std::make_pair(false,""))
        {}

        int                            m_proportion;
        int                            m_border;
        std::string                    m_uid;
        QWidget*                       m_panel;
        std::pair< int, int >          m_minSize;
        bool                           m_autostart;
        std::pair< bool, std::string > m_caption;
    };

public :

    fwCoreServiceClassDefinitionsMacro ( (MultiSizerView)(::guiQt::view::IView::Baseclass) ) ;

    /// Constructor, does nothing.
    GUIQT_API  MultiSizerView() throw() ;

    /// Destructor, does nothing.
    GUIQT_API  virtual ~MultiSizerView() throw() ;

protected:

    /// Analyses xml configuration to configure this service, espacially umber of panel and the layout of this view.
    GUIQT_API void configuring() throw( ::fwTools::Failed );

    /// Reconfigures service, does nothing.
    GUIQT_API void reconfiguring() throw( ::fwTools::Failed );

    /// Builds defined layout and creates wxPanel.
    GUIQT_API void starting() throw( ::fwTools::Failed );

    /// Uninit layout and destroy panel.
    GUIQT_API void stopping() throw( ::fwTools::Failed );

    /// Swap associated object, does nothing.
    GUIQT_API void swappping() throw( ::fwTools::Failed );

    /// Updates service, does nothing.
    GUIQT_API void updating() throw( ::fwTools::Failed );

    /// Updates service on notification, does nothing.
    GUIQT_API void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _msg ) throw( ::fwTools::Failed );

    /// Prints service name
    GUIQT_API void info(std::ostream &_sstream );
     
     QMainWindow* m_manager;


private :

    bool m_bOrient;

    /// Save layout configuration definition and created container
    std::list< ViewInfo>  m_views;

};


} // namespace view
} // namespace gui


#endif /*_GUI_VIEW_MULTI_SIZER_VIEW_HPP_*/
