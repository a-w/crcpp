/*
 * CRCTest.cpp: implementation of the CRCTest class.
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

#include "CRCTest.h"

#include "crcstream.h"
#include <iostream>

#include <cxxtest/RealDescriptions.h>

using CrcPP::CRC;
using CrcPP::CRCResult;
using CrcPP::CRCStream;
using CrcPP::Poly8;
using CrcPP::Poly8N;
using CrcPP::Poly16;
using CrcPP::Poly16N;
using CrcPP::Poly32;
using CrcPP::Poly32N;
using CrcPP::Poly64;
using CrcPP::Poly64N;

namespace
{
    typedef std::basic_string<uint8_t> ByteString;

    template<typename P, typename D> bool operator==(CRCResult<P> const& lhs, D const* rhs)
    {
        return memcmp(lhs.c_str(), rhs, lhs.size()) == 0;
    }

    template<typename P> ByteString operator+(ByteString const& lhs, CRCResult<P> const& rhs)
    {
        return lhs + ByteString(rhs.c_str(), rhs.size());
    }

    // A test pattern used in each test
    ByteString testPattern(reinterpret_cast<uint8_t const*>("123456789"));
}

CRCTest::CRCTest()
{
}

CRCTest::~CRCTest()
{
}

void CRCTest::setUp()
{
}

void CRCTest::tearDown()
{
}

void CRCTest::testNative()
{
    std::cout << "Testing CRC in native bit order...";

    CRC<Poly16> CRCx(0x1021);
    CRCStream <Poly16> cs(CRCx);

    // Basic test
    cs << testPattern;
    TS_ASSERT(cs.crc() == 0xD64E);
    TS_ASSERT(cs.result() == "\xD6\x4E");

    // Test the gen/check routine
    CRCResult<Poly16> sResult = cs.gen(testPattern);
    TS_ASSERT(sResult == "\xD6\x4E");
    TS_ASSERT(cs.check(testPattern + sResult));

    std::cout << "OK." << std::endl;
}

void CRCTest::testNetwork()
{
    std::cout << "Testing CRC-CCITT...";

    CRC<Poly16N> CRC_CCITT(0x8408);
    CRCStream <Poly16N> cs(CRC_CCITT);

    // Basic test
    cs << testPattern;
    TS_ASSERT(cs.result() == "\x6E\x90");

    char const* testData = "\x01\x73";
    cs.reset();
    cs << testData;
    Poly16N result = cs.crc();

    cs.reset();

    cs << testData << result;

    TS_ASSERT(cs.good());

    // Test the gen/check routine
    CRCResult<Poly16N> sResult = cs.gen(testPattern);
    TS_ASSERT(sResult == "\x6E\x90");
    TS_ASSERT(cs.check(testPattern + sResult));

    sResult = cs.gen(testData);
    TS_ASSERT(sResult == "\x83\x57");
    TS_ASSERT(cs.check(ByteString(reinterpret_cast<uint8_t const*>(testData)) + sResult));
    std::cout << "OK." << std::endl;
}

void CRCTest::testEthernet()
{
    std::cout << "Testing Ethernet CRC...";

    CRC<Poly32N> CRC_ETHER(0xEDB88320);
    CRCStream <Poly32N> cs(CRC_ETHER);

    CRCResult<Poly32N> sResult = cs.gen(testPattern);
    Poly32N result = cs.crc();

    // This is the check value one usually finds in catalogs of CRC parameters...
    TS_ASSERT(result == 0xcbf43926U);
    // ... but this leaves the correct output order unclear. Here it is:
    TS_ASSERT(sResult == "\x26\x39\xf4\xcb");

    // Test data recorded from real ethernet, including CRC
    std::string
    sTestData("\x00\x00\x1a\x18\x26\xb4\x00\x50\x04\x6e\x8b\x4f\x08\x00\x45\x00\x00\x3c\xa6\x42\x00\x00\x80\x01\xec\x5e\xc0\xa8\x13\x07\xc0\xa8\x13\xc8\x08\x00\x48\x5c\x02\x00\x03\x00\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x61\x62\x63\x64\x65\x66\x67\x68\x69\x2c\x71\xa1\x0c",
              78);

    TS_ASSERT(cs.check(sTestData));

    std::cout << "OK." << std::endl;
}

void CRCTest::testCRC64()
{
    std::cout << "Testing CRC-64...";

    CRC<Poly64N> CRC64(0xd800000000000000ULL);
    CRCStream <Poly64N> cs(CRC64, 0, 0);
    Poly64N result;

    ByteString sTestData(reinterpret_cast<uint8_t const*>("IHATEMATH"));
    CRCResult<Poly64N> sResult = cs.gen(sTestData);
    result = cs.crc();

    TS_ASSERT(result == 0xE3DCADD69B01ADD1ULL);
    TS_ASSERT(cs.check(sTestData + sResult));
    std::cout << "OK." << std::endl;
}

void CRCTest::testCRC8()
{
    // ITU-T Recommendation I.432.1 (02/99) X^8 + X^2 + X + 1
    std::cout << "Testing CRC-8 (ISDN HEC)...";

    CRC<Poly8N> CRC8(0XE0);
    CRCStream <Poly8N> cs(CRC8, 0, 0x55);
    Poly8N result;

    ByteString sTestData(reinterpret_cast<uint8_t const*>("\0\0\0\0\x55"), 5);

    CRCResult<Poly8N> sResult = cs.gen(sTestData);
    result = cs.crc();
    TS_ASSERT(result == 0xaf);
    TS_ASSERT(sResult.c_str()[0] == 0xaf);

    sTestData = reinterpret_cast<uint8_t const*>("foo-");
    sResult = cs.gen(sTestData);
    result = cs.crc();
    TS_ASSERT(result == 0xaf);
    TS_ASSERT(cs.check(sTestData + sResult));
    std::cout << "OK." << std::endl;
}
