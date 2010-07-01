/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef CTRLFIELD_IMAGESYNCHRONIZER_HPP_
#define CTRLFIELD_IMAGESYNCHRONIZER_HPP_

#include "ctrlField/config.hpp"
#include "ctrlField/IFieldSynchronizer.hpp"

namespace ctrlField
{

/**
 * @brief   Service type dedicated to synchronization of fields.
 * @class   IFieldSynchronizer.
 * @author  LISA Laboratory, Angers University.
 * @date    2010.
 *
 * It observes images: any field modification of an image is propagated to other images.
 * Example of use (configuration):
 * \code
 * <object type="Composite">
 *  <object id="input" type="Image"/>
 *  <object id="output" type="Image"/>
 *	<service implementation="::ctrlField::ImageSynchronizer" type="::ctrlField::IFieldSynchronizer" autoComChannel="no" >
 *	  <object id="input" />
 *	  <object id="output" />
 *	</service>
 * </object>
 * \endcode
 * fgsdfg
 * @note Only concern some fields (slice index, slice type and opacity). This can evolve
 * @note Concerning opacity: if modification lead to a value v, 1-v values are propagated
 */
class CTRLFIELD_CLASS_API ImageSynchronizer : public ::ctrlField::IFieldSynchronizer
{
public:
    fwCoreServiceClassDefinitionsMacro ( (ImageSynchronizer)(::ctrlField::IFieldSynchronizer) ) ;

	///Constructor
	CTRLFIELD_API ImageSynchronizer() throw() ;
	///Destructor
	CTRLFIELD_API virtual ~ImageSynchronizer() throw() ;

    /**
     * @name Service type methods
     */

    //@{
    CTRLFIELD_API virtual void doStart() throw ( ::fwTools::Failed ) ;

    CTRLFIELD_API virtual void doStop() throw ( ::fwTools::Failed ) ;

    CTRLFIELD_API virtual void doConfigure() throw ( ::fwTools::Failed ) ;

    CTRLFIELD_API virtual void doUpdate() throw ( ::fwTools::Failed ) ;

    CTRLFIELD_API virtual void doUpdate( std::vector< ::fwTools::Object::wptr > objectsToUpdate, fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed ) ;
    //@}
private :

    ///Propagation slice index modification
    void updateSliceIndex(std::vector< ::fwTools::Object::wptr > objectsToUpdate, ::fwTools::Object::sptr srcObj);
    ///Propagation opacity modification (i.e. opacity leads to 1-opacity)
    void updateOpacity(std::vector< ::fwTools::Object::wptr > objectsToUpdate, ::fwTools::Object::sptr srcObj);
    ///Propagation slice type (orientation) modification
    void updateSliceType(std::vector< ::fwTools::Object::wptr > objectsToUpdate, fwServices::ObjectMsg::csptr _msg);
} ;

}

#endif /*CTRLFIELD_IMAGESYNCHRONIZER_HPP_*/
