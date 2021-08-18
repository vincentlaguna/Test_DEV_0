# We try to detect the OS we are running on, and adjust commands as needed
# *This Makefile has the provision to incorporate the Unity testing library

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

# Provision to provide Server Address at build step
#SRV_IP=127.0.0.1 #192.168.128.131

#Path Definitions
# PATHU = ../unity/
PATHS = src/
# PATHT = test/
PATHI = inc/
PATHB = build/

# determine our source files
# SRCU = $(PATHU)unity.c
SRCS = $(wildcard $(PATHS)*.c)
#SRCT = $(wildcard $(PATHT)*.c)
#SRC = $(SRCU) $(SRCS) $(SRCT)
SRC = $(SRCS)

# Files We Are To Work With
# OBJU = $(patsubst $(PATHU)%.c,$(PATHB)%.o,$(SRCU))
OBJS = $(patsubst $(PATHS)%.c,$(PATHB)%.o,$(SRCS))
# OBJT = $(patsubst $(PATHT)%.c,$(PATHB)%.o,$(SRCT))
# OBJ = $(OBJU) $(OBJS) $(OBJT)
OBJ = $(OBJS)

# Other files we care about
# DEP = $(PATHU)unity.h $(PATHU)unity_internals.h
TGT = $(PATHB)$(TARGET_EXTENSION) #$(SRV_IP)

# Tool Definitions
CC=gcc
# CFLAGS=-I. -I$(PATHU) -I$(PATHI) -I$(PATHS) -DTEST
CFLAGS=-I. -I$(PATHI) -I$(PATHS)

test: $(PATHB) $(TGT)
	./$(TGT)

# $(PATHB)%.o:: $(PATHS)%.c $(DEP)
$(PATHB)%.o:: $(PATHS)%.c
	$(CC) -c $(CFLAGS) $< -o $@

# $(PATHB)%.o:: $(PATHT)%.c $(DEP)
#	$(CC) -c $(CFLAGS) $< -o $@

# $(PATHB)%.o:: $(PATHU)%.c $(DEP)
#	$(CC) -c $(CFLAGS) $< -o $@

$(TGT): $(OBJ)
	gcc -o $@ $^

clean:
	$(CLEANUP) $(PATHB)*.o
	$(CLEANUP) $(TGT)

$(PATHB):
	$(MKDIR) $(PATHB)

all: clean test

.PHONY: all
.PHONY: clean
.PHONY: test
