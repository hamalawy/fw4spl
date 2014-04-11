/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QHBoxLayout>

#include <boost/foreach.hpp>

#include <fwCore/base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ServiceFactory.hpp>

#include "monitorQt/action/ClassFactoryRegistryInfo.hpp"

namespace monitor
{
namespace action
{

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::monitor::action::ClassFactoryRegistryInfo , ::fwData::Object ) ;

//------------------------------------------------------------------------------

ClassFactoryRegistryInfo::ClassFactoryRegistryInfo( ) throw()
{}

//------------------------------------------------------------------------------

ClassFactoryRegistryInfo::~ClassFactoryRegistryInfo() throw()
{}

//------------------------------------------------------------------------------

void ClassFactoryRegistryInfo::updating( ) throw(::fwTools::Failed)
{
    m_tree->clearSelection();
    m_tree->clear();

    typedef ::fwServices::registry::ServiceFactory ServiceRegistry;
    const ServiceRegistry::KeyVectorType& factoryKeys = ServiceRegistry::getDefault()->getFactoryKeys();

    BOOST_FOREACH( ServiceRegistry::KeyVectorType::value_type key, factoryKeys )
    {
        const std::string objImpl = ServiceRegistry::getDefault()->getObjectImplementation(key);
        QTreeWidgetItem* srvItem = new QTreeWidgetItem();
        srvItem->setText(0, QString::fromStdString(key));
        srvItem->setText(1, QString::fromStdString(objImpl));
        m_tree->addTopLevelItem( srvItem );
    }
    m_dialog->show();
}

//------------------------------------------------------------------------------

void ClassFactoryRegistryInfo::configuring() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::initialize();
}

//------------------------------------------------------------------------------

void ClassFactoryRegistryInfo::starting() throw (::fwTools::Failed)
{
    this->::fwGui::IActionSrv::actionServiceStarting();

    QWidget *parent = qApp->activeWindow();
    m_dialog = new QDialog(parent);
    m_dialog->setWindowTitle("ServiceFactoryRegistry");
    m_dialog->setMinimumSize(800, 600);

    QHBoxLayout* sizer  = new QHBoxLayout();
    m_tree = new QTreeWidget( m_dialog );
    QStringList headerList = (QStringList() << "Service" << "Object");
    m_tree->setColumnCount(2);
    m_tree->setHeaderLabels(headerList);
    m_tree->setColumnWidth(0, 300);
    m_tree->setColumnWidth(1, 460);
    m_tree->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tree->setAlternatingRowColors( true );

    sizer->addWidget( m_tree ) ;
    m_dialog->setLayout( sizer ) ;
}

//------------------------------------------------------------------------------

void ClassFactoryRegistryInfo::stopping() throw (::fwTools::Failed)
{
    m_dialog->hide();
    delete m_tree;
    delete m_dialog;

    this->::fwGui::IActionSrv::actionServiceStopping();
}

//------------------------------------------------------------------------------

} // namespace action

} // namespace monitor

