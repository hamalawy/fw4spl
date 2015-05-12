/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QApplication>
#include <QVBoxLayout>

#include <fwRuntime/Runtime.hpp>
#include <fwRuntime/Bundle.hpp>
#include <fwRuntime/ExtensionPoint.hpp>
#include <fwRuntime/Extension.hpp>

#include <fwCore/base.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>

#include "monitorQt/action/ComponentsTree.hpp"

namespace monitor
{
namespace action
{

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::monitor::action::ComponentsTree, ::fwData::Object );

//------------------------------------------------------------------------------

ComponentsTree::ComponentsTree( ) throw()
{
}

//------------------------------------------------------------------------------

ComponentsTree::~ComponentsTree() throw()
{
}

//------------------------------------------------------------------------------

void ComponentsTree::updating( ) throw(::fwTools::Failed)
{
    m_treeContainer->clearSelection();
    m_treeContainer->clear();

    ::fwRuntime::Runtime * defaultRuntime             = ::fwRuntime::Runtime::getDefault();
    ::fwRuntime::Runtime::BundleIterator iter_bundles = defaultRuntime->bundlesBegin();
    while (iter_bundles != defaultRuntime->bundlesEnd())
    {
        const std::string bundleName = (*iter_bundles)->getIdentifier();
        bool isBundleEnable          = (*iter_bundles)->isEnable();
        QTreeWidgetItem* bundleItem  = new QTreeWidgetItem();
        if(!isBundleEnable)
        {
            bundleItem->setBackground(0, QBrush(QColor(155,155,155)));
        }
        bundleItem->setText(0, QString::fromStdString(bundleName));
        m_treeContainer->addTopLevelItem( bundleItem );

        //Extensions
        QTreeWidgetItem* extensionsItem = new QTreeWidgetItem();
        extensionsItem->setText(0, QObject::tr("Extensions"));
        bundleItem->addChild( extensionsItem );
        ::fwRuntime::Bundle::ExtensionConstIterator iter_extension = (*iter_bundles)->extensionsBegin();
        while (iter_extension != (*iter_bundles)->extensionsEnd())
        {
            std::string point      = (*iter_extension)->getPoint();
            bool isExtensionEnable = (*iter_extension)->isEnable();
            QTreeWidgetItem* item  = new QTreeWidgetItem();
            if(!isExtensionEnable)
            {
                item->setBackground(0, QBrush(QColor(155,155,155)));
            }
            item->setText(0, QString::fromStdString(point));
            extensionsItem->addChild( item );

            ++iter_extension;
        }
        ++iter_bundles;
    }

    m_dialog->show();
}

//------------------------------------------------------------------------------

void ComponentsTree::configuring() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::initialize();
}

//------------------------------------------------------------------------------

void ComponentsTree::starting() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::actionServiceStarting();

    QWidget *parent = qApp->activeWindow();
    m_dialog = new QDialog(parent);
    m_dialog->setWindowTitle("Components Tree");
    m_treeContainer = new QTreeWidget(m_dialog);
    m_treeContainer->setHeaderHidden(true);

    QVBoxLayout* topsizer = new QVBoxLayout();
    topsizer->addWidget(m_treeContainer);
    topsizer->setContentsMargins(0,0,0,0);
    m_dialog->setLayout(topsizer);
}

//------------------------------------------------------------------------------
void ComponentsTree::stopping() throw (::fwTools::Failed)
{
    m_dialog->hide();
    delete m_treeContainer;
    delete m_dialog;

    this->::fwGui::IActionSrv::actionServiceStopping();
}

//------------------------------------------------------------------------------

} // namespace action

} // namespace monitor

