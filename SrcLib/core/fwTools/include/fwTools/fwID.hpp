/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWTOOLS_FWID_HPP_
#define _FWTOOLS_FWID_HPP_

#include <string>

#include <boost/unordered_map.hpp>

#include <fwCore/base.hpp>
#include <fwCore/mt/types.hpp>

#include "fwTools/config.hpp"
#include "fwTools/macros.hpp"

namespace fwTools
{

class Object;
/**
 * @brief   Defines ID for fwTools::Object. It is used to associate ID with object.
 * @class   fwID
 * 
 * @date    2007-2012.
 */
class FWTOOLS_CLASS_API fwID
{
public:

    fwCoreNonInstanciableClassDefinitionsMacro( (fwID) );

    typedef std::string IDType;


    typedef enum {
        EMPTY    = 1, ///< return an empty id if no one set
        GENERATE    , ///< generate a new id if necessary
        MUST_EXIST    ///< throw an exception if object has not id.
    } Policy;

    /**
     * Test if the given id exist (i.e recorded in fwID dictionary)
     * @param[in] _id : the id to test.
     * @return true iff the given id is recorded in fwID dictionary.
     * @note This method is thread-safe.
     */
    FWTOOLS_API static bool exist( IDType _id);

    /**
     * @brief Retrieve the object attached to the given id. Return a null sptr if no correspondence exist.
     * @note This method is thread-safe.
     */
    FWTOOLS_API static SPTR(::fwTools::Object ) getObject( IDType requestID );

    FWTOOLS_API virtual ~fwID() ;

protected :

    // API to expose in fwTools::Object
    /**
     * @brief Return true if the object has an id set.
     * @note This method is thread-safe.
     */
    FWTOOLS_API bool   hasID() const;

    /**
     * @brief Returns the id of the object. If it is not set and the policy value is
     * \li EMPTY then an empty id is returned
     * \li GENERATE (default) then a new ID will be generated (and recorded ) using the pattern "CLASSNAME-NUM". NUM is always increasing
     * \li MUST_EXIST then an exception Failed is raised
     * @note We consider an object be constant whatever if its id is generated.
     * @note This method is thread-safe.
     */
    FWTOOLS_API IDType getID( Policy  policy=GENERATE ) const;

#ifdef COM_LOG
    /**
     * @brief Returns a light version the id of the object. Used for log. This method calls getID and replaces id using
     * pattern "::NS1::NS2::CLASSNAME-NUM" by a light version "CLASSNAME-NUM"
     * \li EMPTY then an empty id is returned
     * \li GENERATE (default) then a new ID will be generated (and recorded ) using the pattern "CLASSNAME-NUM". NUM is always increasing
     * \li MUST_EXIST then an exception Failed is raised
     * @note We consider an object be constant whatever if its id is generated.
     * @note This method is thread-safe.
     */
    FWTOOLS_API IDType getLightID( Policy  policy=GENERATE ) const;
#endif

    /**
      * @brief Set a newID  for the object, (newID must not exist in fwID), the oldest one is released.
      * @warning Cannot set a empty ID.
      * @note This method is thread-safe.
      */
    FWTOOLS_API virtual void   setID( IDType newID ); // cannot set a empty one

    /**
      * @brief Release the id for the object.
      * @note This method is thread-safe
      */
    FWTOOLS_API  void   resetID();

    /**
      * @brief   Constructor : does nothing.
      */
    fwID() {}; // cannot be instantiated


private :

    /**
      * @brief Will generate a new ID using the pattern "CLASSNAME-NUM". NUM is always increasing.
      * @note This method is NOT thread-safe.
      */
    IDType generate() const;

    /**
     * @brief Remove ID from the dictionary.
     * @note This method is NOT thread-safe.
     */
    static void removeIDfromDictionary(IDType _id );

    /**
      * @brief return true if the  _id is found in the dictionary.
      * @note This method is NOT thread-safe.
      */
    static bool isIdFound( IDType _id);

    /**
      * @brief Add newID in the dictionary (newID must not exist in fwID).
      * @note This method is NOT thread-safe
      */
    void addIDInDictionary( IDType newID );

   IDType m_id;

   typedef ::boost::unordered_map< IDType, WPTR(::fwTools::Object ) > Dictionary;
   typedef ::boost::unordered_map< std::string , ::boost::uint32_t >  CategorizedCounter;

   static  Dictionary m_dictionary;
   static  CategorizedCounter m_CategorizedCounter;

   /// Mutex used to lock dictionary access
   static ::fwCore::mt::ReadWriteMutex s_dictionaryMutex;

   /// Mutex used by generate() to lock m_CategorizedCounter changes.
   static ::fwCore::mt::Mutex s_mutexCounter;

   /// Mutex used to lock m_id access
   mutable ::fwCore::mt::ReadWriteMutex m_idMutex;
};

}


#endif /* FWID_HPP_ */
