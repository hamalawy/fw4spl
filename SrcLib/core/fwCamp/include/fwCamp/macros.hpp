/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWCAMP_MACROS_HPP__
#define __FWCAMP_MACROS_HPP__

#include <boost/preprocessor/seq/fold_left.hpp>

#include <camp/class.hpp>
#include <camp/camptype.hpp>
#include <camp/enum.hpp>

#include <fwCore/macros.hpp>

#define __FWCAMP_CAT(_s_, _state_, _elem_) BOOST_PP_CAT(_state_, _elem_)
#define __FWCAMP_NAMESPACE_CAT(_s_, _state_, _elem_) _state_::_elem_

#define __FWCAMP_FUNC_SUFFIX(desc) \
    BOOST_PP_SEQ_FOLD_LEFT( __FWCAMP_CAT, BOOST_PP_SEQ_HEAD(desc), BOOST_PP_SEQ_TAIL(desc))

#define __FWCAMP_NAMESPACE_NAME(desc) \
    ::BOOST_PP_SEQ_FOLD_LEFT( __FWCAMP_NAMESPACE_CAT, BOOST_PP_SEQ_HEAD(desc), BOOST_PP_SEQ_TAIL(desc))

#define __FWCAMP_DECLARE_FUNC_NAME(desc)        BOOST_PP_CAT(fwCampDeclare, __FWCAMP_FUNC_SUFFIX(desc))
#define __FWCAMP_DECLARE_LOCAL_FUNC_NAME(desc)  BOOST_PP_CAT(localDeclare, __FWCAMP_FUNC_SUFFIX(desc))

#define __FWCAMP_CLASS_BUILDER_TYPE(desc) \
    camp::ClassBuilder< __FWCAMP_NAMESPACE_NAME(desc) >

#define __FWCAMP_AUTO_TYPE_NONCOPYABLE(type, registerFunc) \
    CAMP_TYPE_NONCOPYABLE( type)

#define __FWCAMP__AUTO__DECLARE__MACRO(desc) \
    void __FWCAMP_DECLARE_FUNC_NAME(desc)(__FWCAMP_CLASS_BUILDER_TYPE(desc) &); \
    inline void __FWCAMP_DECLARE_LOCAL_FUNC_NAME(desc)() \
    { \
        __FWCAMP_CLASS_BUILDER_TYPE(desc) builder = \
            camp::Class::declare< __FWCAMP_NAMESPACE_NAME(desc) >(BOOST_PP_STRINGIZE( __FWCAMP_NAMESPACE_NAME( \
                                                                                          desc) )); \
        __FWCAMP_DECLARE_FUNC_NAME(desc)(builder); \
    } \
    __FWCAMP_AUTO_TYPE_NONCOPYABLE(__FWCAMP_NAMESPACE_NAME(desc), __FWCAMP_DECLARE_LOCAL_FUNC_NAME(desc))

#define __FWCAMP__REG_NAME(desc) \
    BOOST_PP_CAT(__FWCAMP_FUNC_SUFFIX(desc), Reg)

#define __FWCAMP__USEROBJREG(desc) \
    ::fwCamp::UserObjectRegistrar< __FWCAMP_NAMESPACE_NAME(desc)>
//----------------------------------------------------------------------------


/**
 *
 * desc is a BOOST_PP_SEQ
 * fwCampMakeFriendMacro((a)(b)(c)) expands to : Friend void ::fwCampDeclareabc(camp::ClassBuilder< ::a::b::c > &builder)
 */
#define fwCampMakeFriendDataMacro(desc) \
    friend void ::__FWCAMP_DECLARE_FUNC_NAME(desc)(__FWCAMP_CLASS_BUILDER_TYPE(desc) &builder); \

//----------------------------------------------------------------------------

#define fwCampMakeFriendMacro(desc) \
    friend void ::__FWCAMP_DECLARE_FUNC_NAME(desc)(__FWCAMP_CLASS_BUILDER_TYPE(desc) &builder); \

//----------------------------------------------------------------------------

#define fwCampDeclareMacro(classname) \
    CAMP_TYPE_NONCOPYABLE(classname)

//----------------------------------------------------------------------------

#define fwCampAutoDeclareDataMacro( desc, export) \
    namespace fwCamp \
    { \
    template<typename T> \
    struct UserObjectRegistrar; \
    } \
    fwCorePredeclare(desc) \
    __FWCAMP__AUTO__DECLARE__MACRO(desc)

//----------------------------------------------------------------------------

#define fwCampAutoDeclareMacro( desc, export) \
    fwCorePredeclare(desc) \
    export __FWCAMP__AUTO__DECLARE__MACRO(desc)

//----------------------------------------------------------------------------


#define fwCampAutoDeclareEnumMacro(desc) \
    void __FWCAMP_DECLARE_FUNC_NAME(desc)(camp::EnumBuilder &); \
    inline void __FWCAMP_DECLARE_LOCAL_FUNC_NAME(desc)() \
    { \
        camp::EnumBuilder builder = \
            camp::Enum::declare< __FWCAMP_NAMESPACE_NAME(desc) >(BOOST_PP_STRINGIZE( __FWCAMP_NAMESPACE_NAME(desc) )); \
        __FWCAMP_DECLARE_FUNC_NAME(desc)(builder); \
    } \
    CAMP_TYPE(__FWCAMP_NAMESPACE_NAME(desc))

//----------------------------------------------------------------------------


#define fwCampImplementMacro(desc) \
    void __FWCAMP_DECLARE_FUNC_NAME(desc)(__FWCAMP_CLASS_BUILDER_TYPE(desc) &builder)

//----------------------------------------------------------------------------


#define fwCampImplementDataMacro(desc) \
    static __FWCAMP__USEROBJREG(desc) __FWCAMP__REG_NAME(desc)(BOOST_PP_STRINGIZE( __FWCAMP_NAMESPACE_NAME(desc) )); \
    void __FWCAMP_DECLARE_FUNC_NAME(desc)(__FWCAMP_CLASS_BUILDER_TYPE(desc) &builder)

//----------------------------------------------------------------------------


/**
 * @brief Implementation of enumeration into camp introspection
 * @see fwCampAutoDeclareEnumMacro
 */
#define fwCampImplementEnumMacro(desc) \
    void __FWCAMP_DECLARE_FUNC_NAME(desc)(camp::EnumBuilder &builder)



/**
 * @brief This macro is used when you want to set a smart pointer into an object
 * @param object the type of the main object ex: (fwData)(Mesh)
 * @param attribut the type of smart pointer attribut ex: (fwData)(Array)
 */
#define fwCampDeclareAccessor(object, attribut) \
    namespace camp { \
    namespace detail { \
\
    template <> \
    class Accessor1< __FWCAMP_NAMESPACE_NAME(object), SPTR( __FWCAMP_NAMESPACE_NAME(attribut) )& > \
    { \
    public: \
        typedef SPTR ( __FWCAMP_NAMESPACE_NAME (attribut) )& ReturnType; \
        typedef ObjectTraits< SPTR( __FWCAMP_NAMESPACE_NAME(attribut) )& > Traits; \
        typedef Traits::DataType DataType; \
        typedef __FWCAMP_NAMESPACE_NAME (object) ClassType; \
\
        enum \
        { \
            canRead  = true, \
            canWrite = true \
        }; \
\
        template <typename F> \
        Accessor1(F getter) :              \
            m_getter(getter) \
        { \
        } \
\
        ReturnHelper< ReturnType>::Type get(ClassType& objectptr) const \
        { \
            ReturnType ptr = m_getter(objectptr); \
            return ReturnHelper< ReturnType>::get(ptr); \
        } \
\
        bool set(ClassType& objectptr, const Value &value) const \
        { \
            ReturnType ptr = m_getter(objectptr); \
            ptr = __FWCAMP_NAMESPACE_NAME(attribut) ::dynamicCast(value.to< DataType* >()->getSptr()); \
            return true; \
        } \
\
    private: \
\
        boost::function< ReturnType (ClassType&)> m_getter; \
    }; \
\
    } \
    }




#endif // __FWCAMP_MACROS_HPP__







