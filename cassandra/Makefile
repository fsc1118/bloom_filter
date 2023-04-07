CXX = g++
CXXFLAGS = -std=c++11 -O2

SRCDIR = .
OBJDIR = obj

SRCFILES = BitSet.cc BloomCalculator.cc BloomFilter.cc BloomFilterFactory.cc main.cc Murmurhash.cc util.cc
OBJFILES = $(patsubst %.cc,$(OBJDIR)/%.o,$(SRCFILES))

TARGET = bloom_filter

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
