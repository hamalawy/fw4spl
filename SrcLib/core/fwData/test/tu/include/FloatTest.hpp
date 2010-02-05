/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef DATA_TU_FloatTEST_H
#define DATA_TU_FloatTEST_H

#include <cppunit/extensions/HelperMacros.h>

class FloatTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( FloatTest );
	CPPUNIT_TEST( methode1 );
	CPPUNIT_TEST_SUITE_END();

protected:

public:
	// interface
	void setUp();
	void tearDown();
	// fonctions de tests
	void methode1();
};

#endif
