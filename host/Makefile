CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf


OBJS = main.o

CFLAGS += -Wall -I../lib/3rdparty/include -I../ta/include -I$(TEEC_EXPORT)/include -I./include
#Add/link other required libraries here
LDADD += -lteec -L$(TEEC_EXPORT)/lib

SDFERRY_DIR = ../lib/sdferry
include $(SDFERRY_DIR)/config.mk
OBJS += $(SDFERRY_HOST_OBJS)
CFLAGS += $(SDFERRY_HOST_CFLAGS)

BINARY = sdferry_test

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDADD)

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINARY)
