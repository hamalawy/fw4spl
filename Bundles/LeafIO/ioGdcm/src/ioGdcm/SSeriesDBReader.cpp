/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/bind.hpp>



#include <fwCore/base.hpp>

#include <fwTools/ProgressToLogger.hpp>

#include <fwComEd/helper/SeriesDB.hpp>

#include <fwData/location/Folder.hpp>
#include <fwData/mt/ObjectWriteLock.hpp>

#include <fwMedData/SeriesDB.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/PulseProgressDialog.hpp>

#include <io/IReader.hpp>

#include <gdcmIO/reader/SeriesDB.hpp>

#include "ioGdcm/SSeriesDBReader.hpp"


namespace ioGdcm
{

fwServicesRegisterMacro( ::io::IReader , ::ioGdcm::SSeriesDBReader , ::fwMedData::SeriesDB ) ;

//------------------------------------------------------------------------------

SSeriesDBReader::SSeriesDBReader() throw()
{}

//------------------------------------------------------------------------------

SSeriesDBReader::~SSeriesDBReader() throw()
{}

//------------------------------------------------------------------------------

::io::IOPathType SSeriesDBReader::getIOPathType() const
{
    return ::io::FOLDER;
}

//------------------------------------------------------------------------------

void SSeriesDBReader::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle(this->getSelectorDialogTitle());
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::READ);
    dialogFile.setType(::fwGui::dialog::LocationDialog::FOLDER);

    ::fwData::location::Folder::sptr  result;
    result = ::fwData::location::Folder::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath  = result->getFolder();
        this->setFolder( _sDefaultPath );
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
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
    _sstream << "SSeriesDBReader::info" ;
}

//------------------------------------------------------------------------------

std::vector< std::string > SSeriesDBReader::getSupportedExtensions()
{
    std::vector< std::string > extensions ;
    return extensions ;
}

//------------------------------------------------------------------------------

std::string SSeriesDBReader::getSelectorDialogTitle()
{
    return "Choose a directory with DICOM images";
}

//------------------------------------------------------------------------------

::fwMedData::SeriesDB::sptr SSeriesDBReader::createSeriesDB( const ::boost::filesystem::path & dicomFile)
{
    SLM_TRACE_FUNC();
    ::gdcmIO::reader::SeriesDB::sptr reader = ::gdcmIO::reader::SeriesDB::New();
    ::fwMedData::SeriesDB::sptr dummy = ::fwMedData::SeriesDB::New();

    reader->setObject(dummy);
    reader->setFolder(dicomFile);

    try
    {
        reader->read();
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during loading : " << e.what();
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning", ss.str(), ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning", "Warning during loading", ::fwGui::dialog::IMessageDialog::WARNING);
    }

    return reader->getConcreteObject();
}

//------------------------------------------------------------------------------

void SSeriesDBReader::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    if( this->hasLocationDefined() )
    {
        ::fwMedData::SeriesDB::sptr localSeriesDB = this->createSeriesDB( this->getFolder() );

        if( !localSeriesDB->empty() )
        {
            // Retrieve dataStruct associated with this service
            ::fwMedData::SeriesDB::sptr seriesDB = this->getObject< ::fwMedData::SeriesDB >();
            // seriesDB->shallowCopy( localSeriesDB ) ;

            ::fwComEd::helper::SeriesDB sDBhelper(seriesDB);

            ::fwData::mt::ObjectWriteLock lock(seriesDB);
            sDBhelper.merge(localSeriesDB);
            sDBhelper.notify(this->getSptr());


            ::fwGui::Cursor cursor;
            cursor.setCursor(::fwGui::ICursor::BUSY);
            cursor.setDefaultCursor();
        }
        else
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog(
                    "Image Reader","This file can not be read. Retry with another file reader.", ::fwGui::dialog::IMessageDialog::WARNING);
        }
    }
}

//------------------------------------------------------------------------------

} // namespace ioGdcm
