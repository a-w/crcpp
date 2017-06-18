#pragma once
/*
 * ICRCFactory.h
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

#include <stdint.h>

// Forward
class ICRCAlgorithm;

/**
 * Abstract factory for creating CRC algorithms
 * @ingroup Util
 */
class ICRCFactory
{
public:
    virtual ~ICRCFactory() {}
    /**
     * Create the CRC algorithm with the given parameters
     */
    virtual ICRCAlgorithm* create() const = 0;

    /**
     * Gets the generator
     */
    virtual uint64_t generator() const = 0;

    /**
     * Gets the preset value
     */
    virtual uint64_t preset() const = 0;

    /**
     * Gets the invert value
     */
    virtual uint64_t invert() const = 0;
    /**
     * Set the generator polynomial
     * @param aGenerator The generator polynomial. Must be given in the bit order corresponding to the algorithm.
     */
    virtual void setGenerator(uint64_t aGenerator) = 0;
    /**
     * Set the initial value for CRC computation (value after reset())
     * @param aPreset the initial value
     */
    virtual void setPreset(uint64_t aPreset) = 0;
    /**
     * Sets the bits to invert before returning the computed CRC value
     * @param anInvert a bitmask of bits to invert (usually 0 for none, or ~0 for all)
     */
    virtual void setInvert(uint64_t anInvert) = 0;
};
