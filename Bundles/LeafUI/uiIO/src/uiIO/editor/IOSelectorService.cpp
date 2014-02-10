/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <string>
#include <sstream>

#include <boost/foreach.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/helper.hpp>

#include <fwCore/base.hpp>

#include <fwData/Composite.hpp>
#include <fwComEd/helper/Composite.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/ServiceFactory.hpp>
#include <fwServices/registry/ServiceConfig.hpp>
#include <fwServices/macros.hpp>

#include <fwGui/dialog/SelectorDialog.hpp>
#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <io/IReader.hpp>
#include <io/IWriter.hpp>

#include "uiIO/editor/IOSelectorService.hpp"

namespace uiIO
{

namespace editor
{

//------------------------------------------------------------------------------

fwServicesRegisterMacro( ::gui::editor::IDialogEditor , ::uiIO::editor::IOSelectorService , ::fwData::Object );

//------------------------------------------------------------------------------

IOSelectorService::IOSelectorService() :
    m_mode                  ( READER_MODE ),
    m_servicesAreExcluded   ( true ),
    m_selectedServices      ( std::vector< std::string >() )
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
    //  <selection mode="include" />
    //  <addSelection service="::ioAtoms::SWriter" />

    ::fwRuntime::ConfigurationElementContainer::Iterator iter = this->m_configuration->begin() ;
    for( ; iter != this->m_configuration->end() ; ++iter )
    {
        OSLM_INFO( "IOSelectorService "  << (*iter)->getName());

        if( (*iter)->getName() == "selection" )
        {
            SLM_ASSERT( "Sorry, xml elemenet <selection> must have attribute 'mode'.", (*iter)->hasAttribute("mode")) ;
            std::string mode = (*iter)->getExistingAttributeValue("mode") ;
            m_servicesAreExcluded = ( mode == "exclude" );
            SLM_ASSERT( "Sorry, xml attribut <mode> must be 'exclude' or 'include'.", mode == "exclude" || mode == "include" );
            OSLM_DEBUG( "mode => " << mode );
        }

        if( (*iter)->getName() == "addSelection" )
        {
            if( ! vectorIsAlreadyCleared )
            {
                vectorIsAlreadyCleared = true;
                m_selectedServices.clear();
            }
            SLM_ASSERT( "Sorry, xml elemenet <addSelection> must have attribute 'service'.", (*iter)->hasAttribute("service")) ;
            m_selectedServices.push_back( (*iter)->getExistingAttributeValue("service") ) ;
            OSLM_DEBUG( "add selection => " << (*iter)->getExistingAttributeValue("service") );
        }

        if( (*iter)->getName() == "type" )
        {
            SLM_ASSERT( "Sorry, xml elemenet <type> must have attribute 'mode'.", (*iter)->hasAttribute("mode")) ;
            std::string mode = (*iter)->getExistingAttributeValue("mode") ;
            SLM_ASSERT( "Sorry, xml attribut <mode> must be 'writer' or 'reader'.",  mode == "writer" || mode == "reader" );
            m_mode = ( mode == "writer" ) ? WRITER_MODE : READER_MODE;
            OSLM_DEBUG( "mode => " << mode );
        }

        if( (*iter)->getName() == "config" )
        {
            SLM_ASSERT( "Sorry, xml elemenet <config> must have attribute 'id'.", (*iter)->hasAttribute("id")) ;
            SLM_ASSERT( "Sorry, xml elemenet <config> must have attribute 'service'.", (*iter)->hasAttribute("service")) ;
            std::string configId = (*iter)->getExistingAttributeValue("id") ;
            std::string configSrv = (*iter)->getExistingAttributeValue("service") ;
            m_serviceToConfig[ configSrv ] = configId;
        }

    }

    typedef std::vector < SPTR(::fwRuntime::ConfigurationElement) >  ConfigurationElementContainer;
    ConfigurationElementContainer inject = m_configuration->find("inject");

    if(!inject.empty())
    {
        m_inject = inject.at(0)->getValue();
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

void IOSelectorService::updating() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    // Retrieve implementation of type ::io::IReader for this object
    std::vector< std::string > availableExtensionsId;
    if ( m_mode == READER_MODE )
    {
        availableExtensionsId = ::fwServices::registry::ServiceFactory::getDefault()->getImplementationIdFromObjectAndType(this->getObject()->getClassname(),"::io::IReader") ;
    }
    else // m_mode == WRITER_MODE
    {
        availableExtensionsId = ::fwServices::registry::ServiceFactory::getDefault()->getImplementationIdFromObjectAndType(this->getObject()->getClassname(),"::io::IWriter") ;
    }

    // Filter available extensions and replace id by service description
    std::vector< std::pair < std::string, std::string > > availableExtensionsMap;
    std::vector< std::string > availableExtensionsSelector;

    BOOST_FOREACH( const std::string &serviceId, availableExtensionsId )
    {
        bool serviceIsSelectedByUser = std::find( m_selectedServices.begin(), m_selectedServices.end(), serviceId ) != m_selectedServices.end();

        // Test if the service is considered here as available by users, if yes push in availableExtensionsSelector
        // excluded mode => add services that are not selected by users
        // included mode => add services selected by users
        if( (m_servicesAreExcluded && ! serviceIsSelectedByUser) ||
            (! m_servicesAreExcluded && serviceIsSelectedByUser) )
        {
            // Add this service
            std::string infoUser = ::fwServices::registry::ServiceFactory::getDefault()->getServiceDescription(serviceId);

            std::map< std::string, std::string >::const_iterator iter = m_serviceToConfig.find( serviceId );
            if ( iter != m_serviceToConfig.end() )
            {
                infoUser = ::fwServices::registry::ServiceConfig::getDefault()->getConfigDesc(iter->second);
            }

            if (infoUser != "")
            {
                availableExtensionsMap.push_back( std::pair < std::string, std::string > (serviceId, infoUser) );
                availableExtensionsSelector.push_back( infoUser );
            }
            else
            {
                availableExtensionsMap.push_back( std::pair < std::string, std::string > (serviceId, serviceId) );
                availableExtensionsSelector.push_back( serviceId );
            }
        }
    }

    // Sort available services (lexical string sort)
    std::sort( availableExtensionsSelector.begin(), availableExtensionsSelector.end() );

    // Test if we have an extension
    if ( ! availableExtensionsMap.empty() )
    {
        std::string extensionId = availableExtensionsMap[0].first ;
        bool extensionSelectionIsCanceled = false;

        // Selection of extension when availableExtensions.size() > 1
        if ( availableExtensionsSelector.size() > 1 )
        {
            ::fwGui::dialog::SelectorDialog::sptr selector = ::fwGui::dialog::SelectorDialog::New();

            if ( m_mode != READER_MODE )
            {
                selector->setTitle("Writer to use");
            }
            else
            {
                selector->setTitle("Reader to use");
            }
            selector->setSelections(availableExtensionsSelector);
            std::string selection = selector->show();
            if( !selection.empty() )
            {
                bool extensionIdFound = false;

                typedef std::pair < std::string, std::string > PairType;
                BOOST_FOREACH(PairType pair, availableExtensionsMap)
                {
                    if (pair.second == selection )
                    {
                        extensionId = pair.first ;
                        extensionIdFound = true;
                    }
                }
                OSLM_ASSERT("Problem to find the selected string.", extensionIdFound );
            }
            else
            {
                extensionSelectionIsCanceled = true;
            }
        }

        if ( ! extensionSelectionIsCanceled )
        {

            // Get Config
            bool hasConfigForService = false;
            ::fwRuntime::ConfigurationElement::csptr srvCfg;
            if ( m_serviceToConfig.find( extensionId ) != m_serviceToConfig.end() )
            {
                hasConfigForService = true;
                srvCfg = ::fwServices::registry::ServiceConfig::getDefault()->getServiceConfig(  m_serviceToConfig[extensionId] , extensionId ) ;
                SLM_ASSERT("Sorry, there is not service configuration of type ::fwServices::registry::ServiceConfig found", srvCfg ) ;
            }

            // Configure and start service
            if ( m_mode == READER_MODE )
            {
                ::fwData::Object::sptr object;
                if(m_inject.empty() ||  this->getObject()->getClassname().compare("::fwData::Composite") )
                {
                    object = this->getObject< ::fwData::Object >();
                }
                else
                {
                    ::fwServices::registry::ServiceFactory::sptr services = ::fwServices::registry::ServiceFactory::getDefault();
                    std::string objType = services->getObjectImplementation(extensionId);
                    if(!objType.compare("::fwData::Object"))
                    {
                        object = this->getObject< ::fwData::Composite>();
                    }
                    else
                    {
                        object = ::fwData::factory::New(objType);
                        ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite>();
                        ::fwComEd::helper::Composite helper(composite);
                        helper.add(m_inject, object);

                        helper.notify(this->getSptr());
                    }
                }

                ::io::IReader::sptr reader = ::fwServices::add< ::io::IReader >( object , extensionId ) ;
                if ( hasConfigForService )
                {
                    reader->setConfiguration( ::fwRuntime::ConfigurationElement::constCast(srvCfg) );
                    reader->configure();
                }
                reader->start();
                reader->configureWithIHM();

                ::fwGui::Cursor cursor;
                cursor.setCursor(::fwGui::ICursor::BUSY);
                reader->update();
                cursor.setDefaultCursor();

                reader->stop();
                ::fwServices::OSR::unregisterService(reader);
            }
            else
            {
                ::io::IWriter::sptr writer = ::fwServices::add< ::io::IWriter >( this->getObject() , extensionId ) ;
                if ( hasConfigForService )
                {
                    writer->setConfiguration( ::fwRuntime::ConfigurationElement::constCast(srvCfg) );
                    writer->configure();
                }
                writer->start();
                writer->configureWithIHM();

                ::fwGui::Cursor cursor;
                cursor.setCursor(::fwGui::ICursor::BUSY);
                writer->update();
                cursor.setDefaultCursor();

                writer->stop();
                ::fwServices::OSR::unregisterService(writer);
            }
        }
    }
    else
    {
        SLM_WARN("IOSelectorService::load : availableExtensions is empty.");
        if ( m_mode == READER_MODE )
        {
            ::fwGui::dialog::MessageDialog messageBox;
            messageBox.setTitle("Reader not found");
            messageBox.setMessage( "Sorry, there are not available readers for this data type." );
            messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
            messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
            messageBox.show();
        }
        else // m_mode == WRITER_MODE
        {
            ::fwGui::dialog::MessageDialog messageBox;
            messageBox.setTitle("Writer not found");
            messageBox.setMessage( "Sorry, there are not available writers for this data type." );
            messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
            messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
            messageBox.show();
        }
    }
}

//------------------------------------------------------------------------------

void IOSelectorService::receiving( ::fwServices::ObjectMsg::csptr ) throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
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
