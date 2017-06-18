#pragma once
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

namespace CrcPP
{
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

        /**
         * Allow to access the polynomial as its integer type.
         * @return the value of the polynomial
         */
        operator T() const
        {
            return value;
        }
        uint8_t hibit() const
        {
            return (value & 1) != 0;
        }
        uint8_t hibyte() const
        {
            return static_cast<uint8_t>(value & 0xFF);
        }
        uint8_t lobit() const
        {
            return (value & (static_cast<T>(1) << (bitsize - 1))) != 0;
        }
        void sethibyte(uint8_t data)
        {
            value = data ;
        }
        T shift(int n) const
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

        /**
         * Allow to access the polynomial as its integer type.
         * @return the value of the polynomial
         */
        operator T() const
        {
            return value;
        }
        uint8_t hibit() const
        {
            return (value & (static_cast<T>(1) << (bitsize - 1))) != 0;
        }
        uint8_t hibyte() const
        {
            return value >> (bitsize - 8);
        }
        uint8_t lobit() const
        {
            return (value & 1) != 0;
        }
        void sethibyte(uint8_t data)
        {
            // Cast to T to avoid implicit promotion to (signed) integer
            // See http://en.cppreference.com/w/cpp/language/implicit_cast
            value = static_cast<T>(static_cast<T>(data) << (bitsize - 8));      // Needed for table generation
        }
        T shift(int n) const
        {
            return static_cast<T>(value << n);
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
        CRC(P const generator) :
            _generator(generator)
        {
            if (!generator.lobit())
            {
                throw std::logic_error("Coefficient X^0 of the generator polynomial must be 1");
            }

            for (unsigned int index = 0; index < 256; index++)
            {
                P crc;
                crc.sethibyte(static_cast<uint8_t>(index));

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
        void add(uint8_t data, P& reg) const
        {
            reg = reg.shift(8) ^ _table[reg.hibyte() ^ data];
        }

        /**
         * Add bytes to the calculation.
         * @param data the data to add
         * @param len  the number of bytes to add
         * @param reg  the working register
         */
        void add(uint8_t const* data, unsigned int len, P& reg) const
        {
            while (len > 0)
            {
                add(*data++, reg);
                --len;
            }
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

        void addbit(uint8_t bit, P& reg) const
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

        P const* table() const
        {
            return _table;
        }
    protected:
        P _generator;
        P _table[256];
    };
}
