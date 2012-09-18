/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <wx/panel.h>
#include <wx/sizer.h>

#include <boost/foreach.hpp>

#include <fwCore/base.hpp>
#include <fwGui/registry/macros.hpp>

#include <fwGuiWx/convert.hpp>

#include "fwGuiWx/layoutManager/LineLayoutManager.hpp"


fwGuiRegisterMacro( ::fwGui::LineLayoutManager, ::fwGui::layoutManager::LineLayoutManagerBase::REGISTRY_KEY );


namespace fwGui
{

//-----------------------------------------------------------------------------

LineLayoutManager::LineLayoutManager(::fwGui::GuiBaseObject::Key key)
{}

//-----------------------------------------------------------------------------

LineLayoutManager::~LineLayoutManager()
{}

//-----------------------------------------------------------------------------

void LineLayoutManager::createLayout( ::fwGui::container::fwContainer::sptr parent )
{
    SLM_TRACE_FUNC();
    m_parentContainer = ::fwGuiWx::container::WxContainer::dynamicCast(parent);
    SLM_ASSERT("dynamicCast fwContainer to WxContainer failed", m_parentContainer);

    wxWindow* wxContainer = m_parentContainer->getWxContainer();
    Orientation orientation = this->getOrientation();
    int wxOrientation = ( (orientation==VERTICAL) ? wxVERTICAL : wxHORIZONTAL);
    wxBoxSizer* boxSizer = new wxBoxSizer( wxOrientation );

    wxContainer->SetSizer( boxSizer );

    std::list< ViewInfo> views = this->getViewsInfo();
    BOOST_FOREACH ( ViewInfo viewInfo, views)
    {
        if(viewInfo.m_isSpacer)
        {
            boxSizer->Add( 0, 0, 1, wxEXPAND );
        }
        else
        {
            wxPanel * viewPanel = new wxPanel(  wxContainer, wxNewId() , wxDefaultPosition, wxSize( viewInfo.m_minSize.first, viewInfo.m_minSize.second ), wxNO_BORDER | wxTAB_TRAVERSAL );
            ::fwGuiWx::container::WxContainer::NewSptr subContainer;
            subContainer->setWxContainer(viewPanel);
            m_subViews.push_back(subContainer);
#ifndef __MACOSX__
            if(viewInfo.m_caption.first)
            {
                wxStaticBoxSizer* sizer = new wxStaticBoxSizer( wxOrientation, wxContainer, ::fwGuiWx::std2wx(viewInfo.m_caption.second));
                sizer->Add( viewPanel, 1, wxALL|wxEXPAND, viewInfo.m_border);
                boxSizer->Add(sizer, viewInfo.m_proportion, wxALL|wxEXPAND);
            }
            else
            {
                boxSizer->Add( viewPanel, viewInfo.m_proportion, wxALL|wxEXPAND, viewInfo.m_border);
            }
#else
            boxSizer->Add( viewPanel, viewInfo.m_proportion, wxALL|wxEXPAND, viewInfo.m_border);
#endif
            subContainer->setVisible( viewInfo.m_visible );
        }
    }

    wxContainer->Layout();
}

//-----------------------------------------------------------------------------

void LineLayoutManager::destroyLayout()
{
    this->destroySubViews();
    SLM_ASSERT("Parent container is not available.", m_parentContainer);
    wxWindow* wxContainer = m_parentContainer->getWxContainer();
    wxContainer->SetSizer(NULL);
}

//-----------------------------------------------------------------------------

} // namespace fwGui



