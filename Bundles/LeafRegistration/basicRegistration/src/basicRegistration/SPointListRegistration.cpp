/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/spyLog.hpp>

// Service associated data
#include <fwData/Composite.hpp>
#include <fwData/PointList.hpp>
#include <fwData/String.hpp>
#include <fwData/Mesh.hpp>
#include <fwData/TransformationMatrix3D.hpp>

#include <fwServices/IEditionService.hpp>

#include <fwComEd/TransformationMatrix3DMsg.hpp>
#include <fwComEd/MeshMsg.hpp>
#include <fwComEd/Dictionary.hpp>

#include <fwGui/dialog/MessageDialog.hpp>

#include <arlcore/PointsList.h>

// Services tools
#include <fwServices/Base.hpp>

#include "basicRegistration/SPointListRegistration.hpp"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

using fwTools::fwID;

fwServicesRegisterMacro( ::fwServices::IController, ::basicRegistration::SPointListRegistration, ::fwData::Composite );


namespace basicRegistration
{

SPointListRegistration::SPointListRegistration()
{}

// ----------------------------------------------------------------------------

SPointListRegistration::~SPointListRegistration()
{}

// ----------------------------------------------------------------------------

void SPointListRegistration::configuring() throw ( ::fwTools::Failed )
{
    m_registeredPointsKey = m_configuration->findConfigurationElement("registeredPoints")->getAttributeValue("compositeKey");
    m_referencePointsKey = m_configuration->findConfigurationElement("referencePoints")->getAttributeValue("compositeKey");
    m_matrixKey = m_configuration->findConfigurationElement("matrix")->getAttributeValue("compositeKey");
}

// ----------------------------------------------------------------------------

void SPointListRegistration::starting() throw ( ::fwTools::Failed )
{}

// ----------------------------------------------------------------------------

void SPointListRegistration::stopping() throw ( ::fwTools::Failed )
{}

// ----------------------------------------------------------------------------

void SPointListRegistration::updating() throw ( ::fwTools::Failed )
{
    // Get fw4spl data
    ::fwData::Composite::sptr composite =  this->getObject< ::fwData::Composite >();
    ::fwData::PointList::sptr registeredPL = ::fwData::PointList::dynamicCast( (*composite)[ m_registeredPointsKey ] );
    ::fwData::PointList::sptr referencePL = ::fwData::PointList::dynamicCast( (*composite)[ m_referencePointsKey ] );
    ::fwData::TransformationMatrix3D::sptr matrix = ::fwData::TransformationMatrix3D::dynamicCast( (*composite)[ m_matrixKey ] );

    if(     registeredPL->getPoints().size() >= 3 &&
            registeredPL->getPoints().size() == referencePL->getPoints().size() )
    {
        // the points in modelPoints and regPoints are 3D points
        arlCore::PointList::sptr modelPoints, regPoints;

        modelPoints = arlCore::PointList::New( referencePL->getPoints().size() );
        regPoints = arlCore::PointList::New( registeredPL->getPoints().size() );

        arlCore::vnl_rigid_matrix T; //declaration of a rigid transformation matrix

        // modelPoints is a point list that contains 4 points with a square shape
        BOOST_FOREACH( ::fwData::Point::sptr point, referencePL->getPoints() )
        {
            modelPoints->push_back(point->getCoord()[0],point->getCoord()[1],point->getCoord()[2]);
            OSLM_ERROR("referencePL : " << point->getField< ::fwData::String >( ::fwComEd::Dictionary::m_labelId )->value() );
            OSLM_ERROR("referencePL : " << point->getCoord()[0] << " " << point->getCoord()[1] << " " << point->getCoord()[2] );
        }

        // regPoints is a point list that contains 4 noisy points with a square shape at a Z position = 100
        BOOST_FOREACH( ::fwData::Point::sptr point, registeredPL->getPoints() )
        {
            regPoints->push_back(point->getCoord()[0],point->getCoord()[1],point->getCoord()[2]);
            OSLM_ERROR("registeredPL : " << point->getField< ::fwData::String >( ::fwComEd::Dictionary::m_labelId )->value() );
            OSLM_ERROR("registeredPL : " << point->getCoord()[0] << " " << point->getCoord()[1] << " " << point->getCoord()[2] );
        }

        // T will contain a rigid transformation matrix (rotation and translation) that fit regPoints on modelPoints
        // so that T minimizes : sum_i || T * regPoints[i] - modelPoints[i] ||^2
        // If you want to get the RMS error of the registration, the 3rd parameter should be set to "true"
        T.register3D3D(regPoints, modelPoints, true);
        std::cout << T.getString() << std::endl; //print the result

        std::vector<double> error_vector;
        T.RMS3D3D(regPoints, modelPoints, error_vector);

        // Convert Matrix
        matrix->setCoefficient(0,0, T.get(0,0) ); matrix->setCoefficient(0,1, T.get(0,1) ); matrix->setCoefficient(0,2, T.get(0,2) ); matrix->setCoefficient(0,3, T.get(0,3) );
        matrix->setCoefficient(1,0, T.get(1,0) ); matrix->setCoefficient(1,1, T.get(1,1) ); matrix->setCoefficient(1,2, T.get(1,2) ); matrix->setCoefficient(1,3, T.get(1,3) );
        matrix->setCoefficient(2,0, T.get(2,0) ); matrix->setCoefficient(2,1, T.get(2,1) ); matrix->setCoefficient(2,2, T.get(2,2) ); matrix->setCoefficient(2,3, T.get(2,3) );
        matrix->setCoefficient(3,0, T.get(3,0) ); matrix->setCoefficient(3,1, T.get(3,1) ); matrix->setCoefficient(3,2, T.get(3,2) ); matrix->setCoefficient(3,3, T.get(3,3) );

        // Notify Matrix modified
        ::fwComEd::TransformationMatrix3DMsg::sptr msg = ::fwComEd::TransformationMatrix3DMsg::New();
        msg->addEvent( ::fwComEd::TransformationMatrix3DMsg::MATRIX_IS_MODIFIED ) ;
        ::fwServices::IEditionService::notify(this->getSptr(), matrix, msg);
    }
    else
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog("Error",
                "Sorry, you must put more than 3 points to do registration.",
                ::fwGui::dialog::IMessageDialog::WARNING);
    }
}

// ----------------------------------------------------------------------------

void SPointListRegistration::receiving( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed )
{}

// ----------------------------------------------------------------------------

void SPointListRegistration::swapping() throw ( ::fwTools::Failed )
{
    // Classic default approach to update service when object change
    this->stopping();
    this->starting();
}

// ----------------------------------------------------------------------------

} // namespace basicRegistration


