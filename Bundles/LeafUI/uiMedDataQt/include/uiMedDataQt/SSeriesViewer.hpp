/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIMEDDATAQT_SSERIESVIEWER_HPP__
#define __UIMEDDATAQT_SSERIESVIEWER_HPP__

#include <map>

#include <fwServices/IController.hpp>
#include <fwServices/AppConfigManager.hpp>

#include "uiMedDataQt/config.hpp"

namespace uiMedData
{
/**
 * @brief  This Service allows to preview the selected series in the Vector. For the moment, it works only on a
 * single selection.
 * @class   SSeriesViewer
 *
 * @date    2013.
 */
class UIMEDDATAQT_CLASS_API SSeriesViewer : public ::fwServices::IController
{

public:
    fwCoreServiceClassDefinitionsMacro ( (SSeriesViewer)(::fwServices::IController) );

    /// Constructor
    UIMEDDATAQT_API SSeriesViewer();

    /// Destructor
    UIMEDDATAQT_API virtual ~SSeriesViewer() throw();

protected:

    /// Calls updating on starting.
    virtual void starting() throw(::fwTools::Failed);

    /// Stops the config if it is running.
    virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief Configures the service.
     * @verbatim
       <service uid="seriesViewer" type="::fwServices::IController" impl="::uiMedData::SSeriesViewer" autoConnect="yes">
        <parentView>preview</parentView>
        <configs>
            <config id="2DSimpleConfig" type="::fwMedData::ImageSeries">
                <extract path="@image" pattern="imageID" />
            </config>
            <config id="2DPacsPreviewConfig" type="::fwDicomData::DicomSeries" >
                <parameter replace="PACS_CONFIGURATION" by="None" />
            </config>
            <config id="3DSimpleConfig" type="::fwMedData::ModelSeries" />
        </configs>
       </service>
       @endverbatim
     * - \b parentView : wid of the view where the config will install its windows.
     * - \b config : gives the available association between data type and associated config.
     *   - \b id : identifier of the AppConfig to launch
     *   - \b type : classname of the object stored in Vector associated to this config.
     *   - \b parameter : allow to pass specific value to the associ config
     *     - \b replace : name of the parameter to be replaced
     *     - \b by : specific value to replace for the parameter
     * - \b extract : extracts the object from the path and replaces pattern with its fwID
     */
    virtual void configuring() throw (::fwTools::Failed);

    /**
     * @brief Launch the config on the object if possible.
     *
     * If there is a single selection : it launchs an AppConfig on the object defined in this service configuration
     * (stored in m_seriesConfigs). The selected object fwID replaces the 'objectID' parameter in the AppConfig.
     * no configuration are launched if there is no selection, a multiple selection or if there is no configuration
     * associated with the selected object.
     */
    virtual void updating() throw (::fwTools::Failed);

    /// Listen Vector notification when objects are added or deleted.
    virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    virtual void info( std::ostream &_sstream );

private:

    typedef std::map<std::string, std::string> ReplaceValuesMapType;

    /// Stucture to register configuration informations.
    struct SeriesConfigInfo
    {
        /// Id of the configuration to launch.
        std::string configId;

        /// Stores the pattern to replace in the config by the fwID of the object given by a path.
        ReplaceValuesMapType extractValues;

        /// Stores the parameters to pass to config.
        ReplaceValuesMapType parameters;
    };

    typedef std::map<std::string, SeriesConfigInfo> SeriesConfigMapType;

    /// Config manager
    ::fwServices::AppConfigManager::sptr m_configTemplateManager;

    /// Stores the wid of the view where the config will install its windows.
    std::string m_parentView;

    /// Stores the association between data type and associated configuration.
    SeriesConfigMapType m_seriesConfigs;
};
} // namespace uiMedData

#endif // __UIMEDDATAQT_SSERIESVIEWER_HPP__

