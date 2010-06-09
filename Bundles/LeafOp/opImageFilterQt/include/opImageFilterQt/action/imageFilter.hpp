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

class  OPIMAGEFILTERQT_CLASS_API imageFilter : public ::guiQt::action::IAction
{

public :

    fwCoreServiceClassDefinitionsMacro ( (imageFilter)(::guiQt::action::IAction::Baseclass) ) ;

    OPIMAGEFILTERQT_API imageFilter() throw() ;

    OPIMAGEFILTERQT_API virtual ~imageFilter() throw() ;

    OPIMAGEFILTERQT_API void starting() throw ( ::fwTools::Failed );

    OPIMAGEFILTERQT_API void stopping() throw ( ::fwTools::Failed );

    OPIMAGEFILTERQT_API void updating( fwServices::ObjectMsg::csptr _pMsg ) throw ( ::fwTools::Failed );

    OPIMAGEFILTERQT_API void configuring() throw ( ::fwTools::Failed );

    OPIMAGEFILTERQT_API void updating() throw ( ::fwTools::Failed );

    OPIMAGEFILTERQT_API void info ( std::ostream &_sstream ) ;

private :

    std::string m_image1UID;
    std::string m_image2UID;

};


} // namespace action
} // namespace opImageFilter


#endif // _OPIMAGEFILTER_ACTION_IMAGEFILTER_HPP_
