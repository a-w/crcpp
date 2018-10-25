#pragma once
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

/**
 * @file crcstream.h
 * @brief Contains the supporting class CRCStream
 */

#include "crc.h"

#if !defined(__cpp_range_based_for)
#if __cplusplus >= 201103L
// If the complier is c++11 compliant, it has range based for
#  define __cpp_range_based_for  200907L

// Visual Studio >= 2012 support range based for
// recommended by https://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations
#elif defined(_MSC_VER) && _MSC_VER >= 1700
#  define __cpp_range_based_for 200907L
#endif
#endif

namespace CrcPP
{
    /**
     * @ingroup CRCpp
     * @brief Holds a CRC result in a byte order suitable for insertion into an output stream
     */
    template <class P> class CRCResult
    {
    public:
        /**
         * Construct a CRC result holder
         * @param	nCRC the CRC value in polynomial representation
         */
        explicit CRCResult(P nCRC)
        {
            for (size_t i = 0; i < sizeof(typename P::data_type); ++i)
            {
                rCRC[i] = nCRC.hibyte();
                nCRC = nCRC.shift(8);
            }
        }

        /**
         * Retrieves a pointer to the CRC data.
         * Mimics std::basic_string<uint8_t>::c_str()
         * @return	A pointer to the first byte to be written to the output stream
         */
        uint8_t const* c_str() const
        {
            return rCRC;
        }

        /**
         * Convenience function to retrieve the number of bytes in the CRC
         * @return	The number of bytes in the CRC
         */
        static size_t size()
        {
            return sizeof(typename P::data_type);
        }

    private:
        uint8_t rCRC[sizeof(typename P::data_type)];
    };

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
         * @param data a single byte (unsigned character) to add to CRC calculation
         */
        CRCStream<P>& operator << (uint8_t const data)
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
         * Insertion operator.for data in collections
         * @param data a collection of bytes to add to CRC calculation
         *
         * The main requirement for the collection given in the data parameter is that it
         * has an iterator and that the collection contents are of a data type supported
         * by the CRC class.
         */
        template <class D> CRCStream<P>& operator << (D const& data)
        {
#ifdef  	__cpp_range_based_for

            for (auto const& byte : data)
            {
                _algorithm.add(byte, _crc);
            }

#else
            typename D::const_iterator it;

            for (it = data.begin(); it != data.end(); ++it)
            {
                _algorithm.add(*it, _crc);
            }

#endif

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
         *	Add a single bit
         *	@param bit The bit to add. Only the lowest bit is used
         */
        CRCStream<P>& addBit(uint8_t bit)
        {
            _algorithm.addbit(bit, _crc);
            return *this;
        }

        /**
         * Return calculated CRC
         * @return result of computation in a byte order suitable for insertion into the output stream
         */
        CRCResult<P> result() const
        {
            return CRCResult<P> (crc());
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

        /// Get the preset value
        P preset() const
        {
            return _preset;
        }

        /// Get the invert (xor) value
        P invert() const
        {
            return _invert;
        }


        /**
         * Generate CRC for character data
         * @param data a character string to add to CRC calculation. May contain embedded null characters
         * @param len the length of the string
         *
         * @see gen (D const &data)
         */
        template<typename D> CRCResult<P> gen(D const* data, unsigned int len)
        {
            process<D>(data, len);
            return result();
        }

        /**
         * Generate CRC for data in collections
         * @param data a collection of bytes to add to CRC calculation
         *
         * @see operator << (D const &data)
         */
        template <class D> CRCResult<P> gen(D const& data)
        {
            process<D>(data);
            return result();
        }

        /**
         * Check CRC for byte or character data
         * @param data a sequence of bytes to add to CRC calculation. May contain embedded null characters
         * @param len the length of the sequence
         *
         * @see check (D const &data)
         *
         */
        template<typename D> bool check(D const* data, unsigned int len)
        {
            process<D>(data, len);
            return good();
        }

        /**
         * Check CRC for data in collections
         * @param data a collection of bytes to add to CRC calculation
         *
         * @see operator << (D const &data)
         * @note For check() of a byte or character sequence always use the overload with length parameter, because the CRC value can contain null characters
         */
        template <class D> bool check(D const& data)
        {
            process<D> (data);
            return good();
        }

        /**
         * process a byte sequence
         * @param	data A pointer to the first byte of the sequence
         * @param	len The length of the sequence
         */
        template <typename D> void process(D const* data, unsigned int len)
        {
            reset();

            for (unsigned int i = 0; i < len; ++i)
            {
                *this << data[i];
            }
        }

        /**
         * process a collection
         * @param	data any iterable collection
         */
        template <class D> void process(D const& data)
        {
            reset();
            *this << data;
        }

    private:
        CRCalgorithm    _algorithm;
        P               _crc;
        P const         _preset;
        P const         _invert;

    };
}
