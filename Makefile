CXXFLAGS=-std=c++14 -Ofast

CXXFILES=$(wildcard *.cc)
CXXOBJS=$(patsubst %.cc, %.o, ${CXXFILES})

TARGET=pbxparser
LIB=lib$(TARGET).a

all: $(LIB)

$(LIB): $(CXXOBJS)
	ar rcs $@ $(CXXOBJS)

clean:
	$(RM) $(CXXOBJS) $(LIB)