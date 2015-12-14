/*
 * CRCFactory.h
 *
 * * This file is part of CRC++
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

#ifndef CRCFACTORY_H_
#define CRCFACTORY_H_

#include "ICRCFactory.h"
#include "ICRCInfo.h"
#include "CRCAlgorithm.h"

/**
 * Interface for CRC factories
 * @ingroup Util
 */
class ICRCWithDescriptionFactory :
    public ICRCFactory
{
public:
    virtual ICRCAlgorithm* create(ICRCInfo*& info) const = 0;
};

/**
 * A factory for building CRC algorithm implementations
 * @ingroup Util
 */
template<class P> class CRCFactory :
    public ICRCWithDescriptionFactory
{
public:
    CRCFactory() :
        theGenerator(0),
        thePreset(~0),
        theInvert(~0)
    {
    }

    ICRCAlgorithm* create() const
    {
        return new CRCAlgorithm<P> (theGenerator, thePreset, theInvert);
    }

    ICRCAlgorithm* create(ICRCInfo*& info) const
    {
        CRCAlgorithm<P>* crcAlgorithm = new CRCAlgorithm<P> (theGenerator, thePreset, theInvert);
        info = new CRCInfo<P> (crcAlgorithm->getAlgorithm());
        return crcAlgorithm;
    }

    uint64_t generator() const
    {
        return theGenerator;
    }

    uint64_t preset() const
    {
        return thePreset;
    }

    uint64_t invert() const
    {
        return theInvert;
    }

    void setGenerator(uint64_t aGenerator)
    {
        theGenerator = (typename P::data_type) aGenerator;
    }
    void setPreset(uint64_t aPreset)
    {
        thePreset = (typename P::data_type) aPreset;
    }
    void setInvert(uint64_t anInvert)
    {
        theInvert = (typename P::data_type) anInvert;
    }
private:
    P theGenerator;
    typename P::data_type thePreset;
    typename P::data_type theInvert;
};

#endif /* CRCFACTORY_H_ */
