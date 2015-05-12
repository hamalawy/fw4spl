/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQT_DIALOG_SELECTORDIALOG_HPP__
#define __FWGUIQT_DIALOG_SELECTORDIALOG_HPP__

#include <vector>

#include <QDialog>
#include <QString>
#include <QWidget>

#include <fwGui/dialog/ISelectorDialog.hpp>
#include "fwGuiQt/config.hpp"

namespace fwGuiQt
{
namespace dialog
{
//------------------------------------------------------------------------------

/**
 * @brief   SelectorDialog allowing the choice of an element among severals (_selections)
 * @class   SelectorDialog
 *
 * @date    2009.
 */

class FWGUIQT_CLASS_API SelectorDialog : public ::fwGui::dialog::ISelectorDialog,
                                         public QDialog
{
public:

    fwCoreClassDefinitionsWithFactoryMacro( (SelectorDialog)(::fwGui::dialog::ISelectorDialog),
                                            (()),
                                            ::fwGui::factory::New< SelectorDialog > );

    FWGUIQT_API SelectorDialog(::fwGui::GuiBaseObject::Key key);

    FWGUIQT_API virtual ~SelectorDialog();

    /**
     * @brief The string list that can be chosen by the selector.
     */
    FWGUIQT_API virtual void setSelections(std::vector< std::string > _selections);

    /**
     * @brief Sets the selector title.
     */
    FWGUIQT_API virtual void setTitle(std::string _title);

    /**
     * @brief Show the selector and return the selection.
     */
    FWGUIQT_API virtual std::string show();

    /// Set the message
    FWGUIQT_API virtual void setMessage(const std::string &msg);

private:

    std::vector< std::string > m_selections;
    /// Dialog box message
    std::string m_message;
    std::string m_title;
};

} // namespace dialog
} // namespace fwGuiQt

#endif /*__FWGUIQT_DIALOG_SELECTORDIALOG_HPP__*/
