CC  = gcc
CXX = g++
GCCVER := $(shell $(CC) -dumpversion | awk -F. '{ print $$1"."$$2}' )

CFLAGS += -g -O0 -Wall -fPIC -lseccomp # libseccomp是一个用于 Linux 系统的 syscall 过滤器库
CXXFLAGS = -std=c++2a -Wall -g -pipe -rdynamic -fno-strict-aliasing -Wno-unused-function -Wno-sign-compare -fpermissive -Wno-invalid-offsetof
CXXFLAGS += $(CFLAGS)

INC += -I./core -I./core/rules
INC	+= -I./common -I./common/json -I./judgeclient -I./wrapper
SRCS += $(wildcard ./core/*.c) $(wildcard ./core/rules/*.c)
SRCS += $(wildcard ./common/*.cpp) $(wildcard ./judgeclient/*.cpp) $(wildcard ./wrapper/*.cpp)

CPPOBJ = $(patsubst %.cpp,%.o,$(SRCS))
COBJ += $(patsubst %.c,%.o,$(SRCS))

OBJ = $(CPPOBJ) $(COBJ)

COMPILE_LIB_HOME = .
DYNAMIC_NAME = libjudger.so
STATIC_NAME = libjudger.a
DYNAMIC_LIB	= $(COMPILE_LIB_HOME)/$(DYNAMIC_NAME)
STATIC_LIB = $(COMPILE_LIB_HOME)/$(STATIC_NAME)

all: $(DYNAMIC_LIB) $(STATIC_LIB)

$(DYNAMIC_LIB): $(OBJ)
	$(CXX) -pg -o $@ $^ $(CXXFLAGS)

$(STATIC_LIB): $(OBJ)
	@ar cr $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -pg -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -pg -o $@ $<

clean:
	rm -rf $(OBJ) $(DYNAMIC_LIB) $(STATIC_LIB) $(DYNAMIC_NAME) $(STATIC_NAME)
