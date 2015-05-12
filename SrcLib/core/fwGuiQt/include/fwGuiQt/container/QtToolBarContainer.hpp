/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQT_CONTAINER_QTTOOLBARCONTAINER_HPP__
#define __FWGUIQT_CONTAINER_QTTOOLBARCONTAINER_HPP__

#include <QPointer>

#include <fwCore/base.hpp>
#include <fwTools/Object.hpp>

#include <fwGui/container/fwToolBar.hpp>

#include "fwGuiQt/config.hpp"


QT_BEGIN_NAMESPACE
class QToolBar;
QT_END_NAMESPACE


namespace fwGuiQt
{
namespace container
{

/**
 * @brief   Defines the Qt toolBar bar container for IHM.
 * @class   QtToolBarContainer
 *
 * @date    2009-2010.
 *
 */
class FWGUIQT_CLASS_API QtToolBarContainer : public ::fwGui::container::fwToolBar
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (QtToolBarContainer)(::fwGui::container::fwToolBar),
                                            (()),
                                            ::fwGui::factory::New< QtToolBarContainer > );

    FWGUIQT_API QtToolBarContainer(::fwGui::GuiBaseObject::Key key) throw();

    FWGUIQT_API virtual ~QtToolBarContainer() throw();


    FWGUIQT_API virtual void clean();
    FWGUIQT_API virtual void destroyContainer();

    FWGUIQT_API virtual void setQtToolBar(QToolBar *toolBar);
    FWGUIQT_API virtual QToolBar* getQtToolBar();

private:

    QPointer<QToolBar> m_toolBar;
};

} // namespace container
} // namespace fwGuiQt

#endif /*__FWGUIQT_CONTAINER_QTTOOLBARCONTAINER_HPP__*/


