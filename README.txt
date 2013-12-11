Welcome to CRC++

Copyright (c) 2012 Adrian Weiler, ALDEA Software und Systeme GmbH, Tuebingen, Germany

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


What is it?
===========

CRC++ is a new reference implementation for the CRC algorithm.
Its purpose is to complement Ross Williams famous reference implementation
published in "A Painless Guide to CRC Error Detection Algorithms" 
(see http://www.ross.net/crc/) with a modern, template based version written in C++.

For any questions, please consult the "Never Asked Questions" list at
http://www.aweiler.com/crcnaq.html


The reference values for the CRC-CCITT unit tests have been taken from the
ITU-T recommendation X.25. Those for the CRC32 unit test have been taken from
a frame captured from Ethernet.

How to use it?
==============

CRC++ consists of a single file: crc.h.
To use it, #include "crc.h". An example is provided in the unit test, in the
directory UTest. For instructions, consult the "Quick Start" section below.

You will need a recent C++ compiler which knows about templates.

If you wish to compile the unit tests yourself, you will need the
CxxUnit framework, available at http://sourceforge.net/projects/cxxunit/.

A class reference manual for CRC++ is available in the "doc" directory. This
manual has been generated from the source code with doxygen. To regenerate it,
you will need doxygen, available at http://www.doxygen.org/


Quick Start
===========

First choose a polynomial and a data representation. There are two possible data
representations: one which I call "native", where the meaning of a bit with the
binary value 2^n is the coefficient of X^n. The other is called "network" order.
This applies to network applications, where data is transmitted serially, bit by bit,
over a wire or other media. The meaning of a bit depends on the transmission order,
which is usually least significant bit (LSB) to most significant bit (MSB).
Therefore, in "network" bit order, the LSB bears the highest polynomial coefficient
and the MSB bears the lowest coefficient.

CRC++ provides a template class for each of these data representations:
Poly<> for native order, and PolyN<> for network order. The template argument is the
data type used to carry the binary representation of the polynomial. A number of
typedefs are given in crc.h. These are named PolyXX[N], where XX is the bit size: 8, 16,
or 32. The optional "N" suffix denotes network order. For a CRC-CCITT use in serial
communications, you would therefore use Poly16N.

The CRC algorithm is implemented in the CRC<> class. Its template argument is the
polynomial type, for example CRC<Poly16N>. The constructor takes the generator polynomial
as argument.

CAUTION: Be aware that a polynomial is a mathematical object, and that its computer
representation is  given in one of the two orders above. For example, the CCITT-16
polynomial is X^16 + X^12 + X^2 + 1, which translates to 0x1021 in native order, and
to 0x8408 in network order. To be compatible with serial communications using the
CCITT polynomial, you would therefore have to instantiate your CRC object like this:
  CRC<Poly16N> objCRC_CCITT (0x8408);

In a native order application using the same polynomial, you would use:
  CRC<Poly16> objMyCrc (0x1021);

Most CRC usages include some folkloristic additions, like providing a preset value
and/or inverting the computed CRC before transmission. These additions are implemented
in then CRCStream<> class. The template argument is the same as for CRC<>.

CRCStream maintains the current result of a CRC calculation. Note that the result
is a polynomial, which is _not_ a numeric data type. Therefore, there is no method
to retrieve the result as an integer. The result can be retrieved as polynomial, or
as std::string, in a byte order suitable for inserting into an output stream.
Depending on preset and inversion, different final values are considered to be a
"good" CRC. CRCStream provides support for this with the good() method.

CRCStream provides the insertion operator "<<", which can be used to add single
data bytes, or any STL collection whose elements can be converted to unsigned char.
For examples, see CRCTest.cpp in directory UTest.


Restrictions
============

CRC++ provides a table driven implementation and therefore cannot process
single bits. For simplicity, the degrees of the CRC polynomials that can be
implemented using CRC++ are restricted to the bit sizes of available C++
data types. These are 8, 16, 32 and 64.
