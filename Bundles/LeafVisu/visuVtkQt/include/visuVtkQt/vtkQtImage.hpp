/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef VTK_QT_IMAGE_HPP_
#define  VTK_QT_IMAGE_HPP_


#include <guiQt/editor/IEditor.hpp>

#include "config.hpp"

namespace visuVtkQt
{

/**
* @brief    Renderer service.
* @class    RendererService.
* @author   IRCAD (Research and Development Team).
* @date 2009.
*
* Service rendering a ::fwData::Image using VTK.
*
* Service registered details : \n
* REGISTER_SERVICE( ::fwRender::IRender , ::vtkSimpleNegato::RendererService , ::fwData::Image)
*/
class  vtkQtImage : public ::guiQt::editor::IEditor
{
public :
    fwCoreServiceClassDefinitionsMacro ( (vtkQtImage)(::guiQt::editor::IEditor) );

    /**
    * @brief    Constructor
    */
     vtkQtImage() throw() ;

    /**
    * @brief    Destructor
    */
     virtual ~vtkQtImage() throw() ;
     
protected :

    /**
    * @brief Starting method.
    *
    * This method is used to initialize the service.
    * Initialize VTK renderer and wxWidget containers
    */
     virtual void starting() throw(fwTools::Failed);

    /**
    * @brief Stopping method.
    *
    * Destroy VTK renderer and wxWidget containers
    */
     virtual void stopping() throw(fwTools::Failed);

    /**
    * @brief Updating method.
    *
    * This method is used to update the service.
    * Make a render if necessary
    */
     virtual void updating() throw(fwTools::Failed);

    /**
    * @brief Configuring method.
    *
    * This method is used to configure the service.
    */
     virtual void configuring() throw(::fwTools::Failed);

    /**
    * @brief Updating method (react on data modifications).
    * @param[in] _msg ::fwServices::ObjectMsg::csptr.
    *
    * This method is used to update the service.
    */
     virtual void updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /// @brief vtk renderer
   
};

}


#endif /* VTKSIMPLENEGATO_RENDERER_SERVICE_HPP_ */
