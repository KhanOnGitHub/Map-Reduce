# Define (atleast) the following targets: all, clean
# all must build the executable file named mapreduce.
# clean must remove all object files and executable files created.
# Look at the introduction doc for information on how make a Makefile.
# This make file must build according to the structure described in the
# introduction doc, if it doesn't you will get a ZERO!

CC = gcc
CFLAGS = -Wall -Werror -g -Iinclude
BINS = bin/mapreduce
BINDIR = bin/
BUILDDIR = build/


all: $(BUILDDIR)  $(BINDIR) $(BINS)

$(BINS): $(BUILDDIR)main.o $(BUILDDIR)map_reduce.o
	gcc $(CFLAGS) $^ -o $@

$(BUILDDIR)main.o: main.c
	gcc $(CFLAGS) -c $^ -o $@

$(BUILDDIR)map_reduce.o: map_reduce.c
	gcc $(CFLAGS) -c $^ -o $@

$(BUILDDIR):
	@mkdir -p $@

$(BINDIR):
	@mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(BUILDDIR) $(BINDIR)







