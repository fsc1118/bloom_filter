CC=g++ # the compiler to use
CFLAGS=-c -O2 # flags to pass to the compiler for compiling source files
LDFLAGS= # flags to pass to the linker for linking object files
SOURCES=BloomFilter.cc main.cc Murmurhash.cc # the source files
OBJECTS=$(SOURCES:.cc=.o) # the object files to build
EXECUTABLE=bloom_filter # the name of the executable to build

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
