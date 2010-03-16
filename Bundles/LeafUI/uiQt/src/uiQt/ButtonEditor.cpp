/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <QPushButton>

#include <fwComEd/ImageMsg.hpp>

#include <fwServices/Base.hpp>

#include "uiQt/ButtonEditor.hpp"



REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiQt::ButtonEditor , ::fwTools::Object) ;


namespace uiQt
{

ButtonEditor::ButtonEditor() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

ButtonEditor::~ButtonEditor() throw()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ButtonEditor::configuring() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ButtonEditor::starting() throw(fwTools::Failed)
{
   ::guiQt::editor::IEditor::starting();
   QPushButton *button = new QPushButton("ButtonTest",m_container);
   button->resize(150,150);
   button->show();
   
}

//-----------------------------------------------------------------------------

void ButtonEditor::stopping() throw(fwTools::Failed)
{
  ::guiQt::editor::IEditor::stopping();
}

//-----------------------------------------------------------------------------

void ButtonEditor::updating() throw(fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void ButtonEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(fwTools::Failed)
{
   
}

//-----------------------------------------------------------------------------

}
