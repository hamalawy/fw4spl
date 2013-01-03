/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWXML_MESHFILEFORMATSERVICE_HPP_
#define _FWXML_MESHFILEFORMATSERVICE_HPP_


#include "fwXML/config.hpp"
#include "fwXML/IFileFormatService.hpp"


namespace fwXML
{

/**
 * @brief The purpose of this class is to provide an io service to save data
 */
class FWXML_CLASS_API TriangularMeshFileFormatService : public IFileFormatService
{
public:
    fwCoreServiceClassDefinitionsMacro ( (TriangularMeshFileFormatService)(::fwXML::IFileFormatService) ) ;
    FWXML_API TriangularMeshFileFormatService();

    FWXML_API virtual ~TriangularMeshFileFormatService();

    FWXML_API virtual void load();

    FWXML_API virtual void save();

protected :

    /**
     * @brief install default policies if no explicitly set :
     * @li first try to use INR RW : our application must provide libitkIO ( hard link, bundles started etc...)
     * @li if not set and INR RW policy install failed use gzBufferRW
     */
    void RWPoliciesInstall();

};

}

#endif /*_FWXML_MESHFILEFORMATSERVICE_HPP_*/
