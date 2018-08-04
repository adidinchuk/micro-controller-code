#############################################################################
#
# Makefile for librf24 examples on Linux
#
# License: GPL (General Public License)
# Author:  gnulnulf <arco@appeltaart.mine.nu>
# Date:    2013/02/07 (version 1.0)
#
# Description:
# ------------
# use make all and make install to install the examples
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

LIBS := $(LIBS) -I /home/pi/dev/lib/c/NewSoftSerial

include ../Makefile.avr
