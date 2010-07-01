/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef CTRLFIELD_IFIELDSYNCHRONIZER_HPP_
#define CTRLFIELD_IFIELDSYNCHRONIZER_HPP_

#include "ctrlField/config.hpp"
#include <fwServices/IService.hpp>

namespace ctrlField
{
/**
 * @brief   Service type dedicated to synchronization of fields.
 * @class   IFieldSynchronizer.
 * @author  LISA Laboratory, Angers University.
 * @date    2010.
 *
 * If some fields of an object is modified, modifications are propagated to other objects (copying field content)
 */
class CTRLFIELD_CLASS_API IFieldSynchronizer : public ::fwServices::IService
{
public:

    fwCoreServiceClassDefinitionsMacro ( (IFieldSynchronizer)(::fwServices::IService) ) ;

	///Constructor
	CTRLFIELD_API IFieldSynchronizer() throw() ;
	///Destructor
	CTRLFIELD_API virtual ~IFieldSynchronizer() throw() ;

    /**
     * @name General service methods
     */

    //@{
	CTRLFIELD_API virtual void starting() throw ( ::fwTools::Failed ) ;

	CTRLFIELD_API virtual void stopping() throw ( ::fwTools::Failed ) ;
	///Object identifiers (not unique identifiers) define objects to be synchronized
	CTRLFIELD_API virtual void configuring() throw ( ::fwTools::Failed ) ;

	CTRLFIELD_API virtual void updating() throw ( ::fwTools::Failed ) ;

	CTRLFIELD_API virtual void updating( fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed ) ;
    //@}

protected:

    /**
     * @name Specific methods to be implemented by a specific service of this type
     */

    //@{
    CTRLFIELD_API virtual void doStart() throw ( ::fwTools::Failed ) = 0 ;

    CTRLFIELD_API virtual void doStop() throw ( ::fwTools::Failed ) = 0 ;

    CTRLFIELD_API virtual void doConfigure() throw ( ::fwTools::Failed ) = 0 ;

    CTRLFIELD_API virtual void doUpdate() throw ( ::fwTools::Failed ) = 0 ;

    CTRLFIELD_API virtual void doUpdate( std::vector< ::fwTools::Object::wptr > objectsToUpdate, fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed ) = 0 ;
    //@}

    ///Objects to be synchronized
    std::vector< ::fwTools::Object::wptr > m_objectsToSynchronize;
} ;

}

#endif /*CTRLFIELD_IFIELDSYNCHRONIZER_HPP_*/
