# cwb
# File:/src/Makefile
# Date:2021.09.11
# By MIT
# Copyright(C) 2021 cwb developers.All rights reserved.

# Configure
CONF_DEBUG_OPTION:=-g

# Tool
CC:=gcc
AR:=ar
REMOVE:=rm
ECHO:=echo
COPY:=cp -rf
MAKEDIR:=mkdir

# Flags
FLAG_AR:=rcu
FLAG_REMOVE:=-rf
FLAG_CC:=-Wall -Werror $(CONF_DEBUG_OPTION) -pedantic -Wextra -I./ \
	 -Wwrite-strings -O3
FLAG_CC_COROUTINE:=-Wall -Werror $(CONF_DEBUG_OPTION) -O0 -pedantic \
		   -Wextra -I./ -Wwrite-strings -O0

# Files
FILES_CODERS:=Coder_Base64.o Coder_URI.o
FILES_SERIALIZERS:=Serializer_Urlcoded.o

FILES_OBJS:=Net.o Time.o IO.o Event.o Ds.o Dstr.o Buffer.o	\
	    Coroutine.o Util.o Serializer.o Httpd.o		\
	    Httpd_Response.o Httpd_Request.o			\
	    $(FILES_SERIALIZERS)				\
	    $(FILES_CODERS)
FILES_LIB:=libcwb.a
FILES_HEADERS:=cwb
DIR_INSTALL_INCLUDE:=/usr/include
DIR_INSTALL_LIB:=/usr/lib

default:build

%.o:%.c
	@$(ECHO) [C99] $< $@
	@$(CC) $< -c $(FLAG_CC)

Coroutine.o: Coroutine.c
	@$(ECHO) [C99] $< $@
	@$(CC) $< -c $(FLAG_CC_COROUTINE)

build:$(FILES_OBJS)
	@$(ECHO) [AR] $(FILES_LIB) $(FILES_OBJS)
	@$(AR) $(FLAG_AR) $(FILES_LIB) $(FILES_OBJS)

disbuild:
	@$(ECHO) [REMOVE] $(FILES_OBJS) $(FILES_LIB)
	@$(REMOVE) $(FILES_OBJS) $(FILES_LIB)

install:build
	@$(ECHO) [COPY] $(FILES_HEADERS) $(DIR_INSTALL_INCLUDE)
	@$(COPY) $(FILES_HEADERS) $(DIR_INSTALL_INCLUDE)
	@$(ECHO) [COPY] $(FILES_LIB)
	@$(COPY) $(FILES_LIB) $(DIR_INSTALL_LIB)

uninstall:
	@$(ECHO) [REMOVE] $(DIR_INSTALL_INCLUDE)/cwb
	@$(REMOVE) $(FLAG_REMOVE) $(DIR_INSTALL_HEADERS)/cwb
	@$(ECHO) [REMOVE] $(DIR_INSTALL_LIB)/libcwb.a
	@$(REMOVE) $(DIR_INSTALL_LIB)/libcwb.a
