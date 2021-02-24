# cwb
# File:/Lib/Makefile
# Date:2021.02.15
# By LGPL v3.0 and Anti-996 License
# Copyright(C) 2021 cwb developers.All rights reserved.

# Tool
CC:=c99
AR:=ar
REMOVE:=rm
ECHO:=echo
COPY:=cp
MAKEDIR:=mkdir

# Flags
FLAG_AR:=rcu
FLAG_REMOVE:=-rf
FLAG_CC:=-Wall -Werror -g -O0 -pedantic -Wextra

# Files
FILES_OBJS:=Net.o Time.o Event.o
FILES_LIB:=libcwb.a
DIR_INSTALL_INCLUDE:=/usr/include
DIR_INSTALL_LIB:=/usr/lib
DIR_INSTALL_HEADERS=$(DIR_INSTALL_INCLUDE)/cwb

default:build

%.o:%.c
	@$(ECHO) [C99] $< $@
	@$(CC) $< -c $(FLAG_CC)

build:$(FILES_OBJS)
	@$(ECHO) [AR] $(FILES_LIB) $(FILES_OBJS)
	@$(AR) $(FLAG_AR) $(FILES_LIB) $(FILES_OBJS)

disbuild:
	@$(ECHO) [REMOVE] $(FILES_OBJS) $(FILES_LIB)
	@$(REMOVE) $(FILES_OBJS) $(FILES_LIB)

install:build
	@$(ECHO) [MAKEDIR] $(DIR_INSTALL_HEADERS)
	@$(MAKEDIR) $(DIR_INSTALL_HEADERS)
	@$(ECHO) [COPY] Headers
	@$(COPY) *.h $(DIR_INSTALL_HEADERS)
	@$(ECHO) [COPY] $(FILES_LIB)
	@$(COPY) $(FILES_LIB) $(DIR_INSTALL_LIB)

uninstall:
	@$(ECHO) [REMOVE] $(DIR_INSTALL_HEADERS)
	@$(REMOVE) $(FLAG_REMOVE) $(DIR_INSTALL_HEADERS)
