/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __OPSOFA_SOFASCENEWRITERSRV_HPP__
#define __OPSOFA_SOFASCENEWRITERSRV_HPP__

#include <iostream>
#include <fwTools/Failed.hpp>
#include <fwServices/ObjectMsg.hpp>

#include <io/IWriter.hpp>

namespace opSofa
{

/**
 * @brief Service allow to create scn file
 */
class SofaSceneWriterSrv : public ::io::IWriter
{
    public :

        fwCoreServiceClassDefinitionsMacro ( (SofaSceneWriterSrv)(::io::IWriter) ) ;

        SofaSceneWriterSrv() throw();
        virtual ~SofaSceneWriterSrv() throw();
        void configuring() throw ( ::fwTools::Failed );
        void starting() throw ( ::fwTools::Failed );
        void stopping() throw ( ::fwTools::Failed );
        void updating() throw ( ::fwTools::Failed );
        void receiving( ::fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed );
        void info( std::ostream & ostr );
        void configureWithIHM();

    private :
        bool writeTrian;
        std::string m_sceneTemplate;
        bool m_useTempPath;

};

}

#endif // __OPSOFA_SOFASCENEWRITERSRV_HPP__

