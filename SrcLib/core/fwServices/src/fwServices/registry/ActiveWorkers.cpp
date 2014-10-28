/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include "fwServices/registry/ActiveWorkers.hpp"

namespace fwServices
{
namespace registry
{

//-----------------------------------------------------------------------------

ActiveWorkers::sptr ActiveWorkers::s_currentActiveWorkers = ActiveWorkers::New();

//-----------------------------------------------------------------------------

const std::string ActiveWorkers::s_DEFAULT_WORKER = "DEFAULT_WORKER";

//-----------------------------------------------------------------------------

ActiveWorkers::ActiveWorkers()
{}

//-----------------------------------------------------------------------------

ActiveWorkers::~ActiveWorkers()
{}

//-----------------------------------------------------------------------------

ActiveWorkers::sptr ActiveWorkers::getDefault()
{
    return ActiveWorkers::s_currentActiveWorkers;
}

//-----------------------------------------------------------------------------

::fwThread::Worker::sptr ActiveWorkers::getWorker( const WorkerKeyType & key ) const
{
    ::fwCore::mt::ReadLock lock(m_registryMutex);

    WorkerMapType::const_iterator it = m_workers.find(key);

    if( it != m_workers.end() )
    {
        return it->second;
    }

    return ::fwThread::Worker::sptr();
}

//-----------------------------------------------------------------------------

void ActiveWorkers::addWorker( const WorkerKeyType & key, ::fwThread::Worker::sptr worker )
{
    ::fwCore::mt::WriteLock lock(m_registryMutex);
    m_workers.insert( WorkerMapType::value_type(key, worker) );
}

//-----------------------------------------------------------------------------

void ActiveWorkers::initRegistry()
{
    this->addWorker( s_DEFAULT_WORKER, ::fwThread::Worker::New() );
}

//-----------------------------------------------------------------------------

void ActiveWorkers::clearRegistry()
{
    ::fwCore::mt::WriteLock lock(m_registryMutex);
    m_workers.clear();
}

//-----------------------------------------------------------------------------

} // namespace registry
} // namespace fwServices

