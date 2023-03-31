# General-Use Makefile 2023

ifeq ($(OSTYPE),cygwin)
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=.out
else ifeq ($(OS),Windows_NT)
	CLEANUP = del /F /Q
	MKDIR = mkdir
	TARGET_EXTENSION=.exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=.out
endif

#Path Definitions

PATHS = src/
PATHI = inc/
PATHB = build/


# determine our source files

SRCS = $(wildcard $(PATHS)*.c)
SRC = $(SRCS)


# Files We Are To Work With

OBJS = $(patsubst $(PATHS)%.c,$(PATHB)%.o,$(SRCS))
OBJ = $(OBJS)


# Other files we care about

TGT = $(PATHB)$(TARGET_EXTENSION) #$(SRV_IP)


# Tool Definitions

CC=gcc

CFLAGS=-I. -I$(PATHI) -I$(PATHS) -DTEST


test: $(PATHB) $(TGT)
	./$(TGT)

$(PATHB)%.o:: $(PATHS)%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TGT): $(OBJ)
	gcc -o $@ $^ 


clean:
	$(CLEANUP) $(PATHB)*.o
	$(CLEANUP) $(TGT)*.out

$(PATHB):
	$(MKDIR) $(PATHB)


all: clean test

.PHONY: all
.PHONY: clean
.PHONY: test
