/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWGUIQT_CONTAINER_QTMENUBARCONTAINER_HPP_
#define _FWGUIQT_CONTAINER_QTMENUBARCONTAINER_HPP_

#include <QPointer>


#include <fwCore/base.hpp>
#include <fwTools/Object.hpp>

#include <fwGui/container/fwMenuBar.hpp>

#include "fwGuiQt/config.hpp"

QT_BEGIN_NAMESPACE
class QMenuBar;
QT_END_NAMESPACE


namespace fwGuiQt
{

namespace container
{

/**
 * @brief   Defines the Qt menu bar container for GUI.
 * @class   QtMenuBarContainer
 * 
 * @date    2009-2010.
 *
 */
class FWGUIQT_CLASS_API QtMenuBarContainer : public ::fwGui::container::fwMenuBar
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (QtMenuBarContainer)(::fwGui::container::fwMenuBar),
                                            (()),
                                            ::fwGui::factory::New< QtMenuBarContainer > );

    FWGUIQT_API QtMenuBarContainer(::fwGui::GuiBaseObject::Key key) throw() ;

    FWGUIQT_API virtual ~QtMenuBarContainer() throw() ;


    FWGUIQT_API virtual void clean();
    FWGUIQT_API virtual void destroyContainer();

    FWGUIQT_API virtual void setQtMenuBar(QMenuBar *menuBar);
    FWGUIQT_API virtual QMenuBar* getQtMenuBar();

private :

    QPointer<QMenuBar> m_menuBar;
};

} // namespace container
} // namespace fwGuiQt

#endif /*_FWGUIQT_CONTAINER_QTMENUBARCONTAINER_HPP_*/


