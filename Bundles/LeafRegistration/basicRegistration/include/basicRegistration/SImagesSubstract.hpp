/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _BASICREGISTRATION_SIMAGESSUBSTRACT_HPP_
#define _BASICREGISTRATION_SIMAGESSUBSTRACT_HPP_

#include <QObject>
#include <QPointer>

#include <gui/editor/IEditor.hpp>

#include "basicRegistration/config.hpp"

class QPushButton;

namespace basicRegistration
{

class BASICREGISTRATION_CLASS_API SImagesSubstract : public QObject, public ::gui::editor::IEditor
{
    Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro ( (SImagesSubstract)(::gui::editor::IEditor) ) ;

    BASICREGISTRATION_API SImagesSubstract() throw();

    BASICREGISTRATION_API virtual ~SImagesSubstract() throw();

protected :

    BASICREGISTRATION_API virtual void configuring() throw ( ::fwTools::Failed );

    /// Overrides
    BASICREGISTRATION_API virtual void starting() throw ( ::fwTools::Failed );

    /// Overrides
    BASICREGISTRATION_API virtual void stopping() throw ( ::fwTools::Failed );

    /// Overrides
    BASICREGISTRATION_API virtual void updating() throw ( ::fwTools::Failed );

    /// Overrides
    BASICREGISTRATION_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed );

    /// Overrides
    BASICREGISTRATION_API virtual void swapping() throw ( ::fwTools::Failed );




private Q_SLOTS:

    /// Compute the subtraction between two images.
    void OnCompute();

private :
    QPointer< QPushButton > mpComputeButton;


};



} // namespace basicRegistration

#endif /*_BASICREGISTRATION_SIMAGESSUBSTRACT_HPP_*/

