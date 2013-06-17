/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWZIP_WRITEARCHIVE_HPP__
#define __FWZIP_WRITEARCHIVE_HPP__

#include <ostream>
#include <boost/filesystem/path.hpp>

#include <fwCore/macros.hpp>

namespace fwZip
{

/**
 * @brief   This interface defines functions to write a file in an archive.
 * @class   IWriteArchive.
 */
class IWriteArchive
{

public:

    fwCoreNonInstanciableClassDefinitionsMacro((IWriteArchive));

    virtual ~IWriteArchive(){};

    /**
     * @brief Creates a new file entry in archive and returns output stream for this file.
     * @param path file in archive.
     * @return output stream of file entry in archive.
     */
    virtual SPTR(std::ostream) createFile(const ::boost::filesystem::path &path) = 0;

    /**
     * @brief Writes source file in archive.
     * @param sourceFile source file.
     * @param path file in archive.
     */
    virtual void putFile(const ::boost::filesystem::path &sourceFile, const ::boost::filesystem::path &path) = 0;

    /**
     * @brief Creates a folder in archive.
     * @param path folder to create in archive.
     */
    virtual bool createDir(const ::boost::filesystem::path &path) = 0;

    /**
     * @brief Returns archive path.
     */
    virtual const ::boost::filesystem::path getArchivePath() const = 0;
};

}


#endif /* __FWZIP_WRITEARCHIVE_HPP__ */

