/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <iosfwd> // #include <strstream>
#include <iomanip>
#include <algorithm>

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <fwCore/util/LazyInstantiator.hpp>

#include <fwTools/System.hpp>

#include <fwThread/Worker.hpp>

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

#include "fwMemory/stream/in/Raw.hpp"
#include "fwMemory/stream/in/Buffer.hpp"
#include "fwMemory/policy/NeverDump.hpp"
#include "fwMemory/BufferManager.hpp"

namespace fwMemory
{

SPTR(void) getLock( const BufferManager::sptr &manager, BufferManager::ConstBufferPtrType bufferPtr )
{
    return manager->lockBuffer(bufferPtr).get();
}


//-----------------------------------------------------------------------------

BufferManager::sptr BufferManager::getDefault()
{
    return ::fwCore::util::LazyInstantiator< BufferManager >::getInstance();
}

//-----------------------------------------------------------------------------

BufferManager::BufferManager() :
    m_updatedSig(UpdatedSignalType::New()),
    m_dumpPolicy(::fwMemory::policy::NeverDump::New()),
    m_loadingMode(BufferManager::DIRECT),
    m_worker( ::fwThread::Worker::New() )
{
}

//-----------------------------------------------------------------------------

BufferManager::~BufferManager()
{
    m_worker->stop();
    // TODO restore dumped buffers
}

//-----------------------------------------------------------------------------

::boost::shared_future<void> BufferManager::registerBuffer(BufferManager::BufferPtrType bufferPtr)
{
    return m_worker->postTask<void>( ::boost::bind(&BufferManager::registerBufferImpl, this, bufferPtr) );
}

void BufferManager::registerBufferImpl(BufferManager::BufferPtrType bufferPtr)
{
    m_bufferInfos.insert( BufferInfoMapType::value_type( bufferPtr, BufferInfo() ) );
    m_updatedSig->asyncEmit();
}

//-----------------------------------------------------------------------------

::boost::shared_future<void> BufferManager::unregisterBuffer(BufferManager::BufferPtrType bufferPtr)
{
    return m_worker->postTask<void>( ::boost::bind(&BufferManager::unregisterBufferImpl, this, bufferPtr) );
}

void BufferManager::unregisterBufferImpl(BufferManager::BufferPtrType bufferPtr)
{

    m_bufferInfos.erase(bufferPtr);
    m_updatedSig->asyncEmit();
}

//-----------------------------------------------------------------------------

::boost::shared_future<void> BufferManager::allocateBuffer(BufferManager::BufferPtrType bufferPtr, SizeType size,
                                   const ::fwMemory::BufferAllocationPolicy::sptr &policy)
{
    return m_worker->postTask<void>(::boost::bind(&BufferManager::allocateBufferImpl, this, bufferPtr, size, policy));
}

void BufferManager::allocateBufferImpl(BufferManager::BufferPtrType bufferPtr, SizeType size,
                                   const ::fwMemory::BufferAllocationPolicy::sptr &policy)
{
    BufferInfo & info = m_bufferInfos[bufferPtr];
    SLM_ASSERT("Buffer has already been allocated", info.loaded && (*bufferPtr == NULL));

    if(!info.loaded)
    {
        info.clear();
    }

    m_dumpPolicy->allocationRequest( info, bufferPtr, size );

    try
    {
        policy->allocate(*bufferPtr, size);
    }
    catch( ::fwMemory::exception::Memory & )
    {
        info.clear();
        throw;
    }


    info.istreamFactory =
        ::boost::make_shared< ::fwMemory::stream::in::Buffer >(*bufferPtr, size,
                                                               ::boost::bind(&getLock, this->getSptr(), bufferPtr));

    info.lastAccess.modified();
    info.size = size;
    info.bufferPolicy = policy;
    m_updatedSig->asyncEmit();
}

//-----------------------------------------------------------------------------

::boost::shared_future<void> BufferManager::setBuffer(BufferManager::BufferPtrType bufferPtr,
                                                      ::fwMemory::BufferManager::BufferType buffer,
                                                      SizeType size,
                                                      const ::fwMemory::BufferAllocationPolicy::sptr &policy)
{
    return m_worker->postTask<void>(
                        ::boost::bind(&BufferManager::setBufferImpl, this, bufferPtr, buffer, size, policy));
}

void BufferManager::setBufferImpl(BufferManager::BufferPtrType bufferPtr, ::fwMemory::BufferManager::BufferType buffer,
                              SizeType size, const ::fwMemory::BufferAllocationPolicy::sptr &policy)
{
    BufferInfo & info = m_bufferInfos[bufferPtr];

    SLM_ASSERT("Buffer is already set", *bufferPtr == NULL && info.loaded);

    m_dumpPolicy->setRequest( info, bufferPtr, size );

    if(!info.loaded)
    {
        info.clear();
    }

    *bufferPtr = buffer;

    info.lastAccess.modified();
    info.size = size;
    info.bufferPolicy = policy;
    info.fileFormat = ::fwMemory::OTHER;
    info.fsFile.clear();
    info.istreamFactory =
        ::boost::make_shared< ::fwMemory::stream::in::Buffer >(*bufferPtr, size,
                                                               ::boost::bind(&getLock, this->getSptr(), bufferPtr));
    info.userStreamFactory = false;
    m_updatedSig->asyncEmit();
}

//-----------------------------------------------------------------------------

::boost::shared_future<void> BufferManager::reallocateBuffer(BufferManager::BufferPtrType bufferPtr, SizeType newSize)
{
    return m_worker->postTask<void>( ::boost::bind(&BufferManager::reallocateBufferImpl, this, bufferPtr, newSize) );
}

void BufferManager::reallocateBufferImpl(BufferManager::BufferPtrType bufferPtr, SizeType newSize)
{
    BufferInfo & info = m_bufferInfos[bufferPtr];
    SLM_ASSERT("Buffer must be allocated or dumped", (*bufferPtr != NULL) || !info.loaded);

    m_dumpPolicy->reallocateRequest( info, bufferPtr, newSize );

    try
    {
        if(info.loaded)
        {
            info.bufferPolicy->reallocate(*bufferPtr, newSize);
        }
        else
        {
            this->restoreBuffer( info, bufferPtr, newSize );
        }
    }
    catch( ::fwMemory::exception::Memory & )
    {
        m_updatedSig->asyncEmit();
        throw;
    }

    info.istreamFactory =
        ::boost::make_shared< ::fwMemory::stream::in::Buffer>(*bufferPtr, newSize,
                                                              ::boost::bind(&getLock, this->getSptr(), bufferPtr));

    info.lastAccess.modified();
    info.size = newSize;

    m_updatedSig->asyncEmit();
}

//-----------------------------------------------------------------------------

::boost::shared_future<void> BufferManager::destroyBuffer(BufferManager::BufferPtrType bufferPtr)
{
    return m_worker->postTask<void>( ::boost::bind(&BufferManager::destroyBufferImpl, this, bufferPtr) );
}

void BufferManager::destroyBufferImpl(BufferManager::BufferPtrType bufferPtr)
{
    BufferInfo & info = m_bufferInfos[bufferPtr];
    SLM_ASSERT("Buffer must be allocated or dumped", (*bufferPtr != NULL) || !info.loaded);

    m_dumpPolicy->destroyRequest( info, bufferPtr );

    if(info.loaded)
    {
        info.bufferPolicy->destroy(*bufferPtr);
    }

    info.clear();
    info.lastAccess.modified();
    m_updatedSig->asyncEmit();
}

//-----------------------------------------------------------------------------

::boost::shared_future<void> BufferManager::swapBuffer(BufferManager::BufferPtrType bufA,
                                                       BufferManager::BufferPtrType bufB)
{
    return m_worker->postTask<void>( ::boost::bind(&BufferManager::swapBufferImpl, this, bufA, bufB) );
}

void BufferManager::swapBufferImpl(BufferManager::BufferPtrType bufA, BufferManager::BufferPtrType bufB)
{
    BufferInfo & infoA = m_bufferInfos[bufA];
    BufferInfo & infoB = m_bufferInfos[bufB];

    std::swap(*bufA, *bufB);
    std::swap(infoA.size, infoB.size);
    std::swap(infoA.loaded, infoB.loaded);
    std::swap(infoA.fsFile, infoB.fsFile);
    std::swap(infoA.bufferPolicy, infoB.bufferPolicy);
    std::swap(infoA.istreamFactory, infoB.istreamFactory);
    std::swap(infoA.userStreamFactory, infoB.userStreamFactory);
    infoA.lastAccess.modified();
    infoB.lastAccess.modified();
}

//-----------------------------------------------------------------------------

struct AutoUnlock
{
    AutoUnlock(const BufferManager::sptr &manager, BufferManager::ConstBufferPtrType bufferPtr, const BufferInfo &info):
        m_manager(manager), m_bufferPtr(bufferPtr)
    {
        if ( !info.loaded )
        {
            bool restored = manager->restoreBuffer( bufferPtr ).get();
            OSLM_ASSERT( "restore not OK ( "<< restored << " && " << *bufferPtr <<" != 0 ).",
                         restored && *bufferPtr != 0 );
            FwCoreNotUsedMacro(restored);
        }
    }

    ~AutoUnlock()
    {
        try
        {
            m_manager->unlockBuffer(m_bufferPtr);
        }
        catch(std::exception& e)
        {
            OSLM_ASSERT( "Unlock Failed" << e.what(), 0 );
        }
        catch(...)
        {
            OSLM_ASSERT( "Unlock Failed", 0 );
        }
    }

    BufferManager::sptr m_manager;
    BufferManager::ConstBufferPtrType m_bufferPtr;
};

::boost::shared_future<SPTR(void)> BufferManager::lockBuffer(BufferManager::ConstBufferPtrType bufferPtr)
{
    return m_worker->postTask<SPTR(void)>( ::boost::bind(&BufferManager::lockBufferImpl, this, bufferPtr) );
}

SPTR(void) BufferManager::lockBufferImpl(BufferManager::ConstBufferPtrType bufferPtr)
{
    BufferManager::BufferPtrType castedBuffer = const_cast< BufferManager::BufferPtrType >(bufferPtr);
    BufferInfo & info = m_bufferInfos[castedBuffer];

    m_dumpPolicy->lockRequest( info, castedBuffer );

    SPTR(void) counter = info.lockCounter.lock();
    if ( !counter )
    {
        counter = ::boost::make_shared< AutoUnlock >( this->getSptr(), bufferPtr, info);
        info.lockCounter = counter;
    }

    m_lastAccess.modified();

    m_updatedSig->asyncEmit();

    return counter;
}

//-----------------------------------------------------------------------------

::boost::shared_future<bool> BufferManager::unlockBuffer(BufferManager::ConstBufferPtrType bufferPtr)
{
    return m_worker->postTask<bool>( ::boost::bind(&BufferManager::unlockBufferImpl, this, bufferPtr) );
}

bool BufferManager::unlockBufferImpl(BufferManager::ConstBufferPtrType bufferPtr)
{
    BufferInfo & info = m_bufferInfos[bufferPtr];
    m_dumpPolicy->unlockRequest( info, bufferPtr );

    m_updatedSig->asyncEmit();
    return true;
}

//-----------------------------------------------------------------------------

::boost::shared_future<bool> BufferManager::dumpBuffer(BufferManager::ConstBufferPtrType bufferPtr)
{
    return m_worker->postTask<bool>( ::boost::bind(&BufferManager::dumpBufferImpl, this, bufferPtr) );
}

bool BufferManager::dumpBufferImpl(BufferManager::ConstBufferPtrType bufferPtr)
{
    BufferManager::BufferPtrType castedBuffer = const_cast< BufferManager::BufferPtrType >(bufferPtr);
    BufferInfo & info = m_bufferInfos[castedBuffer];
    return this->dumpBuffer(info, castedBuffer);
}

//-----------------------------------------------------------------------------

bool BufferManager::dumpBuffer(BufferInfo & info, BufferManager::BufferPtrType bufferPtr)
{
    if ( !info.loaded || info.lockCount() > 0 || info.size == 0 )
    {
        return false;
    }

    ::boost::filesystem::path tmp = ::fwTools::System::getTemporaryFolder();
    ::boost::filesystem::path dumpedFile = ::boost::filesystem::unique_path( tmp/"fwMemory-%%%%-%%%%-%%%%-%%%%.raw" );

    OSLM_TRACE("dumping " << bufferPtr << " " << dumpedFile);
    info.lockCounter.reset();

    if ( this->writeBufferImpl(*bufferPtr, info.size, dumpedFile) )
    {
        info.fsFile = ::fwMemory::FileHolder(dumpedFile, true);
        info.fileFormat = ::fwMemory::RAW;
        info.istreamFactory = ::boost::make_shared< ::fwMemory::stream::in::Raw >(info.fsFile);
        info.userStreamFactory = false;
        info.bufferPolicy->destroy(*bufferPtr);
        *bufferPtr = NULL;
        info.loaded = false;

        m_dumpPolicy->dumpSuccess( info, bufferPtr );

        m_updatedSig->asyncEmit();
    }

    return !info.loaded;
}

//-----------------------------------------------------------------------------

::boost::shared_future<bool> BufferManager::restoreBuffer(BufferManager::ConstBufferPtrType  bufferPtr)
{
    return m_worker->postTask<bool>( ::boost::bind(&BufferManager::restoreBufferImpl, this, bufferPtr) );
}

bool BufferManager::restoreBufferImpl(BufferManager::ConstBufferPtrType  bufferPtr)
{
    BufferManager::BufferPtrType castedBuffer = const_cast< BufferManager::BufferPtrType >(bufferPtr);
    BufferInfo & info = m_bufferInfos[castedBuffer];
    return this->restoreBuffer(info, castedBuffer);
}


//-----------------------------------------------------------------------------

bool BufferManager::restoreBuffer(BufferInfo & info,
                                  BufferManager::BufferPtrType bufferPtr, BufferManager::SizeType allocSize)
{

    allocSize = ((allocSize) ? allocSize : info.size);
    if ( !info.loaded )
    {
        OSLM_TRACE("Restoring " << bufferPtr);

        info.bufferPolicy->allocate(*bufferPtr, allocSize);

        char * charBuf = static_cast< char * >(*bufferPtr);
        SizeType size = std::min(allocSize, info.size);
        bool notFailed = false;
        {
            SPTR(std::istream) isptr = (*info.istreamFactory)();
            std::istream &is = *isptr;
            SizeType read = is.read(charBuf, size).gcount();

            FW_RAISE_IF(" Bad file size, expected: " << size << ", was: " << read, size - read != 0);
            notFailed = !is.fail();
        }

        if ( notFailed )
        {
            info.loaded = true;
            info.fsFile.clear();
            info.lastAccess.modified();

            m_dumpPolicy->restoreSuccess( info, bufferPtr );

            info.fileFormat = ::fwMemory::OTHER;
            info.istreamFactory
                = ::boost::make_shared< ::fwMemory::stream::in::Buffer >(*bufferPtr,
                                                                allocSize,
                                                                ::boost::bind(&getLock, this->getSptr(), bufferPtr));
            info.userStreamFactory = false;
            m_updatedSig->asyncEmit();
            return true;
        }

    }
    return false;
}

//-----------------------------------------------------------------------------

::boost::shared_future<bool> BufferManager::writeBuffer(BufferManager::ConstBufferType buffer,
        SizeType size, ::boost::filesystem::path &path)
{
    return m_worker->postTask<bool>( ::boost::bind(&BufferManager::writeBufferImpl, this, buffer, size, path) );
}

bool BufferManager::writeBufferImpl(BufferManager::ConstBufferType buffer,
        SizeType size, ::boost::filesystem::path &path)
{
    ::boost::filesystem::ofstream fs(path, std::ios::binary|std::ios::trunc);
    FW_RAISE_IF("Memory management : Unable to open " << path, !fs.good());
    const char * charBuf = static_cast< const char * >(buffer);
    OSLM_TRACE("writing " << path);
    fs.write(charBuf, size);
    fs.close();
    return !fs.bad();
}

//-----------------------------------------------------------------------------

::boost::shared_future<bool> BufferManager::readBuffer(BufferManager::BufferType buffer, SizeType size,
        ::boost::filesystem::path &path)
{
    return m_worker->postTask<bool>( ::boost::bind(&BufferManager::readBufferImpl, this, buffer, size, path) );
}

bool BufferManager::readBufferImpl(BufferManager::BufferType buffer, SizeType size, ::boost::filesystem::path &path)
{
    ::boost::filesystem::ifstream fs(path, std::ios::in|std::ios::binary|std::ios::ate);
    FW_RAISE_IF("Unable to read " << path, !fs.good());

    std::streampos fileSize = fs.tellg();
    fs.seekg(0, std::ios::beg);

    FW_RAISE_IF(path << ": Bad file size, expected: " << size << ", was: " << fileSize,
            size - fileSize != 0);

    OSLM_TRACE("reading " << path);
    char * charBuf = static_cast< char * >(buffer);
    fs.read(charBuf, size);

    fs.close();
    return !fs.bad();
}

//-----------------------------------------------------------------------------

::boost::shared_future<std::string> BufferManager::toString() const
{
    return m_worker->postTask<std::string>( ::boost::bind(&BufferManager::toStringImpl, this ) );
}

std::string BufferManager::toStringImpl() const
{
    std::stringstream sstr ("");
    sstr << "nb Elem = " << m_bufferInfos.size() << std::endl;
    sstr    << std::setw(18) << "Buffer" << "->" << std::setw(18) << "Address" << " "
            << std::setw(10) << "Size" << " "
            << std::setw(18) << "Policy" << " "
            << std::setw(6) << "Access" << " "
            << std::setw(4) << "Lock" << " "
            << "DumpStatus" << " "
            << "File" << " "
            << std::endl;
    BOOST_FOREACH( BufferInfoMapType::value_type item, m_bufferInfos )
    {
        BufferInfo & info = item.second;
        sstr    << std::setw(18) << item.first << "->" << std::setw(18) << *(item.first) << " "
                << std::setw(10) << info.size << " "
                << std::setw(18) << info.bufferPolicy << " "
                << std::setw(6) << info.lastAccess << " "
                << std::setw(4) << info.lockCount() << " "
                << ((info.loaded)?"   ":"not") << " loaded "
                << ::boost::filesystem::path(info.fsFile) << " "
                << std::endl;
    }
    return sstr.str();
}

//-----------------------------------------------------------------------------

void BufferManager::setDumpPolicy( const ::fwMemory::IPolicy::sptr &policy )
{
    m_dumpPolicy = policy;
    policy->refresh();
}

//-----------------------------------------------------------------------------

::fwMemory::IPolicy::sptr BufferManager::getDumpPolicy() const
{
    return m_dumpPolicy;
}

//-----------------------------------------------------------------------------

::boost::shared_future<BufferManager::BufferInfoMapType> BufferManager::getBufferInfos() const
{
    return m_worker->postTask<BufferInfoMapType>( ::boost::bind(&BufferManager::getBufferInfosImpl, this) );
}

BufferManager::BufferInfoMapType BufferManager::getBufferInfosImpl() const
{
    return m_bufferInfos;
}

//-----------------------------------------------------------------------------

::boost::shared_future<BufferManager::BufferStats> BufferManager::getBufferStats() const
{
    return m_worker->postTask<BufferManager::BufferStats>(
            ::boost::bind(&BufferManager::computeBufferStats, m_bufferInfos) );
}

BufferManager::BufferStats BufferManager::computeBufferStats(const BufferInfoMapType& bufferInfo)
{
    BufferStats stats = {0,0};
    BOOST_FOREACH( const BufferInfoMapType::value_type &item, bufferInfo )
    {
        const BufferInfo & info = item.second;
        if ( !info.loaded )
        {
            stats.totalDumped += info.size;
        }
        stats.totalManaged += info.size;
    }
    return stats;
}

//-----------------------------------------------------------------------------

::boost::shared_future<void> BufferManager::setIStreamFactory(BufferPtrType bufferPtr,
                                      const SPTR(::fwMemory::stream::in::IFactory) &factory,
                                      SizeType size,
                                      ::fwMemory::FileHolder fsFile,
                                      ::fwMemory::FileFormatType format,
                                      const ::fwMemory::BufferAllocationPolicy::sptr &policy
                                     )
{
    return m_worker->postTask<void>(
        ::boost::bind(&BufferManager::setIStreamFactoryImpl, this, bufferPtr, factory, size, fsFile, format, policy) );
}

void BufferManager::setIStreamFactoryImpl(BufferPtrType bufferPtr,
                                      const SPTR(::fwMemory::stream::in::IFactory) &factory,
                                      SizeType size,
                                      ::fwMemory::FileHolder fsFile,
                                      ::fwMemory::FileFormatType format,
                                      const ::fwMemory::BufferAllocationPolicy::sptr &policy
                                     )
{
    BufferInfoMapType::iterator iterInfo = m_bufferInfos.find(bufferPtr);
    FW_RAISE_IF("Buffer is not managed by fwMemory::BufferManager.", iterInfo == m_bufferInfos.end() );
    BufferInfo & info = iterInfo->second;

    SLM_ASSERT("Buffer is already set", *bufferPtr == NULL && info.loaded);

    info.istreamFactory = factory;
    info.userStreamFactory = true;
    info.size = size;
    info.fsFile = fsFile;
    info.fileFormat = format;
    info.bufferPolicy = policy;
    info.loaded = false;

    switch(m_loadingMode)
    {
    case BufferManager::DIRECT:
        this->restoreBuffer(bufferPtr);
        break;
    case BufferManager::LAZY :
        m_dumpPolicy->dumpSuccess( info, bufferPtr );
        m_updatedSig->asyncEmit();
        break;
    default:
        SLM_ASSERT("You shall not pass", 0);
    }
}

//-----------------------------------------------------------------------------

::boost::shared_future<BufferManager::StreamInfo>
    BufferManager::getStreamInfo(const BufferManager::ConstBufferPtrType bufferPtr) const
{
    return m_worker->postTask<BufferManager::StreamInfo>(
            ::boost::bind(&BufferManager::getStreamInfoImpl, this, bufferPtr) );
}

BufferManager::StreamInfo BufferManager::getStreamInfoImpl(const BufferManager::ConstBufferPtrType bufferPtr) const
{
    StreamInfo streamInfo;
    BufferInfoMapType::const_iterator iterInfo = m_bufferInfos.find(bufferPtr);
    FW_RAISE_IF("Buffer is not managed by fwMemory::BufferManager.", iterInfo == m_bufferInfos.end() );
    const BufferInfo & info = iterInfo->second;
    streamInfo.size = info.size;
    streamInfo.fsFile = info.fsFile;
    streamInfo.format = info.fileFormat;
    streamInfo.userStream = info.userStreamFactory;

    if(info.istreamFactory)
    {
        streamInfo.stream = (*info.istreamFactory)();
    }

    return streamInfo;
}

//-----------------------------------------------------------------------------

BufferManager::LoadingModeType BufferManager::getLoadingMode() const
{
    return m_loadingMode;
}

void BufferManager::setLoadingMode(LoadingModeType mode)
{
    m_loadingMode = mode;
}

} //namespace fwMemory



