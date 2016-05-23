CC = gcc
CXX = g++
CFLAGS = -Wall
CXXFLAGS= #-Wall $(DEBUG)
LIBS = -lc
OUTPUT = raw2tree
INCLUDE = -I/home/tcd_cream/include

# Root stuff
ROOTGLIBS = $(shell root-config --glibs)
ROOTCFLAGS = $(shell root-config --cflags)

build: $(OUTPUT)

raw2tree: raw2tree.cpp
	$(CXX) $(ROOTCFLAGS) -o raw2tree raw2tree.cpp $(ROOTGLIBS)

clean:
	rm -f *.o *.d core core.* $(OUTPUT)

all: $(OUTPUT)

% : %.c
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS) $(INCLUDE)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $<