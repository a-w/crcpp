/*
 * crctest.cpp
 *
 * Utility to test various CRC algorithms
 *
 * This file is part of CRC++
 *
 * Copyright (c) 2014 ALDEA Software und Systeme GmbH, Tuebingen, Germany
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

#include <iostream>
#include <iomanip>
#include <cstring>
#include <list>
#include <getopt.h>
#include <stdint.h>

#include "crc.h"
#include "crcstream.h"

#include "CRCInfo.h"
#include "CRCFactory.h"



/**
 * Interface for a CRC test
 */
class ICRCTest
{
public:
    virtual ICRCAlgorithm* getAlgorithm() const = 0;
    virtual bool isNative() const = 0;
    virtual unsigned int numBits() const = 0;
    virtual unsigned int numBytes() const = 0;
    virtual void describe(std::ostream& s) const = 0;
    virtual void writePoly(std::ostream& s) const = 0;
    virtual void writeTable(std::ostream& s) const = 0;
    virtual ~ICRCTest() {}
};


template<class P> class CRCTest :
    public ICRCTest
{
public:
    CRCTest(ICRCWithDescriptionFactory& aFactory) :
        crcAlgorithm(aFactory.create(crcInfo)),
        theGenerator(aFactory.generator()),
        thePreset(aFactory.preset()),
        theInvert(aFactory.invert())
    {
    }
    virtual ~CRCTest()
    {
        delete crcAlgorithm;
        delete crcInfo;
    }
private:

    ICRCAlgorithm* getAlgorithm() const
    {
        return crcAlgorithm;
    }
    bool isNative() const
    {
        return crcInfo->isNative();
    }
    unsigned int numBits() const
    {
        return crcInfo->numBits();
    }

    unsigned int numBytes() const
    {
        return crcInfo->numBytes();
    }

    void writeTable(std::ostream& s) const
    {
        crcInfo->writeTable(s);
    }
    void writePoly(std::ostream& s) const
    {
        crcInfo->writePoly(s);
    }
    void describe(std::ostream& s) const
    {
        writePoly(s);
        s << " in " << (isNative() ? "native" : "network") << " order";
        s << std::hex;
        s << std::setfill('0');
        unsigned int fieldWidth = sizeof(typename P::data_type) * 2;
        s << " = 0x" << std::setw(fieldWidth) << theGenerator;
        s << ", preset 0x" << std::setw(fieldWidth) << thePreset;
        s << ", invert 0x" << std::setw(fieldWidth) << theInvert;
    }

    ICRCAlgorithm* crcAlgorithm;
    ICRCInfo* crcInfo;
    uint64_t theGenerator;
    uint64_t thePreset;
    uint64_t theInvert;
};

class ICRCTestFactory
{
public:
    virtual ~ICRCTestFactory() {}
    virtual ICRCTest* createTest() const = 0;
    virtual ICRCWithDescriptionFactory& getFactory() = 0;
    virtual bool isNative() const = 0;
    virtual unsigned int numBits() const = 0;
    virtual unsigned int numBytes() const = 0;
};

template<class P> class CRCParameters :
    public ICRCTestFactory
{
public:
    CRCParameters(uint64_t aGenerator, uint64_t aPreset = ~0, uint64_t anInvert = ~0) :
        theFactory(new CRCFactory<P>)
    {
        theFactory->setGenerator(aGenerator);
        theFactory->setPreset(aPreset);
        theFactory->setInvert(anInvert);
    }
    virtual ~CRCParameters()
    {
        delete theFactory;
    }

private:
    bool isNative() const
    {
        P one = 1;
        return one.lobit() == 1;
    }

    unsigned int numBits() const
    {
        return P::numbits;
    }

    unsigned int numBytes() const
    {
        return sizeof(typename P::data_type);
    }

    ICRCWithDescriptionFactory& getFactory()
    {
        return *theFactory;
    }

    ICRCTest* createTest() const
    {
        return new CRCTest<P> (*theFactory);
    }
    ICRCWithDescriptionFactory* theFactory;
};


struct AlgorithmFactory
{
    char const* name;
    ICRCTestFactory* factory;
};

static AlgorithmFactory algorithms[] =
{
    // 32-bit tests

    // Standard CRC-32 Network Order (Ethernet etc...)
    { "ieee802.3",  new CRCParameters<CrcPP::Poly32N>(0xEDB88320) },

    // CRC-32 Native order, not inverted
    { "crc32",  new CRCParameters<CrcPP::Poly32> (0x04C11DB7, ~0, 0) },


    // 16-Bit tests

    // Standard CRC-CCITT Network Order (X.25/HDLC)
    { "ccitt", new CRCParameters<CrcPP::Poly16N> (0x8408) },

    // CRC-CCITT Native Order, not inverted
    { "ccitt-native", new CRCParameters<CrcPP::Poly16> (0x1021, ~0, 0) },

    // CRC-CCITT Native Order, inverted
    { "ccitt-native-inverted", new CRCParameters<CrcPP::Poly16> (0x1021) },

    // CRC-16 (IBM)
    { "crc16",  new CRCParameters<CrcPP::Poly16> (0x8005, 0, 0) },

    // CRC-16 Network order
    { "crc16n",  new CRCParameters<CrcPP::Poly16N> (0xA001, ~0, 0) },


    // 8-Bit tests

    // 8-Bit CRC ITU-T Network Order, Result exored with 0x55 (ISDN HEC)
    { "hec", new CRCParameters<CrcPP::Poly8N> (0xE0, 0, 0x55) },

    // 8-Bit CRC ITU-T Native Order
    { "crc8", new CRCParameters<CrcPP::Poly8> (0x07, 0, 0) },

    // 8-Bit CRC SAE J1850
    { "j1850", new CRCParameters<CrcPP::Poly8> (0x1D) },

    // 8-Bit CRC SAE 2F
    { "sae2f", new CRCParameters<CrcPP::Poly8> (0x2F) },
    { 0, 0 }
};


static AlgorithmFactory generics[] =
{
    // 32-bit

    // CRC-32 Network Order
    { "g32r",  new CRCParameters<CrcPP::Poly32N>(0) },

    // CRC-32 Native order
    { "g32",  new CRCParameters<CrcPP::Poly32> (0) },


    // 16-Bit

    // 16 Bit Network Order
    { "g16r", new CRCParameters<CrcPP::Poly16N> (0) },

    // 16 Bit Native Order
    { "g16", new CRCParameters<CrcPP::Poly16> (0) },


    // 8-Bit

    // 8-Bit Network Order
    { "g8r", new CRCParameters<CrcPP::Poly8N> (0) },

    // 8-Bit Native Order
    { "g8", new CRCParameters<CrcPP::Poly8> (0) },
    { 0, 0 }
};


static AlgorithmFactory* lists[] =
{
    generics,
    algorithms,
    0
};

uint64_t toHex(char const* p, unsigned int maxBytes = 2)
{
    uint64_t hexData = 0;

    for (unsigned int i = 0; (i < maxBytes) && (*p != 0); ++i, ++p)
    {
        if (*p >= '0' && *p <= '9')
        {
            hexData = (hexData << 4) | (*p - '0');
        }
        else if (*p >= 'a' && *p <= 'f')
        {
            hexData = (hexData << 4) | (*p - 'a' + 10);
        }
        else if (*p >= 'A' && *p <= 'F')
        {
            hexData = (hexData << 4) | (*p - 'A' + 10);
        }
        else
        {
            std::cerr << "invalid hex char: '" << *p << std::endl;
        }
    }

    if (*p)
    {
        std::cerr << "hex expression should be " << maxBytes << " bytes only. Extra chars: '" << p << "' ignored." << std::endl;
    }

    return hexData;
}

class HexDump
{
public:
    HexDump(unsigned char const* someData, unsigned int aLength)
        : data(someData), size(aLength)
    {
    }

    unsigned char const* data;
    unsigned int size;
};


std::ostream& operator << (std::ostream& stream, HexDump const& d)
{
    stream << std::hex;

    // <debug>
    for (unsigned int i = 0; i < d.size; i++)
    {
        if (i > 0)
        {
            if ((i % 16) == 0)
            {
                stream << std::endl;
            }
            else if ((i % 8) == 0)
            {
                stream << "\t";
            }
        }

        unsigned int value = static_cast<unsigned int>(d.data[i]);

        if (value < 0x10)
        {
            stream << " 0" << value;
        }
        else
        {
            stream << " " << value;
        }
    }

    stream << std::dec;
    return stream;
}

void usage(char* progname)
{
    std::cerr << "Usage:" << std::endl <<
              progname << " -a algo | --algorithm=algo [-b] xx xx xx ... " << std::endl <<
              progname << " -a algo | --algorithm=algo -w | --write-table " << std::endl
              << "    where xx are pairs of hex digits" << std::endl << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "-b | --binary    binary output" << std::endl;
    std::cerr << "-g | --generator specify generator polynomial in hex" << std::endl;
    std::cerr << "-i | --invert    specify invert (xor) in hex" << std::endl;
    std::cerr << "-p | --preset    specify preset value in hex" << std::endl;
    std::cerr << "-v | --verify    exit with status 0 if CRC is OK, 1 if bad" << std::endl << std::endl;
    std::cerr << "If generator, invert or preset is used, algo must be specified first (to determine the number of bytes)" << std::endl << std::endl;
    std::cerr << "algo is one of: " << std::endl;

    for (AlgorithmFactory* a = algorithms; a->name; ++a)
    {
        std::cerr << "  " << a->name << " ";
        a->factory->createTest()->describe(std::cerr);
        std::cerr << std::endl;
    }

    std::cerr << std::endl << "Additionally, one of the following generic specifications may be used: " << std::dec << std::endl;

    for (AlgorithmFactory* a = generics; a->name; ++a)
    {
        ICRCTestFactory& f = *a->factory;

        std::cerr << "  " << a->name << " " <<

                  f.numBits() << " bits, " << (f.isNative() ? "native" : "network") << " order";

        std::cerr << std::endl;
    }

    std::cerr << "These generics do not incude a valid polynomical, so --generator MUST be used." << std::endl << std::endl;

    std::cerr << progname << " -s | --search xx xx xx ... search for algorithm giving good crc with that data" << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
    ICRCTestFactory* theFactory = 0;
    bool doWriteTable = false;
    bool doSearch = false;
    bool doVerify = false;
    int  verbosity = 0;
    bool binaryOutput = false;

    static struct option longOptions[] =
    {
        {"algorithm", 1, 0, 'a'},
        {"binary", 0, 0, 'b'},
        {"generator", 1, 0, 'g'},
        {"help", 0, 0, 'h'},
        {"invert", 1, 0, 'i'},
        {"preset", 1, 0, 'p'},
        {"search", 0, 0, 's'},
        {"verbose", 0, 0, 'V'},
        {"verify", 0, 0, 'v'},
        {"write-table", 0, 0, 'w'},
        {0, 0, 0, 0}
    };

    for (int i = 0; i < argc; ++i)
    {
        std::cerr << argv[i] << " ";
    }

    std::cerr << std::endl;


    do
    {
        int optionIndex = 0;
        int opt = ::getopt_long(argc, argv, "a:bg:hi:p:svVw", longOptions, &optionIndex);

        if (opt == -1)
        {
            break;    // Exit loop
        }

        switch (opt)
        {
            case 0:                // long option
                break;

            case 'a':
            {
                for (AlgorithmFactory** list = lists; *list != 0; ++list)
                {
                    for (AlgorithmFactory* a = *list; a->name; ++a)
                    {
                        if (std::strcmp(a->name, optarg) == 0)
                        {
                            theFactory = a->factory;
                            break;
                        }
                    }

                    if (theFactory != 0)
                    {
                        break;
                    }
                }

                if (theFactory == 0)
                {
                    std::cerr << "No algorithm with that name: " << optarg << std::endl;
                    usage(argv[0]);
                    return 1;
                }
            }
            break;

            case 'b':
                binaryOutput = true;
                break;

            case 'g':
                if (theFactory == 0)
                {
                    std::cerr << "Base algorithm must be specified before --generator" << std::endl;
                    usage(argv[0]);
                    return 1;
                }
                else
                {
                    uint64_t generator = toHex(optarg, theFactory->numBytes() * 2);
                    theFactory->getFactory().setGenerator(generator);
                }

                break;

            case 'h':
                usage(argv[0]);
                return 0;

            case 'i':
                if (theFactory == 0)
                {
                    std::cerr << "Base algorithm must be specified before --invert" << std::endl;
                    usage(argv[0]);
                    return 1;
                }
                else
                {
                    uint64_t invert = toHex(optarg, theFactory->numBytes() * 2);
                    theFactory->getFactory().setInvert(invert);
                }

                break;

            case 'p':
                if (theFactory == 0)
                {
                    std::cerr << "Base algorithm must be specified before --preset" << std::endl;
                    usage(argv[0]);
                    return 1;
                }
                else
                {
                    uint64_t preset = toHex(optarg, theFactory->numBytes() * 2);
                    theFactory->getFactory().setPreset(preset);
                }

                break;

            case 's':
                doSearch = true;
                break;

            case 'v':
                doVerify = true;
                break;

            case 'V':
                ++verbosity;
                break;

            case 'w':
                doWriteTable = true;
                break;
        }
    }
    while (true);    // end by explicit break

    if ((theFactory == 0) && !doSearch)
    {
        std::cerr << "No algorithm selected." << std::endl;
        usage(argv[0]);
        return 1;
    }

    if ((theFactory != 0) && doSearch)
    {
        std::cerr << "Do not specify algorithm with --search." << std::endl;
        usage(argv[0]);
        return 1;
    }

    if (doWriteTable && doSearch)
    {
        std::cerr << "--search and --write-table are mutually exclusive." << std::endl;
        usage(argv[0]);
        return 1;
    }

    if (doSearch)
    {
        bool found = false;
        std::list<uint8_t> src;

        while (argc > optind)
        {
            uint8_t nextByte = (uint8_t) toHex(argv[optind]);
            src.push_back(nextByte);
            std::cout << HexDump(&nextByte, 1);
            ++optind;
        }

        if (src.size() == 0)
        {
            std::cerr << "Please specify data to be checked" << std::endl;
            usage(argv[0]);
            return 1;
        }

        std::cout << std::endl;

        for (AlgorithmFactory* a = algorithms; a->name; ++a)
        {
            ICRCTest* aTest = a->factory->createTest();
            ICRCAlgorithm* algo = aTest->getAlgorithm();

            for (std::list<uint8_t>::const_iterator it = src.begin(); it != src.end(); ++it)
            {
                algo->addByte(*it);
            }

            if (algo->good())
            {
                std::cout << "Algorithm: " << a->name << ": ";
                aTest->describe(std::cout);
                std::cout << std::endl;
                found = true;
            }
        }

        if (found)
        {
            return 0;
        }
        else
        {
            std::cerr << "No algorithm matches data" << std::endl;
            return 1;
        }
    }

    // Normal operation

    ICRCTest* theTest = 0;

    try
    {
        theTest = theFactory->createTest();
    }
    catch (std::exception& ex)
    {
        std::cerr << "ERROR: Invalid polynomial: " << std::endl << ">>>>>> " << ex.what() << std::endl;
        return 1;
    }

    if (doWriteTable)
    {
        theTest->writeTable(std::cout);
        return 0;
    }

    if (verbosity > 0)
    {
        theTest->describe(std::cout);
        std::cout << std::endl;
    }

    ICRCAlgorithm* algo = theTest->getAlgorithm();

    while (argc > optind)
    {
        uint8_t nextByte = (uint8_t) toHex(argv[optind]);
        algo->addByte(nextByte);

        if (binaryOutput)
        {
            std::cout << nextByte;
        }
        else
        {
            std::cout << HexDump(&nextByte, 1);
        }

        ++optind;
    }

    std::string res = algo->result();

    if (binaryOutput)
    {
        std::cout << res;
    }
    else
    {
        std::cout << HexDump((unsigned char const*) res.c_str(), (unsigned int) res.size());

        if (doVerify)
        {
            std::cout << (theTest->getAlgorithm()->good() ? " (OK)" : " (BAD)");
        }

        std::cout << std::endl;
    }

    return !doVerify || theTest->getAlgorithm()->good() ? 0 : 1;
}
