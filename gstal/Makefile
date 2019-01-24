#=========================================================================
# Makefile for making gstal interpreter
#=========================================================================

#=========================================================================
# To allow make to choose the right system, include this in your .login
# file:
#
#     setenv BCSYSTEM [system name]
#
# This is necessary so that selectSystem (see below) will work correctly.
#=========================================================================

CC = gcc
#CCOPTS = -Aa
CCOPTS = -ansi

MATHLIB = -lm

LEX = flex
#LEXLIB = -lfl   #Cygwin
LEXLIB = -ll   #Mac

YACC = bison
YACCOPTS = -d -b y


selectSystem:
	make gstal
#	@ if test "$${BCSYSTEM}" = "minix"; then \
#		make gstal "CCOPTS=-wo" "MATHLIB=-lm" "LEXLIB=-lfl" "LEX=flex"; \
#	else \
#		make gstal "CCOPTS=-Aa" "MATHLIB=-lm" "LEXLIB=-ll" "LEX=lex"; \
#	fi

gstal: gst_main.o gst_run.o gst_strings.o y.tab.o lex.yy.o
	${CC} -o gstal gst_main.o gst_run.o gst_strings.o y.tab.o lex.yy.o ${LEXLIB} ${MATHLIB}

gst_main.o: gst_main.c gst_strings.h
	${CC} -c ${CCOPTS} gst_main.c

gst_run.o: gst_run.c gst_run.h
	${CC} -c ${CCOPTS} gst_run.c

gst_strings.o: gst_strings.c
	${CC} -c ${CCOPTS} gst_strings.c

gst_main.o gst_run.o: y.tab.h gst_run.h

y.tab.o: y.tab.c gst_run.h
	${CC} -c ${CCOPTS} y.tab.c

lex.yy.o: lex.yy.c gst_debug.h
	${CC} -c ${CCOPTS} lex.yy.c

y.tab.o lex.yy.o: y.tab.h

y.tab.h y.tab.c: gst_parser.y
	${YACC} ${YACCOPTS} gst_parser.y

lex.yy.c: gst_scanner.l
	${LEX} gst_scanner.l

clean:
	rm -f  gstal gstal.exe *.o lex.yy.c y.tab.c y.tab.h stackdump core

debug:
	cp gst_debug.on gst_debug.h
	make selectSystem

nodebug:
	cp gst_debug.off gst_debug.h
	make selectSystem

install:
	@ echo "Installation unavailable"
