/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/LabeledPointList.hpp"
#include "visuVTKAdaptor/PointLabel.hpp"
#include "visuVTKAdaptor/PointList.hpp"

#include <fwData/Point.hpp>
#include <fwData/PointList.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/macros.hpp>

#include <fwComEd/Dictionary.hpp>
#include <fwComEd/PointListMsg.hpp>

#include <fwCom/Signal.hxx>

#include <vtkActor.h>
#include <vtkAssemblyNode.h>
#include <vtkAssemblyPath.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <boost/foreach.hpp>

#include <algorithm>

fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::LabeledPointList, ::fwData::PointList );

namespace visuVTKAdaptor
{

//------------------------------------------------------------------------------

void notifyRemoveLandMark( ::fwData::Point::sptr point )
{
    ::fwComEd::PointListMsg::sptr msgPointList = ::fwComEd::PointListMsg::New();
    msgPointList->addEvent( ::fwComEd::PointListMsg::ELEMENT_REMOVED, point );

    ::fwData::Object::ObjectModifiedSignalType::sptr sig;
    sig = point->signal< ::fwData::Object::ObjectModifiedSignalType >( ::fwData::Object::s_OBJECT_MODIFIED_SIG );

    fwServicesNotifyMsgMacro( point->getLightID(), sig, msgPointList );
}

//------------------------------------------------------------------------------

class vtkLabeledPointDeleteCallBack : public vtkCommand
{

public:
    static vtkLabeledPointDeleteCallBack *New( ::fwRenderVTK::IVtkAdaptorService *service)
    {
        return new vtkLabeledPointDeleteCallBack(service);
    }

    vtkLabeledPointDeleteCallBack( ::fwRenderVTK::IVtkAdaptorService *service )
        : m_service(service),
          m_picker( vtkCellPicker::New() ),
          m_propCollection( vtkPropCollection::New() )
    {
        m_lastPos[0] = -1;
        m_lastPos[1] = -1;
        m_picker->PickFromListOn();
        m_picker->SetTolerance(0.001);

        m_display[2] = 0.0;
    }

    ~vtkLabeledPointDeleteCallBack( )
    {
        m_picker->Delete();
        m_picker = NULL;

        m_propCollection->Delete();
        m_propCollection = NULL;
    }


    void fillPickList()
    {
        m_picker->InitializePickList();
        m_propCollection->RemoveAllItems();
        m_service->getAllSubProps(m_propCollection);
        m_propCollection->InitTraversal();

        vtkProp *prop;

        while ( (prop = m_propCollection->GetNextProp()) )
        {
            m_picker->AddPickList(prop);
        }
    }

    virtual void Execute( vtkObject *caller, unsigned long eventId, void *)
    {
        int pos[2];
        m_service->getInteractor()->GetLastEventPosition(pos);
        OSLM_TRACE( "EventId: " << eventId);

        if ( eventId == vtkCommand::RightButtonPressEvent )
        {
            std::copy(pos, pos+1, m_lastPos);
            m_display[0] = pos[0];
            m_display[1] = pos[1];

            this->fillPickList();
            if (m_picker->Pick( m_display, m_service->getRenderer() ) )
            {
                if(getSelectedPoint())
                {
                    SetAbortFlag(1);
                }
                else
                {
                    m_pickedPoint.reset();
                    m_pickedPointList.reset();
                }
            }
        }
        else if ( (eventId == vtkCommand::RightButtonReleaseEvent ) && !m_pickedPoint.expired() &&
                  !m_pickedPointList.expired() && std::equal(pos, pos+1, m_lastPos) )
        {
            ::fwData::PointList::PointListContainer::iterator itr = std::find(
                m_pickedPointList.lock()->getRefPoints().begin(),
                m_pickedPointList.lock()->getRefPoints().end(), m_pickedPoint.lock() );
            if(itr != m_pickedPointList.lock()->getRefPoints().end())
            {
                ::fwData::Point::sptr point = *itr;
                m_pickedPointList.lock()->getRefPoints().erase(itr);
                notifyRemoveLandMark( point );
            }
        }
    }
    bool getSelectedPoint()
    {
        bool isFind              = false;
        vtkPropCollection *propc = m_picker->GetActors();
        vtkProp *prop;

        propc->InitTraversal();
        while ( (prop = propc->GetNextProp()) )
        {
            m_pickedPoint     = ::fwData::Point::dynamicCast(m_service->getAssociatedObject(prop,2));
            m_pickedPointList = ::fwData::PointList::dynamicCast(m_service->getAssociatedObject(prop,1));

            if( !m_pickedPoint.expired() && !m_pickedPointList.expired() )
            {
                ::fwData::PointList::PointListContainer::iterator itr = std::find(
                    m_pickedPointList.lock()->getRefPoints().begin(),
                    m_pickedPointList.lock()->getRefPoints().end(), m_pickedPoint.lock());
                if(itr != m_pickedPointList.lock()->getRefPoints().end() )
                {
                    isFind = true;
                    break;
                }
            }
        }
        return isFind;
    }

protected:
    ::fwRenderVTK::IVtkAdaptorService *m_service;
    vtkPicker * m_picker;
    vtkPropCollection * m_propCollection;
    double m_display[3];
    int m_lastPos[2];
    ::fwData::Point::wptr m_pickedPoint;
    ::fwData::PointList::wptr m_pickedPointList;

};

//------------------------------------------------------------------------------

LabeledPointList::LabeledPointList() throw() :
    m_rightButtonCommand(0),
    m_needSubservicesDeletion(false)
{
    //addNewHandledEvent( ::fwComEd::PointListMsg::ELEMENT_ADDED );
    //addNewHandledEvent( ::fwComEd::PointListMsg::ELEMENT_MODIFIED );
    //addNewHandledEvent( ::fwComEd::PointListMsg::ELEMENT_REMOVED );
}

//------------------------------------------------------------------------------

LabeledPointList::~LabeledPointList() throw()
{
}

//------------------------------------------------------------------------------

void LabeledPointList::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    assert(m_configuration->getName() == "config");
    this->setPickerId( m_configuration->getAttributeValue("picker") );
    this->setRenderId( m_configuration->getAttributeValue("renderer") );
}

//------------------------------------------------------------------------------

void LabeledPointList::doStart() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    m_rightButtonCommand = vtkLabeledPointDeleteCallBack::New(this);
    this->getInteractor()->AddObserver( "RightButtonPressEvent", m_rightButtonCommand, 1 );
    this->getInteractor()->AddObserver( "RightButtonReleaseEvent", m_rightButtonCommand, 1 );

    this->doUpdate();
}

//------------------------------------------------------------------------------

void LabeledPointList::doSwap() throw(fwTools::Failed)
{
    SLM_TRACE("SWAPPING LabeledPointList **TODO**");
    this->doStop();
    this->doStart();
}

//------------------------------------------------------------------------------

void LabeledPointList::doUpdate() throw(fwTools::Failed)
{
    // get PointList in image Field then install distance service if required
    ::fwData::PointList::sptr landmarks = this->getObject< ::fwData::PointList >();

    if ( m_needSubservicesDeletion )
    {
        this->unregisterServices();
        m_needSubservicesDeletion = false;
    }

    if ( !landmarks->getPoints().empty() )
    {
        ::fwRenderVTK::IVtkAdaptorService::sptr servicePointList;
        servicePointList = ::fwServices::add< ::fwRenderVTK::IVtkAdaptorService >( landmarks,
                                                                                   "::visuVTKAdaptor::PointList");
        SLM_ASSERT("servicePointList not instanced", servicePointList);

        servicePointList->setPickerId( this->getPickerId() );
        servicePointList->setRenderService( this->getRenderService() );
        servicePointList->setAutoRender( this->getAutoRender() );
        servicePointList->start();

        this->registerService( servicePointList );


        BOOST_FOREACH( ::fwData::Point::sptr point, landmarks->getRefPoints() )
        {
            ::fwRenderVTK::IVtkAdaptorService::sptr serviceLabel;
            serviceLabel =
                ::fwServices::add< ::fwRenderVTK::IVtkAdaptorService >(point, "::visuVTKAdaptor::PointLabel");
            SLM_ASSERT("serviceLabel not instanced", serviceLabel);
            serviceLabel->setRenderService( this->getRenderService() );
            serviceLabel->setAutoRender( this->getAutoRender() );
            serviceLabel->start();
            this->registerService( serviceLabel );
        }
    }

    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void LabeledPointList::doReceive( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    // update only if new LandMarks
    ::fwComEd::PointListMsg::csptr plMsg = ::fwComEd::PointListMsg::dynamicConstCast( msg );
    if ( plMsg &&
         ( plMsg->hasEvent( ::fwComEd::PointListMsg::ELEMENT_ADDED ) ||
           plMsg->hasEvent( ::fwComEd::PointListMsg::ELEMENT_REMOVED )||
           plMsg->hasEvent( ::fwComEd::PointListMsg::ELEMENT_MODIFIED ) ) )
    {
        m_needSubservicesDeletion = true; // to manage point deletion
        doUpdate();
    }
}

//------------------------------------------------------------------------------

void LabeledPointList::doStop() throw(fwTools::Failed)
{
    if ( m_rightButtonCommand ) // can be not instanciated (use of LabeledPointList::show() )
    {
        this->getInteractor()->RemoveObserver(m_rightButtonCommand);
        m_rightButtonCommand->Delete();
        m_rightButtonCommand = 0;
    }

    this->unregisterServices();
}

//------------------------------------------------------------------------------


} //namespace visuVTKAdaptor
