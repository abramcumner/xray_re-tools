CFLAGS=-Wall -Os -mno-cygwin
LDFLAGS=-s
CC=gcc

TARGETS:=ttuner
EXECUTABLES:=$(addsuffix .exe, $(TARGETS))

all: $(TARGETS)

clean:
	@rm -f $(EXECUTABLES) $(TARGETS)
