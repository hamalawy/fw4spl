/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include "gui/editor/IEditor.hpp"

namespace gui
{
namespace editor
{


IEditor::IEditor() throw()
{}

//-----------------------------------------------------------------------------

IEditor::~IEditor() throw()
{}

//-----------------------------------------------------------------------------

void IEditor::info(std::ostream &_sstream )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

}
}

