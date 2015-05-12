/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __TUTO02DATASERVICEBASICCTRL_PLUGIN_HPP__
#define __TUTO02DATASERVICEBASICCTRL_PLUGIN_HPP__

#include <fwRuntime/Plugin.hpp>

#include <fwCom/Connection.hpp>

#include <fwData/Image.hpp>

#include <fwServices/IService.hpp>

#include "Tuto02DataServiceBasicCtrl/config.hpp"

namespace Tuto02DataServiceBasicCtrl
{

/**
 * @brief   This class is started when the bundles is loaded.
 * @class   Plugin
 */
class TUTO02DATASERVICEBASICCTRL_CLASS_API Plugin : public ::fwRuntime::Plugin
{
public:
    /// Constructor.
    TUTO02DATASERVICEBASICCTRL_API Plugin() throw();

    /// Destructor. Do nothing.
    TUTO02DATASERVICEBASICCTRL_API ~Plugin() throw();

    /// Overrides start method. .
    TUTO02DATASERVICEBASICCTRL_API void start() throw(::fwRuntime::RuntimeException);

    /// Overrides stop method. Do nothing
    TUTO02DATASERVICEBASICCTRL_API void stop() throw();

    TUTO02DATASERVICEBASICCTRL_API void initialize() throw( ::fwRuntime::RuntimeException );

    TUTO02DATASERVICEBASICCTRL_API void uninitialize() throw();

private:
    ::fwData::Image::sptr m_image;

    ::fwCom::Connection m_connection;

    ::fwServices::IService::sptr m_frameSrv;
    ::fwServices::IService::sptr m_renderSrv;
    ::fwServices::IService::sptr m_readerSrv;
};

} // namespace Tuto02DataServiceBasicCtrl

#endif // __TUTO02DATASERVICEBASICCTRL_PLUGIN_HPP__
