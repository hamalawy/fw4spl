/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWRUNTIME_PROFILE_ACTIVATER_HPP__
#define __FWRUNTIME_PROFILE_ACTIVATER_HPP__

#include <map>
#include <vector>
#include <string>
#include <boost/utility.hpp>

#include "fwRuntime/config.hpp"
#include "fwRuntime/Version.hpp"



namespace fwRuntime
{

namespace profile
{



/**
 * @brief   Activates a given bundle with optional parameters.
 * @class  Activater
 * @date    2007-2009.
 *
 */
class Activater : public boost::noncopyable
{
public:

    /**
     * @brief       Constructor
     *
     * @param[in]   identifier  a string containing a bundle identifier
     * @param[in]   version     a string a bundle version
     */
    FWRUNTIME_API Activater( const std::string & identifier, const std::string & version );


    /**
     * @brief       Adds a new parameter to the activater.
     *
     * @param[in]   identifier  a string containing the parameter name
     * @param[in]   value       a string containing the parameter value
     */
    FWRUNTIME_API void addParameter( const std::string & identifier, const std::string & value );

    /**
     * @brief       Adds a new disable extension point  to the activater.
     *
     * @param[in]   identifier    a string containing the parameter name
     */
    FWRUNTIME_API void addDisableExtensionPoint( const std::string & identifier );

    /**
     * @brief       Adds a new disable extension  to the activater.
     *
     * @param[in]   identifier    a string containing the parameter name
     */
    FWRUNTIME_API void addDisableExtension( const std::string & identifier );

    /**
     * @brief   Applies the activater on the system.
     *
     * @remark  This method should be called directly.
     */
    void apply();


private:

    typedef std::map< std::string, std::string >    ParameterContainer;
    typedef std::vector< std::string >              DisableExtensionPointContainer;
    typedef std::vector< std::string >              DisableExtensionContainer;


    const std::string m_identifier;                             ///< a bundle identifier
    const Version m_version;                                    ///< a bundle version

    ParameterContainer m_parameters;                            ///< all parameters
    DisableExtensionPointContainer m_disableExtensionPoints;    ///< all disable extension points
    DisableExtensionContainer m_disableExtensions;              ///< all disable extensions

};



} // namespace profile

} // namespace fwRuntime



#endif /*__FWRUNTIME_PROFILE_ACTIVATER_HPP__*/
