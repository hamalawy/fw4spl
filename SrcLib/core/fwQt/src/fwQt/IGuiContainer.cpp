/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <QApplication>
#include <QLayout>

#include <fwCore/base.hpp>
#include <fwTools/UUID.hpp>
#include <fwServices/helper.hpp>

#include "fwQt/IGuiContainer.hpp"

namespace fwQt
{

IGuiContainer::ContainerMapType IGuiContainer::m_globalUIDToQtContainer;

IGuiContainer::IGuiContainer() throw() :
    m_container( 0 ),
    m_isContainerLocallyCreated( false )
{
}

//-----------------------------------------------------------------------------

IGuiContainer::~IGuiContainer() throw()
{
}

//-----------------------------------------------------------------------------

QWidget * IGuiContainer::getQtContainer()
{
    SLM_FATAL_IF("Sorry, parent qtContainer not yet initialized, please initGuiParentContainer before", !m_container ) ;
    return m_container ;
}

//-----------------------------------------------------------------------------

void IGuiContainer::initGuiParentContainer()
{
    if(m_globalUIDToQtContainer.find(this->getUUID()) != m_globalUIDToQtContainer.end())
    {
        OSLM_TRACE( "GuiContainer fund for this Service " <<  this->getUUID() );
        m_container = m_globalUIDToQtContainer[this->getUUID()];
        SLM_ASSERT("Sorry, qtContainer is not correctly initialized", m_container);
    }
    else
    {
        OSLM_TRACE( "GuiContainer not fund for this Service " <<  this->getUUID() );
        m_container = new QWidget();
        m_container->show();
        m_isContainerLocallyCreated = true ;
    }
}

//-----------------------------------------------------------------------------

void IGuiContainer::resetGuiParentContainer()
{
    if(m_container != QApplication::activeWindow())
    {
        if (!m_container->layout())
        {
            QLayout* layout = m_container->layout();
            QLayoutItem *child;
            while( (child = layout->takeAt( 0 )) )
            {
                delete child->widget();
                delete child;
             }
        }
    }
    if( m_isContainerLocallyCreated && m_container != 0 )
    {
        SLM_DEBUG("Destroying container") ;
        delete m_container;
        m_container = 0 ;
    }
}

//-----------------------------------------------------------------------------

void IGuiContainer::registerQtContainer(std::string uid , QWidget * container)
{
    OSLM_ASSERT("Sorry, qtContainer for "<<uid<<" already exists in SubUIDToQtContainer map.", m_subUIDToQtContainer.find(uid) == m_subUIDToQtContainer.end());
    m_subUIDToQtContainer[uid] = container;

    ::fwQt::IGuiContainer::registerGlobalQtContainer(uid, container);
}

//-----------------------------------------------------------------------------

void IGuiContainer::unregisterQtContainer(std::string uid)
{
    bool service_exists = ::fwTools::UUID::exist(uid, ::fwTools::UUID::SIMPLE );
    OSLM_INFO_IF("Service "<<uid <<" not exists.",!service_exists );
    if(service_exists)
    {
        ::fwServices::IService::sptr service = ::fwServices::get( uid ) ;
        service->stop();
    }

    OSLM_ASSERT("Sorry, qtContainer for "<<uid<<" not exists in SubUIDToQtContainer map.",
            m_subUIDToQtContainer.find(uid) != m_subUIDToQtContainer.end());

    QWidget* container = m_subUIDToQtContainer[uid];
    SLM_ASSERT("Sorry, qtContainer is not correctly initialized", container);
    // Destroys the window safely
    delete container;
    container = 0;
    // Removes container in internal map
    m_subUIDToQtContainer.erase(uid);

    ::fwQt::IGuiContainer::unregisterGlobalQtContainer(uid);
}

//-----------------------------------------------------------------------------

void IGuiContainer::unregisterAllQtContainer()
{
    ContainerMapType subUIDToQtContainer = m_subUIDToQtContainer;
    BOOST_FOREACH( ContainerMapType::value_type uidContainerElmt, subUIDToQtContainer)
    {
        this->unregisterQtContainer(uidContainerElmt.first);
    }
    m_subUIDToQtContainer.clear();
}

//-----------------------------------------------------------------------------

void IGuiContainer::registerGlobalQtContainer(std::string uid, QWidget * container)
{
    OSLM_ASSERT("Sorry, qtContainer for "<<uid<<" already exists in GlobalUIDToQtContainer map.",
            m_globalUIDToQtContainer.find(uid) == m_globalUIDToQtContainer.end());
    m_globalUIDToQtContainer[uid] = container;
}

//-----------------------------------------------------------------------------

void IGuiContainer::unregisterGlobalQtContainer(std::string uid)
{
    OSLM_ASSERT("Sorry, qtContainer for "<<uid<<" not exists in GlobalUIDToQtContainer map.",
            m_globalUIDToQtContainer.find(uid) != m_globalUIDToQtContainer.end());
    // Removes container in global map
    m_globalUIDToQtContainer.erase(uid);
}

//-----------------------------------------------------------------------------

QWidget* IGuiContainer::getQtContainer(std::string uid)
{
    OSLM_ASSERT("Sorry, qtContainer for "<<uid<<" not exists in GlobalUIDToQtContainer map.", m_globalUIDToQtContainer.find(uid) != m_globalUIDToQtContainer.end());
    // returns container in global map
    return  m_globalUIDToQtContainer[uid];
}

}
