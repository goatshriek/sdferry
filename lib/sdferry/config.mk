# To use SDFerry in your application, you will need to do two things:
#  (1) define the SDFERRY_DIR variable in your makefile first
#  (2) include this config.mk file in your makefile
#
# After these two things have been done, you can use the variables provided here
# to add the appropriate sources and options to your application.

SDFERRY_INCLUDE_DIR = $(SDFERRY_DIR)/include

SDFERRY_HOST_OBJS = $(SDFERRY_DIR)/src/host/call.o $(SDFERRY_DIR)/src/host/memory.o $(SDFERRY_DIR)/src/basic.o
SDFERRY_HOST_CFLAGS = -I$(SDFERRY_INCLUDE_DIR)

SDFERRY_TA_SOURCES = $(SDFERRY_DIR)/src/basic.c $(SDFERRY_DIR)/src/ta/memory.c
