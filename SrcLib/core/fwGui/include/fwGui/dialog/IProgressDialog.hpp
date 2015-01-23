/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWGUI_IPROGRESSDIALOG_HPP_
#define _FWGUI_IPROGRESSDIALOG_HPP_

#include <string>
#include <boost/function.hpp>
#include <boost/signals2.hpp>

#include <fwData/location/ILocation.hpp>

#include "fwGui/GuiBaseObject.hpp"
#include "fwGui/config.hpp"

namespace fwGui
{
namespace dialog
{
/**
 * @brief   Defines the generic Progress dialog for IHM.
 * @todo    add methods for behavior like autoClose, flying window or in status bar
 * @class   IProgressDialog
 * 
 * @date    2009-2010.
 *
 */
class FWGUI_CLASS_API IProgressDialog : public ::fwGui::GuiBaseObject, public ::boost::signals2::trackable // to autoDisconnect if handler is destroyed before the notifier
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (IProgressDialog)(::fwGui::GuiBaseObject), (()), progressDialogFactory);

    typedef std::string FactoryRegistryKeyType;
    typedef boost::function< void () >  CancelCallbackType;


    /// this *unique* key should  be used *for all* factory for specific LocationDialog(qt,wx,...)
    FWGUI_API static const FactoryRegistryKeyType REGISTRY_KEY;

    FWGUI_API virtual ~IProgressDialog();
    FWGUI_API IProgressDialog();

    ///set the title for the dialog
    FWGUI_API virtual void setTitle(const std::string &title) = 0;

    ///set the message for the dialog
    FWGUI_API virtual void setMessage(const std::string &msg) = 0;

    /// action called by ::fwTools::ProgressAdviser
    FWGUI_API virtual void operator()(float percent,std::string msg) = 0;

    FWGUI_API virtual void setCancelCallback(CancelCallbackType callback);

    virtual void setCancelRaiseException(bool raise){m_raise = raise;};

    virtual bool getCanceled(){return m_canceled;};

    virtual void hideCancelButton() {};

    virtual void setProcessUserEvents(bool process){m_processUserEvents = process;}

protected :

    FWGUI_API virtual void cancelPressed();

    CancelCallbackType m_cancelCallback;
    bool m_canceled;
    bool m_raise;
    ///progress bar's current value: [0-100]
    int  m_value;
    bool m_processUserEvents;

protected :
    static sptr progressDialogFactory()
    {
        ::fwGui::GuiBaseObject::sptr guiObj = ::fwGui::factory::New(::fwGui::dialog::IProgressDialog::REGISTRY_KEY);
        ::fwGui::dialog::IProgressDialog::sptr progressDlg = ::fwGui::dialog::IProgressDialog::dynamicCast(guiObj);
        return progressDlg;
    }

};

} //namespace dialog
} // namespace fwGui

#endif /*_FWGUI_LOCATIONDIALOG_HPP_*/


