/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _ARLCORE_UNITTEST_H
#define _ARLCORE_UNITTEST_H

//#ifdef _WIN32
#include <arlcore/Common.h>

#include <vector>
#include <vnl/vnl_vector.h>

namespace arlCore
{
/**
 * @Brief Tests sur les reconstructions 3D, recalages 3D/2D, 3D/3D
 */
    // 3D Reconstruction
    ARLCORE_API bool testReconstructionPolyscopic( long int nbIterations, double tolerance );//TODO rajouter le tirage des transformations aléatoires 
    ARLCORE_API bool testReconstructionPolyscopicUncertainty( long int nbIterations, double tolerance);

    // Calibration
    ARLCORE_API bool testIntrinsicCalibration( long int nbIterations, double tolerance );
    ARLCORE_API bool testInitIntrinsicCalibration( long int nbIterations, double tolerance );
    ARLCORE_API bool testExtrinsicCalibration( long int nbIterations, double tolerance );
    ARLCORE_API bool testInitExtrinsicCalibration( long int nbIterations, double tolerance );

    // Matching
    ARLCORE_API bool test3D3DMatching( long int nbIterations, double tolerance );
    ARLCORE_API bool testEpipolarMatching( long int nbIterations, double tolerance );

    /*
     *@brief  Validate the 3D3D Point Registration algorithm available in MatrixR::register3D3D.
     * Each iteration generates 2 point clouds of identical shape. The algorithm registers them
     * minimizing the registration error sum || TxM_i - N_i ||^2 where T is the rigid transformation,
     * M_i (N_i) the set of the 1st (2nd) point cloud. The accuracy of the found 
     * rigid transformation is evaluated by comparing T and the ground truth
     */ 
    ARLCORE_API bool testRegistration3D3D( long int nbIterations, double tolerance );
    /**
     * @brief TODO
     * 
     */
    ARLCORE_API bool testICPRegistration( long int nbIterations, double tolerance );
    /**
     * @brief Validate the 3D3D Point Registration algorithm available in MatrixR::register3D3DUncertainty
     * Each iteration generates 2 point clouds of identical shape. The algorithm registers them
     * minimizing the registration error sum ( TxM_i - N_i )^t * cov^(-1) * ( TxM_i - N_i )
     *  where T is the rigid transformation M_i (N_i) the set of the 1st (2nd) point cloud. 
     * The accuracy of the found rigid transformation is evaluated by comparing T and the ground truth
     */
    ARLCORE_API bool testRegistration3D3DUncertainty( long int nbIterations, double tolerance );
    /**
     * @brief TODO
     * 
     */
    ARLCORE_API bool testRegistration3D3DwithoutMatching( long int nbIterations, double tolerance );

    /**
     * @brief Validate the 3D/2D point registration algorithms. A scene with several cameras is
     * generated. The cameras are looking at an object positioned in camera space (3D point list M_i).
     * The 2D reprojection of these 3D points in each camera are available. 
     * A 3D model of this object in its own frame is available (3D point list_N_i).
     * The Unit test compute the 3D 2D point registration between the model and the camera frame. For
     * example, when the ISPPC criterion is used, we seek T so that sum || P_i(TxN_j) - m_i_j ||^2
     * is minimal (where P_i is the projective function of the i-th camera, m_i_j the reprojection of
     * the j-th point in the i-th camera.
     * The accuracy of the found rigid transformation is evaluated by comparing T and the ground truth 
     * 
     */
    ARLCORE_API bool testProjectiveRegistration( long int nbIterations, double tolerance );
    ARLCORE_API bool testHomographyRegistration( long int nbIterations, double tolerance );

    /**
     * @brief TODO
     * 
     */
    ARLCORE_API bool testRegistrationCriteriaComparison( long int nbIterations, std::vector<double> parameters, std::vector<double> staticStatus,std::vector< vnl_vector<double>* > , std::vector< vnl_vector<double>* > , std::vector< vnl_vector<double>* >   );

    // Sphere Center Estimation from a 3D point cloud
    ARLCORE_API bool testSphereCenterEstimation( long int nbIterations, double tolerance );
    
    // Others
    ARLCORE_API bool testRigidTransfo( unsigned int nbIterations, double tolerance );
    ARLCORE_API bool testPolynomialFieldDistortion( long int nbIterations, double tolerance );
} // namespace arlCore

//#endif // _WIN32

#endif // _ARLCORE_UNITTEST_H
