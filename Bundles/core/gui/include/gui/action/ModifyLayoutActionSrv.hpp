/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GUI_ACTION_MODIFYLAYOUTACTIONSRV_HPP__
#define __GUI_ACTION_MODIFYLAYOUTACTIONSRV_HPP__

#include <boost/logic/tribool.hpp>

#include <vector>
#include <fwServices/IService.hpp>

#include <fwGui/IActionSrv.hpp>

#include "gui/config.hpp"

namespace gui
{
namespace action
{

/**
 * @brief This action is used to modify the layout configuration.
 * @class   ModifyLayoutActionSrv
 */
class GUI_CLASS_API ModifyLayoutActionSrv : public ::fwGui::IActionSrv
{

public:

    fwCoreServiceClassDefinitionsMacro ( (ModifyLayoutActionSrv)(::fwGui::IActionSrv) );
    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;

    /**
     * @brief Constructor. Do nothing.
     */
    GUI_API ModifyLayoutActionSrv() throw();

    /**
     * @brief Destructor. Do nothing.
     */
    GUI_API virtual ~ModifyLayoutActionSrv() throw();

protected:

    enum ActionType
    {
        MOVE,
        SHOW,
        HIDE,
        SHOW_OR_HIDE,
        ENABLE,
        DISABLE,
        DO_NOTHING
    };

    /**
     * @brief This method gives information about the class. Do nothing.
     */
    GUI_API virtual void info(std::ostream &_sstream );

    /**
     * @brief This method starts-updates or stops the specified services
     */
    GUI_API void updating() throw (fwTools::Failed);

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
    GUI_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /**
     * @brief This method is used to configure the service parameters: specifies which views to show/hide/mode.
     *
     * @verbatim
       <config>
         <move uid="srv_uid" wid="new_view_wid" />
         <show wid="view_to_show" />
         <hide wid="view_to_hide />
         <show_or_hide wid="view_to_show_or_hide />
         <enable uid="action_to_enabled" />
         <disable uid="action_to_disabled" />
       </config>
       @endverbatim
     * - \b <move> configures the services to move in the new view given by wid
     * - \b <show> configures the element to show
     * - \b <hide> configures the element to hide
     * - \b <show_or_hide> configures the element to show or hide.
     *      If action is activated, element is shown else it is hidden.
     *
     * <show>, <hide> and <show_or_hide> tags can have 2 type of attribute :
     * - wid to show  or hide windows container (view)
     * - sid  to show or hide a gui container service (IEditor, IView, IRender...)
     *
     * The action in the menu/toolbar must be checkable (style="check").
     */
    GUI_API void configuring() throw( ::fwTools::Failed );

    GUI_API virtual void starting() throw(::fwTools::Failed);

    GUI_API virtual void stopping() throw(::fwTools::Failed);

private:

    /// < fwID, WIDContainer>
    typedef std::vector< std::pair< std::string, std::string > > MoveSrvVectType;
    /// map representing fwID's services and new associated wid parent container
    MoveSrvVectType m_moveSrv;

    /// < wid, showState>
    typedef std::vector< std::pair< std::string, ::boost::logic::tribool > > ShowSrvVectType;
    /// map representing wid container and show state
    ShowSrvVectType m_showSrvWid;

    /// map representing sid container and show state
    ShowSrvVectType m_showSrvSid;

    /// < fwID, enableState>
    typedef std::vector< std::pair< std::string, bool > > EnableSrvVectType;
    /// map representing fwID's services and enable state
    EnableSrvVectType m_enableSrv;
};


} // namespace action
} // namespace gui


#endif /*__GUI_ACTION_MODIFYLAYOUTACTIONSRV_HPP__*/
