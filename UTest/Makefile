CXXFLAGS=-I .. -I /usr/local/include -g 

all:	UTest

UTest:	UTest.o CRCTest.o
	$(CXX) -o UTest $^ -L .

clean:
	rm -f UTest *.o
