/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWTHREAD_WORKER_HXX__
#define __FWTHREAD_WORKER_HXX__

#include <boost/thread/future.hpp>

#include <fwThread/TaskHandler.hpp>


namespace fwThread
{

template< typename R, typename TASK >
::boost::shared_future< R > Worker::postTask(TASK f)
{
    ::boost::packaged_task< R > task( f );
    ::boost::future< R > ufuture = task.get_future();

    ::boost::function< void () > ftask = ::fwThread::moveTaskIntoFunction(task);

    if( ::fwThread::getCurrentThreadId() == this->getThreadId())
    {
        ftask();
    }
    else
    {
        this->post(ftask);
    }

    return ::boost::move(ufuture);
}

} //namespace fwThread

#endif // __FWTHREAD_WORKER_HXX__
