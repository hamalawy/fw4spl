/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UITF_TRANSFERFUNCTIONEDITOR_HPP__
#define __UITF_TRANSFERFUNCTIONEDITOR_HPP__

#include <QObject>

#include <fwData/Composite.hpp>
#include <fwData/TransferFunction.hpp>

#include <gui/editor/IEditor.hpp>

#include "uiTF/config.hpp"

class QComboBox;
class QPushButton;
class QIcon;

namespace uiTF
{

/**
 * @brief   TransferFunctionEditor service.
 * @class   TransferFunctionEditor
 *
 * @date    2011.
 */
class UITF_CLASS_API TransferFunctionEditor : public QObject,
                                              public ::gui::editor::IEditor
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro ( (TransferFunctionEditor)(::gui::editor::IEditor) );

    /// Basic constructor, do nothing.
    UITF_API TransferFunctionEditor();

    /// Basic destructor, do nothing.
    UITF_API virtual ~TransferFunctionEditor() throw ();

protected:

    /**
     * @brief Configuring the transfer function editor.
     *
     * Example of configuration
     * @verbatim
       <service uid="GENERIC_UID_tfm" type="::gui::editor::IEditor" impl="::uiTF::TransferFunctionEditor" autoConnect="yes" >
        <config selectedTFKey="SelectedTF" tfSelectionFwID="TFSelections" />
       </service>
       @endverbatim
     * - \<image id="myImage" /\> : Set the link between the service and the associated image.
     *
     * \b id : mandatory (no default value) : set the id of the associated image.
     */
    UITF_API virtual void configuring() throw( ::fwTools::Failed );

    /// Start the TransferFunctionEditor, create Container, place in Buttons, ComboBox, Layout, and connect them.
    UITF_API virtual void starting() throw( ::fwTools::Failed );

    /// Update the TransferFunctionEditor, do nothing.
    UITF_API virtual void updating() throw( ::fwTools::Failed );

    /// Update the TransferFunctionEditor when message, do nothing.
    UITF_API virtual void receiving(::fwServices::ObjectMsg::csptr _msg) throw( ::fwTools::Failed );

    /// Stop the TransferFunctionEditor, disconnect Buttons and Combo Box, delete them and clean the container.
    UITF_API virtual void stopping() throw( ::fwTools::Failed );

    /// Initialize the transfer functions, get fields m_transferFunctionCompositeId and m_transferFunctionId associated to the related image, add their names to the ComboBox. If the image does not contain any TF, the create a few from the ressources of the Bundle.
    UITF_API void initTransferFunctions();

    /// Check if the image contain the specified TF.
    UITF_API bool hasTransferFunctionName(const std::string & _sName);

    /// Create a string that represents a TF name not already present in the image. For example, if blabla is already used, it will return blabla_1.
    UITF_API std::string createTransferFunctionName( const std::string & _sBasename );

    /// Update the image with the selected TF in the ComboBox.
    UITF_API void updateTransferFunction();

    /// Update the TF preset from the TF pool
    UITF_API void updateTransferFunctionPreset();

    /// Get the current transfer function pool
    UITF_API ::fwData::Composite::sptr getTFSelection() const;

    /// Get the current transfer function
    UITF_API ::fwData::TransferFunction::sptr getSelectedTransferFunction() const;

private Q_SLOTS:

    void deleteTF();
    void newTF();
    void reinitializeTFPool();
    void renameTF();
    void importTF();
    void exportTF();
    void presetChoice(int index);

private:

    QComboBox *m_pTransferFunctionPreset;
    QPushButton *m_deleteButton;
    QPushButton *m_newButton;
    QPushButton *m_reinitializeButton;
    QPushButton *m_renameButton;
    QPushButton *m_importButton;
    QPushButton *m_exportButton;
    QWidget* m_container;

    /// fwID of tf selection ( used during configuration )
    std::string m_tfSelectionFwID;

    /// Identifier of the key containing the current selection of TransferFunction in TFSelection.
    std::string m_selectedTFKey;
};

}
#endif //__UITF_TRANSFERFUNCTIONEDITOR_HPP__
