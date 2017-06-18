#pragma once
/*
 * ICRCInfo.h
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

/**
 * Abstract description for the parameters of a CRC algorithms
 * @ingroup Util
 */
class ICRCInfo
{
public:
    virtual ~ICRCInfo() {}
    virtual bool isNative() const = 0;
    virtual unsigned int numBits() const = 0;
    virtual unsigned int numBytes() const = 0;
    virtual void writePoly(std::ostream& s) const = 0;
    virtual void writeTable(std::ostream& s) const = 0;

};
