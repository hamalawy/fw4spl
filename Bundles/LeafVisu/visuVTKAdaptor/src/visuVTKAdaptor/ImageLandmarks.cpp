/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <algorithm>

#include <boost/foreach.hpp>

#include <fwData/Boolean.hpp>
#include <fwData/Image.hpp>
#include <fwData/Material.hpp>
#include <fwData/PointList.hpp>
#include <fwData/Point.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>

#include <fwComEd/Dictionary.hpp>
#include <fwComEd/ImageMsg.hpp>
#include <fwComEd/PointListMsg.hpp>

#include <vtkActor.h>
#include <vtkAssemblyNode.h>
#include <vtkAssemblyPath.h>
#include <vtkCommand.h>
#include <vtkCubeSource.h>
#include <vtkCellPicker.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include "visuVTKAdaptor/ImageLandmarks.hpp"
#include "visuVTKAdaptor/PointList.hpp"
#include "visuVTKAdaptor/PointLabel.hpp"
#include <fwServices/IEditionService.hpp>



fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::ImageLandmarks, ::fwData::Image ) ;

namespace visuVTKAdaptor
{

void notifyRemoveLandMark( ::fwData::Image::sptr image, ::fwServices::IService* _service, ::fwData::Point::sptr point )
{
    SLM_ASSERT("NULL Service", _service);

    ::fwComEd::PointListMsg::sptr msgPointList = ::fwComEd::PointListMsg::New();
    msgPointList->addEvent( ::fwComEd::PointListMsg::ELEMENT_REMOVED, point );
    ::fwData::PointList::sptr pointList = image->getField< ::fwData::PointList >(  ::fwComEd::Dictionary::m_imageLandmarksId );
    ::fwServices::IEditionService::notify( _service->getSptr(), pointList, msgPointList);

    ::fwComEd::ImageMsg::sptr msgLandmark = ::fwComEd::ImageMsg::New();
    msgLandmark->addEvent( ::fwComEd::ImageMsg::LANDMARK, point );
    ::fwData::Object::ObjectModifiedSignalType::sptr sig;
    sig = image->signal< ::fwData::Object::ObjectModifiedSignalType >( ::fwData::Object::s_OBJECT_MODIFIED_SIG );
    fwServicesNotifyMsgMacro( image->getLightID(), sig, msgLandmark );
}

//------------------------------------------------------------------------------

class vtkPointDeleteCallBack : public vtkCommand
{

public :
    static vtkPointDeleteCallBack *New( ::fwRenderVTK::IVtkAdaptorService *service)
    { return new vtkPointDeleteCallBack(service); }

    vtkPointDeleteCallBack( ::fwRenderVTK::IVtkAdaptorService *service )
    : m_service(service),
      m_picker( vtkCellPicker::New() ),
      m_propCollection( vtkPropCollection::New() )
    {
        m_lastPos[0] = -1;
        m_lastPos[1] = -1;
        m_picker->PickFromListOn();
        m_picker->SetTolerance(0.001);

        m_display[2]=0.0;
    }

    ~vtkPointDeleteCallBack( )
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
            if (m_picker->Pick( m_display , m_service->getRenderer() ) )
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
        else if ( (eventId == vtkCommand::RightButtonReleaseEvent ) && !m_pickedPoint.expired() && !m_pickedPointList.expired() && std::equal(pos, pos+1, m_lastPos) )
        {
            ::fwData::Image::sptr image = m_service->getObject< ::fwData::Image >();
            ::fwData::PointList::PointListContainer::iterator itr = std::find( m_pickedPointList.lock()->getRefPoints().begin(), m_pickedPointList.lock()->getRefPoints().end(), m_pickedPoint.lock() );
            if(itr != m_pickedPointList.lock()->getRefPoints().end())
            {
                ::fwData::Point::sptr point = *itr;
                m_pickedPointList.lock()->getRefPoints().erase(itr);
                notifyRemoveLandMark(image, m_service, point);
            }
        }
    }
    bool getSelectedPoint()
    {
        bool isFind = false;
        vtkPropCollection *propc = m_picker->GetActors();
        vtkProp *prop;

        propc->InitTraversal();
        while ( (prop = propc->GetNextProp()) )
        {
            m_pickedPoint = ::fwData::Point::dynamicCast(m_service->getAssociatedObject(prop,2));
            m_pickedPointList = ::fwData::PointList::dynamicCast(m_service->getAssociatedObject(prop,1));

            if( !m_pickedPoint.expired() && !m_pickedPointList.expired() )
            {
                ::fwData::PointList::PointListContainer::iterator itr = std::find( m_pickedPointList.lock()->getRefPoints().begin(), m_pickedPointList.lock()->getRefPoints().end(), m_pickedPoint.lock());
                if(itr != m_pickedPointList.lock()->getRefPoints().end() )
                {
                    isFind = true;
                    break;
                }
            }
        }
        return isFind;
    }

protected :
    ::fwRenderVTK::IVtkAdaptorService *m_service;
    vtkPicker * m_picker;
    vtkPropCollection * m_propCollection;
    double m_display[3];
    int m_lastPos[2];
    ::fwData::Point::wptr m_pickedPoint;
    ::fwData::PointList::wptr m_pickedPointList;

};

//------------------------------------------------------------------------------

ImageLandmarks::ImageLandmarks() throw():
    m_rightButtonCommand(0),
    m_needSubservicesDeletion(false)
{
    //addNewHandledEvent( ::fwComEd::ImageMsg::LANDMARK );
}

//------------------------------------------------------------------------------

ImageLandmarks::~ImageLandmarks() throw()
{}

//------------------------------------------------------------------------------

void ImageLandmarks::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    assert(m_configuration->getName() == "config");
    this->setPickerId( m_configuration->getAttributeValue("picker") );
    this->setRenderId( m_configuration->getAttributeValue("renderer") );
}

//------------------------------------------------------------------------------

void ImageLandmarks::doStart() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    m_rightButtonCommand = vtkPointDeleteCallBack::New(this);
    this->getInteractor()->AddObserver( "RightButtonPressEvent" , m_rightButtonCommand, 1 );
    this->getInteractor()->AddObserver( "RightButtonReleaseEvent" , m_rightButtonCommand, 1 );

    this->doUpdate();
}

//------------------------------------------------------------------------------

void ImageLandmarks::doSwap() throw(fwTools::Failed)
{
    SLM_TRACE("SWAPPING ImageLandmarks **TODO**");
    this->doStop();
    this->doStart();
}

//------------------------------------------------------------------------------

void ImageLandmarks::doUpdate() throw(fwTools::Failed)
{
    // get PointList in image Field then install distance service if required
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

    ::fwData::PointList::sptr landmarks;
    bool isShown;
    landmarks = image->getField< ::fwData::PointList >(  ::fwComEd::Dictionary::m_imageLandmarksId );
    isShown = image->getField("ShowLandmarks", ::fwData::Boolean::New(true))->value();

    if (!isShown || !landmarks || m_needSubservicesDeletion)
    {
        this->unregisterServices();
        m_needSubservicesDeletion = false;
    }

    if( isShown && landmarks )
    {
        if ( ! landmarks->getPoints().empty() )
        {
            ::fwRenderVTK::IVtkAdaptorService::sptr servicePointList;
            servicePointList = ::fwServices::add< ::fwRenderVTK::IVtkAdaptorService >( landmarks , "::visuVTKAdaptor::PointList");
            SLM_ASSERT("servicePointList not instanced", servicePointList);

            servicePointList->setPickerId( this->getPickerId() );
            servicePointList->setRenderService( this->getRenderService() );
            servicePointList->start();

            this->registerService( servicePointList );


            BOOST_FOREACH( ::fwData::Point::sptr point, landmarks->getRefPoints() )
            {
                ::fwRenderVTK::IVtkAdaptorService::sptr serviceLabel;
                serviceLabel = ::fwServices::add< ::fwRenderVTK::IVtkAdaptorService >(point , "::visuVTKAdaptor::PointLabel");
                SLM_ASSERT("serviceLabel not instanced", serviceLabel);
                serviceLabel->setRenderService( this->getRenderService() );
                serviceLabel->start();
                this->registerService( serviceLabel );
            }
        }
    }
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void ImageLandmarks::doReceive( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    // update only if new LandMarks
     ::fwComEd::ImageMsg::csptr imgMsg =  ::fwComEd::ImageMsg::dynamicConstCast( msg );
    if ( imgMsg && imgMsg->hasEvent( ::fwComEd::ImageMsg::LANDMARK ) )
    {
        m_needSubservicesDeletion = true; // to manage point deletion
        doUpdate();
    }
}

//------------------------------------------------------------------------------

void ImageLandmarks::doStop() throw(fwTools::Failed)
{
    if ( m_rightButtonCommand ) // can be not instanciated (use of ImageLandmarks::show() )
    {
        this->getInteractor()->RemoveObserver(m_rightButtonCommand);
        m_rightButtonCommand->Delete();
        m_rightButtonCommand = 0;
    }

    this->unregisterServices();
}

//------------------------------------------------------------------------------

void ImageLandmarks::show(bool b)
{
    if (b)
    {
        this->doStart();
    }
    else
    {
        this->doStop();
    }
}


} //namespace visuVTKAdaptor
