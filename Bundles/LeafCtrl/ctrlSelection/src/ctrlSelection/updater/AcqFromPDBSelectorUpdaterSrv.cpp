/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/spyLog.hpp>

#include <fwComEd/fieldHelper/BackupHelper.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Integer.hpp>

#include <fwTools/fwID.hpp>

#include <fwServices/macros.hpp>

#include <fwComEd/PatientDBMsg.hpp>

#include "ctrlSelection/updater/AcqFromPDBSelectorUpdaterSrv.hpp"

namespace ctrlSelection
{

namespace updater
{

//-----------------------------------------------------------------------------

fwServicesRegisterMacro( ::ctrlSelection::IUpdaterSrv, ::ctrlSelection::updater::AcqFromPDBSelectorUpdaterSrv, ::fwData::Composite ) ;

//-----------------------------------------------------------------------------

AcqFromPDBSelectorUpdaterSrv::AcqFromPDBSelectorUpdaterSrv() throw()
{}

//-----------------------------------------------------------------------------

AcqFromPDBSelectorUpdaterSrv::~AcqFromPDBSelectorUpdaterSrv() throw()
{}

//-----------------------------------------------------------------------------

void AcqFromPDBSelectorUpdaterSrv::updating( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed )
{
    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >();
    for (   ManagedEvents::iterator it = m_managedEvents.begin();
            it != m_managedEvents.end();
            ++it )
    {
        // First element of tuple ( it->get<0>() ) is the event, test if message correspond to a defined event
        if( _msg->hasEvent( it->get<0>() ) )
        {
            ::fwData::Object::sptr obj = ::fwData::Object::dynamicCast( _msg->getSubject().lock() );
            SLM_ASSERT(obj,"Sorry, the subject of message is not a ::fwData::Object");

            // Test if we manage this event from this object message uid ( it->get<1>() )
            if( obj->getID() == it->get<1>() )
            {
                ::fwData::PatientDB::sptr patientDB = ::fwData::PatientDB::dynamicCast(obj);
                SLM_ASSERT("Sorry, the subject of message is not a ::fwData::PatientDB", patientDB);
                ::fwData::Acquisition::sptr acq;
                if( it->get<3>() != REMOVE )
                {
                    acq = ::fwComEd::fieldHelper::BackupHelper::getSelectedAcquisition(patientDB);
                }
                // Udapte the composite object referenced by the composite key ( it->get<2>() )
                this->updateComposite(composite, acq, it->get<2>(), it->get<3>() );
            }
        }
    }
}

//-----------------------------------------------------------------------------

void AcqFromPDBSelectorUpdaterSrv::starting()  throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void AcqFromPDBSelectorUpdaterSrv::stopping()  throw ( ::fwTools::Failed )
{}

//-----------------------------------------------------------------------------

void AcqFromPDBSelectorUpdaterSrv::configuring()  throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    this->configureManagedEvents(m_configuration);
}

//-----------------------------------------------------------------------------

void AcqFromPDBSelectorUpdaterSrv::reconfiguring()  throw ( ::fwTools::Failed )
{}

//-----------------------------------------------------------------------------

void AcqFromPDBSelectorUpdaterSrv::updating() throw ( ::fwTools::Failed )
{}

//-----------------------------------------------------------------------------

void AcqFromPDBSelectorUpdaterSrv::info( std::ostream &_sstream )
{}

//-----------------------------------------------------------------------------

} // updater
} // ctrlSelection
