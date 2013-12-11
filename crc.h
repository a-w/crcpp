/*
 * crc.h
 *
 * This file is part of CRC++
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

#if !defined(EA_B99886C8_0733_4705_BD37_67537E5E1BD1__INCLUDED_)
#define EA_B99886C8_0733_4705_BD37_67537E5E1BD1__INCLUDED_

#include <stdexcept>

/**
 * @file crc.h
 * @brief Contains the template classes which make up CRC++
 */

#if defined (WIN32)
#  include <basetsd.h>
#  define uint64_t UINT64
#  define uint32_t UINT32
#  define uint16_t UINT16
#  define uint8_t  UINT8
#else
#  include <stdint.h>   // uintxx_t types
#endif
/**
* A polynomial in network order
* @ingroup CRCpp
*/

template <class T, unsigned int bitsize = sizeof(T) * 8> class PolyN
{
public:
    typedef T   data_type;
    static unsigned int const numbits = bitsize;
    PolyN(T v = 0) : value(v) {}

    inline operator T() const
    {
        return value;
    }
    inline unsigned char hibit() const
    {
        return (value & 1) != 0;
    }
    inline unsigned char hibyte() const
    {
        return (unsigned char)(value & 0xFF);
    }
    inline unsigned char lobit() const
    {
        return (value & ((T)1 << (bitsize - 1))) != 0;
    }
    inline void sethibyte(unsigned char data)
    {
        value = data ;
    }
    inline T shift(int n) const
    {
        return value >> n;
    }

private:
    T value;
};

/**
 * A polynomial in "natural" order
 * @ingroup CRCpp
 */

template <class T, unsigned int bitsize = sizeof(T) * 8> class Poly
{
public:
    typedef T   data_type;
    static unsigned int const numbits = bitsize;
    Poly(T v = 0) : value(v) {}

    inline operator T() const
    {
        return value;
    }
    inline unsigned char hibit() const
    {
        return (value & ((T)1 << (bitsize - 1))) != 0;
    }
    inline unsigned char hibyte() const
    {
        return value >> (bitsize - 8);
    }
    inline unsigned char lobit() const
    {
        return (value & 1) != 0;
    }
    inline void sethibyte(unsigned char data)
    {
        // Cast to T to avoid implicit promotion to (signed) integer
        // See http://en.cppreference.com/w/cpp/language/implicit_cast
        value = (T)((T) data << (bitsize - 8));     // Needed for table generation
    }
    inline T shift(int n) const
    {
        return (T)(value << n);
    }

private:
    T value;
};

typedef PolyN<uint64_t> Poly64N;
typedef PolyN<uint32_t> Poly32N;
typedef PolyN<uint16_t> Poly16N;
typedef PolyN<uint8_t> Poly8N;
typedef Poly<uint64_t> Poly64;
typedef Poly<uint32_t> Poly32;
typedef Poly<uint16_t> Poly16;
typedef Poly<uint8_t> Poly8;

/**
 * The CRC implementation
 * @ingroup CRCpp
 */

template <class P> class CRC
{
public:
    typedef P   poly_type;
    /**
    * Constructor.
    * @param generator   The generator polynomial
    */
    CRC(P const generator)
        : _generator(generator)
    {
        if (!generator.lobit())
        {
            throw std::logic_error("Coefficient X^0 of the generator Polynomial must be 1");
        }

        for (unsigned int index = 0; index < 256; index++)
        {
            P crc;
            crc.sethibyte((unsigned char) index);

            for (int bit = 0; bit < 8; bit++)
            {
                crc = crc.hibit() ? (crc.shift(1) ^ generator) : crc.shift(1);
            }

            _table[ index ] = crc;
        }

    }

    /**
    * Add a single byte to the calculation.
    * @param data    the data byte to add
    * @param reg the working register
    */
    void add(unsigned char data, P& reg) const
    {
        reg = reg.shift(8) ^ _table[reg.hibyte() ^ data];
    }

    /**
    * Add n bits to the calculation.
    * @param data    the data to add
    * @param reg the working register
    */
    void add(P const& data, P& reg) const
    {
        P work = data;

        unsigned int i = P::numbits;

        for (; i > 7; i -= 8)
        {
            add(work.hibyte(), reg);
            work = work.shift(8);
        }

        for (; i > 0; --i)
        {
            addbit(work.hibit(), reg);
            work = work.shift(1);
        }
    }

    void addbit(unsigned char bit, P& reg) const
    {
        if (bit ^ reg.hibit())
        {
            reg = reg.shift(1) ^ _generator;
        }
        else
        {
            reg = reg.shift(1);
        }
    }

    P generator() const
    {
        return _generator;
    }
protected:
    P _generator;
    P _table[256];
};
#endif // !defined(EA_B99886C8_0733_4705_BD37_67537E5E1BD1__INCLUDED_)
