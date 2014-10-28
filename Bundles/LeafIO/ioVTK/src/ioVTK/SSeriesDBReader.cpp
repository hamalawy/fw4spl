/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/operations.hpp>

#include <fwMedData/SeriesDB.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwServices/ObjectMsg.hpp>

#include <fwData/mt/ObjectWriteLock.hpp>
#include <fwData/location/Folder.hpp>

#include <fwComEd/helper/SeriesDB.hpp>

#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/Cursor.hpp>

#include <fwGui/dialog/ProgressDialog.hpp>
#include <fwVtkIO/SeriesDBReader.hpp>

#include "ioVTK/SSeriesDBReader.hpp"


namespace ioVTK
{

fwServicesRegisterMacro( ::io::IReader , ::ioVTK::SSeriesDBReader , ::fwMedData::SeriesDB ) ;

//------------------------------------------------------------------------------

::io::IOPathType SSeriesDBReader::getIOPathType() const
{
    return ::io::FILES;
}

//------------------------------------------------------------------------------

void SSeriesDBReader::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath("");

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.setType(::fwGui::dialog::ILocationDialog::MULTI_FILES);
    dialogFile.setTitle("Choose vtk files to load Series");
    dialogFile.addFilter("Vtk","*.vtk *.vti *.mhd");
    dialogFile.addFilter("Vtk files","*.vtk");
    dialogFile.addFilter("Vti files","*.vti");
    dialogFile.addFilter("MetaImage files","*.mhd");
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::READ);
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::FILE_MUST_EXIST);

    ::fwData::location::MultiFiles::sptr  result;
    result = ::fwData::location::MultiFiles::dynamicCast( dialogFile.show() );
    if (result)
    {
        const ::fwData::location::ILocation::VectPathType paths = result->getPaths();
        if(!paths.empty())
        {
            _sDefaultPath = paths[0].parent_path();
            dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
        }
        this->setFiles(paths);
    }
    else
    {
        this->clearLocations();
    }
}

//------------------------------------------------------------------------------

void SSeriesDBReader::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SSeriesDBReader::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SSeriesDBReader::info(std::ostream &_sstream )
{
    _sstream << "SSeriesDBReader::info";
}

//------------------------------------------------------------------------------

void SSeriesDBReader::loadSeriesDB( const ::fwData::location::ILocation::VectPathType& vtkFiles,
                                    ::fwMedData::SeriesDB::sptr seriesDB )
{
    ::fwVtkIO::SeriesDBReader::sptr reader = ::fwVtkIO::SeriesDBReader::New();
    reader->setObject(seriesDB);
    reader->setFiles(vtkFiles);

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Loading SeriesDB");
        reader->addHandler( progressMeterGUI );
        reader->read();
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during loading : " << e.what();

        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning",
                ss.str(),
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        std::stringstream ss;
        ss << "Warning during loading. ";
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning",
                "Warning during loading.",
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
}

//------------------------------------------------------------------------------

void SSeriesDBReader::updating() throw(::fwTools::Failed)
{
    if( this->hasLocationDefined() )
    {
        // Retrieve dataStruct associated with this service
        ::fwMedData::SeriesDB::sptr seriesDB = this->getObject< ::fwMedData::SeriesDB >() ;
        SLM_ASSERT("SeriesDB not instanced", seriesDB);

        ::fwMedData::SeriesDB::sptr localSeriesDB = ::fwMedData::SeriesDB::New();

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);

        this->loadSeriesDB(this->getFiles(), localSeriesDB);

        ::fwComEd::helper::SeriesDB sDBhelper(seriesDB);

        ::fwData::mt::ObjectWriteLock lock(seriesDB);
        sDBhelper.merge(localSeriesDB);
        sDBhelper.notify(this->getSptr());

        cursor.setDefaultCursor();
    }
}

//------------------------------------------------------------------------------


} // namespace ioVtk
