CXXFLAGS=-I .. -I /usr/local/include -g 

all:	UTest

UTest:	UTest.o CRCTest.o
	$(CXX) -o UTest $^ -L . -L /usr/local/lib -l cppunit 

clean:
	rm -f UTest *.o
