/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSTRUCTURALPATCH_TEST_TU_FWDATA_RECONSTRUCTION_V1TOV2TEST_HPP__
#define __FWSTRUCTURALPATCH_TEST_TU_FWDATA_RECONSTRUCTION_V1TOV2TEST_HPP__

#include <cppunit/extensions/HelperMacros.h>


namespace fwStructuralPatch
{
namespace ut
{
namespace fwData
{
namespace Reconstruction
{

/**
 * @brief Test patch to convert fwData::Reconstruction from version 1 to version 2.
 */
class V1ToV2Test : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( V1ToV2Test );
    CPPUNIT_TEST( applyPatchTest );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    void applyPatchTest();
};

} //namespace Reconstruction
} //namespace fwData
} //namespace ut
} //namespace fwStructuralPatch

#endif //__FWSTRUCTURALPATCH_TEST_TU_FWDATA_RECONSTRUCTION_V1TOV2TEST_HPP__
