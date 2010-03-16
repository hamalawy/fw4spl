/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/helper.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include <QApplication>
#include <QWidget>
#include <QFrame>
#include "guiQt/view/IView.hpp"
#include "guiQt/Manager.hpp"

namespace guiQt
{
namespace view
{


IView::IView() throw(): m_minWidth(-1), m_minHeight(-1)
{
}

//-----------------------------------------------------------------------------

IView::~IView() throw()
{
}

//-----------------------------------------------------------------------------

void IView::configuring() throw( ::fwTools::Failed )
{
  
  std::cout<<" \nIQtView::configuring() \n\n";  
  
    OSLM_ASSERT( "m_configuration->getName() == '"<< m_configuration->getName()  <<"'" , m_configuration->getName() == "service" );

    ::fwRuntime::ConfigurationElement::sptr viewsCfgElt = m_configuration->findConfigurationElement("views");

    std::vector < ::fwRuntime::ConfigurationElement::sptr > vectConfig = viewsCfgElt->find("view");
    assert(!vectConfig.empty());
    m_panels.clear();
    ::fwRuntime::ConfigurationElementContainer::Iterator iter;
    for (iter = vectConfig.begin() ; iter != vectConfig.end() ; ++iter)
    {
        SLM_FATAL_IF("<views> node can only contain <view> node", (*iter)->getName()!="view" );
        ViewInfo vi;
        std::string uid;

        SLM_FATAL_IF("<view> node must contain uid attribute", !(*iter)->hasAttribute("uid") );
        uid = (*iter)->getExistingAttributeValue("uid");

       if( (*iter)->hasAttribute("minWidth") )
        {
            std::string width = (*iter)->getExistingAttributeValue("minWidth") ;
            vi.m_minSize.first = ::boost::lexical_cast< int >(width) ;
        }

        if( (*iter)->hasAttribute("minHeight") )
        {
            std::string height = (*iter)->getExistingAttributeValue("minHeight") ;
            vi.m_minSize.second = ::boost::lexical_cast< int >(height) ;
        }
        if((*iter)->hasAttribute("autoStart"))
        {
            std::string autostart = (*iter)->getExistingAttributeValue("autoStart");
            OSLM_ASSERT("Sorry, value "<<autostart<<" is not correct for attribute autoStart.",
                    autostart == "yes" || autostart == "no");
            vi.m_autostart = (autostart == "yes");
        }
        m_panels[uid] = vi;
    }
}

//-----------------------------------------------------------------------------

void IView::info(std::ostream &_sstream )
{
    _sstream << "GUI View with ID = " <<  m_guiContainerId;
}

//-----------------------------------------------------------------------------

void IView::starting() throw(::fwTools::Failed)
{
      std::cout<<" \n IView::configuring()  \n";

}
//-----------------------------------------------------------------------------

void IView::updating() throw(::fwTools::Failed)
{
    SLM_TRACE("IView::updating");
}

//-----------------------------------------------------------------------------

void IView::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
    SLM_FATAL("An IView service does not received a message." );
}
//-----------------------------------------------------------------------------

void IView::stopping() throw(::fwTools::Failed)
{
   
}

//-----------------------------------------------------------------------------

int IView::getGuiContainerId()
{
    return m_guiContainerId ;
}

//-----------------------------------------------------------------------------

void IView::getQtContainer() const
{
}

//-----------------------------------------------------------------------------

}

}

