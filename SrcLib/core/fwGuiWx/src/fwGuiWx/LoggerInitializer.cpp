/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <wx/log.h>
#include <wx/version.h>

#include <fwCore/base.hpp>

#include "fwGuiWx/LoggerInitializer.hpp"
#include "fwGuiWx/convert.hpp"

//------------------------------------------------------------------------------

namespace fwGuiWx
{

class fwGuiWxLog : public wxLog
{
public:
    fwGuiWxLog(){};
    virtual ~fwGuiWxLog(){};

protected:
#if wxCHECK_VERSION(2, 9, 1)
    virtual void DoLogRecord(wxLogLevel level, const wxString & msg, const wxLogRecordInfo & info);
#else
    virtual void DoLog(wxLogLevel level, const wxString& szString, time_t t);
#endif

};

//------------------------------------------------------------------------------

#if wxCHECK_VERSION(2, 9, 1)
void fwGuiWxLog::DoLogRecord(wxLogLevel level, const wxString& szString, const wxLogRecordInfo & info)
#else
void fwGuiWxLog::DoLog(wxLogLevel level, const wxString& szString, time_t t)
#endif
{
    wxString logLevel;

    switch ( level )
    {
        case wxLOG_FatalError:
            logLevel = wxT("FatalError: ");
            break;
        case wxLOG_Error:
            logLevel = wxT("Error: ");
            break;
        case wxLOG_Status:
            logLevel = wxT("Status: ");
            break;
        case wxLOG_Warning:
            logLevel = wxT("Warning: ");
            break;
        case wxLOG_Info:
            logLevel = wxT("Info: ");
            break;
        case wxLOG_Message:
            logLevel = wxT("Message: ");
            break;

        default:
            logLevel = wxT("Unknown log level: ");
            break;
#if wxUSE_LOG_TRACE
        case wxLOG_Trace:
            logLevel = wxT("Trace: ");
            break;
#endif
#if wxUSE_LOG_DEBUG
        case wxLOG_Debug:
            logLevel = wxT("Debug: ");
            break;
#endif
    }

    wxString timestamp;
    this->TimeStamp(&timestamp);
    std::string msg = "[WX]: " + wx2std(timestamp +": "+ logLevel + szString);
    SLM_WARN(msg);

    switch ( level )
    {
#if wxCHECK_VERSION(2, 9, 1)
        case wxLOG_FatalError:
            wxLog::DoLogRecord(level, szString, info);
            break;
        case wxLOG_Error:
            wxLog::DoLogRecord(level, szString, info);
            break;
        case wxLOG_Status:
            wxLog::DoLogRecord(level, szString, info);
            break;
#else
        case wxLOG_FatalError:
            wxLog::DoLog(level, szString, t);
            break;
        case wxLOG_Error:
            wxLog::DoLog(level, szString, t);
            break;
        case wxLOG_Status:
            wxLog::DoLog(level, szString, t);
            break;
#endif
    }
}

//------------------------------------------------------------------------------

LoggerInitializer::LoggerInitializer()
{
    delete wxLog::SetActiveTarget(new fwGuiWxLog);
}

//------------------------------------------------------------------------------

void LoggerInitializer::initialize()
{
    WPTR(LoggerInitializer) reg = SPTR(LoggerInitializer)(new LoggerInitializer) ;
}

//------------------------------------------------------------------------------

} // namespace fwGuiWx
