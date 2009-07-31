/*
 * $Id$
 * $Date$
 * 
 * Welcome to CRC++
 * 
 * Copyright (c) 2003-2008 INTEC International GmbH, Hechingen, Germany
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
/**
 * @class CRCStream
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
	* @param algorithm	The CRC algorithm to use
	*
	*/
	CRCStream(CRC<P> const & algorithm)
	  : _algorithm (algorithm),
		_preset(~0),
		_invert(~0)
	{
		reset();
	}

   /**
	* Constructor which allows to specify preset and invert.
	* @param algorithm	The CRC algorithm to use
	* @param preset		The preset value
	* @param invert		The inversion value for the returned CRC
	*/
	CRCStream(CRC<P> const & algorithm, typename P::data_type preset, typename P::data_type invert)
	  : _algorithm (algorithm),
		_preset(preset),
		_invert(invert)
	{
		reset();
	}



   /**
	* Insertion operator.for C style strings
	* @param data a null terminated character string to add to CRC calculation
	*/
	CRCStream<P> & operator << (char const *data)
	{
		while (*data)
			_algorithm.add (*data++, _crc );
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
	template <class D> CRCStream<P> & operator << (D const &data)
	{
		typename D::const_iterator it;
		for (it = data.begin(); it != data.end(); it++)
			_algorithm.add (*it, _crc );
		return *this;
	}

   /**
	* Insertion operator.for data in polynomial representation
	* @param data a P to be included in the calculation
	*
	* This is mainly useful for test, in order to check whether the calculated CRC is OK.
	*/
	CRCStream<P> & operator << (P const &data)
	{
		_algorithm.add (data, _crc );
		return *this;
	}

   /**
	* Return calculated CRC
	* @return result of computation in a byte order suitable for insertion into the output stream
	*/
	std::string result() const
	{
		char rCRC[sizeof(typename P::data_type)];
		P nCRC = CRC();

		for (int i=0; i<sizeof(typename P::data_type);i++)
		{
			rCRC[i] = nCRC.hibyte();
			nCRC = nCRC.shift(8);
		}
		return std::string (rCRC,sizeof(rCRC));
	}


   /**
	* Return calculated CRC
	* @return result of computation in raw format
	*/

	P CRC() const
	{
		return  _crc ^ _invert;
	}

   /**
	* Returns whether CRC checking has been successful
	* @retval true CRC is valid
	* @retval false CRC is invalid
	*/
	bool good()
	{
		P goodcrc = 0;
		_algorithm.add(_preset,goodcrc);
		return _crc == goodcrc;
	};

	/// Reset the stream for reuse in another calculation
	void reset()
	{
		_crc = _preset;
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

	std::string gen (char const *data)
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

	std::string gen (char const *data, unsigned int len)
	{
		return gen(std::string(data,len));
	}

   /**
	* Generate CRC for data in STL collections
	* @param data a std::string or similar collection to add to CRC calculation
	*
	* @see operator << (D const &data)
	*/

	template <class D> std::string gen (D const &data)
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

	bool check (char const *data, unsigned int len)
	{
		return check(std::string(data,len));
	}

   /**
	* Check CRC for data in STL collections
	* @param data a std::string or similar collection to add to CRC calculation
	*
	* @see operator << (D const &data)
	*/

	template <class D> bool check (D const &data)
	{
		reset();
		*this << data;
		return good();
	}



private:
	CRCalgorithm  	_algorithm;
	P				_crc;
	P const			_preset;
	P const			_invert;

};
#endif // !defined(EA_814D913B_5D86_45b9_B814_280230E73242__INCLUDED_)
