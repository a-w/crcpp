#pragma once
/*
 * CRCAlgorithm.h
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

#include "ICRCAlgorithm.h"
#include "CRCInfo.h"

#include "crc.h"
#include "crcstream.h"

#include <string>

/**
 * A Concrete algorithm
 * @ingroup Util
 */

template<class P> class CRCAlgorithm :
    public ICRCAlgorithm
{
public:
    CRCAlgorithm(P const generator, typename P::data_type preset = ~0, typename P::data_type invert = ~0) :
        crcAlgorithm(generator),
        crcStream(crcAlgorithm, preset, invert)
    {
    }

    CrcPP::CRC<P> const& getAlgorithm() const
    {
        return crcAlgorithm;
    }

private:
    void addByte(uint8_t b)
    {
        crcStream << b;
    }
    void reset()
    {
        crcStream.reset();
    }
    bool good() const
    {
        return crcStream.good();
    }
    std::string result() const
    {
        return crcStream.result();
    }

    CrcPP::CRC<P> crcAlgorithm;
    CrcPP::CRCStream<P> crcStream;
};
