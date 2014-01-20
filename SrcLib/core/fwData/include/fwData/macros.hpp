/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_MACROS_HPP__
#define __FWDATA_MACROS_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/mpl/or.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_fundamental.hpp>

#include <fwCore/macros.hpp>
/**
 * @file fwData/macros.hpp
 * @brief fwData specific macros.
 *
 * 
 */


namespace fwData
{
template<typename T>
struct is_smart_ptr : ::boost::mpl::false_ { };

template<typename T>
struct is_smart_ptr< SPTR(T) > : ::boost::mpl::true_ { };

template<typename T>
struct is_smart_ptr< WPTR(T) > : ::boost::mpl::true_ { };

} //namespace fwData

/*
 * macros used for getters/setters
 *
 * funcName : name of the function ("Firstname" for getFirstname() function)
 * var : name of the variable without "m_" ("firstname" for m_firstname variable)
 * type : type of the variable (float, std::string, ...)
 * text : comment used in doxygen
 */
//-----------------------------------
// Getter/Setter macros
//-----------------------------------
#define __FWDATA_CLASS_ATTRIBUTE_PREFIX m_
#define __FWDATA_GENERATED_ATTRIBUTE_PREFIX attr

//-----------------------------------

#define fwDataGetMacro(attrName, attrType)                                                    \
attrType get##attrName () const                                                               \
{                                                                                             \
BOOST_MPL_ASSERT(                                                                             \
        ( boost::mpl::or_< boost::is_fundamental< attrType > , boost::is_enum< attrType > > ) \
        /* This macro is intended to be used with fundamental types. */                       \
        );                                                                                    \
    return BOOST_PP_CAT(                                                                      \
                       __FWDATA_CLASS_ATTRIBUTE_PREFIX,                                       \
                       BOOST_PP_CAT(__FWDATA_GENERATED_ATTRIBUTE_PREFIX,attrName)             \
                       );                                                                     \
}

#define fwDataSetMacro(attrName, attrType)                                                    \
void set##attrName (attrType val)                                                             \
{                                                                                             \
BOOST_MPL_ASSERT(                                                                             \
        ( boost::mpl::or_< boost::is_fundamental< attrType > , boost::is_enum< attrType > > ) \
        /* This macro is intended to be used with fundamental types. */                       \
        );                                                                                    \
    BOOST_PP_CAT(                                                                             \
                __FWDATA_CLASS_ATTRIBUTE_PREFIX,                                              \
                BOOST_PP_CAT(__FWDATA_GENERATED_ATTRIBUTE_PREFIX,attrName)                    \
                ) = val;                                                                      \
}

#define fwDataGetSptrMacro(attrName, attrType)                                       \
attrType get##attrName () const                                                      \
{                                                                                    \
BOOST_MPL_ASSERT(                                                                    \
        ( ::fwData::is_smart_ptr< attrType > )                                       \
        /* This macro is intended to be used with smart_ptr types. */                \
        );                                                                           \
    return BOOST_PP_CAT(                                                             \
                        __FWDATA_CLASS_ATTRIBUTE_PREFIX,                             \
                        BOOST_PP_CAT( __FWDATA_GENERATED_ATTRIBUTE_PREFIX, attrName) \
                        );                                                           \
}

#define fwDataSetSptrMacro(attrName, attrType)                               \
void set##attrName (attrType val)                                            \
{                                                                            \
BOOST_MPL_ASSERT(                                                            \
        ( ::fwData::is_smart_ptr< attrType > )                               \
        /* This macro is intended to be used with smart_ptr types. */        \
        );                                                                   \
    BOOST_PP_CAT(                                                            \
                __FWDATA_CLASS_ATTRIBUTE_PREFIX,                             \
                BOOST_PP_CAT( __FWDATA_GENERATED_ATTRIBUTE_PREFIX, attrName) \
                ) = val;                                                     \
}

//-----------------------------------

#define fwDataGetCRefMacro(attrName, attrType)                                                     \
const attrType &get##attrName () const                                                             \
{                                                                                                  \
BOOST_MPL_ASSERT(                                                                                  \
        ( boost::is_class< attrType > )                                                            \
        /* This macro is intended to be used with non fundamental types and non smart-pointers. */ \
        );                                                                                         \
    return BOOST_PP_CAT(                                                                           \
                        __FWDATA_CLASS_ATTRIBUTE_PREFIX,                                           \
                        BOOST_PP_CAT(__FWDATA_GENERATED_ATTRIBUTE_PREFIX, attrName)                \
                        );                                                                         \
}

#define fwDataSetCRefMacro(attrName, attrType)                                                     \
void set##attrName (const attrType &val)                                                           \
{                                                                                                  \
BOOST_MPL_ASSERT(                                                                                  \
        ( boost::is_class< attrType > )                                                            \
        /* This macro is intended to be used with non fundamental types and non smart-pointers. */ \
        );                                                                                         \
    BOOST_PP_CAT(                                                                                  \
                __FWDATA_CLASS_ATTRIBUTE_PREFIX,                                                   \
                BOOST_PP_CAT(__FWDATA_GENERATED_ATTRIBUTE_PREFIX, attrName)                        \
                ) = val;                                                                           \
}

//-----------------------------------

#define fwDataGetSetMacro(attrName, attrType) \
    /** @{ */                                 \
    fwDataGetMacro(attrName, attrType)        \
    fwDataSetMacro(attrName, attrType)        \
    /** @}  */

#define fwDataGetSetSptrMacro(attrName, attrType) \
    /** @{ */                                     \
    fwDataGetSptrMacro(attrName, attrType)        \
    fwDataSetSptrMacro(attrName, attrType)        \
    /** @}  */

#define fwDataGetSetCRefMacro(attrName, attrType) \
    /** @{ */                                     \
    fwDataGetCRefMacro(attrName, attrType)        \
    fwDataSetCRefMacro(attrName, attrType)        \
    /** @}  */



//-----------------------------------
// Old Getter/Setter macros
//-----------------------------------

#define fwGettersSettersMacro(funcName, var, type) \
    /** @name funcName accessor      */            \
    /** @{                           */            \
    fwGettersMacro(funcName, var, type);           \
    fwSettersMacro(funcName, var, type);           \
    /**@}                            */

#define fwGettersSettersDocMacro(funcName, var, type, text ) \
    /** text                         */                      \
    /** @name funcName accessor      */                      \
    /** Get/Set text                 */                      \
    /** @{                           */                      \
    fwGettersMacro(funcName, var, type);                     \
    fwSettersMacro(funcName, var, type);                     \
    /**@}                            */

//-----------------------------------
// Setter
//-----------------------------------
#define fwSettersMacro(funcName, var, type) \
    fwSetMacro(funcName, var, type);        \
    fwSetCRefMacro(funcName, var, type);

//-----------------------------------
//
#define fwSetMacro(funcName, var, type)    \
    void set##funcName (const type _##var) \
    {                                      \
        this->m_##var = _##var;            \
    }

//-----------------------------------
//
#define fwSetCRefMacro(funcName, var, type)      \
    void setCRef##funcName (const type & _##var) \
    {                                            \
        this->m_##var = _##var;                  \
    }

//-----------------------------------


//-----------------------------------
// Getter
//-----------------------------------
#define fwGettersMacro(funcName, var, type) \
    fwGetMacro(funcName, var, type);        \
    fwGetRefMacro(funcName, var, type);     \
    fwGetCRefMacro(funcName, var, type);


//-----------------------------------
//
#define fwGetMacro(funcName, var, type) \
    const type get##funcName () const   \
    {                                   \
        return m_##var;                 \
    }

//-----------------------------------
//
#define fwGetRefMacro(funcName, var, type) \
    type & getRef##funcName ()             \
    {                                      \
        return this->m_##var;              \
    }

//-----------------------------------
//
#define fwGetCRefMacro(funcName, var, type) \
    const type & getCRef##funcName () const \
    {                                       \
        return this->m_##var;               \
    }

#endif //__FWDATA_MACROS_HPP__

