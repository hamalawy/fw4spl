/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWQT_ZIPFOLDER_HPP_
#define _FWQT_ZIPFOLDER_HPP_


#include "fwQt/config.hpp"
#include <QString>


namespace fwQt
{

//------------------------------------------------------------------------------

/**
 * @brief   This interface defines static functions to zip or unzip a folder.
 * @class   wxZipFolder.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 *
 * This class provides static method to zip or unzip folder. Methods use wxWidgets API to archive data. The compression rate is equal to 0 ( simple archive ).
 */
class  ZipFolder
{

public:

    /**
     * @brief This method is used to compress (zip format) a folder
     * @param[in] srcFolderName source folder path
     * @param[in] destZipFileName file path resulting to folder compression
     * @return true if folder compression is a success
     * @see this method use wxDirTraverserZIP class to realize the directory exploration
     * @todo Set in parameter the compression rate.
     */
     static bool packFolder(QString _srcFolderName, QString _destZipFileName );

    /**
     * @brief This method is used to uncompress (zip format) a folder
     * @param[in] srcZipFileName source zip folder path
     * @param[in] destFolderName file path resulting to folder decompression
     * @return true if folder decompression is a success
     */
     static bool unpackFolder( QString _srcZipFileName, QString _destFolderName );

};

} //namespace fwWX

#endif // _FWWX_WXZIPFOLDER_HPP_
