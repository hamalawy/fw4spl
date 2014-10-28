/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <QScrollArea>
#include <QVBoxLayout>
#include <QStyle>

#include <fwCore/base.hpp>
#include <fwGui/registry/macros.hpp>


#include "fwGuiQt/container/QtContainer.hpp"
#include "fwGuiQt/layoutManager/ToolboxLayoutManager.hpp"


fwGuiRegisterMacro( ::fwGui::ToolboxLayoutManager, ::fwGui::layoutManager::ToolboxLayoutManagerBase::REGISTRY_KEY );


namespace fwGui
{

//-----------------------------------------------------------------------------

ToolboxLayoutManager::ToolboxLayoutManager(::fwGui::GuiBaseObject::Key key)
{}

//-----------------------------------------------------------------------------

ToolboxLayoutManager::~ToolboxLayoutManager()
{}

//-----------------------------------------------------------------------------

void ToolboxLayoutManager::createLayout( ::fwGui::container::fwContainer::sptr parent )
{
    SLM_TRACE_FUNC();

    ::fwGuiQt::container::QtContainer::sptr parentContainer;
    parentContainer = ::fwGuiQt::container::QtContainer::dynamicCast(parent);
    SLM_ASSERT("dynamicCast fwContainer to QtContainer failed", parentContainer);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *qtContainer = parentContainer->getQtContainer();
    if (qtContainer->layout())
    {
        QWidget().setLayout(qtContainer->layout());
    }
    qtContainer->setLayout(layout);

    QScrollArea* sv = new QScrollArea(qtContainer);
    ::fwGuiQt::widget::QfwToolBox* toolbox = new ::fwGuiQt::widget::QfwToolBox(sv);
    sv->setWidget(toolbox);
    sv->setWidgetResizable(true);

    const std::list< ViewInfo> &views = this->getViewsInfo();
    BOOST_FOREACH ( ViewInfo viewInfo, views)
    {
        int border = viewInfo.m_border;
        QWidget* panel = new QWidget();
        panel->setMinimumSize(std::max(viewInfo.m_minSize.first,0), std::max(viewInfo.m_minSize.second,0));
        panel->setContentsMargins(border, border,border, border);

        ::fwGuiQt::container::QtContainer::sptr subContainer = ::fwGuiQt::container::QtContainer::New();
        subContainer->setQtContainer(panel);
        m_subViews.push_back(subContainer);

        int index = 0;
        if(viewInfo.m_useScrollBar)
        {
            QScrollArea *scrollArea = new QScrollArea(toolbox);
            scrollArea->setWidget(panel);
            scrollArea->setWidgetResizable ( true );
            index = toolbox->addItem(scrollArea, QString::fromStdString(viewInfo.m_caption));
        }
        else
        {
            index = toolbox->addItem(panel, QString::fromStdString(viewInfo.m_caption));
        }

        if(viewInfo.m_expanded)
        {
            toolbox->expandItem(index);
        }
        subContainer->setVisible( viewInfo.m_visible );
    }
    layout->addWidget(sv);
}

//-----------------------------------------------------------------------------

void ToolboxLayoutManager::destroyLayout()
{
    this->destroySubViews();
}

//-----------------------------------------------------------------------------

} // namespace fwGui



