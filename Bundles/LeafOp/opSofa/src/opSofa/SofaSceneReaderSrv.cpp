/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <sstream>

#include <QInputDialog>
#include <QFileDialog>

#include <fwRuntime/ConfigurationElement.hpp>

#include <fwData/String.hpp>
#include <fwMedData/ModelSeries.hpp>

//#include <fwServices/helper.hpp>
#include <fwServices/Base.hpp> // new
#include <fwServices/IEditionService.hpp> // new
#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>

#include <fwCore/spyLog.hpp>

#include "opSofa/SofaSceneReaderSrv.hpp"

namespace opSofa
{

fwServicesRegisterMacro( ::io::IReader, ::opSofa::SofaSceneReaderSrv, ::fwMedData::ModelSeries );

/**
 * @brief Constructor
 */
SofaSceneReaderSrv::SofaSceneReaderSrv() throw()
{
}

/**
 * @brief Destructor
 */
SofaSceneReaderSrv::~SofaSceneReaderSrv() throw()
{
}

/**
 * @brief Used to define the service parameters and analyze its configuration.
 */
void SofaSceneReaderSrv::configuring() throw ( ::fwTools::Failed )
{
}

/**
 * @brief Used to launch the service.
 */
void SofaSceneReaderSrv::starting() throw ( ::fwTools::Failed )
{
}

/**
 * @brief Used to stop the service.
 */
void SofaSceneReaderSrv::stopping() throw ( ::fwTools::Failed )
{
}

/**
 * @brief Called to do an action on the data associated to the service.
 */
void SofaSceneReaderSrv::updating() throw ( ::fwTools::Failed )
{
    ::fwMedData::ModelSeries::sptr ms = this->getObject< ::fwMedData::ModelSeries >();
    SLM_ASSERT("Invalid object", ms);

     // Open dialog box to select file .scn
    static QString defaultPath;
    std::string fileScn = QFileDialog::getOpenFileName(0, "Ouvrir un fichier scene", defaultPath, "Scene (*.scn)").toStdString();
    defaultPath = QString::fromStdString(fileScn);

    if (fileScn != "") {
        // Notification
        ::fwServices::ObjectMsg::sptr msg = ::fwServices::ObjectMsg::New();
        ::fwData::String::sptr data = ::fwData::String::New(fileScn);
        msg->addEvent( "NEW_SOFA_SCENE", data );
        ::fwServices::IEditionService::notify(this->getSptr(), ms, msg);
    }
}

/**
 * @brief Called by a data to notify a service.
 *
 * @param msg : Incoming message
 */
void SofaSceneReaderSrv::receiving( ::fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed )
{}

/**
 * @brief info of the class
 */
void SofaSceneReaderSrv::info( std::ostream & ostr )
{
}

/**
 * @brief Configure IHM
 */
void SofaSceneReaderSrv::configureWithIHM()
{
}

}
