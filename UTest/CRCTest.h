/*
 * $Id$
 * $Date$
 * 
 * CRCTest.h: interface for the CRCTest class.
 * 
 * Copyright (c) 2003-2010 INTEC International GmbH, Hechingen, Germany
 * Author: Adrian Weiler
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 */


#if !defined(AFX_CRCTEST_H__6D89A340_D737_4064_B4ED_EFFD994D5EAD__INCLUDED_)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#  define AFX_CRCTEST_H__6D89A340_D737_4064_B4ED_EFFD994D5EAD__INCLUDED_
#endif

/**
 * @file CRCTest.h
 * @brief Contains the unit tests for CRC++
 */

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <cxxtest/TestSuite.h>

  /**
	* @class CRCTest
	* @ingroup Tests
	* @brief CRC tests
	*
	* This test suite tests the CRC class
	*
	* @note You will need the <a href="http://sourceforge.net/projects/cxxtest">CxxTest</a>
	* package to compile and run these tests.
	* CxxTest documentation can be found <a href="http://cxxtest.sourceforge.net/">here</a>.
	*/

class CRCTest : public CxxTest::TestSuite  
{
public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	CRCTest();
	virtual ~CRCTest();

	void setUp();
	void tearDown(); 
#endif


  /**
	* @brief Test CRC in native bit order
	*
	* This tests a CRC CCITT like algorithm, with native bit order
	*/
	void testNative ();

  /**
	* @brief Test CRC in network bit order
	*
	* This tests the CRC CCITT in original definition
	*/
	void testNetwork ();

  /**
	* @brief Test Ethernet CRC
	*
	*/
	void testEthernet ();

  /**
	* @brief Test CRC-64
	*
	* Tests CRC-64 according to test results published in
	* http://swissknife.sourceforge.net/docs/CRC64.html
	*/
	void testCRC64 ();

	/**
	* @brief Test CRC-8 (ITU-T Recommendation I.432.1)
	*
	*/
	void testCRC8 ();

private:
	std::string testPattern;	// A test pattern used in each test
	std::string sResult;		// holds the generated CRC

};
#endif // !defined(AFX_CRCTEST_H__6D89A340_D737_4064_B4ED_EFFD994D5EAD__INCLUDED_)
