/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <string>
#include <sstream>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/helper.hpp>

#include <fwCore/base.hpp>

#include <fwTools/Object.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwQt/Selector.hpp>


#include <io/IReader.hpp>
#include <io/IWriter.hpp>

#include "uiIOQt/editor/IOSelectorService.hpp"



#include <QApplication>
#include <QStringList>
#include <QListWidgetItem>


namespace uiIOQt
{

namespace editor
{

//------------------------------------------------------------------------------

REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiIOQt::editor::IOSelectorService , ::fwTools::Object );

//------------------------------------------------------------------------------

IOSelectorService::IOSelectorService() :
    m_mode                  ( READER_MODE ),
    m_servicesAreExcluded   ( true ),
    m_selectedServices      ( std::vector< std::string >()),
    m_ok (false)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

IOSelectorService::~IOSelectorService()  throw()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void IOSelectorService::configuring() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    bool vectorIsAlreadyCleared = false;

    //  Config Elem
    //  <selection mode="exclude">
    //  <addSelection service="::ioMfo::MfoPatientDBReaderService" />
    //  <addSelection service="::ioMfo::MfoDBPatientDBReaderService" />

    this->::guiQt::editor::IEditor::configuring() ;

    ::fwRuntime::ConfigurationElementContainer::Iterator iter = this->m_configuration->begin() ;
    for( ; iter != this->m_configuration->end() ; ++iter )
    {
        OSLM_INFO( "IOSelectorService "  << (*iter)->getName());

        if( (*iter)->getName() == "selection" )
        {
            assert( (*iter)->hasAttribute("mode")) ;
            std::string mode = (*iter)->getExistingAttributeValue("mode") ;
            m_servicesAreExcluded = ( mode == "exclude" );
            assert( mode == "exclude" || mode == "include" );
            OSLM_DEBUG( "mode => " << mode );
        }

        if( (*iter)->getName() == "addSelection" )
        {
            if( ! vectorIsAlreadyCleared )
            {
                vectorIsAlreadyCleared = true;
                m_selectedServices.clear();
            }
            assert( (*iter)->hasAttribute("service")) ;
            m_selectedServices.push_back( (*iter)->getExistingAttributeValue("service") ) ;
            OSLM_DEBUG( "add selection => " << (*iter)->getExistingAttributeValue("service") );
        }

        if( (*iter)->getName() == "type" )
        {
            assert( (*iter)->hasAttribute("mode")) ;
            std::string mode = (*iter)->getExistingAttributeValue("mode") ;
            assert( mode == "writer" || mode == "reader" );
            m_mode = ( mode == "writer" ) ? WRITER_MODE : READER_MODE;
	    
	    std::cout<<"\n\n MODE = "<<m_mode<<"\n\n";
	    
            OSLM_DEBUG( "mode => " << mode );
        }
    }

}

//------------------------------------------------------------------------------

void IOSelectorService::starting() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void IOSelectorService::stopping() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void IOSelectorService::startSelectedService()
{
  std::string extensionId;
  
  for(    std::vector< std::pair < std::string, std::string > >::iterator itExt = availableExtensionsMap.begin();itExt < availableExtensionsMap.end(); itExt++ )
  {
//	std::cout<<m_selectedString<<"    FOR --> Second : "<<itExt->second<<" First :"<<itExt->first<<" \n";
     if (itExt->first == m_selectedString)
      {
//	   std::cout<<"FOR->IF---> "<<itExt->first<<"\n";
         extensionId = itExt->first ;
      }
   }
   
  if(m_list->selectedItems().count()==0)
  {
      //std::cout<<"\n NO ITEM SELECTED \n";
	 //extensionId = selection.first();
  }

 std::cout<<"\n\n connect MODE = "<<m_mode<<"\n\n";


  if ( m_mode == READER_MODE )
  {

      std::cout<<"REAZDER_MODE => "<<extensionId<<"\n";
      ::io::IReader::sptr reader = ::fwServices::add< ::io::IReader >( this->getObject() , extensionId ) ;
		
      std::cout<<"\n\n START ===> "<<extensionId<<"\n";	
      OSLM_INFO("\n START ===> "<<extensionId<<"\n");

      reader->start();
            //    reader->configureWithIHM();
      reader->update();
      reader->stop();
  }
  else
  {
    std::cout<<"WRITER_MODE => "<<extensionId<<"\n";
    ::io::IWriter::sptr writer = ::fwServices::add< ::io::IWriter >( this->getObject() , extensionId ) ;	
    writer->start();
            //   writer->configureWithIHM();
    writer->update();
    writer->stop();
  }
        
   selection.clear();
   box->close();
}

void IOSelectorService::getListIndex()
{
  QList<QListWidgetItem *> l = m_list->selectedItems();
  QListWidgetItem *first =  l.first();
  
  m_selectedString = first->text().toStdString();
  m_list->row(first);
  
  std::cout<< m_list->row(first)<<" : "<<first->text().toStdString()<<" \n"; 
}

void IOSelectorService::cancel()
{
 selection.clear();
 box->close();
}


void IOSelectorService::updating() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    
    QWidget *mainWidget = qApp->activeWindow();
 //   QStringList selection;
 
    // Retrieve implementation of type ::io::IReader for this object
    std::vector< std::string > availableExtensionsId;
    if ( m_mode == READER_MODE )
    {
        // Erase all services of type ::io::IReader on the object
        // TODO : comment this line, because must be useless
        ::fwServices::eraseServices< ::io::IReader >( this->getObject() ) ;
	
	std::cout<<"    ADD REDER implementation \n";
	
        availableExtensionsId = ::fwServices::getImplementationIds< ::io::IReader >( this->getObject() ) ;
    }
    else // m_mode == WRITER_MODE
    {
        ::fwServices::eraseServices< ::io::IWriter >( this->getObject() ) ;
	
	std::cout<<"    ADD WRITER implementation \n";
	
        availableExtensionsId = ::fwServices::getImplementationIds< ::io::IWriter >( this->getObject() ) ;
    }

    // Filter available extensions and replace id by service description
    //std::vector< std::pair < std::string, std::string > > availableExtensionsMap;
    std::vector< std::string > availableExtensionsSelector;

    for(    std::vector< std::string >::iterator itExt = availableExtensionsId.begin();
            itExt < availableExtensionsId.end();
            itExt++ )
    {
        std::string serviceId = *itExt;

        bool serviceIsSelectedByUser = std::find( m_selectedServices.begin(), m_selectedServices.end(), serviceId ) != m_selectedServices.end();

        // Test if the service is considered here as available by users, if yes push in availableExtensionsSelector
        // excluded mode => add services that are not selected by users
        // included mode => add services selected by users
        if( m_servicesAreExcluded && ! serviceIsSelectedByUser ||
            ! m_servicesAreExcluded && serviceIsSelectedByUser )
        {
	   
            // Add this service
            const std::string infoUser = ::fwRuntime::getInfoForPoint( serviceId );
	    
	  //   std::cout<<"\n  UPDATING() :  ServiceID "<<serviceId<<"  infoUser "<< infoUser <<"\n";
	     
            if (infoUser != "")
            {
                availableExtensionsMap.push_back( std::pair < std::string, std::string > (serviceId, infoUser) );
                availableExtensionsSelector.push_back( infoUser );
		
		std::cout<<"       ADD Selection infoUser :  "<<infoUser<<"\n";
		selection << infoUser.c_str();
            }
            else
            {
                availableExtensionsMap.push_back( std::pair < std::string, std::string > (serviceId, serviceId) );
                availableExtensionsSelector.push_back( serviceId );
		
		std::cout<<"       ADD Selection serviceId :  "<<serviceId<<"\n";
		
		selection << serviceId.c_str();
            }
	   
        }
    }

    // Sort available services (lexical string sort)
    std::sort( availableExtensionsSelector.begin(), availableExtensionsSelector.end() );


    // Test if we have an extension
    if ( ! availableExtensionsMap.empty() )
    {
        std::string extensionId = availableExtensionsMap[0].first ;
	
//	std::cout<<"    AvailableExt -> extensionId : "<<extensionId<<"\n";
	
        extensionSelectionIsCanceled = false;

        // Selection of extension when availableExtensions.size() > 1
        bool extensionIdFound = false;
	
        if ( availableExtensionsSelector.size() > 1 )
        {
            //::fwWX::Selector selector( wxTheApp->GetTopWindow() , _("Reader to use") , availableExtensionsSelector );
	    ::fwQt::Selector selector(mainWidget, selection);
	    m_list = selector.m_list;
	    box = selector.box;
	    
	  //  std::cout<<"    CONNECT \n";
	    QObject::connect(selector.m_okButton, SIGNAL(clicked()), this, SLOT(startSelectedService()));
	    QObject::connect(selector.m_cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	    QObject::connect(selector.m_list, SIGNAL(itemSelectionChanged()), this, SLOT(getListIndex()));

            if ( m_mode != READER_MODE )
            {
               // selector.SetTitle( _("Writer to use") );
            }

	 
	  selector.box->setModal(true);
	  selector.box->show();
	  std::cout<<"  BOX SHOW MODAL \n";
	}
	else
	{
	   if ( m_mode == READER_MODE )
	   {

	    std::cout<<"ONLY_ONE   REAZDER_MODE => "<<extensionId<<"\n";
	    ::io::IReader::sptr reader = ::fwServices::add< ::io::IReader >( this->getObject() , extensionId ) ;
		
	    std::cout<<"\n\n START ===> "<<extensionId<<"\n";	

	    reader->start();
            //    reader->configureWithIHM();
	    reader->update();
	    reader->stop();
	    }
	    else
	    {
	      std::cout<<"ONLY_ONE       WRITER_MODE => "<<extensionId<<"\n";
	      ::io::IWriter::sptr writer = ::fwServices::add< ::io::IWriter >( this->getObject() , extensionId ) ;	
	      writer->start();
            //   writer->configureWithIHM();
	      writer->update();
	      writer->stop();
	    }
	}

    }
    else
    {
        SLM_WARN("IOSelectorService::load : availableExtensions is empty.");
        if ( m_mode == READER_MODE )
        {

        }
        else // m_mode == WRITER_MODE
        {

        }
    }

    // Erase all reader/writer services on this object
    // Todo : replace this line to erase only the used service
    if ( m_mode == READER_MODE )
    {
        ::fwServices::eraseServices< ::io::IReader >( this->getObject() ) ;
    }
    else // m_mode == WRITER_MODE
    {
        ::fwServices::eraseServices< ::io::IWriter >( this->getObject() ) ;
    }
    
}

//------------------------------------------------------------------------------

void IOSelectorService::info( std::ostream &_sstream )
{
    // Update message
    _sstream << "IOSelectorService";
}

//------------------------------------------------------------------------------

void IOSelectorService::setIOMode( IOMode _mode )
{
    m_mode = _mode;
}

//------------------------------------------------------------------------------

} // namespace editor

} // namespace gui
