/*
 * $Id$
 * $Date$
 *
 * This file is part of CRC++
 *
 * Copyright (c) 2003-2009 INTEC International GmbH, Hechingen, Germany
 * Author: Adrian Weiler
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Please be aware that the GNU GPL does not permit linking with non-free
 * modules. If you wish to include CRC++ into non-free software, you are
 * required to obtain a commercial license from INTEC International GmbH.
 * For details, see
 *   http://www.gnu.org/licenses/gpl-faq.html#GPLInProprietarySystem
 * Our mail address for all enquiries concerning CRC++ is
 *   crcpp@intec-international.com
 *
 */

#if !defined(EA_B99886C8_0733_4705_BD37_67537E5E1BD1__INCLUDED_)
#define EA_B99886C8_0733_4705_BD37_67537E5E1BD1__INCLUDED_

/**
 * @file crc.h
 * @brief Contains the template classes which make up CRC++
 */

#include <stdint.h>	// uintxx_t types

 /**
 * A polynomial in network order
 * @ingroup CRCpp
 */

template <class T, unsigned int bitsize = sizeof(T) * 8> class PolyN
{
public:
	typedef T	data_type;
	static unsigned int const numbits = bitsize;
	PolyN(T v=0) : value(v) {}

	inline operator T() const { return value; }
	inline bool          hibit  () const { return (value&1) != 0; }
	inline unsigned char hibyte () const { return (unsigned char) (value & 0xFF); }
	inline void sethibyte (unsigned char data) { value = data ; }
	inline T shift (int n) const  { return value >> n; }

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
	typedef T	data_type;
	static unsigned int const numbits = bitsize;
	Poly(T v=0) : value(v) {}

	inline operator T() const { return value; }
	inline bool          hibit  () const { return (value & (1 << (bitsize-1))) != 0; }
	inline unsigned char hibyte () const { return value >> (bitsize-8); }
	inline void sethibyte (unsigned char data) { value = data << (bitsize-8); } // Needed for table generation
	inline T shift (int n) const  { return value << n; }

private:
	T value;
};


typedef PolyN<uint32_t> Poly32N;
typedef PolyN<uint16_t> Poly16N;
typedef PolyN<uint8_t> Poly8N;
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
	typedef P	poly_type;
   /**
	* Constructor.
	* @param generator	The generator polynomial
	*/
	CRC (P const generator)
	  : _generator (generator)
	{
		for(unsigned int index = 0; index < 256; index++)
		{
			P crc;
			crc.sethibyte(index);

			for(int bit = 0; bit < 8; bit++)
				crc = crc.hibit() ?	(crc.shift(1) ^ generator) : crc.shift(1);

			_table[ index ] = crc;
		}

	};

   /**
	* Add a single byte to the calculation.
	* @param data	the data byte to add
	* @param reg	the working register
	*/
	void add(unsigned char data, P & reg) const
	{
		reg = reg.shift(8) ^ _table[reg.hibyte() ^ data];
	}

   /**
	* Add n bits to the calculation.
	* @param data	the data to add
	* @param reg	the working register
	*/
	void add(P const& data, P & reg) const
	{
		P work = data;

		unsigned int i=P::numbits;
		for (; i > 7; i -= 8)
		{
			add (work.hibyte(), reg);
			work = work.shift(8);
		}
		for (; i > 0; --i)
		{
			addbit (work.hibit(), reg);
			work = work.shift(1);
		}
	}

	void addbit (unsigned char bit, P& reg) const
	{
		if (bit ^reg.hibit())
			reg = reg.shift(1) ^ _generator;
		else
			reg = reg.shift(1);
	}

protected:
	P _generator;
	P _table[256];
};
#endif // !defined(EA_B99886C8_0733_4705_BD37_67537E5E1BD1__INCLUDED_)
