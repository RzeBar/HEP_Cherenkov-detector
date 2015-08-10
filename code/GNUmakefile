
name := LBarQuartic
G4TARGET := $(name)
G4EXLIB := true
G4WORKDIR :=.


.PHONY: all
all:  lib bin

CPPFLAGS += -I$(ROOTSYS)/include 
EXTRALIBS = $(shell root-config --glibs) 

include $(G4INSTALL)/config/binmake.gmk

