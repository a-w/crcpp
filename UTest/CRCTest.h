#pragma once
/*
 * CRCTest.h: interface for the CRCTest class.
 *
 * Copyright (c) 2012 ALDEA Software und Systeme GmbH, Tuebingen, Germany
 * Author: Adrian Weiler
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

/**
 * @file CRCTest.h
 * @brief Contains the unit tests for CRC++
 */

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

    void setUp() override;
    void tearDown() override;
#endif


    /**
     * @brief Test CRC in native bit order
     *
     * This tests a CRC CCITT like algorithm, with native bit order
     */
    static void testNative();

    /**
     * @brief Test CRC in network bit order
     *
     * This tests the CRC CCITT in original definition
     */
    static void testNetwork();

    /**
     * @brief Test Ethernet CRC
     *
     */
    static void testEthernet();

    /**
     * @brief Test CRC-64
     *
     * Tests CRC-64 according to test results published in
     * http://swissknife.sourceforge.net/docs/CRC64.html
     */
    static void testCrc64();

    /**
     * @brief Test CRC-8 (ITU-T Recommendation I.432.1)
     *
     */
    static void testCrc8();

    /**
     * @brief Test CAN CRC - 15 bits wide
     * Test CAN CRC_15 described in ISO 11898-1:2015(E)
     */
    static void testCanCrC15();
};
