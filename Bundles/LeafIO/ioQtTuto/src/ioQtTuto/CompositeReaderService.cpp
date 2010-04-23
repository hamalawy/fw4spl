/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
#include <fwCore/base.hpp>

#include <fwData/Composite.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/ObjectMsg.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/bundle/runtime.hpp>
#include <fwXML/reader/FwXMLObjectReader.hpp>

#include "ioQtTuto/CompositeReaderService.hpp"
#include <QFileDialog>
#include <QString>

REGISTER_SERVICE( ::io::IReader , ::ioTuto::CompositeReaderService , ::fwData::Composite ) ;

namespace ioTuto
{
//------------------------------------------------------------------------------
CompositeReaderService::CompositeReaderService( ) throw()
: m_fsExternalDataPath("")
{}

//------------------------------------------------------------------------------
CompositeReaderService::~CompositeReaderService() throw()
{}

//------------------------------------------------------------------------------
void CompositeReaderService::info(std::ostream &_sstream )
{
}

//------------------------------------------------------------------------------

void CompositeReaderService::configuring() throw( ::fwTools::Failed )
{
}

//-----------------------------------------------------------------------------
void CompositeReaderService::starting() throw( ::fwTools::Failed )
{
}

//------------------------------------------------------------------------------
void CompositeReaderService::stopping() throw( ::fwTools::Failed )
{
}

//------------------------------------------------------------------------------
void CompositeReaderService::updating( ) throw(::fwTools::Failed)
{
    configureWithIHM();
    ::fwData::Composite::sptr pComposite = this->getObject< ::fwData::Composite >();
    assert( pComposite );


    ::fwXML::reader::FwXMLObjectReader myReader;

    myReader.setFile(m_fsExternalDataPath);
    if (m_fsExternalDataPath != "")
    {
        try
        {
            myReader.read();
            ::fwData::Composite::sptr newCompo;
            newCompo = ::fwData::Composite::dynamicCast( myReader.getObject() );
//            *pComposite = *newCompo;
            pComposite->shallowCopy(newCompo);
            std::copy( newCompo->children().begin(),  newCompo->children().end(), std::back_inserter( pComposite->children() ) );

        }
        catch (const std::exception & e)
        {/*
            std::stringstream ss;
            ss << "Warning during loading : " << e.what();
            wxString wxStmp( ss.str().c_str(), wxConvLocal );
            wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );*/
        }
        catch( ... )
        {/*
            std::stringstream ss;
            ss << "Warning during loading : ";
            wxString wxStmp( ss.str().c_str(), wxConvLocal );
            wxMessageBox( wxStmp, _("Warning"), wxOK|wxICON_WARNING );*/
        }
    }
}

//------------------------------------------------------------------------------
void CompositeReaderService::updating(::boost::shared_ptr< const ::fwServices::ObjectMsg > _msg) throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------
void CompositeReaderService::configureWithIHM()
{
   
    
    QString file = QFileDialog::getOpenFileName(0,QObject::tr("Xml file to read"), QDir::currentPath(), QObject::tr("xml file (*.xml)|*.xml"));
    std::cout<<"PATH_FILE : "<<file.toStdString()<<"\n";

    if( file.isEmpty() == false )
    {
      m_fsExternalDataPath = ::boost::filesystem::path( file.toStdString(), ::boost::filesystem::native );
    }
    
    
}
} // namespace ioTuto
