/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWCOM_SLOT_HXX__
#define __FWCOM_SLOT_HXX__

#ifndef __FWCOM_SLOT_HPP__
#error fwCom/Slot.hpp not included
#endif

#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/make_shared.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include "fwCom/util/AutoBind.hpp"
#include "fwCom/util/AutoBind.hxx"

#include "fwCom/SlotCall.hxx"

namespace fwCom
{

//-----------------------------------------------------------------------------

template<typename R, typename ... A >
Slot< R ( A ... ) >::Slot() : SlotCall< R (A ...) >()
{
    // 'this->' is needed by gcc 4.2
    this->SlotBase::m_signature = SlotBase::getTypeName< R ( A ... ) >();
}

//-----------------------------------------------------------------------------

template<typename R, typename ... A >
template<typename F>
SPTR( Slot< R ( A ... ) > ) Slot< R ( A ... ) >::New( F f )
{
    return newSlot(f);
}

//-----------------------------------------------------------------------------

template<typename R, typename ... A >
template<typename F, typename O>
SPTR( Slot< R ( A ... ) > ) Slot< R ( A ... ) >::New( F f, O o )
{
    return newSlot(f, o);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename R, typename ... A >
template< typename F >
Slot< Slot< R ( A ... ) > >::Slot( SPTR( SlotRun< F > )slot )
    : Slot< FunctionType >(
          ::fwCom::util::AutoBind<
              SignatureType,
              ::boost::function_types::function_arity< F >::value
              >::wrap( &SlotRun< F >::run, slot.get() ) )
{
    BOOST_STATIC_ASSERT( (::boost::is_same<void, R>::value) );
    this->setWorker(slot->getWorker());
#ifdef COM_LOG
    this->setID("wrapped_"+ slot->getID());
#endif
}

//-----------------------------------------------------------------------------

template<typename R, typename ... A >
template< typename F >
Slot< Slot< R ( A ... ) > >::Slot( SPTR( Slot< F > )slot ) : Slot< FunctionType >(
                                                                 ::fwCom::util::AutoBind<
                                                                     SignatureType,
                                                                     ::boost::function_types::function_arity< F >::value
                                                                     >::wrap( &Slot< F >::call, slot.get() ) )
{
    this->setWorker(slot->getWorker());
#ifdef COM_LOG
    this->setID("wrapped_"+ slot->getID());
#endif
}

//-----------------------------------------------------------------------------

template<typename R, typename ... A >
template<typename F >
SPTR(Slot< R( A ...  )>) Slot< Slot< R ( A ... )> >::New( SPTR( SlotRun< F > ) slot )
{
    assert (::boost::function_types::function_arity< F >::value <=
            ::boost::function_types::function_arity< R ( A ... ) >::value);
    return ::boost::make_shared< Slot< Slot< R ( A ... ) > > >( slot );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename F, typename ... BINDING>
SPTR(Slot< typename ::fwCom::util::convert_function_type< F >::type >) newSlot(F f, BINDING ... binding)
{
    SLM_ASSERT( "Too many arguments", ( sizeof ... (binding) < 2 ) );
    typedef ::boost::function< typename ::fwCom::util::convert_function_type< F >::type > FunctionType;
    FunctionType func = ::fwCom::util::autobind(f, binding ...);
    return ::boost::make_shared< Slot< FunctionType > > ( func );
}

//-----------------------------------------------------------------------------

} // namespace fwCom

#endif /* __FWCOM_SLOT_HXX__ */


