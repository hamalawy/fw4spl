/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/regex.hpp>


#include "gui/action/ConfigActionSrv.hpp"

#include <fwCom/Signal.hxx>
#include <fwServices/Base.hpp>
#include <fwServices/registry/AppConfig.hpp>

namespace gui
{
namespace action
{

//------------------------------------------------------------------------------

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::gui::action::ConfigActionSrv, ::fwData::Object );
const ::fwCom::Signals::SignalKeyType  ConfigActionSrv::s_LAUNCHED_SIG = "launched";

//------------------------------------------------------------------------------

ConfigActionSrv::ConfigActionSrv() throw() :
    m_viewConfigId(""),
    m_configIsRunning(false),
    m_sigLaunched(LaunchedSignalType::New())
{
    //addNewHandledEvent("WINDOW_CLOSED");
    ::fwCom::HasSignals::m_signals( s_LAUNCHED_SIG, m_sigLaunched );

#ifdef COM_LOG
    m_sigLaunched->setID( s_LAUNCHED_SIG );
#endif
}

//------------------------------------------------------------------------------

ConfigActionSrv::~ConfigActionSrv() throw()
{}

//------------------------------------------------------------------------------

void ConfigActionSrv::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    m_connections = ::fwServices::helper::SigSlotConnection::New();
    this->::fwGui::IActionSrv::actionServiceStarting();
}

//------------------------------------------------------------------------------

void ConfigActionSrv::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    bool configIsRunning = this->::fwGui::IActionSrv::getIsActive();

    if ( configIsRunning )
    {
        this->stopConfig();
    }

    m_connections.reset();
    this->::fwGui::IActionSrv::actionServiceStopping();
}

//------------------------------------------------------------------------------

void ConfigActionSrv::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    this->::fwGui::IActionSrv::initialize();

    std::vector < ConfigurationType > vectConfig = m_configuration->find("config");

    SLM_ASSERT("Sorry you must have one (and only one) <config id=... /> element.", vectConfig.size() == 1 );
    ::fwRuntime::ConfigurationElement::sptr configElement = vectConfig.at(0);

    SLM_ASSERT( "Sorry, missing attribute id in <config> xml element.", configElement->hasAttribute("id") );
    m_viewConfigId = configElement->getExistingAttributeValue("id");

    SLM_ASSERT( "Sorry, the attribute id in <config> xml element is empty.", ! m_viewConfigId.empty() );

    std::vector < ConfigurationType > replaceTagsConfig = m_configuration->find("replace");
    std::string adaptor("");
    std::string pattern("");
    BOOST_FOREACH( ConfigurationType replaceItem, replaceTagsConfig)
    {
        SLM_ASSERT("<replace> tag must have one attribut val.", replaceItem->hasAttribute("val"));
        adaptor = replaceItem->getAttributeValue("val");
        SLM_ASSERT("<replace> tag must have one attribut pattern.", replaceItem->hasAttribute("pattern"));
        pattern = replaceItem->getAttributeValue("pattern");
        m_fieldAdaptors[pattern] = adaptor;
    }
}

//-----------------------------------------------------------------------------

void ConfigActionSrv::setIsActive(bool isActive)
{
    this->::fwGui::IActionSrv::setIsActive(isActive);
    if ( isActive )
    {
        this->startConfig();
    }
    else
    {
        this->stopConfig();
    }
}

//------------------------------------------------------------------------------

void ConfigActionSrv::updating() throw(::fwTools::Failed)
{}

//------------------------------------------------------------------------------

void ConfigActionSrv::receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    if ( _msg->hasEvent("WINDOW_CLOSED") )
    {
        this->::fwGui::IActionSrv::setIsActive( false );
        this->stopConfig();
    }
}

//------------------------------------------------------------------------------

void ConfigActionSrv::info( std::ostream &_sstream )
{}

//------------------------------------------------------------------------------
void ConfigActionSrv::AddGenericUidToFieldApadtor( )
{
    // Generate generic UID
    std::string genericUidAdaptor = ::fwServices::registry::AppConfig::getUniqueIdentifier( this->getID() );

    // Init manager
    m_fieldAdaptors["GENERIC_UID"] = genericUidAdaptor;
}

//------------------------------------------------------------------------------

void ConfigActionSrv::startConfig()
{
    AddGenericUidToFieldApadtor();

    ::fwRuntime::ConfigurationElement::csptr config = ::fwServices::registry::AppConfig::getDefault()->getAdaptedTemplateConfig( m_viewConfigId, m_fieldAdaptors );
    m_configTemplateManager = ::fwServices::AppConfigManager::New();
    m_configTemplateManager->setConfig( config );


    // Launch config
    m_configTemplateManager->launch();

    // Add connection
    this->connectToConfigRoot();

    m_configIsRunning = true;
    fwServicesNotifyMacro(this->getLightID(), m_sigLaunched, ());
}

//------------------------------------------------------------------------------

void ConfigActionSrv::stopConfig()
{
    if( m_configIsRunning )
    {
        // Remove connection
        this->disconnectToConfigRoot();

        // Delete manager
        m_configTemplateManager->stopAndDestroy();
        m_configTemplateManager.reset();
    }
    m_configIsRunning = false;
}

//------------------------------------------------------------------------------

void ConfigActionSrv::connectToConfigRoot()
{
    ::fwData::Object::sptr root = m_configTemplateManager->getConfigRoot();
    m_connections->connect( root, this->getSptr(), this->getObjSrvConnections() );
}

//------------------------------------------------------------------------------

void ConfigActionSrv::disconnectToConfigRoot()
{
    m_connections->disconnect();
}

//------------------------------------------------------------------------------

}
}

