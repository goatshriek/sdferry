global-incdirs-y += include
global-incdirs-y += ../lib/3rdparty/include
srcs-y += sdferry_test_ta.c

SDFERRY_DIR = ../lib/sdferry
include $(SDFERRY_DIR)/config.mk
srcs-y += $(SDFERRY_TA_SOURCES)
global-incdirs-y += $(SDFERRY_INCLUDE_DIR)

# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes
