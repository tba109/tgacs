####################################################################
# Tyler Anderson Tue Dec 13 13:37:31 EST 2011
####################################################################
CC      = g++
CFLAGS  = -Wall -g -Wno-write-strings
LDFLAGS = -lusb

###################################################################
# Directories
HDR = -I./gse/include -I./sfc

##################################################################
# Objects and build targets
GSE_STUFF = $(patsubst gse/src/%.cpp, gse/src/%, $(shell ls gse/src/*.cpp))
SFC_STUFF = $(patsubst sfc/%.cpp, sfc/%, $(shell ls sfc/*.cpp))
BUILD     = $(patsubst gse/util/%.cpp, gse/util/%, $(shell ls gse/util/*.cpp))
OBJS      = $(addsuffix .o, $(addprefix tmp/, $(notdir $(GSE_STUFF)))) $(addsuffix .o, $(addprefix tmp/, $(notdir $(SFC_STUFF))))

all : $(BUILD)

gse/util/% : $(GSE_STUFF) $(SFC_STUFF)
	$(CC) $(HDR) $(CFLAGS) $@.cpp $(OBJS) -o $(notdir $@).exe $(LDFLAGS)

gse/src/% : 
	$(CC) $(HDR) $(CFLAGS) -c $@.cpp -o tmp/$(notdir $@).o

sfc/%:
	$(CC) $(HDR) $(CFLAGS) -c $@.cpp -o tmp/$(notdir $@).o

clean :
	@rm -f tmp/*.o *.exe 

.PHONY : all clean