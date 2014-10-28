/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _ARL_RECORDER_H
#define _ARL_RECORDER_H
#include <arlcore/Common.h>

#include <string>
#include <fstream>

namespace arlCore
{
    class Recorder
    {
    /**
     * @class   Recorder
     * @date    04/2008
     * @brief   Abstract class for recorder devices
     */
    public:
        //! @brief Constructor
        ARLCORE_API Recorder( void );

        //! @brief Destructor
        ARLCORE_API ~Recorder( void );

        //! @brief Get the full pathname and header's filename
        ARLCORE_API bool getRecorderFilename( std::string &fileName );

        //! @brief Get the full pathname and header's filename
        ARLCORE_API bool getRecorderDirectory( std::string &directory );

        /**
        * @brief Demarre l'enregistrement des images avant ou apres le traitement
        * Si nbSnapshots n�gatif, l'enregistrement dure ind�finiment
        * Si nbSnapshots positif, il est r�alis� n prises de vue, puis stopRecorder est activ�
        * Si nbSnapshots nul, l'enregistrement s'arr�te
        * Par d�faut, les donnees brutes sont enregistr�es pour une dur�e ind�termin�e
        */
        ARLCORE_API bool startRecorder( long int nbSnapshots=-1, bool afterRender=false );

        /**
        * @brief Arrete l'enregistrement, mais peut-�tre poursuivi avec un nouveau startRecorder
        * Il sera poursuivi dans le meme repertoire, si un initRecorder n'est pas lanc� entre temps
        */
        ARLCORE_API bool stopRecorder( void );

        //! @return True if it's recording
        ARLCORE_API bool isRecording( void );

        //! @brief Close recorder. You should call initRecorder to start again
        ARLCORE_API void closeRecorder( void );

    protected:
        std::fstream m_recorderHandle;

        std::string m_recorderDirectory;
        std::string m_fullRecorderDirectory;
        std::string m_recorderFullName;

        bool m_recorderFlag;
        bool m_recorderAfterRender;
        long int m_nbSnapshots;
    };
} // namespace ARLCORE
#endif // _ARL_RECORDER_H
