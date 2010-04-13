/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _OPIMAGEFILTER_ACTION_IMAGEFILTER_HPP_
#define _OPIMAGEFILTER_ACTION_IMAGEFILTER_HPP_

#include <guiQt/action/IAction.hpp>


#include "opImageFilterQt/config.hpp"

namespace opImageFilter
{

namespace action
{

class  imageFilter : public ::guiQt::action::IAction
{

public :

    fwCoreServiceClassDefinitionsMacro ( (imageFilter)(::guiQt::action::IAction::Baseclass) ) ;

     imageFilter() throw() ;

     virtual ~imageFilter() throw() ;

     void starting() throw ( ::fwTools::Failed );

     void stopping() throw ( ::fwTools::Failed );

     void updating( fwServices::ObjectMsg::csptr _pMsg ) throw ( ::fwTools::Failed );

     void configuring() throw ( ::fwTools::Failed );

     void updating() throw ( ::fwTools::Failed );

     void info ( std::ostream &_sstream ) ;

private :

    std::string m_image1UID;
    std::string m_image2UID;

};


} // namespace action
} // namespace opImageFilter


#endif // _OPIMAGEFILTER_ACTION_IMAGEFILTER_HPP_
