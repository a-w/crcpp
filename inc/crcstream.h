/*
 * crcstream.h
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

#if !defined(EA_814D913B_5D86_45b9_B814_280230E73242__INCLUDED_)
#define EA_814D913B_5D86_45b9_B814_280230E73242__INCLUDED_

/**
 * @file crcstream.h
 * @brief Contains the supporting class CRCStream
 */

#include "crc.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include <string>
#endif

namespace CrcPP
{

    /**
     * @ingroup CRCpp
     * @brief Maintain CRC information for a data stream
     *
     * The CRCStream class is used to generate or check the CRC for a given stream of data.
     * This is merely a frontend for the CRC class, which does the algorithmic work.
     */

    template <class P> class CRCStream
    {
    public:
        typedef CRC<P> CRCalgorithm;

        /**
        * Constructor. Preset and inversion are set
        * @param algorithm   The CRC algorithm to use
        *
        */
        CRCStream(CRC<P> const& algorithm)
            : _algorithm(algorithm),
              _preset(~0),
              _invert(~0)
        {
            reset();
        }

        /**
        * Constructor which allows to specify preset and invert.
        * @param algorithm   The CRC algorithm to use
        * @param preset      The preset value
        * @param invert      The inversion value for the returned CRC
        */
        CRCStream(CRC<P> const& algorithm, typename P::data_type preset, typename P::data_type invert)
            : _algorithm(algorithm),
              _preset(preset),
              _invert(invert)
        {
            reset();
        }


        /**
        * Insertion operator for a single byte
        * @param data a character to add to CRC calculation
        */
        CRCStream<P>& operator << (char const data)
        {
            _algorithm.add(data, _crc);
            return *this;
        }

        /**
        * Insertion operator for a single byte
        * @param data a character to add to CRC calculation
        */
        CRCStream<P>& operator << (unsigned char const data)
        {
            _algorithm.add(data, _crc);
            return *this;
        }

        /**
        * Insertion operator.for C style strings
        * @param data a null terminated character string to add to CRC calculation
        */
        CRCStream<P>& operator << (char const* data)
        {
            while (*data)
            {
                _algorithm.add(*data++, _crc);
            }

            return *this;
        }

        /**
        * Insertion operator.for data in STL collections
        * @param data a std::string or similar collection to add to CRC calculation
        *
        * The main requirement for the collection given in the data parameter is that it
        * has an iterator and that the collection contents are of a data type supported
        * by the CRC class.
        */
        template <class D> CRCStream<P>& operator << (D const& data)
        {
            typename D::const_iterator it;

            for (it = data.begin(); it != data.end(); it++)
            {
                _algorithm.add(*it, _crc);
            }

            return *this;
        }

        /**
        * Insertion operator.for data in polynomial representation
        * @param data a P to be included in the calculation
        *
        * This is mainly useful for test, in order to check whether the calculated CRC is OK.
        */
        CRCStream<P>& operator << (P const& data)
        {
            _algorithm.add(data, _crc);
            return *this;
        }

        /**
        * Return calculated CRC
        * @return result of computation in a byte order suitable for insertion into the output stream
        */
        std::string result() const
        {
            char rCRC[sizeof(typename P::data_type)];
            P nCRC = crc();

            for (size_t i = 0; i < sizeof(typename P::data_type); ++i)
            {
                rCRC[i] = nCRC.hibyte();
                nCRC = nCRC.shift(8);
            }

            return std::string(rCRC, sizeof(rCRC));
        }


        /**
        * Return calculated CRC
        * @return result of computation in raw format
        */

        P crc() const
        {
            return  _crc ^ _invert;
        }

        /**
        * Returns whether CRC checking has been successful
        * @retval true CRC is valid
        * @retval false CRC is invalid
        */
        bool good() const
        {
            P goodcrc = 0;
            _algorithm.add(_invert, goodcrc);
            return _crc == goodcrc;
        }

        /**
        * Returns whether CRC checking has been successful.
        * Use if the CRC size is not a multiple of 8 bits, but multiples of 8 bits
        * have been used to calculate the CRC.
        * @param extrabits The number of additional bits that have been shifted into the CRC.
        *      For example if the CRC size is 15 bits, but you have fed two bytes into CRC, specify one bit.
        * @retval true CRC is valid
        * @retval false CRC is invalid
        */
        bool good(unsigned int extrabits)
        {
            P goodcrc = 0;
            _algorithm.add(_invert, goodcrc);

            for (unsigned int i = 0; i < extrabits; ++i)
            {
                _algorithm.addbit(0, goodcrc);
            }

            return _crc == goodcrc;
        }

        /// Reset the stream for reuse in another calculation
        void reset()
        {
            _crc = _preset;
        }

        // Get the preset value
        P preset() const
        {
            return _preset;
        }

        // Get the invert (xor) value
        P invert() const
        {
            return _invert;
        }

        /////////////////////////////////////////////////////
        // Other helper routines
        /////////////////////////////////////////////////////
        /**
        * Generate CRC for C string data
        * @param data a null terminated character string to add to CRC calculation
        *
        * @see gen (D const &data)
        */

        std::string gen(char const* data)
        {
            return gen(std::string(data));
        }

        /**
        * Generate CRC for character data
        * @param data a character string to add to CRC calculation. May contain embedded null characters
        * @param len the length of the string
        *
        * @see gen (D const &data)
        */

        std::string gen(char const* data, unsigned int len)
        {
            return gen(std::string(data, len));
        }

        /**
        * Generate CRC for data in STL collections
        * @param data a std::string or similar collection to add to CRC calculation
        *
        * @see operator << (D const &data)
        */

        template <class D> std::string gen(D const& data)
        {
            reset();
            *this << data;
            return result();
        }

        /**
        * Check CRC for character data
        * @param data a character string to add to CRC calculation. May contain embedded null characters
        * @param len the length of the string
        *
        * @see check (D const &data)
        *
        * @note For checking the length is always required, because the CRC value can contain null charaters
        */

        bool check(char const* data, unsigned int len)
        {
            return check(std::string(data, len));
        }

        /**
        * Check CRC for data in STL collections
        * @param data a std::string or similar collection to add to CRC calculation
        *
        * @see operator << (D const &data)
        */

        template <class D> bool check(D const& data)
        {
            reset();
            *this << data;
            return good();
        }



    private:
        CRCalgorithm    _algorithm;
        P               _crc;
        P const         _preset;
        P const         _invert;

    };
}
#endif // !defined(EA_814D913B_5D86_45b9_B814_280230E73242__INCLUDED_)
