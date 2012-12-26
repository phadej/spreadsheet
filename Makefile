# Clang
CXX=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++ -g

# G++
#CXX=g++
#CXXFLAGS=-std=c++0x -g -Wall -pedantic

PARTS := cellindex functions parser ast spreadsheet
TESTS := first second circular

.PHONY : all clean tests

SOURCES := $(PARTS:%=%.cc)
OBJECTS := $(PARTS:%=%.cc.o)
HEADERS := exceptions.hh definitions.hh table.hh $(PARTS:%=%.hh)

TEST_EXECUTABLES := $(TESTS:%=tests/%.test)
TEST_OUTPUTS     := $(TESTS:%=tests/%.output.txt)

all : tests

tests : $(TEST_OUTPUTS)

tests/%.test : tests/%.test.cc $(OBJECTS)
	$(CXX) $(CXXFLAGS) -I. -o $@ $^

.SECONDARY : $(OBJECTS) $(TEST_EXECUTABLES) $(TEST_OUTPUTS)

%.cc.o : %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

tests/%.output.txt : tests/%.test tests/%.expected.txt
	(valgrind $^ || ./$^) | tee $@
	diff -u $(@:output.txt=expected.txt) $@

clean :
	rm -f $(OBJECTS)
	rm -f $(TEST_OUTPUTS)
	rm -f $(TEST_EXECUTABLES)
	rm -rf *.dSYM tests/*.dSYM
	rm -rf doc
