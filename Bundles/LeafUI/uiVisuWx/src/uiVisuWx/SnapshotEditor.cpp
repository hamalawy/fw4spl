/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <wx/wx.h>
#include <wx/window.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/event.h>
#include <wx/filedlg.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>

#include <fwCore/base.hpp>

#include <fwTools/Object.hpp>

#include <fwData/String.hpp>
#include <fwData/Composite.hpp>
#include <fwData/location/SingleFile.hpp>
#include <fwData/location/Folder.hpp>

#include <fwComEd/CompositeMsg.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IService.hpp>
#include <fwServices/op/Get.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwWX/convert.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>

#include <fwGuiWx/container/WxContainer.hpp>

#include "uiVisuWx/SnapshotEditor.hpp"

namespace uiVisu
{

REGISTER_SERVICE( ::gui::editor::IEditor , ::uiVisu::SnapshotEditor , ::fwData::Object ) ;


SnapshotEditor::SnapshotEditor() throw()
{
    m_idSnapButton = wxNewId();
}

//------------------------------------------------------------------------------

SnapshotEditor::~SnapshotEditor() throw()
{}

//------------------------------------------------------------------------------

void SnapshotEditor::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->create();

    ::fwGuiWx::container::WxContainer::sptr wxContainer =  ::fwGuiWx::container::WxContainer::dynamicCast( this->getContainer() );
    wxWindow* container = wxContainer->getWxContainer();
    SLM_ASSERT("container not instanced", container);

    namespace fs = ::boost::filesystem;
    fs::path pathImageSnap ("Bundles/uiVisuWx_" + std::string(UIVISUWX_VER) + "/camera-photo.png");
    OSLM_ASSERT("Image "<< pathImageSnap << "is missing", fs::exists(pathImageSnap));
    wxString filenameSnap ( ::fwWX::std2wx(pathImageSnap.string() ) );
    wxImage imageSnap;
    imageSnap.LoadFile(filenameSnap);
    wxBitmapButton* snapButton = new wxBitmapButton( container, m_idSnapButton, imageSnap, wxDefaultPosition, wxSize(25,-1) ) ;
    snapButton->SetToolTip(_("Snapshot"));

    wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
    sizer->Add( snapButton, 1, wxALL|wxEXPAND, 1 );

    container->SetSizer( sizer );
    container->Layout();

    container->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &SnapshotEditor::onSnapButton, this,  m_idSnapButton);
}

//------------------------------------------------------------------------------

void SnapshotEditor::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::fwGuiWx::container::WxContainer::sptr wxContainer =  ::fwGuiWx::container::WxContainer::dynamicCast( this->getContainer() );
    wxWindow* const container = wxContainer->getWxContainer();
    SLM_ASSERT("container not instanced", container);
    container->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &SnapshotEditor::onSnapButton, this, m_idSnapButton);

    wxContainer->clean();
    this->destroy();
}

//------------------------------------------------------------------------------

void SnapshotEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    this->initialize();

    std::vector < Configuration > snapConfig = m_configuration->find("snap");
    if(!snapConfig.empty())
    {
        std::vector < Configuration > sceneConfig = snapConfig.at(0)->find("scene");
        for(unsigned int i =0; i < sceneConfig.size(); i++)
        {
            OSLM_ASSERT("Wrong tag name: " << sceneConfig.at(i)->getName(), sceneConfig.at(i)->getName() == "scene");
            SLM_ASSERT("UID attribute is missing", sceneConfig.at(i)->hasAttribute("uid"));

            std::string value(sceneConfig.at(i)->getAttributeValue("uid"));
            m_scenesUID.push_back(value);
        }
    }
}

//------------------------------------------------------------------------------

void SnapshotEditor::updating() throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void SnapshotEditor::swapping() throw(::fwTools::Failed)
{

}
//------------------------------------------------------------------------------

void SnapshotEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void SnapshotEditor::info( std::ostream &_sstream )
{
}

//------------------------------------------------------------------------------

void SnapshotEditor::onSnapButton( wxCommandEvent& event )
{
    SLM_TRACE_FUNC();
    ::fwGuiWx::container::WxContainer::sptr wxContainer =  ::fwGuiWx::container::WxContainer::dynamicCast( this->getContainer() );
    wxWindow* const container = wxContainer->getWxContainer();
    SLM_ASSERT("container not instanced", container);
    if( container->IsShownOnScreen() )
    {
        for(unsigned int i=0; i < m_scenesUID.size(); i++)
        {
            ::fwServices::IService::sptr service = ::fwServices::get(m_scenesUID.at(i));
            ::fwData::Composite::sptr composite = service->getObject< ::fwData::Composite >();
            SLM_ASSERT("SnapshotEditor sceneUID " << m_scenesUID.at(i) <<" isn't a GenericScene?" , composite);

            ::fwData::Object::NewSptr dataInfo;

            ::fwData::String::NewSptr sceneID;
            sceneID->value() = m_scenesUID.at(i);
            ::fwData::String::NewSptr filename;

            filename->value() = this->requestFileName();
            if(!filename->value().empty())
            {
                dataInfo->setField("sceneID", sceneID);
                dataInfo->setField("filename", filename);
                ::fwComEd::CompositeMsg::NewSptr compositeMsg;
                compositeMsg->addEvent( "SNAP", dataInfo );
                ::fwServices::IEditionService::notify(this->getSptr(), composite, compositeMsg);
            }
        }
    }
    else
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Negato view snapshot",
                "Sorry, it is not possible to snapshot the negato view. This view is not shown on screen.",
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
}

//------------------------------------------------------------------------------

std::string SnapshotEditor::requestFileName()
{
    std::string fileName = "";

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle("Save snapshot as");
    dialogFile.addFilter("Image file","*.jpg *.jpeg *.bmp *.png *.tiff");
    dialogFile.addFilter("jpeg","*.jpg *.jpeg");
    dialogFile.addFilter("bmp","*.bmp");
    dialogFile.addFilter("png","*.png");
    dialogFile.addFilter("tiff","*.tiff");
    dialogFile.addFilter("all","*.*");
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);

    ::fwData::location::SingleFile::sptr  result;
    result= ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );
    if (result)
    {
        fileName = result->getPath().string();
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(result->getPath().parent_path()) );
    }

    return fileName;
}
//------------------------------------------------------------------------------

}

