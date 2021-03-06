/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWGUIWX_LAYOUTMANAGER_FRAMELAYOUTMANAGER_HPP_
#define _FWGUIWX_LAYOUTMANAGER_FRAMELAYOUTMANAGER_HPP_

#include <wx/aui/aui.h>

#include <fwCore/base.hpp>

#include <fwGui/layoutManager/IFrameLayoutManager.hpp>

#include "fwGuiWx/container/WxContainer.hpp"
#include "fwGuiWx/config.hpp"

namespace fwGui
{

/**
 * @brief   Defines the frame manager.
 * @class   FrameLayoutManager
 * 
 * @date    2009-2010.
 *
 */
class FWGUIWX_CLASS_API FrameLayoutManager : public ::fwGui::layoutManager::IFrameLayoutManager
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (FrameLayoutManager)(::fwGui::layoutManager::IFrameLayoutManager),
                                            (()),
                                            ::fwGui::factory::New< FrameLayoutManager > );

    FWGUIWX_API FrameLayoutManager(::fwGui::GuiBaseObject::Key key);

    FWGUIWX_API virtual ~FrameLayoutManager();

    /**
     * @brief Instantiate frame.
     */
    FWGUIWX_API virtual void createFrame();

    /**
     * @brief Destroy local frame with sub containers.
     */
    FWGUIWX_API virtual void destroyFrame();

private:

    static const std::map< ::fwGui::layoutManager::IFrameLayoutManager::Style, long> FWSTYLE_TO_WXSTYLE;

    void onCloseFrame(wxCloseEvent& event);

    void setState(FrameState state);
    FrameState getState();

    wxFrame* m_wxFrame;
};

} // namespace fwGui

#endif /*_FWGUIWX_LAYOUTMANAGER_FRAMELAYOUTMANAGER_HPP_*/


