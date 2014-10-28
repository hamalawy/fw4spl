/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <fwCore/base.hpp>
#include <fwTools/fwID.hpp>
#include <fwServices/Base.hpp>

#include "fwGui/IActionSrv.hpp"
#include "fwGui/dialog/IMessageDialog.hpp"
#include "fwGui/dialog/MessageDialog.hpp"

namespace fwGui
{

IActionSrv::IActionSrv() :
        m_activeStateValue(true),
        m_isActive(false),
        m_isExecutable(true),
        m_confirmAction(false)
{}

//-----------------------------------------------------------------------------

IActionSrv::~IActionSrv()
{}

//-----------------------------------------------------------------------------

void IActionSrv::initialize()
{
    m_registrar = ::fwGui::registrar::ActionRegistrar::New(this->getID());

    OSLM_ASSERT("Depreciated tag <name> in "<< this->getID() << " configuration.", ! m_configuration->hasAttribute("name"));
    OSLM_ASSERT("Depreciated tag <shortcut> in "<< this->getID() << " configuration.", ! m_configuration->hasAttribute("shortcut"));
    OSLM_ASSERT("Depreciated tag <enable> in "<< this->getID() << " configuration.", ! m_configuration->hasAttribute("enable"));
    OSLM_ASSERT("Depreciated tag <specialAction> in "<< this->getID() << " configuration.", ! m_configuration->hasAttribute("specialAction"));
    OSLM_ASSERT("Depreciated tag <style> in "<< this->getID() << " configuration.", ! m_configuration->hasAttribute("style"));
    OSLM_ASSERT("Depreciated tag <state> in "<< this->getID() << " configuration.", ! m_configuration->hasAttribute("state"));

    ::fwRuntime::ConfigurationElementContainer::Iterator iter ;
    for( iter = m_configuration->begin() ; iter != m_configuration->end() ; ++iter )
    {
        if( (*iter)->getName() == "state" )
        {
            ConfigurationType stateCfg = *iter;

            if( stateCfg->hasAttribute("inverse") )
            {
                std::string invertState = stateCfg->getExistingAttributeValue("inverse");
                SLM_ASSERT("Wrong attribute value : must be 'true' or 'false'", (invertState == "true") || (invertState == "false"));
                m_activeStateValue = !(invertState == "true") ;
            }

            if( stateCfg->hasAttribute("active") )
            {
                std::string isActive = stateCfg->getExistingAttributeValue("active");
                SLM_ASSERT("Wrong attribute value : must be 'true' or 'false'", (isActive == "true") || (isActive == "false"));
                m_isActive = (isActive == "true") ;
            }

            if( stateCfg->hasAttribute("executable") )
            {
                std::string isExecutable = stateCfg->getExistingAttributeValue("executable");
                SLM_ASSERT("Wrong attribute value : must be 'true' or 'false'", (isExecutable == "true") || (isExecutable == "false"));
                m_isExecutable = (isExecutable == "true") ;
            }
        }

        if( (*iter)->getName() == "confirmation" )
        {
            ConfigurationType cfg = *iter;

            SLM_ASSERT("Missing attribute 'value'", cfg->hasAttribute("value"));
            std::string confirm = cfg->getExistingAttributeValue("value");
            SLM_ASSERT("Wrong attribute value : must be 'true' or 'false'", (confirm == "true") || (confirm == "false"));
            m_confirmAction = (confirm == "true") ;

            if( cfg->hasAttribute("message") )
            {
                m_confirmMessage = cfg->getExistingAttributeValue("message");
            }

            if( cfg->hasAttribute("defaultbutton") )
            {
                m_defaultButton = cfg->getExistingAttributeValue("defaultbutton");
            }

        }
    }
}

//-----------------------------------------------------------------------------

void IActionSrv::actionServiceStopping()
{
    this->m_registrar->actionServiceStopping();
}

//-----------------------------------------------------------------------------

void IActionSrv::actionServiceStarting()
{
    this->m_registrar->actionServiceStarting();
    this->setIsActive(m_isActive);
}

//-----------------------------------------------------------------------------

void IActionSrv::setIsActive(bool isActive)
{
    m_isActive = isActive;
    this->m_registrar->actionServiceSetActive(m_activeStateValue == isActive);
}

//-----------------------------------------------------------------------------

bool IActionSrv::getIsActive()
{
    return m_isActive;
}

//-----------------------------------------------------------------------------

void IActionSrv::setIsExecutable(bool isExecutable)
{
    m_isExecutable = isExecutable;
    this->m_registrar->actionServiceSetExecutable(isExecutable);
}

//-----------------------------------------------------------------------------

bool IActionSrv::getIsExecutable()
{
    return m_isExecutable;
}

//-----------------------------------------------------------------------------

bool IActionSrv::confirmAction()
{
    bool actionIsConfirmed = true;

    if (m_confirmAction)
    {
        ::fwGui::dialog::MessageDialog dialog;
        dialog.setTitle("Confirmation");
        std::stringstream ss;
        ss << "Do you really want to execute this action ? ";
        if (!m_confirmMessage.empty())
        {
            ss << std::endl << m_confirmMessage;
        }
        dialog.setMessage( ss.str() );

        if(m_defaultButton == "yes")
        {
            dialog.setDefaultButton( ::fwGui::dialog::IMessageDialog::YES );
        }
        else if(m_defaultButton == "no")
        {
            dialog.setDefaultButton( ::fwGui::dialog::IMessageDialog::NO );
        }
        else if(!m_defaultButton.empty())
        {
            SLM_WARN("unknown button: " + m_defaultButton);
        }

        dialog.setIcon( ::fwGui::dialog::IMessageDialog::QUESTION );
        dialog.addButton( ::fwGui::dialog::IMessageDialog::YES_NO );
        ::fwGui::dialog::IMessageDialog::Buttons button = dialog.show();

        actionIsConfirmed = (button == ::fwGui::dialog::IMessageDialog::YES);
    }

    return actionIsConfirmed;
}
//-----------------------------------------------------------------------------

}
