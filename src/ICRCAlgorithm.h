/*
 * ICRCAlgorithm.h
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

#ifndef ICRCALGORITHM_H_
#define ICRCALGORITHM_H_


/**
 * Abstract CRC Algorithm
 */
class ICRCAlgorithm
{
public:
    virtual ~ICRCAlgorithm() {}
    /**
     * Add a byte to the CRC calculation
     * @param b the byte to add
     */
    virtual void addByte(uint8_t b) = 0;

    /**
     * Reset CRC calculation.
     * The crc value is reset to the preset value
     */
    virtual void reset() = 0;
    /**
     * Determines whether the data fed into the algorithm has a good crc value.
     * To that end, _all_ bytes of the data stream must have been fed into the algorithm (with addByte())
     * _including_ the received CRC bytes.
     */
    virtual bool good() const = 0;
    /**
     * Gets the current result of CRC computation in a byte order suitable for adding to the output stream.
     */
    virtual std::string result() const = 0;
};


#endif /* ICRCALGORITHM_H_ */
