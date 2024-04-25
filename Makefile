CC  = gcc
CXX = g++
GCCVER := $(shell $(CC) -dumpversion | awk -F. '{ print $$1"."$$2}' )

OPT = -pipe -fno-ident -fPIC -shared -z defs
CFLAGS = $(OPT) -g -Wall -Werror -O3 -std=c99 -lseccomp # libseccomp是一个用于 Linux 系统的 syscall 过滤器库
ifeq ($(MEMCHECK),1)
CFLAGS += -DMEMCHECK
endif

CXXFLAGS = -std=c++2a -Wall -g -pipe -rdynamic -fno-strict-aliasing -Wno-unused-function -Wno-sign-compare -fpermissive -Wno-invalid-offsetof
CXXFLAGS += $(CFLAGS)

INC += -I./core -I./core/rules
INC	+= -I./common -I./common/json -I./judgeclient -I./wrapper
SRCS += $(wildcard ./core/*.c) $(wildcard ./core/rules/*.c)
SRCS += $(wildcard ./common/*.cpp) $(wildcard ./judgeclient/*.cpp) $(wildcard ./wrapper/*.cpp)

COMPILE_LIB_HOME = .
DYNAMIC_NAME = libjudger.so
STATIC_NAME = libjudger.a
DYNAMIC_LIB	= $(COMPILE_LIB_HOME)/$(DYNAMIC_NAME)
STATIC_LIB = $(COMPILE_LIB_HOME)/$(STATIC_NAME)

all: $(DYNAMIC_LIB) $(STATIC_LIB)

$(DYNAMIC_LIB): $(SRCS:.c=.o)
	$(CC) -pg -o $@ $^ $(CXXFLAGS)

$(STATIC_LIB): $(SRCS:.c=.o)
	@ar cr $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -pg -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -pg -o $@ $<

clean:
	rm -f *.o .po *.so *.d $(SRCS:.c=.o) $(SRCS:.c=.d) $(DYNAMIC_LIB) $(STATIC_LIB) $(DYNAMIC_NAME) $(STATIC_NAME)
