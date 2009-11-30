/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as 
 * published by the Free Software Foundation.  
 * ****** END LICENSE BLOCK ****** */

#ifndef DATA_TU_IMAGETEST_H
#define DATA_TU_IMAGETEST_H

#include <cppunit/extensions/HelperMacros.h>

class ImageTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( ImageTest );
	CPPUNIT_TEST( methode1 );
	CPPUNIT_TEST( methode2 );
	CPPUNIT_TEST_SUITE_END();

protected:

public:
	// interface
	void setUp();
	void tearDown();
	// fonctions de tests
	void methode1();
	void methode2();
};

#endif
