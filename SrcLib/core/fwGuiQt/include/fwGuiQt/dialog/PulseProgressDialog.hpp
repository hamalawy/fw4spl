/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWGUIQT_PULSEPROGRESSDIALOG_HPP
#define _FWGUIQT_PULSEPROGRESSDIALOG_HPP


#include <string>


#include <QProgressDialog>
#include <QPointer>

#include <fwGui/dialog/IPulseProgressDialog.hpp>

#include "fwGuiQt/config.hpp"


QT_BEGIN_NAMESPACE
class QProgressDialog;
QT_END_NAMESPACE


namespace fwGuiQt
{
namespace dialog
{
/**
 * @brief   This class allows us to show a pulse progress bar.
 * @class   PulseProgressDialog.
 * @author  IRCAD (Research and Development Team).
 * @date    2009.
 */
class FWGUIQT_CLASS_API PulseProgressDialog : public ::fwGui::dialog::IPulseProgressDialog
{
public:

    fwCoreClassDefinitionsWithFactoryMacro( (PulseProgressDialog)(::fwGui::dialog::IPulseProgressDialog),
                                            (()),
                                            ::fwGui::factory::New< PulseProgressDialog > );

    FWGUIQT_API PulseProgressDialog(::fwGui::GuiBaseObject::Key key);

    FWGUIQT_API virtual ~PulseProgressDialog();

    /// override
    FWGUIQT_API void setTitle(const std::string &title);

    /// override
    FWGUIQT_API void setMessage(const std::string &message);

    FWGUIQT_API void show();

protected :

    QPointer< QProgressDialog >  m_pdialog;

};
} // namespace dialog
} // namespace fwGuiQt


#endif /* _FWGUIQT_PULSEPROGRESSDIALOG_HPP */
