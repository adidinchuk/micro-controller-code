#############################################################################
#
# Makefile shell
#
# Description:
# ------------
# use make all and make install the code to the chip
#

ifeq ($(wildcard ../Makefile.common), )
        $(error Configuration not found.)
endif

ifeq ($(wildcard ../Makefile.avr), )
        $(error AVR make file not found.)
endif

include ../Makefile.common

# define all programs
PROGRAMS = code

include ../Makefile.avr
