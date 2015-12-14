/*
 * CRCInfo.h
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
 *  Created on: Apr 16, 2014
 */

#ifndef CRCINFO_H_
#define CRCINFO_H_

#include "ICRCInfo.h"

namespace CrcPP
{
    // Forward
    template<class P> class CRC;
}

/**
 * CRC information
 * @ingroup Util
 */
template <class P> class CRCInfo :
    public ICRCInfo
{
public:
    CRCInfo(CrcPP::CRC<P> const& algorithm) :
        _algorithm(algorithm)
    {
    }

    virtual ~CRCInfo() {}

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

    void writePoly(std::ostream& s) const
    {
        P g = _algorithm.generator();
        s << std::dec;
        s << "X^" << P::numbits;

        for (unsigned int i = P::numbits - 1; i > 0; --i)
        {
            if (g.hibit())
            {
                s << " + X";

                if (i > 1)
                {
                    s << "^" << i;
                }
            }

            g = g.shift(1);
        }

        s << " + 1";
    }

    void writeTable(std::ostream& s) const
    {
        unsigned int fieldWidth = sizeof(typename P::data_type) * 2;
        s << "// CRC Table for poly ";
        writePoly(s);
        s << std::endl;
        s << "// Bit order: " << (isNative() ? "native (lsb is lowest coefficient)"
                                  : "network (lsb is highest coefficient)") << std::endl;
        s << std::hex;
        s << std::setfill('0');
        s << "// Representation of poly in that bit order: 0x" << std::setw(fieldWidth) << (unsigned long) _algorithm.generator() << std::endl;
        s << "const uint" << fieldWidth * 4 << "_t crcTable[] =" << std::endl;
        s << "{" << std::endl;

        for (unsigned int i = 0; i < 256; ++i)
        {
            if (i % 8 == 0)
            {
                s << "    ";
            }

            s << "0x";

            s << std::setw(fieldWidth) << (unsigned long) _algorithm.table()[i];

            if (i != 255)
            {
                s << ", ";
            }

            if (i % 8 == 7)
            {
                s << std::endl;
            }
        }

        s << "};" << std::endl;
        s << std::dec;
    }
private:
    CrcPP::CRC<P> const& _algorithm;
};

#endif /* CRCINFO_H_ */
