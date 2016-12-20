## the following disable builtin rules which we don't need
## and make debugging harder
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

# Use VERBOSE=1 to enable verbose make
TARGET	   = dump_bootloader
TYPE       = dev
LIBOPENCM3 = ../libopencm3/lib/libopencm3_stm32f1.a

CRC_OFFSET       := 8192
# for Devo X use -c X
DFU_ARGS   := -D 0x0483:0xdf11 -c 4 -b 0x08003000


###############################################
# Source Files                                #
###############################################
SRC_C    +=  $(wildcard *.c) $(wildcard target/*.c)
SRC_C    +=  ../target/common/filesystems/printf.c \
             ../target/common/filesystems/fgets.c \
             ../target/common/devo/uart.c \
             ../target/common/devo/power.c \
             ../target/common/stm32/clock.c \
             ../target/common/stm32/adc.c \
             ../target/common/stm32/spi_flash.c \
             ../target/common/stm32/printstr.c

###############################################
#This section defines binaries needed to build#
###############################################
    CROSS = arm-none-eabi-
    CC   = $(CROSS)gcc
    CXX  = $(CROSS)g++
    LD   = $(CROSS)ld
    AR   = $(CROSS)ar
    AS   = $(CROSS)as
    CP   = $(CROSS)objcopy
    DUMP = $(CROSS)objdump
    NM   = $(CROSS)nm
###############################################
#END SECTION                                  #
###############################################

############################################
#This section intermediate build files     #
############################################
ODIR	= ../objs/$(TARGET)
OBJS	= $(addprefix $(ODIR)/, $(notdir $(SRC_C:.c=.o) $(SRC_S:.s=.o)))

ifeq "$(wildcard ../../.hg )" ""
  HGVERSION := ${TARGET}-Unknown
else
  HGVERSION := $(shell ../../utils/get_version.pl ${TARGET})
endif

# Rebuild on hg version change.
# Based on http://mercurial.selenic.com/wiki/VersioningWithMake
# We need to always run this code, as opposed to running it from a prerequisite
# The HGTEST variable is not used
HGTEST := $(shell mkdir -p $(ODIR); \
	[ -f $(ODIR)/hgstamp ] || touch $(ODIR)/hgstamp; \
	echo $(HGVERSION) | cmp -s $(ODIR)/hgstamp - \
	|| echo $(HGVERSION) > $(ODIR)/hgstamp)
############################################
#END SECTION                               #
############################################

##################################################
#This section contains switches used for building#
##################################################
CFLAGS = -DSTM32F10X_HD -DSTM32F1 -mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd \
         -fdata-sections -ffunction-sections -fno-builtin-printf \
         -Wall -Wextra -Werror=undef -std=gnu99 --specs=nano.specs \
         -DHGVERSION="\"${HGVERSION}\"" \
         -I../libopencm3/include/ -Iinclude -I../target/common/devo

LFLAGS = --static -nostartfiles -T$(TARGET).ld -Wl,-Map=$(TARGET).map \
         -Wl,--gc-sections -L../libopencm3/lib/ -L../libopencm3/lib/stm32/f1/ \
         -lopencm3_stm32f1 -Wl,--start-group -lc -lnosys -Wl,--end-group
ifeq "$(TYPE)" "dev"
  CFLAGS   := $(CFLAGS) -g -DBUILDTYPE_DEV
endif
##################################################
#END SECTION                                     #
##################################################

all: $(LIBOPENCM3) $(TARGET).dfu

####################################
# recompile if the Makefile changes#
####################################
$(OBJS) $(PROTO_OBJS) $(PROTO_EXTRA_OBJS): Makefile

##################################################################################
# The following enables quiet output unless you use VERBOSE=1                    #
# Note that this must be after the 1st rule so that it doesn't execute by default#
##################################################################################
$(VERBOSE).SILENT:

.PHONY: clean

##########################################
#Ensure necessray directories are created#
##########################################
$(OBJS): | $(ODIR)

$(ODIR):
	@mkdir -p $@

######################
#The main executable#
######################
$(TARGET).elf: $(LINKFILE) $(OBJS) $(LIBOPENCM3)
	@echo " + Building '$@'"
ifdef LINKFILE #Create an empty 'obj/optimize.ld' just in case the linker script needs it
	echo "" > objs/optimize.ld
endif
	$(CC) -o $@ $(OBJS) $(LIBOPENCM3) $(LFLAGS) $(CFLAGS)

$(TARGET).dfu: $(TARGET).elf
	$(CP) -O binary $(TARGET).elf $(TARGET).bin
	$(DUMP) -d $(TARGET).elf > $(TARGET).list
	../../utils/dfu.py $(DFU_ARGS):$(TARGET).bin $(TARGET).dfu
	rm -f $(TARGET).bin
	../../utils/get_mem_usage.pl $(TARGET).map

##############################
#Build rules for all .o files#
##############################
## The autodependency magic below was adapeted from:
## http://mad-scientist.net/make/autodep.html
-include $(OBJS:.o=.P)
-include $(PROTO_OBJS:.o=.P)
-include $(PROTO_EXTRA_OBJS:.o=.P)

dollar = $$
define define_compile_rules
$(ODIR)/%.o: $(1)%.c
	@echo " + Compiling '$$<'"
	$(CC) $$(CFLAGS) -MD -c -o $$@ $$<
	@cp $(ODIR)/$$*.d $(ODIR)/$$*.P; \
            sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$(dollar)//' \
                -e '/^$$(dollar)/ d' -e 's/$$(dollar)/ :/' < $(ODIR)/$$*.d >> $(ODIR)/$$*.P; \
            rm -f $(ODIR)/$$*.d
endef
$(foreach directory,$(dir $(SRC_C) $(PROTO_EXTRA_C)),$(eval $(call define_compile_rules,$(directory))))

##############################
#Ensure version is up to date#
##############################
# Rebuild on hg version change.
$(ODIR)/version.o: $(ODIR)/hgstamp

$(ODIR)/hgstamp:
	echo $(HGVERSION) > $(ODIR)/hgstamp

$(LIBOPENCM3):
	make -C ../libopencm3 TARGETS=stm32/f1 lib

clean:
	rm -f $(TARGET).elf $(TARGET).bin $(TARGET).dfu $(TARGET).list \
		$(TARGET).map $(ODIR)/*.o $(ODIR)/*.o_ $(ODIR)/*.P  $(ODIR)/*.bin \
		2> /dev/null || true
