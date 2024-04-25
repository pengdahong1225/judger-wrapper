CXX = g++
GCCVER := $(shell $(CC) -dumpversion | awk -F. '{ print $$1"."$$2}' )

CXXFLAGS = -std=c++2a -Wall -g -pipe -rdynamic -fno-strict-aliasing -Wno-unused-function -Wno-sign-compare -fpermissive -Wno-invalid-offsetof

LIB := -L./core -lcore

INC	+= -I./common -I./common/json -I./judgeclient -I./wrapper
CSRC += $(wildcard ./common/*.cpp) $(wildcard ./judgeclient/*.cpp) $(wildcard ./wrapper/*.cpp)

OBJ = $(patsubst %.cpp,%.o,$(CSRC))

COMPILE_LIB_HOME = .
DYNAMIC_NAME = libjudger.so
STATIC_NAME = libjudger.a
DYNAMIC_LIB	= $(COMPILE_LIB_HOME)/$(DYNAMIC_NAME)
STATIC_LIB = $(COMPILE_LIB_HOME)/$(STATIC_NAME)

all: $(DYNAMIC_LIB) $(STATIC_LIB)

$(DYNAMIC_LIB): $(OBJ)
	$(CXX) -pg -o $@ $^ $(LIB) $(CXXFLAGS)

$(STATIC_LIB): $(OBJ)
	@ar cr $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -pg -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -pg -o $@ $<

clean:
	rm -rf $(OBJ) $(DYNAMIC_LIB) $(STATIC_LIB) $(DYNAMIC_NAME) $(STATIC_NAME)
