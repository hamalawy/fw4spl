/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef _PROC_IPROC_HPP_
#define _PROC_IPROC_HPP_

#include <fwServices/IService.hpp>
#include "proc/config.hpp"

/**
 * Namespace dedicated to data processing
 */
namespace proc
{

/**
 * @brief Service type dedicated to data processing such as image processing
 * It can concern tasks such as segmentation algorithms, excluding visualization, GUI, ...
 * This class is able to manage the interpretation of such a configuration (where implementation :
 * \code
 * <object ...>
 * 	 <object id="input" type="::fwData::Image" />
 *   <service uid="..." implementation="..." 	type="::proc::IProc" autoComChannel="...">
 *       		<object id="InImage" 		type="::fwData::Image" export="input"/>
 *       		<object id="parameter" 		type="::fwData::Float" value="255.5"/>
 *   </service>
 * </object>
 * \endcode
 * Where the implementation is related to an inheritating class.
 * The inheritated class declares a element named InImage of type Image invoking, in its constructor:
 * \code
 * this->addAttribute< ::fwData::Image >("InImage")
 * \endcode
 * InImage is an hard coded string in the implementation class.
 *
 * From the configuration, if the key word export is present, as for InImage (with value input"), the associate
 * attribute is assumed to be affected to the object to which this service is attached to. In the configuring method,
 * this information is maintained in the m_localToExportedAttributes attribute and is managed by using the method
 * \code
 * this->exportAttribute("InImage","input")
 * \endcode
 * If the export key word is not specified (as for parameter), the attribute remain local to the service (m_localAttributes)
 * Using this formalism, the configuring method is concise and reuse the general fwServices method
 * \code
 *  ::fwTools::Object::sptr ::fwServices::New(::fwRuntime::ConfigurationElement::sptr _elt)
 * \endcode
 *
 * The getAttribute method enable to access the attribute, hiding the fact that it is exported or not
 *
 * @note An implementation service can be attached either to ::fwTools::Object or ::fwData::ProcessObject (warning: not completely implemented) or ::fwData::Composite.
 * For generic purposes, it should be limited to ::fwTools::Object. Indeed, otherwise a service can not be attached to data of type ::fwData::Image for instance, which
 * remains possible with ::fwTools::Object
 */
class PROC_CLASS_API IProc : public fwServices::IService
{
public :

    fwCoreServiceClassDefinitionsMacro ( (IProc)(::fwServices::IService) ) ;

    /// Constructor
	PROC_API IProc() throw() ;

    /// Desstructor
	PROC_API virtual ~IProc() throw() ;

	/// Manage configuration
	PROC_API virtual void configuring() throw(::fwTools::Failed);

	/// Invoke doStart()
	PROC_API virtual void starting() throw(::fwTools::Failed);
	/// Invoke doStop()
	PROC_API virtual void stopping() throw(::fwTools::Failed) ;
	/// Invoke doUpdate()
	PROC_API virtual void updating() throw(::fwTools::Failed) ;
	/// Invoke doUpdate(_mssg)
	PROC_API virtual void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _mssg ) throw(::fwTools::Failed) ;

protected :

	/// To be overwritten
	PROC_API virtual void doConfigure() throw(::fwTools::Failed) = 0  ;
	/// To be overwritten
	PROC_API virtual void doStart() throw(::fwTools::Failed) = 0  ;
	/// To be overwritten
	PROC_API virtual void doUpdate() throw(::fwTools::Failed) = 0  ;
	/// To be overwritten
	PROC_API virtual void doUpdate(::boost::shared_ptr< const fwServices::ObjectMsg > _mssg) throw(::fwTools::Failed) = 0  ;
	/// To be overwritten
	PROC_API virtual void doStop() throw(::fwTools::Failed) = 0  ;


	template< class TYPE > SPTR(TYPE) addAttribute(std::string _name);

	template< class TYPE > SPTR(TYPE) getAttribute(std::string _name);

	/**
	 * @brief Set the attribute value
	 * @note be carefull using this method because the pointer changes which can lead to unexpected behavior. E.g. in the example given, object referenced by input is
	 * replaced by the the _obj. This means the initial input image is removed and probably deleted, as well as all its services: an application can become unstable.
	 */
	PROC_API void setAttribute(std::string _name, ::fwTools::Object::sptr _obj);


	PROC_API ::fwTools::Object::sptr getAttribute(std::string _name);

private :

	/**
	 * The local attributes is exported to this->getObject(). If the object with a similar id is already present, nothing is done.
	 * At the end of this method: m_localAttributes has no entry key for _localId and m_exportedAttributes has a key _exportId.
	 * m_localToExportedAttributes preserves the mapping.
	 */
	void exportAttribute(std::string _localId, std::string _exportId);

	/**
	 * using the API of ::fwData::Composite -> export using the refMap
	 * @DEPRECIATED
	 */
	void exportForComposite(std::string _exportId , ::fwTools::Object::sptr _obj) ;
	/**
	 * using the API of ::fwData::ProcessObject -> Does nothing
	 * @DEPRECIATED
	 */
	void exportForProcessObject(std::string _exportId , ::fwTools::Object::sptr _obj) ;

	/// using the API of ::fwTools::Object
	void exportForObject(std::string _exportId , ::fwTools::Object::sptr _obj) ;

	typedef std::vector< std::string > DynamicAttributeIdentifierContainer;
	DynamicAttributeIdentifierContainer m_dynamicAttributeIdentifiers;
	/**
	 * Equivalent to the identified field in ::fwTools::Object
	 */
	typedef std::map< std::string , ::fwTools::Object::sptr > AttributeContainer;
	AttributeContainer  m_localAttributes ;

	/**
	 * Map binding local to export
	 */
	typedef std::map< std::string , std::string > MappingContainer;
	MappingContainer m_localToExportedAttributes ;

	///Helper
	void printInfo();
};


}

#include "proc/IProc.hxx"

#endif // _PROC_IPROC_HPP_
