/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwData/Vector.hpp>
#include <fwData/String.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/registry/AppConfig.hpp>

#include <fwComEd/VectorMsg.hpp>

#include <fwDataCamp/getObject.hpp>

#include <fwMedData/Series.hpp>

#include "uiMedDataQt/SSeriesViewer.hpp"


namespace uiMedData
{

//------------------------------------------------------------------------------

fwServicesRegisterMacro( ::fwServices::IController , ::uiMedData::SSeriesViewer , ::fwData::Vector);

//------------------------------------------------------------------------------

SSeriesViewer::SSeriesViewer()
{
}

//------------------------------------------------------------------------------

SSeriesViewer::~SSeriesViewer() throw()
{
}

//------------------------------------------------------------------------------

void SSeriesViewer::info(std::ostream &_sstream )
{
    // Update message
    _sstream << std::string("SSeriesViewer");
}

//------------------------------------------------------------------------------

void SSeriesViewer::starting() throw(::fwTools::Failed)
{
    this->updating();
}

//------------------------------------------------------------------------------

void SSeriesViewer::receiving( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    ::fwComEd::VectorMsg::csptr vectorMsg = ::fwComEd::VectorMsg::dynamicConstCast(msg);

    if ( vectorMsg && (vectorMsg->hasEvent( ::fwComEd::VectorMsg::ADDED_OBJECTS )
                      || vectorMsg->hasEvent( ::fwComEd::VectorMsg::REMOVED_OBJECTS ) ))
    {
        this->updating();
    }
}

//------------------------------------------------------------------------------

void SSeriesViewer::stopping() throw(::fwTools::Failed)
{
    if(m_configTemplateManager)
    {
        m_configTemplateManager->stopAndDestroy();
        m_configTemplateManager.reset();
    }
}

//------------------------------------------------------------------------------

void SSeriesViewer::updating() throw(::fwTools::Failed)
{
    ::fwData::Vector::sptr vector = this->getObject< ::fwData::Vector >();

    if(m_configTemplateManager)
    {
        m_configTemplateManager->stopAndDestroy();
        m_configTemplateManager.reset();
    }

    if(vector->size() == 1)
    {
        ::fwData::Object::sptr obj = vector->front();
        std::string classname = obj->getClassname();
        SeriesConfigMapType::iterator itr = m_seriesConfigs.find(classname);

        if(itr != m_seriesConfigs.end())
        {
            SeriesConfigInfo info = itr->second;
            std::string configId =  info.configId;

            std::map< std::string, std::string > replaceMap;
            // Generate generic UID
            std::string genericUidAdaptor = ::fwServices::registry::AppConfig::getUniqueIdentifier( this->getID() );
            replaceMap["GENERIC_UID"] = genericUidAdaptor;
            replaceMap["WID_PARENT"] = m_parentView;
            replaceMap["objectID"] = obj->getID();

            BOOST_FOREACH(const ReplaceValuesMapType::value_type &elt, info.extractValues)
            {
                ::fwData::Object::sptr object = ::fwDataCamp::getObject( obj, elt.second );
                OSLM_ASSERT("Object from name "<< elt.second <<" not found", object);
                replaceMap[elt.first] = object->getID();
            }

            BOOST_FOREACH(const ReplaceValuesMapType::value_type &elt, info.parameters)
            {
                SLM_ASSERT("Value '" << elt.first << "' already used in extracted values.",
                           replaceMap.find(elt.first) == replaceMap.end());
                replaceMap[elt.first] = elt.second;
            }

            // Init manager
            ::fwRuntime::ConfigurationElement::csptr config =
                   ::fwServices::registry::AppConfig::getDefault()->getAdaptedTemplateConfig( configId, replaceMap );
            m_configTemplateManager = ::fwServices::AppConfigManager::New();
            m_configTemplateManager->setConfig( config );

            // Launch config
            m_configTemplateManager->launch();
        }
    }
}

//------------------------------------------------------------------------------

void SSeriesViewer::configuring() throw(::fwTools::Failed)
{
    std::vector < ::fwRuntime::ConfigurationElement::sptr > viewCfg = m_configuration->find("parentView");
    SLM_ASSERT("Missing tag 'parentView'", viewCfg.size() == 1);
    m_parentView = viewCfg[0]->getValue();
    SLM_ASSERT("'parentView' value must not be empty ", !m_parentView.empty());

    std::vector < ::fwRuntime::ConfigurationElement::sptr > configsCfg = m_configuration->find("configs");
    SLM_ASSERT("Missing tag 'configs'", configsCfg.size() == 1);

    std::vector < ::fwRuntime::ConfigurationElement::sptr > config = configsCfg[0]->find("config");
    SLM_ASSERT("Missing tag 'config'", !config.empty());

    BOOST_FOREACH(const ::fwRuntime::ConfigurationElement::sptr &elt, config)
    {
        SeriesConfigInfo info;
        info.configId = elt->getAttributeValue("id");
        SLM_ASSERT("'id' attribute must not be empty", !info.configId.empty());
        std::string seriesType = elt->getAttributeValue("type");
        SLM_ASSERT("'type' attribute must not be empty", !seriesType.empty());
        OSLM_ASSERT("Type " << seriesType << " is already defined.",
                    m_seriesConfigs.find(seriesType)== m_seriesConfigs.end() );

        BOOST_FOREACH(const ::fwRuntime::ConfigurationElement::sptr &extractElt, elt->find("extract"))
        {
            std::string path = extractElt->getAttributeValue("path");
            SLM_ASSERT("'path' attribute must not be empty", !path.empty());
            std::string pattern = extractElt->getAttributeValue("pattern");
            SLM_ASSERT("'pattern' attribute must not be empty", !pattern.empty());
            info.extractValues[pattern] = path;
        }

        BOOST_FOREACH(const ::fwRuntime::ConfigurationElement::sptr &param, elt->find("parameter"))
        {
            std::string replace = param->getAttributeValue("replace");
            SLM_ASSERT("'replace' attribute must not be empty", !replace.empty());
            std::string by = param->getAttributeValue("by");
            SLM_ASSERT("'by' attribute must not be empty", !by.empty());
            info.parameters[replace] = by;
        }

        m_seriesConfigs[seriesType] = info;
    }

}

//------------------------------------------------------------------------------

} // namespace uiMedData
