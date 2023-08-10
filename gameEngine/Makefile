rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
src := $(call rwildcard,./,*.cpp)

obj = $(patsubst %.cpp,%.o,$(src))

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -pthread -lzmq

UBUNTU_INTEL_INCLUDEDIR=/usr/include
UBUNTU_INTEL_LIBPATH=/usr/lib/x86_64-linux-gnu
UBUNTU_LIB=$(UBUNTU_INTEL_LIBPATH)
UBUNTU_COMPILER=/usr/bin/g++

all: server client

server: $(filter-out ./client.o, $(obj))
	$(UBUNTU_COMPILER) -o $@ $^ $(LDFLAGS) -L$(UBUNTU_LIB)

client: $(filter-out ./server.o, $(obj))
	$(UBUNTU_COMPILER) -o $@ $^ $(LDFLAGS) -L$(UBUNTU_LIB)

%.o: %.cpp
	$(UBUNTU_COMPILER) -std=c++11 -c $^ -o $@ -I$(UBUNTU_INTEL_INCLUDEDIR)

clean:
	rm -f $(obj) main