/*
 * $Id$
 * $Date$
 * 
 * UTest.cpp: Unit test main program.
 * 
 * Copyright (c) 2003 INTEC International GmbH, Hechingen, Germany
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

#include "cppunit/TestSuite.h"
#include <cppunit/ui/text/TestRunner.h>  //CppUnit::TextUi::TestRunner
#include <cppunit/TextOutputter.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

int main(int argc, char* argv[])
{
	try { throw 1; } catch (int) {}		// QNX has problems with exceptions. Throwing an exception at the begin of the program usually helps.

	bool bTestResult;					// Result of tests
	bool bResultIsXml = false;			// Result is formatted as XML and needs postprocessing
	std::ostream* ostr = NULL;

	CppUnit::TextUi::TestRunner runner; // A test runner

	runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );

	// Select output method
	CppUnit::Outputter* outputter;

	if( argc > 1 && ::strcmp( "-x", argv[1] ) == 0 )
	{
		ostr = new std::ofstream( "lasttest.xml" );
		outputter = new CppUnit::XmlOutputter( &runner.result(), *ostr );
		bResultIsXml = true;
	}
	else
	{
		outputter = new CppUnit::TextOutputter( &runner.result(), std::cerr );
	}

	runner.setOutputter( outputter );

	// Run the test.
	// Parameters: 
	//   testName  Name of the test case to run. If an empty is given, then all added test are run. The name must be the name of of an added test.  
	//   doWait  if true then the user must press the RETURN key before the run() method exit.  
	//   doPrintResult  if true (default) then the test result are printed on the standard output.  
	//   doPrintProgress  if true (default) then TextTestProgressListener is used to show the progress.  

	bool bDoWait = false;
#ifdef WIN32
	bDoWait = !bResultIsXml;
#endif
	bTestResult = runner.run( "", bDoWait, true, false );

	if (bResultIsXml)
	{
		delete ostr;				// Close output stream
#ifdef WIN32
		system ("PostProcess.vbs");
#else
		system ("./PostProcess.sh");
#endif
	}

	// Return error code 1 if the one of test failed.
	return bTestResult ? 0 : 1;

}
