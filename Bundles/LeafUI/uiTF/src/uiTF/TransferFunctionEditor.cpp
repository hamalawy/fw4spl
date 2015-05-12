/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>

#include <fwRuntime/EConfigurationElement.hpp>

#include <QWidget>
#include <QBoxLayout>
#include <QComboBox>
#include <QString>
#include <QPushButton>
#include <QIcon>

#include <fwCore/base.hpp>

#include <fwData/Composite.hpp>
#include <fwData/TransferFunction.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <fwComEd/CompositeMsg.hpp>
#include <fwComEd/helper/Composite.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/InputDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include <io/IWriter.hpp>
#include <io/IReader.hpp>

#include "uiTF/TransferFunctionEditor.hpp"

namespace uiTF
{

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

fwServicesRegisterMacro( ::gui::editor::IEditor, ::uiTF::TransferFunctionEditor, ::fwData::Composite );

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

TransferFunctionEditor::TransferFunctionEditor() : m_selectedTFKey("")
{
//    this->addNewHandledEvent(::fwComEd::CompositeMsg::CHANGED_KEYS);
//    this->addNewHandledEvent(::fwComEd::CompositeMsg::ADDED_KEYS);
//    this->addNewHandledEvent(::fwComEd::CompositeMsg::REMOVED_KEYS);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

TransferFunctionEditor::~TransferFunctionEditor() throw ()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::configuring() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    this->initialize();
    ::fwRuntime::ConfigurationElement::sptr configuration = m_configuration->findConfigurationElement("config");
    SLM_ASSERT("Sorry, analyzed configuration is not conformed.", configuration);

    SLM_ASSERT("Sorry, analyzed configuration is not conformed.", configuration->getName() == "config");
    if ( configuration->hasAttribute("selectedTFKey") )
    {
        m_selectedTFKey = configuration->getAttributeValue("selectedTFKey");
        SLM_FATAL_IF("'selectedTFKey' must not be empty", m_selectedTFKey.empty());
    }
    if ( configuration->hasAttribute("tfSelectionFwID") )
    {
        m_tfSelectionFwID = configuration->getAttributeValue("tfSelectionFwID");
        SLM_FATAL_IF("'tfSelectionFwID' must not be empty", m_tfSelectionFwID.empty());
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::starting() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    this->create();

    // Get the Qt container
    ::fwGuiQt::container::QtContainer::sptr qtContainer = ::fwGuiQt::container::QtContainer::dynamicCast(
        this->getContainer());
    m_container = qtContainer->getQtContainer();
    SLM_ASSERT("Sorry, the qt container is not valid", m_container);

    // Buttons creation
    m_pTransferFunctionPreset = new QComboBox(m_container);

    ::boost::filesystem::path deletePath ("Bundles/uiTF_" + std::string(UITF_VER) + "/delete.png");
    m_deleteButton = new QPushButton(QIcon(deletePath.string().c_str()), "", m_container);
    m_deleteButton->setToolTip(QString("Delete"));

    ::boost::filesystem::path newPath ("Bundles/uiTF_" + std::string(UITF_VER) + "/new.png");
    m_newButton = new QPushButton(QIcon(newPath.string().c_str()), "", m_container);
    m_newButton->setToolTip(QString("New"));

    ::boost::filesystem::path reinitializePath ("Bundles/uiTF_" + std::string(UITF_VER) + "/reinitialize.png");
    m_reinitializeButton = new QPushButton(QIcon(reinitializePath.string().c_str()), "", m_container);
    m_reinitializeButton->setToolTip(QString("Reinitialize"));

    ::boost::filesystem::path renamePath ("Bundles/uiTF_" + std::string(UITF_VER) + "/rename.png");
    m_renameButton = new QPushButton(QIcon(renamePath.string().c_str()), "", m_container);
    m_renameButton->setToolTip(QString("Rename"));

    ::boost::filesystem::path importPath ("Bundles/uiTF_" + std::string(UITF_VER) + "/import.png");
    m_importButton = new QPushButton(QIcon(importPath.string().c_str()), "", m_container);
    m_importButton->setToolTip(QString("Import"));

    ::boost::filesystem::path exportPath ("Bundles/uiTF_" + std::string(UITF_VER) + "/export.png");
    m_exportButton = new QPushButton(QIcon(exportPath.string().c_str()), "", m_container);
    m_exportButton->setToolTip(QString("Export"));

    // Layout management
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight, m_container);

    layout->addWidget(m_pTransferFunctionPreset);
    layout->addWidget(m_deleteButton);
    layout->addWidget(m_newButton);
    layout->addWidget(m_reinitializeButton);
    layout->addWidget(m_renameButton);
    layout->addWidget(m_importButton);
    layout->addWidget(m_exportButton);

    m_container->setLayout(layout);

    // Qt signals management ( connection to buttons )
    QObject::connect(m_pTransferFunctionPreset, SIGNAL(   activated(int)), this, SLOT(presetChoice(int)));
    QObject::connect(m_deleteButton, SIGNAL(   clicked()), this, SLOT(deleteTF()));
    QObject::connect(m_newButton, SIGNAL(   clicked()), this, SLOT(newTF()));
    QObject::connect(m_reinitializeButton, SIGNAL(   clicked()), this, SLOT(reinitializeTFPool()));
    QObject::connect(m_renameButton, SIGNAL(   clicked()), this, SLOT(renameTF()));
    QObject::connect(m_importButton, SIGNAL(   clicked()), this, SLOT(importTF()));
    QObject::connect(m_exportButton, SIGNAL(   clicked()), this, SLOT(exportTF()));

    // preset initialization
    this->initTransferFunctions();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::updating() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::receiving(::fwServices::ObjectMsg::csptr msg) throw( ::fwTools::Failed )
{
    ::fwComEd::CompositeMsg::csptr compositeMsg = ::fwComEd::CompositeMsg::dynamicConstCast(msg);
    if(compositeMsg &&
       ( compositeMsg->hasEvent(::fwComEd::CompositeMsg::CHANGED_KEYS) ||
         compositeMsg->hasEvent(::fwComEd::CompositeMsg::ADDED_KEYS)   ||
         compositeMsg->hasEvent(::fwComEd::CompositeMsg::REMOVED_KEYS) ) )
    {
        this->updateTransferFunctionPreset();
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::stopping() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    // Qt signals management ( disconnection )
    QObject::disconnect(m_pTransferFunctionPreset, SIGNAL(   activated(int)), this, SLOT(presetChoice(int)));
    QObject::disconnect(m_deleteButton, SIGNAL(   clicked()), this, SLOT(deleteTF()));
    QObject::disconnect(m_newButton, SIGNAL(   clicked()), this, SLOT(newTF()));
    QObject::disconnect(m_reinitializeButton, SIGNAL(   clicked()), this, SLOT(reinitializeTFPool()));
    QObject::disconnect(m_renameButton, SIGNAL(   clicked()), this, SLOT(renameTF()));
    QObject::disconnect(m_importButton, SIGNAL(   clicked()), this, SLOT(importTF()));
    QObject::disconnect(m_exportButton, SIGNAL(   clicked()), this, SLOT(exportTF()));

    // deletes contained widgets
    ::fwGuiQt::container::QtContainer::sptr qtContainer = ::fwGuiQt::container::QtContainer::dynamicCast(
        this->getContainer());
    qtContainer->clean();

    this->destroy();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::presetChoice(int index)
{
    SLM_TRACE_FUNC();
    m_pTransferFunctionPreset->setCurrentIndex(index);
    this->updateTransferFunction();

    std::string tfName = m_pTransferFunctionPreset->currentText().toStdString();
    bool isEnabled     = (tfName != ::fwData::TransferFunction::s_DEFAULT_TF_NAME);

    m_renameButton->setEnabled(isEnabled);
    m_deleteButton->setEnabled(isEnabled);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::deleteTF()
{
    ::fwGui::dialog::MessageDialog messageBox;
    messageBox.setTitle("Deleting confirmation");
    messageBox.setMessage("Are you sure you want to delete this transfer function?");
    messageBox.setIcon(::fwGui::dialog::IMessageDialog::QUESTION);
    messageBox.addButton(::fwGui::dialog::IMessageDialog::YES);
    messageBox.addButton(::fwGui::dialog::IMessageDialog::CANCEL);
    ::fwGui::dialog::IMessageDialog::Buttons answerCopy = messageBox.show();

    if (answerCopy != ::fwGui::dialog::IMessageDialog::CANCEL)
    {
        ::fwData::Composite::sptr poolTF = this->getObject< ::fwData::Composite > ();

        if( poolTF->size() > 1 )
        {
            int indexSelectedTF       = m_pTransferFunctionPreset->currentIndex();
            std::string selectedTFKey = m_pTransferFunctionPreset->currentText().toStdString();

            ::fwData::Composite::sptr poolTF = this->getObject< ::fwData::Composite >();

            ::fwComEd::helper::Composite compositeHelper(poolTF);
            OSLM_ASSERT("TF "<< m_selectedTFKey <<" missing in pool", this->hasTransferFunctionName(selectedTFKey));
            compositeHelper.remove(selectedTFKey);
            compositeHelper.notify(this->getSptr());

            m_pTransferFunctionPreset->removeItem(indexSelectedTF);
            std::string defaultTFName = ::fwData::TransferFunction::s_DEFAULT_TF_NAME;

            int index = m_pTransferFunctionPreset->findText(QString::fromStdString(defaultTFName));
            index = std::max(index, 0);
            this->presetChoice(index);
        }
        else
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning",
                "You can not remove this transfer function because the program requires at least one.",
                ::fwGui::dialog::IMessageDialog::WARNING );
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::newTF()
{
    std::string newName = this->getSelectedTransferFunction()->getName();
    if( this->hasTransferFunctionName(newName) )
    {
        newName = this->createTransferFunctionName(newName);
    }

    fwGui::dialog::InputDialog inputDialog;
    inputDialog.setTitle("Creating transfer function");
    inputDialog.setMessage("Transfer function name:");
    inputDialog.setInput( newName );
    newName = inputDialog.getInput();

    if (!newName.empty())
    {
        if(!this->hasTransferFunctionName(newName))
        {
            ::fwData::TransferFunction::sptr pNewTransferFunction;
            ::fwData::TransferFunction::sptr selectedTF = this->getSelectedTransferFunction();

            pNewTransferFunction = ::fwData::Object::copy(selectedTF);
            pNewTransferFunction->setName(newName);
            ::fwData::Composite::sptr poolTF = this->getObject< ::fwData::Composite >();
            ::fwComEd::helper::Composite compositeHelper(poolTF);
            compositeHelper.add(newName, pNewTransferFunction);

            m_pTransferFunctionPreset->addItem(QString(newName.c_str()));
            m_pTransferFunctionPreset->setCurrentIndex(m_pTransferFunctionPreset->count()-1);
            this->updateTransferFunction();

            compositeHelper.notify(this->getSptr());
        }
        else
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning",
                "This transfer function name already exists so you can not overwrite it.",
                ::fwGui::dialog::IMessageDialog::WARNING);
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::reinitializeTFPool()
{
    ::fwGui::dialog::MessageDialog messageBox;
    messageBox.setTitle("Reinitializing confirmation");
    messageBox.setMessage("Are you sure you want to reinitialize all transfer functions?");
    messageBox.setIcon(::fwGui::dialog::IMessageDialog::QUESTION);
    messageBox.addButton(::fwGui::dialog::IMessageDialog::YES);
    messageBox.addButton(::fwGui::dialog::IMessageDialog::CANCEL);
    ::fwGui::dialog::IMessageDialog::Buttons answerCopy = messageBox.show();

    if (answerCopy != ::fwGui::dialog::IMessageDialog::CANCEL)
    {
        ::fwData::Composite::sptr tfPool = this->getObject< ::fwData::Composite > ();

        ::fwComEd::helper::Composite compositeHelper(tfPool);
        compositeHelper.clear();
        compositeHelper.notify(this->getSptr());

        this->initTransferFunctions();

        this->updateTransferFunction();
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::renameTF()
{
    if ( m_selectedTFKey.find("STD") != std::string::npos )
    {
        ::fwGui::dialog::MessageDialog messageBox;
        messageBox.setTitle("Warning");
        messageBox.setMessage("STD transfer function can not be renamed.");
        messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
        messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
        messageBox.show();
        return;
    }

    std::string str = m_pTransferFunctionPreset->currentText().toStdString();
    std::string newName (str);

    fwGui::dialog::InputDialog inputDialog;
    inputDialog.setTitle("Creating transfer function");
    inputDialog.setMessage("Transfer function name:");
    inputDialog.setInput( newName );
    newName = inputDialog.getInput();

    if (    !newName.empty() &&
            newName != str)
    {
        if( !this->hasTransferFunctionName(newName) )
        {

            ::fwData::Composite::sptr tfPool = this->getObject< ::fwData::Composite >();
            ::fwData::TransferFunction::sptr pTF;
            pTF = ::fwData::TransferFunction::dynamicCast((*tfPool)[str]);
            pTF->setName(newName);

            ::fwComEd::helper::Composite compositeHelper(tfPool);
            compositeHelper.remove(str);
            compositeHelper.add(newName, pTF);
            compositeHelper.notify(this->getSptr());

            m_pTransferFunctionPreset->setItemText(m_pTransferFunctionPreset->currentIndex(), QString(newName.c_str()));
            m_pTransferFunctionPreset->setCurrentIndex(m_pTransferFunctionPreset->findText(QString(newName.c_str())));

            this->updateTransferFunction();
        }
        else
        {
            ::fwGui::dialog::MessageDialog messageBox;
            messageBox.setTitle("Warning");
            messageBox.setMessage("This transfer function name already exists so you can not overwrite it.");
            messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
            messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
            messageBox.show();
        }
    }
    if ( newName.empty() )
    {
        ::fwGui::dialog::MessageDialog messageBox;
        messageBox.setTitle("Warning");
        messageBox.setMessage("You have to give a name to your transfer function.");
        messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
        messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
        messageBox.show();
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::importTF()
{
    ::fwData::Composite::sptr tfPool = this->getObject< ::fwData::Composite >();
    ::fwComEd::helper::Composite compositeHelper(tfPool);

    ::fwData::TransferFunction::sptr tf = ::fwData::TransferFunction::New();
    ::fwServices::IService::sptr srv    =
        ::fwServices::registry::ServiceFactory::getDefault()->create("::ioAtoms::SReader");

    ::fwServices::OSR::registerService(tf, srv);

    ::io::IReader::sptr reader = ::io::IReader::dynamicCast(srv);
    reader->start();
    reader->configureWithIHM();
    reader->update();
    reader->stop();
    ::fwServices::OSR::unregisterService(srv);

    if (!tf->getName().empty())
    {
        if( this->hasTransferFunctionName( tf->getName() ) )
        {
            tf->setName( this->createTransferFunctionName( tf->getName() ) );
        }

        compositeHelper.add(tf->getName(), tf);
        m_pTransferFunctionPreset->addItem(QString(tf->getName().c_str()));
        this->presetChoice((*tfPool).size()-1);

        compositeHelper.notify(this->getSptr());
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::exportTF()
{
    SLM_TRACE_FUNC();

    if (m_selectedTFKey.find("STD")!=std::string::npos)
    {
        ::fwGui::dialog::MessageDialog messageBox;
        messageBox.setTitle("Warning");
        messageBox.setMessage("STD transfer function can not be exported.");
        messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
        messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
        messageBox.show();
        return;
    }
    ::fwData::TransferFunction::sptr tf = this->getSelectedTransferFunction();
    ::fwServices::IService::sptr srv    =
        ::fwServices::registry::ServiceFactory::getDefault()->create("::ioAtoms::SWriter");

    ::fwServices::OSR::registerService(tf, srv);

    ::io::IWriter::sptr writer = ::io::IWriter::dynamicCast(srv);
    writer->start();
    writer->configureWithIHM();
    writer->update();
    writer->stop();
    ::fwServices::OSR::unregisterService(srv);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::initTransferFunctions()
{
    // Get transfer function composite (pool TF)
    ::fwData::Composite::sptr poolTF = this->getObject< ::fwData::Composite >( );

    ::fwComEd::helper::Composite compositeHelper(poolTF);

    const std::string defaultTFName = ::fwData::TransferFunction::s_DEFAULT_TF_NAME;
    if(!this->hasTransferFunctionName(defaultTFName))
    {
        ::fwData::TransferFunction::sptr defaultTf = ::fwData::TransferFunction::createDefaultTF();
        defaultTf->setWindow( 50. );
        defaultTf->setLevel( 500. );
        compositeHelper.add(defaultTFName, defaultTf);
    }

    // Test if transfer function composite has few TF
    if( poolTF->size() <= 1 )
    {
        // Parse all TF contained in uiTF Bundle's resources
        std::vector< ::boost::filesystem::path > paths;
        ::boost::filesystem::path pathRoot ("Bundles/uiTF_" + std::string(UITF_VER) + "/tf");
        for(::boost::filesystem::directory_iterator it(pathRoot);
            it != ::boost::filesystem::directory_iterator();
            ++it )
        {
            if(!::boost::filesystem::is_directory(*it) &&
               ::boost::filesystem::extension(*it) == ".json")
            {
                paths.push_back(*it);
            }
        }

        ::fwData::TransferFunction::sptr tf = ::fwData::TransferFunction::New();
        ::fwServices::IService::sptr srv    =
            ::fwServices::registry::ServiceFactory::getDefault()->create("::ioAtoms::SReader");
        ::fwServices::OSR::registerService(tf, srv);
        ::io::IReader::sptr reader = ::io::IReader::dynamicCast(srv);

        ::fwRuntime::EConfigurationElement::sptr srvCfg  = ::fwRuntime::EConfigurationElement::New("service");
        ::fwRuntime::EConfigurationElement::sptr fileCfg = ::fwRuntime::EConfigurationElement::New("file");
        srvCfg->addConfigurationElement(fileCfg);

        BOOST_FOREACH( ::boost::filesystem::path file, paths )
        {
            fileCfg->setValue(file.string());
            reader->setConfiguration(srvCfg);
            reader->configure();
            reader->start();
            reader->update();
            reader->stop();

            if (!tf->getName().empty())
            {
                ::fwData::TransferFunction::sptr newTF = ::fwData::Object::copy< ::fwData::TransferFunction >(tf);
                if( this->hasTransferFunctionName( newTF->getName() ) )
                {
                    newTF->setName( this->createTransferFunctionName( newTF->getName() ) );
                }

                compositeHelper.add(newTF->getName(), newTF);
            }
            tf->initTF();
        }
        ::fwServices::OSR::unregisterService(srv);
    }
    compositeHelper.notify(this->getSptr());

    this->updateTransferFunctionPreset();
}

//---------------------------------------------------------------------------------------------------------------------


void TransferFunctionEditor::updateTransferFunctionPreset()
{
    ::fwData::Composite::sptr poolTF = this->getObject< ::fwData::Composite >( );
    const std::string defaultTFName = ::fwData::TransferFunction::s_DEFAULT_TF_NAME;
    // Manage TF preset
    m_pTransferFunctionPreset->clear();
    BOOST_FOREACH(::fwData::Composite::value_type elt, *poolTF)
    {
        m_pTransferFunctionPreset->addItem( elt.first.c_str() );
    }

    int index = m_pTransferFunctionPreset->findText( QString::fromStdString(defaultTFName) );
    index                                       = std::max(index, 0);
    ::fwData::TransferFunction::sptr selectedTF = this->getSelectedTransferFunction();
    if(selectedTF)
    {
        std::string tfName = selectedTF->getName();
        int tmpIdx         = m_pTransferFunctionPreset->findText(QString::fromStdString(tfName));
        if(tmpIdx >= 0)
        {
            index = tmpIdx;
        }
    }
    this->presetChoice(index);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

bool TransferFunctionEditor::hasTransferFunctionName(const std::string & _sName)
{
    ::fwData::Composite::sptr poolTf = this->getObject< ::fwData::Composite >();
    return poolTf->find(_sName) != poolTf->end();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

std::string TransferFunctionEditor::createTransferFunctionName(const std::string & _sBasename)
{
    bool bHasTransferFunctionName = true;
    std::string newName           = _sBasename;
    int cpt                       = 1;
    while (bHasTransferFunctionName)
    {
        std::stringstream tmpStr;
        tmpStr <<  _sBasename <<  "_" <<  cpt;
        newName                  = tmpStr.str();
        bHasTransferFunctionName = this->hasTransferFunctionName(newName);
        cpt++;
    }

    return newName;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void TransferFunctionEditor::updateTransferFunction()
{
    std::string newSelectedTFKey = m_pTransferFunctionPreset->currentText().toStdString();
    OSLM_DEBUG("Transfer function selected : " <<  newSelectedTFKey);

    ::fwData::Composite::sptr tfSelection = this->getTFSelection();

    OSLM_ASSERT("TF "<< newSelectedTFKey <<" missing in pool", this->hasTransferFunctionName(newSelectedTFKey));
    ::fwData::Composite::sptr poolTF     = this->getObject< ::fwData::Composite >();
    ::fwData::Object::sptr newSelectedTF = (*poolTF)[newSelectedTFKey];
    if(this->getSelectedTransferFunction() != newSelectedTF)
    {
        ::fwComEd::helper::Composite compositeHelper(tfSelection);
        if(tfSelection->getContainer().find(m_selectedTFKey) != tfSelection->getContainer().end())
        {
            compositeHelper.swap(m_selectedTFKey, newSelectedTF);
        }
        else
        {
            compositeHelper.add(m_selectedTFKey, newSelectedTF);
        }
        compositeHelper.notify(this->getSptr());
    }
}

//------------------------------------------------------------------------------

::fwData::Composite::sptr TransferFunctionEditor::getTFSelection() const
{
    ::fwData::Composite::sptr tfSelection =
        ::fwData::Composite::dynamicCast( ::fwTools::fwID::getObject( m_tfSelectionFwID ) );
    return tfSelection;
}

//------------------------------------------------------------------------------

::fwData::TransferFunction::sptr TransferFunctionEditor::getSelectedTransferFunction() const
{
    ::fwData::Composite::sptr tfSelection = this->getTFSelection();
    return ::fwData::TransferFunction::dynamicCast((*tfSelection)[m_selectedTFKey]);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

} // end namespace
