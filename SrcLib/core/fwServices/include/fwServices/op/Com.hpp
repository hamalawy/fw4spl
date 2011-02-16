/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWSERVICES_COM_HPP_
#define _FWSERVICES_COM_HPP_

#include <fwTools/Object.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include "fwServices/ObjectServiceRegistry.hpp"
#include "fwServices/IService.hpp"
#include "fwServices/ComChannelService.hpp"

namespace fwServices
{

/**
 * @name Methods regarding communications
 */

//@{

/**
 * @brief return the existing communication channel between _src and _dest if any, or a empty shared pointer.
 */
FWSERVICES_API ::fwServices::ComChannelService::sptr getCommunicationChannel( ::fwTools::Object::sptr _src, ::fwServices::IService::sptr _dest);

/**
 * @brief Create and register a communication channel between the source (_src) and the destination (_dest)
 * @return The communication channel service so that it can be directly started.
 * @note if the communication channel is already existing, it is returned.
 * @author IRCAD (Research and Development Team).
 */
FWSERVICES_API ::fwServices::ComChannelService::sptr registerCommunicationChannel( ::fwTools::Object::sptr _src , ::fwServices::IService::sptr _dest) ;

/**
 * @brief Unregister all communication services involving the IEditionService of _src as subject and _dest as observer
 */
FWSERVICES_API void unregisterCommunicationChannel( ::fwTools::Object::sptr _src , ::fwServices::IService::sptr  _dest) ;

//@}


}

#endif /*_FWSERVICES_COM_HPP_*/
