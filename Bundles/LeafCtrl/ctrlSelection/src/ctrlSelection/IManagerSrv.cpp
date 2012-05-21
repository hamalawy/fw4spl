/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "ctrlSelection/IManagerSrv.hpp"

namespace ctrlSelection
{

//-----------------------------------------------------------------------------

IManagerSrv::IManagerSrv()
{}

//-----------------------------------------------------------------------------

IManagerSrv::~IManagerSrv()
{}

//-----------------------------------------------------------------------------

void IManagerSrv::swapping() throw ( ::fwTools::Failed )
{
    this->stopping();
    this->starting();
}

//-----------------------------------------------------------------------------

}
