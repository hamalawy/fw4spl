/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWSERVICES_OBJECTMSG_HPP_
#define _FWSERVICES_OBJECTMSG_HPP_

#include <map>
#include <string>
#include <vector>

#include <boost/function.hpp>

#include <fwTools/Object.hpp>
#include <fwData/Object.hpp>

#include "fwServices/factory/message/new.hpp"
#include "fwServices/registry/message/detail.hpp"
#include "fwServices/config.hpp"

namespace fwServices
{

class IService;

/**
 * @brief Object message : indicate modifications having occurred or being performed on a object.
 * @class ObjectMsg
 * 
 * @date 2007-2009.
 *
 * The message only provides identifiers enabling to retrieve the modified information on the object. This should be embed the information. The m_subject is present
 * to check that the considered object (subject to modifications) is the one known (or referenced) by the service receiving the message through the observation mechanism.
 *
 * The m_subject should a priori not be used by a service to access the object. Indeed, to be able to work without observation (therefore without messages, this being possible as observations are optional), a service requires
 * a reference to the object allowing to perform computations. This reference is generally acquired either at attachment time (e.g. the object of interest being the one the considered service is attached to) or at configuration time (e.g. passing an unique universal identifier in the XML configuration).
 * Therefore, for coherence and homogeneity, a service should be able to access object(s) it requires in the same way, whatever the working mode (i.e. explicit execution without observations or implicit execution with observation).
 *
 * Concerning the type of m_subject : a software instance is based on a (root) tree structured complex object (to which services are attached, corresponding to functionalities). So that
 * the root object naturally destroys through unreferencement, it must be avoided that instances (except pure data objects) keep strong references on data objects composing the root, otherwise
 * there is a risk of cross strong references, forgiving the natural destruction of objects. For this reason, messages should also respect this rule and the m_subject should be a weak reference (weak pointer).
 *
 * The ObjectMsg contains event identifiers with associated additional information.
 */
class FWSERVICES_CLASS_API ObjectMsg : public ::fwTools::Object
{

public:

    typedef ::fwServices::factory::message::Key Key;

    /**
     * @brief Class used to register a class factory in factory registry.
     * This class defines also the objectMsg factory ( 'create' )
     *
     * @tparam T Factory product type
     */
    template <typename T>
    class Registrar
    {
    public:
        Registrar()
        {
            ::fwServices::registry::message::get()->addFactory(T::classname(), &::fwServices::factory::message::New<T>);
        }
    };


    fwCoreClassDefinitionsWithFactoryMacro( (ObjectMsg)(::fwTools::Object), (()), ::fwServices::factory::message::New< ObjectMsg > );

    /// Defines callback type
    typedef ::boost::function< void () > MessageCallbackType;

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWSERVICES_API ObjectMsg(::fwServices::ObjectMsg::Key key);

    /// Destructor, do nothing.
    FWSERVICES_API virtual ~ObjectMsg();

    // generic message

    /// Event identifier used to inform that an object was created
    FWSERVICES_API static std::string NEW_OBJECT;

    /// Event identifier used to inform that an object was modified
    FWSERVICES_API static std::string UPDATED_OBJECT;

    /// Event identifier used to inform that an object has to be removed
    FWSERVICES_API static std::string DELETE_OBJECT;

    /// Event identifier used to inform that fields were added, removed or changed
    //@{
    FWSERVICES_API static std::string ADDED_FIELDS;
    FWSERVICES_API static std::string REMOVED_FIELDS;
    FWSERVICES_API static std::string CHANGED_FIELDS;
    //@}

    /**
     * @name Event management
     */
    //@{

    /**
     * @brief This method add an event to the message before message notification.
     * @param[in] _eventId the event added in the message
     * @param[in] _pDataInfo additional information concerning message ( it is an optional parameter )
     * @note a message can contain many events.
     */
    FWSERVICES_API void addEvent( std::string _eventId , ::fwData::Object::csptr _pDataInfo = ::fwData::Object::sptr() );

    /**
     * @brief This method is used to know if a specific event declared in the message
     * @param[in] _eventId id of the event researched
     * @return true if message contain the event id
     */
    FWSERVICES_API bool hasEvent( std::string _eventId ) const;

    /**
     * @brief This method returns all id event contained in the message
     * @return the list of event
     */
    FWSERVICES_API std::vector< std::string > getEventIds() const ;

    /**
     * @brief This method returns the data info associated to event, this data can be an null ptr when the event not needs some additional informations.
     * @param[in] _eventId event id used to find his associated data
     * @return associated data information
     */
    FWSERVICES_API ::fwData::Object::csptr getDataInfo( std::string _eventId ) const;

    //@}


    /**
     * @name Management of source/subject message
     */
    //@{

    /// Set message source (service which notifies subject obsevers)
    FWSERVICES_API void setSource( WPTR(::fwServices::IService) _source );

    /// Get message source (service which notifies subject obsevers)
    FWSERVICES_API WPTR(::fwServices::IService) getSource() const;

    /// Set message subject (object observed)
    FWSERVICES_API void setSubject( ::fwData::Object::wptr _subject );

    /// Get message subject (object observed)
    FWSERVICES_API ::fwData::Object::wptr getSubject() const;

    //@}


    /**
     * @name Field events
     */
    //@{

    typedef std::map<std::string, ::fwData::Object::sptr> ModifiedFieldsContainerType;
    typedef ::fwData::Object::FieldNameType FieldNameType;

    /**
     * @brief       Append an object to the added fields list.
     * @param[in]   fieldName : the field name of the added object.
     * @param[in]   object    : the added object.
     */
    FWSERVICES_API void appendAddedField( const FieldNameType &fieldName, ::fwData::Object::sptr object );

    /**
     * @brief   Return the map of the added fields.
     */
    FWSERVICES_API const ModifiedFieldsContainerType &getAddedFields() const;

    /**
     * @brief       Append an object to the removed fields list.
     * @param[in]   fieldName : the field name of the removed object.
     * @param[in]   object    : the object which will be removed.
     */
    FWSERVICES_API void appendRemovedField( const FieldNameType &fieldName, ::fwData::Object::sptr object );

    /**
     * @brief   Return the map of the removed fields.
     */
    FWSERVICES_API const ModifiedFieldsContainerType &getRemovedFields() const;

    /**
     * @brief       Append an object to the changed fields list.
     * @param[in]   fieldName : the field name of the changed object.
     * @param[in]   oldObject : the old object which will be replaced.
     * @param[in]   newObject : the new object.
     */
    FWSERVICES_API void appendChangedField( const FieldNameType &fieldName, ::fwData::Object::sptr oldObject, ::fwData::Object::sptr newObject );

    /**
     * @brief   Return a map of old object in changed fields list.
     */
    FWSERVICES_API const ModifiedFieldsContainerType &getOldChangedFields() const;

    /**
    * @brief   Return a map of new object in changed fields list.
    */
    FWSERVICES_API const ModifiedFieldsContainerType &getNewChangedFields() const;

    //@}


    /// Give some message informations in a string.
    FWSERVICES_API std::string getGeneralInfo() const;

    /**
     * @brief Streaming a service
     * @see ObjectMsg::operator<<(std::ostream & _ostream, ObjectMsg& _service)
     * @note Invoke ObjectMsg::info( std::ostream )
     */
    FWSERVICES_API friend std::ostream & operator<<(std::ostream & _sstream, const ObjectMsg& _message) ;

    /// Set a callback to the message which will be executed during message destruction
    FWSERVICES_API void setMessageCallback( MessageCallbackType callback );

protected :

    /// Constructor, do nothing.
    FWSERVICES_API ObjectMsg();

    /**
     * @brief Intern map which associates an Event Id (string) to a dataInfo ( ::fwData::Object ).
     *
     * Some event has not data info, in this cases, the event is in map  but the ::fwData::Object::sptr is null.
     */
    std::map< std::string , ::fwData::Object::csptr > m_eventId2DataInfo;

    /// Give some message informations, this method uses getGeneralInfo.
    FWSERVICES_API virtual void info(std::ostream &_sstream ) const ;

    ModifiedFieldsContainerType m_removedFields;
    ModifiedFieldsContainerType m_addedFields;
    ModifiedFieldsContainerType m_oldChangedFields;
    ModifiedFieldsContainerType m_newChangedFields;

private :

    /// Message source. It is the service which creates msg and sends it at all subject observers
    WPTR(::fwServices::IService) m_source;

    /// Message subject. All subject observers will be notified.
    ::fwData::Object::wptr m_subject;

    /// Helper to convert string UUID/Classname in pretty string
    static std::string convertToLightString( std::string _initialString );

    /// Callback calls during destruction
    MessageCallbackType m_callback;

    /// Permits to know if message has a callback
    bool m_hasCallback;
};


} // namespace fwServices

#endif //_FWSERVICES_OBJECTMSG_HPP_

