/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
#ifndef __FWCOM_SLOTBASE_HPP__
#define __FWCOM_SLOTBASE_HPP__

#include <set>

#include <boost/thread/future.hpp>

#ifdef COM_LOG
#include <boost/lexical_cast.hpp>
#endif

#include <fwCore/mt/types.hpp>
#include <fwCore/BaseObject.hpp>

#include <fwThread/Worker.hpp>

#include "fwCom/config.hpp"
#include "fwCom/util/convert_function_type.hpp"
#include "fwCom/SlotConnectionBase.hpp"

namespace fwCom
{

template< typename F >
struct SlotRun;

template< typename F >
struct Slot;

/**
 * @brief Base class for Slot implementations.
 */
struct FWCOM_CLASS_API SlotBase : virtual fwCore::BaseObject
{

    /**
     * @name typedefs
     * @{
     * Slot pointer types. */
    typedef SPTR( SlotBase ) sptr;
    typedef WPTR( SlotBase ) wptr;
    typedef SPTR( SlotBase const ) csptr;
    typedef WPTR( SlotBase const ) cwptr;

    typedef std::string IDType;
    /**  @} */

    /// SlotBase::asyncRun return type.
    typedef ::boost::shared_future< void > VoidSharedFutureType;

    /// Connections container type
    typedef std::set< SlotConnectionBase::csptr > ConnectionSetType;

    virtual ~SlotBase() {};

    /**
     * @brief Returns Slot's arity.
     * The arity defines the number of parameters in Slot signature.
     */
    unsigned int arity() const
    {
        return m_arity;
    }

    /// Sets Slot's Worker.
    void setWorker(const ::fwThread::Worker::sptr &worker)
    {
        SLM_ASSERT("Changing worker on the fly is currently not supported", !m_worker || worker == m_worker);
        ::fwCore::mt::WriteLock lock(m_workerMutex);
        m_worker = worker;
    }

    /// Returns Slot's Worker.
    ::fwThread::Worker::sptr getWorker() const
    {
        ::fwCore::mt::ReadLock lock(m_workerMutex);
        return m_worker;
    }

    /**
     * @brief  Run the Slot.
     * @throw  BadRun if given arguments do not match the slot implementation
     */
    /**
     * @name Run helpers
     * @{ */
    template< typename A1, typename A2, typename A3 > void run(A1 a1, A2 a2, A3 a3) const;
    template< typename A1, typename A2 > void run(A1 a1, A2 a2) const;
    template< typename A1 > void run(A1 a1) const;
    FWCOM_API void run() const;
    /**  @} */

    /**
     * @brief  Call the Slot (with return value).
     * @throw  BadCall if given arguments do not match the slot implementation
     */
    /**
     * @name Call helpers
     * @{ */
    template< typename R, typename A1, typename A2, typename A3 > R call(A1 a1, A2 a2, A3 a3) const;
    template< typename R, typename A1, typename A2 > R call(A1 a1, A2 a2) const;
    template< typename R, typename A1 > R call(A1 a1) const;
    template< typename R > R call() const;
    /**  @} */

    /**
     * @brief  Run the Slot asynchronously.
     * @throw  BadRun if given arguments do not match the slot implementation
     * @throws NoWorker if given worker is not valid.
     */
    /**
     * @name Asynchronous run helpers
     * @{ */
    template< typename A1, typename A2, typename A3 > VoidSharedFutureType asyncRun(A1 a1, A2 a2, A3 a3) const;
    template< typename A1, typename A2 > VoidSharedFutureType asyncRun(A1 a1, A2 a2) const;
    template< typename A1 > VoidSharedFutureType asyncRun(A1 a1) const;
    FWCOM_API VoidSharedFutureType asyncRun() const;
    /**  @} */

    /**
     * @brief  Call the Slot asynchronously (with return value).
     * @throw  BadCall if given arguments do not match the slot implementation
     * @throws NoWorker if given worker is not valid.
     */
    /**
     * @name Asynchronous call helpers
     * @{ */
    template< typename R, typename A1, typename A2, typename A3 > ::boost::shared_future< R > asyncCall(A1 a1, A2 a2, A3 a3) const;
    template< typename R, typename A1, typename A2 > ::boost::shared_future< R > asyncCall(A1 a1, A2 a2) const;
    template< typename R, typename A1 > ::boost::shared_future< R > asyncCall(A1 a1) const;
    template< typename R > ::boost::shared_future< R > asyncCall() const;
    /**  @} */

    /// Returns number of connections.
    size_t getNumberOfConnections() const
    {
        ::fwCore::mt::ReadLock lock(m_connectionsMutex);
        return m_connections.size();
    }


protected:

    /**
     * @name SlotBase's friends
     * @{ */
    template < typename F >
    friend struct SlotConnection;

    template < typename F >
    friend struct Signal;
    /**  @} */


    /// Returns F typeid name.
    template < typename F >
    std::string getTypeName() const
    {
        std::string signature = std::string("function_type(") + typeid(F).name() + ")";
        return signature;
    }

    SlotBase(unsigned int arity) : m_arity(arity)
    {
#ifdef COM_LOG
        ::fwCore::mt::ScopedLock lock(s_mutexCounter);
        m_id = "Slot-" + ::boost::lexical_cast<std::string>(s_idCount++);
#endif
    };


    /// Slot's signature based on typeid.
    std::string m_signature;

    /// Slot's arity.
    const unsigned int m_arity;

    /// Slot's Worker.
    ::fwThread::Worker::sptr m_worker;

    /// Container of current connections.
    ConnectionSetType m_connections;

    mutable ::fwCore::mt::ReadWriteMutex m_connectionsMutex;
    mutable ::fwCore::mt::ReadWriteMutex m_workerMutex;

#ifdef COM_LOG

public:

    /// Gets current m_id
    IDType getID() const
    {
        return m_id;
    }

    /// Sets new m_id
    void setID( IDType newId )
    {
        m_id = newId;
    }

private :

    /// Id of signal (not mandatory)
    IDType m_id;

    /// Id counter
    FWCOM_API static size_t s_idCount;

    /// Mutex to protect id counter
    FWCOM_API static ::fwCore::mt::Mutex s_mutexCounter;

#endif
};

} // namespace fwCom

#endif /* __FWCOM_SLOTBASE_HPP__ */

