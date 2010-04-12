/*
 * $Id$
 * $Date$
 * 
 * UTest.cpp: Unit test main program.
 * 
 * Copyright (c) 2003-2010 INTEC International GmbH, Hechingen, Germany
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
 */


#include <string.h>
#include <stdlib.h> // system()

#include <iostream>
#include <fstream>

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_EH
#define _CXXTEST_ABORT_TEST_ON_FAIL


#include <cxxtest/ErrorPrinter.h>


int main()
{
	return CxxTest::ErrorPrinter().run();
}


#include <cxxtest/Root.cpp>
