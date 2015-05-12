/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <cassert>
#include <string>
#include <limits.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/regex.hpp>

#include "fwRuntime/Bundle.hpp"
#include "fwRuntime/dl/INameDecorator.hpp"
#include "fwRuntime/dl/Native.hpp"



namespace fwRuntime
{

namespace dl
{

//------------------------------------------------------------------------------

Native::Native( const ::boost::filesystem::path & modulePath,
                const ::boost::shared_ptr< INameDecorator > nameDecorator ) throw()
    :   m_modulePath        ( modulePath ),
      m_nameDecorator ( nameDecorator ),
      m_bundle            ( 0 )
{
}

//------------------------------------------------------------------------------

Native::~Native() throw()
{
}

//------------------------------------------------------------------------------

const ::boost::filesystem::path Native::getFullPath( const bool _bMustBeFile ) const throw(RuntimeException)
{
    // Pre-condition
    SLM_ASSERT("bundle not initialized", m_bundle != 0 );

    ::boost::filesystem::path result = m_bundle->getLocation() / getPath();

    // Test that the result path exists.
    if(result.empty())
    {
        throw RuntimeException("Unable to find a native library for the bundle.");
    }
    if( !::boost::filesystem::exists(result) )
    {
#if BOOST_FILESYSTEM_VERSION > 2
        throw RuntimeException("'" + result.string() + "': invalid native module file name.");
#else
        throw RuntimeException("'" + result.native_file_string() + "': invalid native module file name.");
#endif
    }
    if(_bMustBeFile && ::boost::filesystem::is_directory(result) )
    {
#if BOOST_FILESYSTEM_VERSION > 2
        throw RuntimeException("'" + result.string() + "': is a file. Perhaps dynamic librairie is missing.");
#else
        throw RuntimeException("'" + result.native_file_string() +
                               "': is a file. Perhaps dynamic librairie is missing.");
#endif
    }
    return result;
}

//------------------------------------------------------------------------------

const ::boost::filesystem::path Native::getPath() const throw(RuntimeException)
{
    // Pre-condition
    SLM_ASSERT("bundle not initialized", m_bundle != 0 );

    ::boost::filesystem::path result;

    const ::boost::filesystem::path fullModulePath( m_bundle->getLocation() / m_modulePath );
#if BOOST_FILESYSTEM_VERSION > 2
    const ::boost::regex nativeFileRegex( m_nameDecorator->getNativeName(fullModulePath.filename().string()) );
#else
    const ::boost::regex nativeFileRegex( m_nameDecorator->getNativeName(fullModulePath.leaf()) );
#endif // BOOST_FILESYSTEM_VERSION > 2

    // Walk through the module directory, seeking for a matching file.
    ::boost::filesystem::directory_iterator curDirEntry(fullModulePath.parent_path());
    ::boost::filesystem::directory_iterator endDirEntry;
    for(; curDirEntry != endDirEntry; ++curDirEntry)
    {
        ::boost::filesystem::path curEntryPath( *curDirEntry );
#if BOOST_FILESYSTEM_VERSION > 2
        if( ::boost::regex_match( curEntryPath.filename().string(), nativeFileRegex ) )
        {
            result = m_modulePath.parent_path() / curEntryPath.filename();
            break;
        }
#else
        if( ::boost::regex_match( curEntryPath.leaf(), nativeFileRegex ) )
        {
            result = m_modulePath.parent_path() / curEntryPath.leaf();
            break;
        }
#endif // BOOST_FILESYSTEM_VERSION > 2
    }

    return result;
}

//------------------------------------------------------------------------------

void Native::setBundle( const Bundle * bundle ) throw()
{
    // Pre-condition
    SLM_ASSERT("bundle already initialized", m_bundle == 0 );
    m_bundle = bundle;
    // Post-condition
    SLM_ASSERT("bundle not correctly attached", m_bundle == bundle );
}

//------------------------------------------------------------------------------

void Native::operator=(const Native&) throw()
{
}

//------------------------------------------------------------------------------

} // namespace dl


} // namespace fwRuntime
