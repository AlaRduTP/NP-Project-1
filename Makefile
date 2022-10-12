# CFLAGS := -std=c99 -O3
CFLAGS := -g -O0 -Q
CINCLUDE := -I ./include

TARGETS := cmd main npshell parser pipe utils
OBJECTS = $(patsubst %,%.o,$(TARGETS))

all: npshell

npshell: $(OBJECTS)
	$(CC) $^ -o npshell

$(OBJECTS): %.o: ./src/%.c
	$(CC) -c $(CFLAGS) $(CINCLUDE) $< -o $@

tools: bin
	cd bin && $(MAKE)

tools-clean: bin
	cd bin && $(MAKE) clean

clean:
	rm -f *.o npshell
