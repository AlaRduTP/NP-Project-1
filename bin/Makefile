CXXFLAGS := -std=c++11 -O3
BINS := noop number removetag removetag0
TOOLS := cat ls date

all: $(BINS) $(TOOLS)

$(TOOLS): %:
	cp `which $@` $@

clean:
	rm -rf $(BINS) $(TOOLS)
