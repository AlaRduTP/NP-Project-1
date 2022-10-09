CXXFLAGS := -std=c99 -O3

all:
	echo

tools: bin
	cd bin && $(MAKE)

tools-clean: bin
	cd bin && $(MAKE) clean
