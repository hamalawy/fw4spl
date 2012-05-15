/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <vector>

#include <fwCore/base.hpp>


// Service associated data
#include <fwData/Object.hpp>

// Services tools
#include <fwServices/Base.hpp>

#include <fwMemory/BufferManager.hpp>
#include <fwMemory/IPolicy.hpp>

#include "memory/SDumpPolicy.hpp"


namespace memory
{


REGISTER_SERVICE( ::fwServices::IController, ::memory::SDumpPolicy, ::fwData::Object );

SDumpPolicy::SDumpPolicy()
{}

SDumpPolicy::~SDumpPolicy()
{}


void SDumpPolicy::configuring() throw ( ::fwTools::Failed )
{
    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;
    std::vector < ConfigurationType > config = m_configuration->find("config");

    if (!config.empty())
    {
        std::vector < ConfigurationType > policy = config.at(0)->find("policy");
        std::vector < ConfigurationType > paramsElement = config.at(0)->find("params");

        m_policyParams.clear();
        m_policy = "";

        if (!policy.empty())
        {
            m_policy = policy.at(0)->getValue();
        }

        if(!paramsElement.empty())
        {
            const ConfigurationType &params = paramsElement.at(0);

            ::fwRuntime::ConfigurationElement::Container::const_iterator iter;

            for (iter = params->begin(); iter != params->end(); ++iter)
            {
                m_policyParams.push_back( ParametersType::value_type((*iter)->getName(), (*iter)->getValue()));
            }
        }
    }

}

void SDumpPolicy::starting() throw ( ::fwTools::Failed )
{
    ::fwMemory::BufferManager::sptr manager;
    manager = ::fwMemory::BufferManager::dynamicCast(::fwTools::IBufferManager::getCurrent());

    if (manager)
    {
        ::fwMemory::IPolicy::sptr policy = ::fwMemory::IPolicy::createPolicy(m_policy);

        if (policy)
        {
            bool success;
            BOOST_FOREACH(const ParametersType::value_type &param, m_policyParams)
            {
                success = policy->setParam(param.first, param.second);
                OSLM_ERROR_IF( "[" << m_policy << "] Unable to set '"
                               << param.first << "' parameter to " << param.second,
                               !success);
                OSLM_INFO_IF("Set '" << param.first << "' policy parameter to " << param.second , success);
            }

            manager->setDumpPolicy(policy);
            OSLM_INFO("Set dump policy to : " << m_policy);
        }
        OSLM_ERROR_IF("Unable to instanciate '" << m_policy << "' dump policy", !policy);
    }

}

void SDumpPolicy::stopping() throw ( ::fwTools::Failed )
{}

void SDumpPolicy::updating() throw ( ::fwTools::Failed )
{}

void SDumpPolicy::updating( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed )
{}

void SDumpPolicy::swapping() throw ( ::fwTools::Failed )
{
    this->stopping();
    this->starting();
}

} // namespace memory


