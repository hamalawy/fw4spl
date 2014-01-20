/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _IODATA_ACTION_SMESHMODIFIER_HPP_
#define _IODATA_ACTION_SMESHMODIFIER_HPP_

#include <fwServices/IService.hpp>

#include <fwGui/IActionSrv.hpp>
#include <fwDataTools/AlgoMeshDeformation.hpp>

#include "ioData/export.hpp"

namespace ioData
{
namespace action
{

/**
 * @brief   This action modifies a mesh using specified functor in configuration.
 * The purpose is to test all possibilities provide by the new mesh structure.
 * @class   SMeshModifier
 */
class IODATA_CLASS_API SMeshModifier : public ::fwGui::IActionSrv
{

public :

    fwCoreServiceClassDefinitionsMacro ( (SMeshModifier)(::fwGui::IActionSrv) ) ;

    /**
    * @brief Constructor. Do nothing.
    */
    IODATA_API SMeshModifier() throw() ;

    /**
    * @brief Destructor. Do nothing.
    */
    IODATA_API virtual ~SMeshModifier() throw() ;

protected:

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
      * @brief Configure the functor used to generate the mesh.
      *
      * Example of configuration :
      * @verbatim
         <service ... >
             <config functor="ShakeMeshPoint" />
         </service>
        @endverbatim
      * Functor available :
      *  - ShakeMeshPoint
      *  - ColorizeMeshPoints
      *  - ColorizeMeshCells
      *  - ComputePointNormals
      *  - ComputeCellNormals
      *  - ShakePointNormals
      *  - ShakeCellNormals
      *  - MeshDeformation
      */
    IODATA_API virtual void configuring() throw( ::fwTools::Failed ) ;

    IODATA_API virtual void starting() throw(::fwTools::Failed);

    IODATA_API virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief This method is used to update services on notification. Do nothing.
     */
    IODATA_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed);

    /**
     * @brief Process the action: modifies the mesh using the selected functor.
     */
    IODATA_API virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief This method gives information about the class.
     */
    IODATA_API virtual void info(std::ostream &_sstream ) ;

    ///@}

private:

    /// Functor name used to generate mesh.
    std::string m_functor;

    /// Algo use to animate a deformation on the mesh
    ::fwDataTools::AlgoMeshDeformation m_animator;
};


} // namespace action
} // namespace ioData


#endif /*_IODATA_ACTION_SMESHMODIFIER_HPP_*/
