/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/operations.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/ObjectMsg.hpp>

#include <fwCore/base.hpp>

#include <fwData/Mesh.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwData/location/Folder.hpp>

#include <fwMedData/ModelSeries.hpp>

#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/ILocationDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/dialog/ProgressDialog.hpp>

#include <fwTools/UUID.hpp>

#include <fwVtkIO/ModelSeriesObjWriter.hpp>

#include <fwVtkIO/ModelSeriesObjWriter.hpp>

#include "ioVTK/MeshWriterService.hpp"
#include "ioVTK/SModelSeriesObjWriter.hpp"


namespace ioVTK
{

fwServicesRegisterMacro( ::io::IWriter, ::ioVTK::SModelSeriesObjWriter, ::fwMedData::ModelSeries );

//------------------------------------------------------------------------------

::io::IOPathType SModelSeriesObjWriter::getIOPathType() const
{
    return ::io::FOLDER;
}

//------------------------------------------------------------------------------

void SModelSeriesObjWriter::configureWithIHM()
{
    SLM_TRACE_FUNC();
    static ::boost::filesystem::path _sDefaultPath("");

    ::fwGui::dialog::LocationDialog dialog;
    dialog.setTitle("Choose a directory to save meshes");
    dialog.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialog.setOption(::fwGui::dialog::ILocationDialog::WRITE);
    dialog.setType(::fwGui::dialog::ILocationDialog::FOLDER);

    ::fwData::location::Folder::sptr result;

    while (result = ::fwData::location::Folder::dynamicCast( dialog.show() ))
    {
        if( ::boost::filesystem::is_empty(result->getFolder()) )
        {
            break;
        }
        // message box
        ::fwGui::dialog::MessageDialog messageBox;
        messageBox.setTitle("Overwrite confirmation");
        messageBox.setMessage("The selected directory is not empty. Write anyway ?");
        messageBox.setIcon(::fwGui::dialog::IMessageDialog::QUESTION);
        messageBox.addButton(::fwGui::dialog::IMessageDialog::YES);
        messageBox.addButton(::fwGui::dialog::IMessageDialog::CANCEL);
        if( messageBox.show() == ::fwGui::dialog::IMessageDialog::YES)
        {
            break;
        }
    }

    if (result)
    {
        _sDefaultPath = result->getFolder().parent_path();
        dialog.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
        this->setFolder(result->getFolder());
    }
    else
    {
        this->clearLocations();
    }
}

//------------------------------------------------------------------------------

void SModelSeriesObjWriter::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SModelSeriesObjWriter::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SModelSeriesObjWriter::info(std::ostream &_sstream )
{
    _sstream << "SModelSeriesObjWriter::info";
}

//------------------------------------------------------------------------------

void SModelSeriesObjWriter::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    if(  this->hasLocationDefined() )
    {
        // Retrieve dataStruct associated with this service
        ::fwMedData::ModelSeries::sptr modelSeries = this->getObject< ::fwMedData::ModelSeries >();
        SLM_ASSERT("ModelSeries is not instanced", modelSeries);

        ::fwVtkIO::ModelSeriesObjWriter::sptr writer = ::fwVtkIO::ModelSeriesObjWriter::New();
        writer->setObject(modelSeries);
        writer->setFolder(this->getFolder());

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);

        try
        {
            ::fwGui::dialog::ProgressDialog progressMeterGUI("Saving reconstructions...");
            writer->addHandler( progressMeterGUI );
            writer->write();

        }
        catch (const std::exception & e)
        {
            std::stringstream ss;
            ss << "Warning during saving : " << e.what();

            ::fwGui::dialog::MessageDialog messageBox;
            messageBox.setTitle("Warning");
            messageBox.setMessage( ss.str() );
            messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
            messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
            messageBox.show();
        }
        catch( ... )
        {
            std::stringstream ss;
            ss << "Warning during saving.";

            ::fwGui::dialog::MessageDialog messageBox;
            messageBox.setTitle("Warning");
            messageBox.setMessage( ss.str() );
            messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
            messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
            messageBox.show();
        }

        cursor.setDefaultCursor();
    }
}

//------------------------------------------------------------------------------

} // namespace ioVtk

