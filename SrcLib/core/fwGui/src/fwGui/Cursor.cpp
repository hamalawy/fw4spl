/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwTools/Factory.hpp>

#include "fwGui/Cursor.hpp"

namespace fwGui
{

//-----------------------------------------------------------------------------

Cursor::Cursor()
{
    m_implementation = ::fwTools::ClassFactoryRegistry::create< ::fwGui::ICursor>( ::fwGui::ICursor::factoryRegistryKey);
}

//-----------------------------------------------------------------------------

Cursor::~Cursor()
{}

//-----------------------------------------------------------------------------

void Cursor::setCursor( ::fwGui::ICursor::CursorType cursor)
{
    m_implementation->setCursor(cursor);
}

//-----------------------------------------------------------------------------

void Cursor::setDefaultCursor()
{
    m_implementation->setDefaultCursor();
}

//-----------------------------------------------------------------------------

} // namespace fwGui



