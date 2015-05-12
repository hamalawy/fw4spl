/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <QComboBox>
#include <QHeaderView>
#include <QItemDelegate>
#include <QStringList>
#include <QTableWidgetItem>
#include <QTimer>
#include <QVBoxLayout>
#include <QFuture>
#include <QtConcurrentRun>

#include <fwCore/base.hpp>

#include <fwTools/fwID.hpp>
#include <fwTools/Stringizer.hpp>

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>

#include <fwMemory/BufferManager.hpp>
#include <fwMemory/ByteSize.hpp>
#include <fwMemory/tools/MemoryMonitorTools.hpp>
#include <fwMemory/BufferManager.hpp>
#include <fwMemory/IPolicy.hpp>

#include <fwData/Object.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>

#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/IMessageDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include "monitorQt/DumpEditor.hpp"

namespace monitor
{

fwServicesRegisterMacro( ::gui::editor::IEditor, ::monitor::DumpEditor, ::fwData::Object );

::fwMemory::BufferManager::BufferInfoMapType m_bufferInfos;
::fwMemory::BufferManager::BufferStats m_bufferStats = {0,0};

QString getHumanReadableSize(::fwMemory::ByteSize::SizeType bytes)
{
    return QString::fromStdString(::fwMemory::ByteSize(bytes));
}

//------------------------------------------------------------------------------


class PolicyComboBoxDelegate : public QItemDelegate
{

public:
    PolicyComboBoxDelegate(QObject *parent = 0) : QItemDelegate(parent)
    {
    }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


QWidget *PolicyComboBoxDelegate::createEditor(QWidget *parent,
                                              const QStyleOptionViewItem & option,
                                              const QModelIndex & index ) const
{
    QComboBox *policyComboBox = new QComboBox(parent);

    const std::string value = index.model()->data(index, Qt::DisplayRole).toString().toStdString();

    const ::fwMemory::policy::registry::Type::KeyVectorType &factories =
        ::fwMemory::policy::registry::get()->getFactoryKeys();

    BOOST_FOREACH( const ::fwMemory::policy::registry::KeyType &policy, factories)
    {
        policyComboBox->addItem(QString::fromStdString(policy));
        if(value == policy)
        {
            policyComboBox->setCurrentIndex(policyComboBox->count()-1);
        }
    }
    return policyComboBox;
}

void PolicyComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();

    QComboBox *policyComboBox = static_cast<QComboBox*>(editor);

    int idx = policyComboBox->findText(value);
    if( idx != -1 )
    {
        policyComboBox->setCurrentIndex(idx);
    }
}

void PolicyComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *policyComboBox = static_cast<QComboBox*>(editor);
    QString value             = policyComboBox->currentText();

    model->setData(index, value, Qt::EditRole);
}

void PolicyComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex & index ) const
{
    editor->setGeometry(option.rect);
}





class PolicyTableModel : public QAbstractTableModel
{

public:
    PolicyTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    static const int s_EXTRA_INFO_NB;
private:

    ::fwMemory::BufferManager::sptr m_buffManager;
};

const int PolicyTableModel::s_EXTRA_INFO_NB = 1;

PolicyTableModel::PolicyTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_buffManager = ::fwMemory::BufferManager::getDefault();
}

int PolicyTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    size_t nbParam = 0;
    if(m_buffManager)
    {
        ::fwCore::mt::ReadLock lock( m_buffManager->getMutex() );
        ::fwMemory::IPolicy::sptr currentPolicy = m_buffManager->getDumpPolicy();
        nbParam                                 = currentPolicy->getParamNames().size();
    }

    return static_cast<int>(nbParam + s_EXTRA_INFO_NB);
}

int PolicyTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant PolicyTableModel::data(const QModelIndex &index, int role) const
{
    if (!m_buffManager && !index.isValid())
    {
        return QVariant();
    }
    ::fwCore::mt::ReadLock lock( m_buffManager->getMutex() );
    ::fwMemory::IPolicy::sptr currentPolicy = m_buffManager->getDumpPolicy();

    if (index.row() > (s_EXTRA_INFO_NB + currentPolicy->getParamNames().size()) || index.row() < 0)
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {

        if (index.column() == 0)
        {
            const ::fwMemory::IPolicy::ParamNamesType &names = currentPolicy->getParamNames();
            if(index.row() == 0)
            {
                return QString::fromStdString(currentPolicy->getLeafClassname());
            }
            else if( (unsigned int)index.row() <= names.size())
            {
                const ::fwMemory::IPolicy::ParamNamesType::value_type &name = names.at(index.row() - 1);

                return QString::fromStdString(currentPolicy->getParam(name));

            }
        }
    }
    return QVariant();
}


QVariant PolicyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (m_buffManager && orientation == Qt::Vertical)
    {
        ::fwCore::mt::ReadLock lock( m_buffManager->getMutex() );
        ::fwMemory::IPolicy::sptr currentPolicy = m_buffManager->getDumpPolicy();
        const ::fwMemory::IPolicy::ParamNamesType &names = currentPolicy->getParamNames();
        if (section <= 0)
        {
            return QString("Current policy");
        }
        else if( (unsigned int)section <= names.size() )
        {
            const ::fwMemory::IPolicy::ParamNamesType::value_type &name = names.at(section - 1);
            return QString::fromStdString(name);
        }
    }
    return QVariant();
}


bool PolicyTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (m_buffManager && index.isValid() && role == Qt::EditRole)
    {
        int row                    = index.row();
        int col                    = index.column();
        const std::string strvalue = value.toString().toStdString();

        ::fwCore::mt::ReadLock lock( m_buffManager->getMutex() );
        ::fwMemory::IPolicy::sptr currentPolicy = m_buffManager->getDumpPolicy();
        const ::fwMemory::IPolicy::ParamNamesType &names = currentPolicy->getParamNames();

        if (col == 0 && (unsigned int)row <= names.size() )
        {
            ::fwMemory::IPolicy::sptr dumpPolicy;
            switch (row)
            {
                case 0:
                    if(strvalue != currentPolicy->getLeafClassname())
                    {
                        dumpPolicy = ::fwMemory::policy::registry::get()->create(strvalue);
                        if(dumpPolicy)
                        {
                            ::fwCore::mt::ReadToWriteLock lock( m_buffManager->getMutex() );
                            m_buffManager->setDumpPolicy(dumpPolicy);
                        }
                        this->beginResetModel();
                        this->endResetModel();
                    }
                    break;
                default:
                    const ::fwMemory::IPolicy::ParamNamesType::value_type &name = names.at(row - 1);
                    currentPolicy->setParam(name, strvalue);
                    return true;
            }
        }
    }
    return false;
}

Qt::ItemFlags PolicyTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}


//------------------------------------------------------------------------------


class InfoTableModel : public QAbstractTableModel
{

public:
    InfoTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:

    ::fwMemory::BufferManager::sptr m_buffManager;
};



InfoTableModel::InfoTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_buffManager = ::fwMemory::BufferManager::getDefault();
}

int InfoTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

int InfoTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant InfoTableModel::data(const QModelIndex &index, int role) const
{
    if (!m_buffManager || !index.isValid())
    {
        return QVariant();
    }

    if (index.row() > this->rowCount(index))
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            ::boost::uint64_t sysMem;
            ::fwMemory::BufferManager::SizeType bufferManagerMem;
            switch (index.row())
            {
                case 0:
                    sysMem = ::fwMemory::tools::MemoryMonitorTools::getTotalSystemMemory();
                    return QString(getHumanReadableSize(sysMem));
                    break;
                case 1:
                    sysMem = ::fwMemory::tools::MemoryMonitorTools::getFreeSystemMemory();
                    return QString(getHumanReadableSize(sysMem));
                    break;
                case 2:
                    bufferManagerMem = m_bufferStats.totalManaged;
                    return QString(getHumanReadableSize(bufferManagerMem));
                    break;
                case 3:
                    bufferManagerMem = m_bufferStats.totalDumped;
                    return QString(getHumanReadableSize(bufferManagerMem));
                    break;
            }
        }
    }
    return QVariant();
}


QVariant InfoTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Vertical)
    {
        switch (section)
        {
            case 0:
                return QString("Total System Memory");
                break;
            case 1:
                return QString("Free System Memory");
                break;
            case 2:
                return QString("Managed");
                break;
            case 3:
                return QString("Dumped");
                break;
        }

    }

    return QVariant();
}

//------------------------------------------------------------------------------


DumpEditor::DumpEditor() throw()
{
}

//------------------------------------------------------------------------------

DumpEditor::~DumpEditor() throw()
{
}

//------------------------------------------------------------------------------

void DumpEditor::starting() throw(::fwTools::Failed)
{
    this->::fwGui::IGuiContainerSrv::create();

    ::fwGuiQt::container::QtContainer::sptr qtContainer = ::fwGuiQt::container::QtContainer::dynamicCast(
        this->getContainer() );
    QWidget* const container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);

    m_updateTimer = new QTimer(container);
    m_updateTimer->setInterval(300);
    m_updateTimer->setSingleShot(true);

    m_list   = new QTableWidget(container);
    m_mapper = new QSignalMapper();

    m_list->setColumnCount(5);
    QStringList header;
    header.push_back("Size");
    header.push_back("Status");
    header.push_back("Timestamp");
    header.push_back("Locked");
    header.push_back("Action");
    m_list->setHorizontalHeaderLabels(header);

    m_refresh = new QPushButton(tr("Refresh"), container);
    QVBoxLayout* sizer = new QVBoxLayout();

    QHBoxLayout* sizerButton = new QHBoxLayout();
    sizerButton->addWidget(m_refresh);

    sizerButton->addItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    QFrame* verticalLine = new QFrame(container);
    verticalLine->setFrameShape(QFrame::VLine);
    verticalLine->setFrameShadow(QFrame::Sunken);
    sizerButton->addWidget(verticalLine);

    sizer->addLayout(sizerButton);
    sizer->addWidget(m_list,2);
    container->setLayout( sizer );

    PolicyComboBoxDelegate *policyComboBoxDelegate = new PolicyComboBoxDelegate(container);
    PolicyTableModel *policyTableModel             = new PolicyTableModel(container);
    m_policyEditor = new QTableView(container);
    m_policyEditor->setModel(policyTableModel);
    m_policyEditor->setItemDelegateForRow(0, policyComboBoxDelegate);
    m_policyEditor->setSortingEnabled(false);
    m_policyEditor->horizontalHeader()->hide();

    InfoTableModel *infoTableModel = new InfoTableModel(container);
    m_infoEditor = new QTableView(container);
    m_infoEditor->setModel(infoTableModel);
    m_infoEditor->horizontalHeader()->hide();


    QHBoxLayout* tablesLayout = new QHBoxLayout();
    tablesLayout->addWidget(m_infoEditor);
    tablesLayout->addWidget(m_policyEditor);

    sizer->addLayout(tablesLayout);
    // sizer->addWidget(m_policyEditor);

    QObject::connect(m_refresh, SIGNAL(clicked ()), this, SLOT(onRefreshButton( )));
    QObject::connect(m_mapper, SIGNAL(mapped(int)), this, SLOT(changeStatus(int)));

    QObject::connect(m_updateTimer, SIGNAL(timeout ()), this, SLOT(onRefreshButton( )));
    QObject::connect(&m_watcher, SIGNAL(finished()), this, SLOT(onBufferInfo()));

    ::fwMemory::BufferManager::sptr buffManager = ::fwMemory::BufferManager::getDefault();
    if (buffManager)
    {
        m_updateSlot                                        = ::fwCom::newSlot( &DumpEditor::onUpdate, this );
        ::fwServices::registry::ActiveWorkers::sptr workers = ::fwServices::registry::ActiveWorkers::getDefault();
        m_updateSlot->setWorker( workers->getWorker( ::fwServices::registry::ActiveWorkers::s_DEFAULT_WORKER ));
        m_connection = buffManager->getUpdatedSignal()->connect( m_updateSlot );
    }

    this->updating();
}

//------------------------------------------------------------------------------

void DumpEditor::stopping() throw(::fwTools::Failed)
{
    m_connection.disconnect();
    QObject::disconnect(m_refresh, SIGNAL(clicked ()), this, SLOT(onRefreshButton()));
    QObject::disconnect(m_mapper, SIGNAL(mapped(int)), this, SLOT(changeStatus(int)));
    QObject::disconnect(&m_watcher, SIGNAL(finished()), this, SLOT(onBufferInfo()));

    this->getContainer()->clean();
    this->::fwGui::IGuiContainerSrv::destroy();
}

//------------------------------------------------------------------------------

void DumpEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->::fwGui::IGuiContainerSrv::initialize();
}

//------------------------------------------------------------------------------


class SizeTableWidgetItem : public QTableWidgetItem
{
public:

    SizeTableWidgetItem(const QString &text) : QTableWidgetItem(text)
    {
    }

    virtual bool operator< ( const QTableWidgetItem & other ) const
    {
        return data(Qt::UserRole).toULongLong() < other.data(Qt::UserRole).toULongLong();
    }

protected:

    size_t m_size;
};

//------------------------------------------------------------------------------

::fwMemory::BufferManager::BufferInfoMapType getInfoMap()
{
    ::fwMemory::BufferManager::BufferInfoMapType infoMap;
    ::fwMemory::BufferManager::sptr buffManager = ::fwMemory::BufferManager::getDefault();
    if(buffManager)
    {
        infoMap = buffManager->getBufferInfos().get();
    }
    return infoMap;
}

//------------------------------------------------------------------------------

void DumpEditor::updating() throw(::fwTools::Failed)
{
    m_policyEditor->reset();
    m_policyEditor->resizeColumnsToContents();

    QFuture< ::fwMemory::BufferManager::BufferInfoMapType > qFuture = QtConcurrent::run(getInfoMap);
    m_watcher.setFuture(qFuture);
}

//------------------------------------------------------------------------------

void DumpEditor::onBufferInfo()
{
    m_bufferInfos = m_watcher.result();
    m_bufferStats = ::fwMemory::BufferManager::computeBufferStats(m_bufferInfos);

    m_mapper->blockSignals(true);
    ::fwCom::Connection::Blocker block(m_connection);

    for(int row = 0; row < m_list->rowCount(); row++)
    {
        m_mapper->removeMappings( m_list->cellWidget(row, 4) );
    }
    m_list->clearContents();
    m_objectsUID.clear();


    int itemCount = 0;
    m_list->setSortingEnabled(false);
    m_list->setRowCount(static_cast<int>(m_bufferInfos.size()));
    m_list->setColumnCount(5);
    QColor backColor;
    BOOST_FOREACH(const ::fwMemory::BufferManager::BufferInfoMapType::value_type &elt, m_bufferInfos)
    {
        m_objectsUID.push_back(elt.first);

        std::string status     = "?";
        std::string date       = "?";
        std::string lockStatus = "?";


        const ::fwMemory::BufferInfo &dumpBuffInfo = elt.second;
        bool loaded                                = dumpBuffInfo.loaded;
        if(!loaded)
        {
            backColor = Qt::darkYellow;
            status    = "Dumped";
        }
        else
        {
            backColor = Qt::white;
            status    = "-";
        }

        bool isLock = dumpBuffInfo.lockCount() > 0;
        if ( isLock )
        {
            lockStatus = "locked(" +  ::fwTools::getString(dumpBuffInfo.lockCount()) +")";
        }
        else
        {
            lockStatus = "unlocked";
        }


        date = ::fwTools::getString(dumpBuffInfo.lastAccess.getLogicStamp());

        QTableWidgetItem* currentSizeItem = new SizeTableWidgetItem( getHumanReadableSize(dumpBuffInfo.size) );
        currentSizeItem->setData(Qt::UserRole, (qulonglong)dumpBuffInfo.size );
        currentSizeItem->setFlags(Qt::ItemIsEnabled);
        currentSizeItem->setBackgroundColor(backColor);
        m_list->setItem(itemCount, 0, currentSizeItem );

        QTableWidgetItem* statusItem = new QTableWidgetItem( QString::fromStdString(status));
        statusItem->setFlags(Qt::ItemIsEnabled);
        statusItem->setBackgroundColor(backColor);
        m_list->setItem(itemCount, 1, statusItem );

        QTableWidgetItem* dateItem = new QTableWidgetItem( QString::fromStdString(date));
        dateItem->setFlags(Qt::ItemIsEnabled);
        dateItem->setBackgroundColor(backColor);
        m_list->setItem(itemCount, 2, dateItem );

        QTableWidgetItem* lockStatusItem = new QTableWidgetItem( QString::fromStdString(lockStatus));
        lockStatusItem->setFlags(Qt::ItemIsEnabled);
        lockStatusItem->setBackgroundColor(backColor);
        m_list->setItem(itemCount, 3, lockStatusItem );


        QPushButton* actionItem = new QPushButton(QString::fromStdString((loaded) ? "Dump" : "Restore"), m_list);
        actionItem->setEnabled(!isLock && (dumpBuffInfo.size > 0) );
        m_list->setCellWidget(itemCount, 4, actionItem );
        QObject::connect(actionItem, SIGNAL(pressed()), m_mapper, SLOT(map()));
        m_mapper->setMapping(actionItem, itemCount);

        ++itemCount;
    }
    m_list->setSortingEnabled(true);

    m_mapper->blockSignals(false);

    m_infoEditor->reset();
    m_infoEditor->resizeColumnsToContents();
}

//------------------------------------------------------------------------------

void DumpEditor::swapping() throw(::fwTools::Failed)
{
    this->updating();
}

//------------------------------------------------------------------------------

void DumpEditor::info( std::ostream &_sstream )
{
    _sstream << "Dump Editor";
}

//------------------------------------------------------------------------------

void DumpEditor::onUpdate()
{
    m_updateTimer->start();
}

//------------------------------------------------------------------------------

void DumpEditor::onRefreshButton()
{
    this->updating();
}

//------------------------------------------------------------------------------

void DumpEditor::changeStatus( int index )
{
    ::fwMemory::BufferManager::sptr buffManager = ::fwMemory::BufferManager::getDefault();
    if(buffManager)
    {
        const ::fwMemory::BufferManager::BufferInfoMapType buffInfoMap = m_bufferInfos;
        ::fwMemory::BufferManager::ConstBufferPtrType selectedBuffer = m_objectsUID[index];

        ::fwMemory::BufferManager::BufferInfoMapType::const_iterator iter;
        iter = buffInfoMap.find(selectedBuffer);
        if( iter != buffInfoMap.end())
        {
            ::fwGui::Cursor cursor;
            cursor.setCursor(::fwGui::ICursor::BUSY);
            const ::fwMemory::BufferInfo &dumpBuffInfo = iter->second;

            bool isLock = dumpBuffInfo.lockCount() > 0;
            if ( !isLock )
            {
                if ( dumpBuffInfo.loaded )
                {
                    buffManager->dumpBuffer(selectedBuffer);
                }
                else
                {
                    buffManager->restoreBuffer(selectedBuffer);
                }
            }
            else
            {
                ::fwGui::dialog::MessageDialog::showMessageDialog(
                    "Dump process information",
                    "Dump process is locked. It is impossible to dump or restore this object.",
                    ::fwGui::dialog::IMessageDialog::WARNING);
            }

            cursor.setDefaultCursor();

            this->updating();
        }
        else
        {
            std::stringstream stream;
            stream << "Object " << selectedBuffer << " not found, please refresh the grid.";
            ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Dump process information",
                stream.str(),
                ::fwGui::dialog::IMessageDialog::WARNING);
        }
    }
}

//------------------------------------------------------------------------------

}

